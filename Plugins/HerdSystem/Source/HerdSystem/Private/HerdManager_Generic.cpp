#include "HerdManager_Generic.h"

#include "Components/SceneComponent.h"
#include "DrawDebugHelpers.h"
#include "EngineUtils.h"
#include "HerdMemberComponent.h"
#include "HerdScanDatasetComponent.h"
#include "Net/UnrealNetwork.h"
#include "SpeciesData_Base.h"

AHerdManager_Generic::AHerdManager_Generic()
{
    PrimaryActorTick.bCanEverTick = true;
    bReplicates = true;
    SetReplicateMovement(true);

    USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = SceneRoot;

    ScanDatasetComponent = CreateDefaultSubobject<UHerdScanDatasetComponent>(TEXT("ScanDataset"));
    ScanDatasetComponent->bAutoScan = false;
}

void AHerdManager_Generic::BeginPlay()
{
    Super::BeginPlay();

    if (HasAuthority())
    {
        if (bAutoRegisterNearbyMembers)
        {
            AutoRegisterNearbyMembers();
        }

        RebuildSpeciesRuntime();
        ForceRescan();
    }
}

void AHerdManager_Generic::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (!HasAuthority())
    {
        return;
    }

    DecisionTimer += DeltaSeconds;
    if (DecisionTimer < DecisionInterval)
    {
        return;
    }

    DecisionTimer = 0.0f;
    const FHerdScanDataset ScanDataset = ForceRescan();
    UpdateHerd(DeltaSeconds);
    ApplyHerdStateToMembers(ScanDataset, HerdState);

    if (bEnableDebugDraw)
    {
        DrawDebugHerd(ScanDataset);
    }
}

void AHerdManager_Generic::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AHerdManager_Generic, AssignedSpecies);
    DOREPLIFETIME(AHerdManager_Generic, CurrentHerdSize);
    DOREPLIFETIME(AHerdManager_Generic, RegisteredMembers);
    DOREPLIFETIME(AHerdManager_Generic, SpeciesRuntime);
    DOREPLIFETIME(AHerdManager_Generic, HerdState);
    DOREPLIFETIME(AHerdManager_Generic, CurrentMigrationRouteIndex);
    DOREPLIFETIME(AHerdManager_Generic, CurrentMigrationWaypointIndex);
}

void AHerdManager_Generic::RegisterMember(UHerdMemberComponent* Member, UDA_SpeciesData_Base* SpeciesOverride)
{
    if (!Member)
    {
        return;
    }

    if (!HasAuthority())
    {
        ServerRegisterMember(Member, SpeciesOverride);
        return;
    }

    const bool bAlreadyRegistered = RegisteredMembers.Contains(Member);
    if (!bAlreadyRegistered)
    {
        RegisteredMembers.Add(Member);
    }

    UDA_SpeciesData_Base* ResolvedSpecies = SpeciesOverride ? SpeciesOverride : Member->SpeciesData;
    if (ResolvedSpecies)
    {
        Member->InitializeFromSpecies(ResolvedSpecies, this);
        AssignedSpecies.AddUnique(ResolvedSpecies);
    }
    else
    {
        Member->SetHerdManager(this);
    }

    CurrentHerdSize = RegisteredMembers.Num();
    RebuildSpeciesRuntime();
    if (!bAlreadyRegistered)
    {
        OnHerdCompositionChanged.Broadcast(this);
    }
}

void AHerdManager_Generic::ServerRegisterMember_Implementation(UHerdMemberComponent* Member, UDA_SpeciesData_Base* SpeciesOverride)
{
    RegisterMember(Member, SpeciesOverride);
}

void AHerdManager_Generic::UnregisterMember(UHerdMemberComponent* Member)
{
    if (!Member)
    {
        return;
    }

    if (!HasAuthority())
    {
        ServerUnregisterMember(Member);
        return;
    }

    RegisteredMembers.Remove(Member);
    if (Member->HerdManager == this)
    {
        Member->SetHerdManager(nullptr);
    }

    CurrentHerdSize = RegisteredMembers.Num();
    RebuildSpeciesRuntime();
    OnHerdCompositionChanged.Broadcast(this);
}

void AHerdManager_Generic::ServerUnregisterMember_Implementation(UHerdMemberComponent* Member)
{
    UnregisterMember(Member);
}

FHerdScanDataset AHerdManager_Generic::ForceRescan()
{
    return ScanDatasetComponent ? ScanDatasetComponent->Scan(this) : FHerdScanDataset();
}

void AHerdManager_Generic::RebuildSpeciesRuntime()
{
    if (!HasAuthority())
    {
        return;
    }

    RegisteredMembers.RemoveAll([](const TObjectPtr<UHerdMemberComponent>& Member)
    {
        return !IsValid(Member);
    });

    SpeciesRuntime.Empty();
    CurrentHerdSize = RegisteredMembers.Num();

    for (UHerdMemberComponent* Member : RegisteredMembers)
    {
        if (!Member || !Member->SpeciesData)
        {
            continue;
        }

        FHerdRuntimeSpeciesEntry* Entry = SpeciesRuntime.FindByPredicate([Member](const FHerdRuntimeSpeciesEntry& Candidate)
        {
            return Candidate.SpeciesData == Member->SpeciesData;
        });

        if (!Entry)
        {
            FHerdRuntimeSpeciesEntry NewEntry;
            NewEntry.SpeciesData = Member->SpeciesData;
            SpeciesRuntime.Add(NewEntry);
            Entry = &SpeciesRuntime.Last();
        }

        Entry->Members.Add(Member);
        Entry->CurrentCount++;
        Entry->AverageHealth += Member->GetHealthRatio();
        Entry->AverageHunger += Member->GetHungerRatio();
        Entry->AverageThirst += Member->GetThirstRatio();
        Entry->AveragePanic += Member->Panic;
    }

    for (FHerdRuntimeSpeciesEntry& Entry : SpeciesRuntime)
    {
        if (Entry.CurrentCount <= 0)
        {
            continue;
        }

        const float Count = static_cast<float>(Entry.CurrentCount);
        Entry.AverageHealth /= Count;
        Entry.AverageHunger /= Count;
        Entry.AverageThirst /= Count;
        Entry.AveragePanic /= Count;
    }
}

bool AHerdManager_Generic::StartMigrationRoute(int32 RouteIndex)
{
    if (!HasAuthority() || !MigrationRoutes.IsValidIndex(RouteIndex) || MigrationRoutes[RouteIndex].Waypoints.IsEmpty())
    {
        return false;
    }

    CurrentMigrationRouteIndex = RouteIndex;
    CurrentMigrationWaypointIndex = 0;
    SetHerdState(EHerdMemberState::Migrate);
    return true;
}

FVector AHerdManager_Generic::GetHerdCenter() const
{
    if (RegisteredMembers.IsEmpty())
    {
        return GetActorLocation();
    }

    FVector Sum = FVector::ZeroVector;
    int32 Count = 0;
    for (const UHerdMemberComponent* Member : RegisteredMembers)
    {
        const AActor* Owner = Member ? Member->GetOwner() : nullptr;
        if (!Owner)
        {
            continue;
        }

        Sum += Owner->GetActorLocation();
        Count++;
    }

    return Count > 0 ? Sum / static_cast<float>(Count) : GetActorLocation();
}

UDA_SpeciesData_Base* AHerdManager_Generic::GetDominantSpecies() const
{
    const FHerdRuntimeSpeciesEntry* BestEntry = nullptr;
    for (const FHerdRuntimeSpeciesEntry& Entry : SpeciesRuntime)
    {
        if (!BestEntry || Entry.CurrentCount > BestEntry->CurrentCount)
        {
            BestEntry = &Entry;
        }
    }

    if (BestEntry && BestEntry->SpeciesData)
    {
        return BestEntry->SpeciesData.Get();
    }

    return AssignedSpecies.Num() > 0 ? AssignedSpecies[0].Get() : nullptr;
}

TArray<UHerdMemberComponent*> AHerdManager_Generic::GetMembersBySpecies(UDA_SpeciesData_Base* Species) const
{
    TArray<UHerdMemberComponent*> Result;
    for (UHerdMemberComponent* Member : RegisteredMembers)
    {
        if (Member && Member->SpeciesData == Species)
        {
            Result.Add(Member);
        }
    }
    return Result;
}

UHerdMemberComponent* AHerdManager_Generic::GetLeader() const
{
    for (UHerdMemberComponent* Member : RegisteredMembers)
    {
        if (Member && Member->HerdRole == EHerdRole::Leader)
        {
            return Member;
        }
    }

    return RegisteredMembers.Num() > 0 ? RegisteredMembers[0].Get() : nullptr;
}

FHerdScanDataset AHerdManager_Generic::GetCurrentScanDataset() const
{
    return ScanDatasetComponent ? ScanDatasetComponent->GetScanDataset() : FHerdScanDataset();
}

EHerdInteractionDisposition AHerdManager_Generic::GetInteractionBetweenSpecies(const UDA_SpeciesData_Base* ObserverSpecies, const UDA_SpeciesData_Base* OtherSpecies) const
{
    if (!ObserverSpecies || !OtherSpecies)
    {
        return EHerdInteractionDisposition::Neutral;
    }

    if (ObserverSpecies == OtherSpecies)
    {
        return EHerdInteractionDisposition::Friendly;
    }

    FHerdInteractionRule Rule;
    if (ObserverSpecies->FindInteractionRule(OtherSpecies, Rule))
    {
        return Rule.Disposition;
    }

    const bool bObserverPredator = IsPredatoryDiet(ObserverSpecies);
    const bool bOtherPredator = IsPredatoryDiet(OtherSpecies);

    if (bObserverPredator && bOtherPredator)
    {
        return ObserverSpecies->DefaultPredatorPredatorDisposition;
    }

    if (!bObserverPredator && !bOtherPredator)
    {
        return ObserverSpecies->DefaultHerbivoreHerbivoreDisposition;
    }

    return bObserverPredator ? EHerdInteractionDisposition::Prey : EHerdInteractionDisposition::Predator;
}

EHerdThreatType AHerdManager_Generic::GetThreatTypeBetweenSpecies(const UDA_SpeciesData_Base* ObserverSpecies, const UDA_SpeciesData_Base* OtherSpecies, EHerdThreatType FallbackThreatType) const
{
    if (!ObserverSpecies || !OtherSpecies || ObserverSpecies == OtherSpecies)
    {
        return FallbackThreatType;
    }

    FHerdInteractionRule Rule;
    if (ObserverSpecies->FindInteractionRule(OtherSpecies, Rule) && Rule.ThreatType != EHerdThreatType::Unknown)
    {
        return Rule.ThreatType;
    }

    const EHerdInteractionDisposition Disposition = GetInteractionBetweenSpecies(ObserverSpecies, OtherSpecies);
    switch (Disposition)
    {
    case EHerdInteractionDisposition::Predator:
        return EHerdThreatType::Predator;
    case EHerdInteractionDisposition::Prey:
        return EHerdThreatType::Prey;
    case EHerdInteractionDisposition::Hostile:
        return IsPredatoryDiet(OtherSpecies) ? EHerdThreatType::RivalPredator : EHerdThreatType::RivalHerbivore;
    case EHerdInteractionDisposition::Avoid:
        return EHerdThreatType::Unknown;
    default:
        return FallbackThreatType;
    }
}

float AHerdManager_Generic::ComputeThreatLevelForScanItem(const FHerdScanItem& Item) const
{
    if (const UHerdMemberComponent* OtherMember = Item.HerdMember)
    {
        if (OtherMember->HerdManager == this)
        {
            return 0.0f;
        }
    }

    float BaseThreat = Item.ThreatLevel;
    switch (Item.Relationship)
    {
    case EHerdInteractionDisposition::Predator:
        BaseThreat = FMath::Max(BaseThreat, 6.0f);
        break;
    case EHerdInteractionDisposition::Hostile:
        BaseThreat = FMath::Max(BaseThreat, 5.0f);
        break;
    case EHerdInteractionDisposition::Avoid:
        BaseThreat = FMath::Max(BaseThreat, 2.0f);
        break;
    case EHerdInteractionDisposition::Prey:
        BaseThreat = FMath::Max(BaseThreat, 1.0f);
        break;
    default:
        break;
    }

    if (Item.ThreatType == EHerdThreatType::Player)
    {
        if (const UDA_SpeciesData_Base* DominantSpecies = GetDominantSpecies())
        {
            BaseThreat *= DominantSpecies->PlayerThreatWeight;
        }
    }
    else if (Item.ThreatType == EHerdThreatType::EnvironmentalHazard || Item.ThreatType == EHerdThreatType::Fire || Item.ThreatType == EHerdThreatType::Weather)
    {
        if (const UDA_SpeciesData_Base* DominantSpecies = GetDominantSpecies())
        {
            BaseThreat *= DominantSpecies->EnvironmentalThreatWeight;
        }
    }

    const float DistanceFactor = Item.Distance > 0.0f ? FMath::Clamp(1.0f - (Item.Distance / FMath::Max(1.0f, MaxSpreadRadius * 2.0f)), 0.15f, 1.0f) : 1.0f;
    return FMath::Clamp(BaseThreat * DistanceFactor, 0.0f, 10.0f);
}

void AHerdManager_Generic::OnRep_AssignedSpecies()
{
    OnHerdCompositionChanged.Broadcast(this);
}

void AHerdManager_Generic::OnRep_HerdState(EHerdMemberState PreviousState)
{
    if (PreviousState != HerdState)
    {
        OnHerdStateChanged.Broadcast(this, HerdState);
    }
}

void AHerdManager_Generic::AutoRegisterNearbyMembers()
{
    UWorld* World = GetWorld();
    if (!World)
    {
        return;
    }

    for (TActorIterator<AActor> It(World); It; ++It)
    {
        AActor* Actor = *It;
        if (!Actor || Actor == this || FVector::DistSquared(Actor->GetActorLocation(), GetActorLocation()) > FMath::Square(AutoRegisterRadius))
        {
            continue;
        }

        if (UHerdMemberComponent* Member = Actor->FindComponentByClass<UHerdMemberComponent>())
        {
            RegisterMember(Member, Member->SpeciesData);
        }
    }
}

void AHerdManager_Generic::UpdateHerd(float DeltaSeconds)
{
    RebuildSpeciesRuntime();
    AdvanceMigrationIfNeeded();
    const FHerdScanDataset ScanDataset = GetCurrentScanDataset();
    SetHerdState(EvaluateHerdState(ScanDataset));
}

EHerdMemberState AHerdManager_Generic::EvaluateHerdState(const FHerdScanDataset& ScanDataset) const
{
    FHerdScanItem HighestThreat;
    const bool bHasThreat = ScanDatasetComponent && ScanDatasetComponent->GetHighestThreat(HighestThreat);
    const float HighestThreatLevel = bHasThreat ? HighestThreat.ThreatLevel : 0.0f;

    if (bHasThreat && HighestThreatLevel >= 5.0f && HasAnySpeciesThatCanStampede())
    {
        for (const FHerdRuntimeSpeciesEntry& Entry : SpeciesRuntime)
        {
            if (Entry.SpeciesData && Entry.CurrentCount >= Entry.SpeciesData->StampedeMinMembers && Entry.AveragePanic >= Entry.SpeciesData->StampedePanicThreshold)
            {
                return EHerdMemberState::Stampede;
            }
        }
    }

    if (bHasThreat && HighestThreatLevel >= 4.0f && HasAnySpeciesWithDefensiveCircle())
    {
        return EHerdMemberState::Defensive;
    }

    if (HasAnySpeciesWithMigration() && CurrentMigrationRouteIndex != INDEX_NONE)
    {
        return EHerdMemberState::Migrate;
    }

    const FVector Center = GetHerdCenter();
    if (GetMaxDistanceFromCenter(Center) > MaxSpreadRadius)
    {
        return EHerdMemberState::Regroup;
    }

    for (const FHerdRuntimeSpeciesEntry& Entry : SpeciesRuntime)
    {
        if (!Entry.SpeciesData)
        {
            continue;
        }

        if (Entry.SpeciesData->bUseHerdLevelHunger && Entry.AverageHunger >= FMath::Max(HerdLevelHungerThreshold, Entry.SpeciesData->HerdLevelHungerThreshold))
        {
            return EHerdMemberState::Graze;
        }

        if (Entry.AverageThirst >= FMath::Max(HerdLevelThirstThreshold, Entry.SpeciesData->HerdLevelThirstThreshold))
        {
            return EHerdMemberState::Drink;
        }
    }

    if (bHasThreat && HighestThreatLevel > 0.0f)
    {
        return EHerdMemberState::Alert;
    }

    return EHerdMemberState::Idle;
}

void AHerdManager_Generic::ApplyHerdStateToMembers(const FHerdScanDataset& ScanDataset, EHerdMemberState NewHerdState)
{
    FVector Target = GetHerdCenter();
    float Priority = 0.5f;

    switch (NewHerdState)
    {
    case EHerdMemberState::Stampede:
        Target = GetThreatAvoidanceTarget(ScanDataset);
        Priority = 2.0f;
        break;
    case EHerdMemberState::Defensive:
        Priority = 1.5f;
        break;
    case EHerdMemberState::Migrate:
        Target = GetCurrentMigrationTarget();
        Priority = 1.0f;
        break;
    case EHerdMemberState::Regroup:
        Priority = 1.0f;
        break;
    case EHerdMemberState::Graze:
        if (ScanDatasetComponent)
        {
            FHerdScanItem Food;
            if (ScanDatasetComponent->FindNearestResource(EHerdResourceType::Food, Food))
            {
                Target = Food.Location;
            }
        }
        Priority = 0.35f;
        break;
    case EHerdMemberState::Drink:
        if (ScanDatasetComponent)
        {
            FHerdScanItem Water;
            if (ScanDatasetComponent->FindNearestResource(EHerdResourceType::Water, Water))
            {
                Target = Water.Location;
            }
        }
        Priority = 0.45f;
        break;
    default:
        break;
    }

    int32 MemberIndex = 0;
    for (UHerdMemberComponent* Member : RegisteredMembers)
    {
        if (!Member)
        {
            continue;
        }

        Member->EvaluateDecision(ScanDataset);

        if (NewHerdState == EHerdMemberState::Idle || NewHerdState == EHerdMemberState::Alert)
        {
            MemberIndex++;
            continue;
        }

        if (NewHerdState == EHerdMemberState::Graze && Member->SpeciesData && Member->SpeciesData->UsesPredatorHungerLogic())
        {
            MemberIndex++;
            continue;
        }

        const FVector MemberTarget = NewHerdState == EHerdMemberState::Defensive ? GetDefensiveCircleTarget(MemberIndex) : Target;
        Member->ApplyManagerState(NewHerdState, MemberTarget, Priority);
        MemberIndex++;
    }
}

void AHerdManager_Generic::SetHerdState(EHerdMemberState NewState)
{
    if (HerdState == NewState)
    {
        return;
    }

    HerdState = NewState;
    OnHerdStateChanged.Broadcast(this, HerdState);
}

void AHerdManager_Generic::AdvanceMigrationIfNeeded()
{
    if (CurrentMigrationRouteIndex == INDEX_NONE || !MigrationRoutes.IsValidIndex(CurrentMigrationRouteIndex))
    {
        return;
    }

    const FHerdMigrationRoute& Route = MigrationRoutes[CurrentMigrationRouteIndex];
    if (!Route.Waypoints.IsValidIndex(CurrentMigrationWaypointIndex))
    {
        CurrentMigrationWaypointIndex = 0;
        return;
    }

    const FVector Target = Route.Waypoints[CurrentMigrationWaypointIndex];
    if (FVector::DistSquared(GetHerdCenter(), Target) > FMath::Square(Route.ArrivalRadius))
    {
        return;
    }

    CurrentMigrationWaypointIndex++;
    if (!Route.Waypoints.IsValidIndex(CurrentMigrationWaypointIndex))
    {
        if (Route.bLoop)
        {
            CurrentMigrationWaypointIndex = 0;
        }
        else
        {
            CurrentMigrationRouteIndex = INDEX_NONE;
        }
    }
}

FVector AHerdManager_Generic::GetCurrentMigrationTarget() const
{
    if (!MigrationRoutes.IsValidIndex(CurrentMigrationRouteIndex))
    {
        return GetHerdCenter();
    }

    const FHerdMigrationRoute& Route = MigrationRoutes[CurrentMigrationRouteIndex];
    return Route.Waypoints.IsValidIndex(CurrentMigrationWaypointIndex) ? Route.Waypoints[CurrentMigrationWaypointIndex] : GetHerdCenter();
}

FVector AHerdManager_Generic::GetThreatAvoidanceTarget(const FHerdScanDataset& ScanDataset) const
{
    FVector FleeDirection = FVector::ZeroVector;
    const FVector Center = GetHerdCenter();

    for (const FHerdScanItem& Threat : ScanDataset.NearbyThreats)
    {
        FleeDirection += (Center - Threat.Location).GetSafeNormal() * FMath::Max(1.0f, Threat.ThreatLevel);
    }

    if (FleeDirection.IsNearlyZero())
    {
        FleeDirection = GetActorForwardVector();
    }

    return Center + FleeDirection.GetSafeNormal() * MaxSpreadRadius;
}

FVector AHerdManager_Generic::GetDefensiveCircleTarget(int32 MemberIndex) const
{
    const FVector Center = GetHerdCenter();
    const int32 Count = FMath::Max(1, RegisteredMembers.Num());
    const float Angle = (2.0f * PI * static_cast<float>(MemberIndex)) / static_cast<float>(Count);
    float Radius = 700.0f;

    if (const UDA_SpeciesData_Base* DominantSpecies = GetDominantSpecies())
    {
        Radius = DominantSpecies->DefensiveCircleRadius;
    }

    return Center + FVector(FMath::Cos(Angle), FMath::Sin(Angle), 0.0f) * Radius;
}

bool AHerdManager_Generic::HasAnySpeciesThatCanStampede() const
{
    for (const UDA_SpeciesData_Base* Species : AssignedSpecies)
    {
        if (Species && Species->bCanStampede)
        {
            return true;
        }
    }
    return false;
}

bool AHerdManager_Generic::HasAnySpeciesWithDefensiveCircle() const
{
    for (const UDA_SpeciesData_Base* Species : AssignedSpecies)
    {
        if (Species && Species->bDefensiveCircle)
        {
            return true;
        }
    }
    return false;
}

bool AHerdManager_Generic::HasAnySpeciesWithMigration() const
{
    for (const UDA_SpeciesData_Base* Species : AssignedSpecies)
    {
        if (Species && Species->bMigrationEnabled)
        {
            return true;
        }
    }
    return false;
}

float AHerdManager_Generic::GetMaxDistanceFromCenter(FVector Center) const
{
    float MaxDistance = 0.0f;
    for (const UHerdMemberComponent* Member : RegisteredMembers)
    {
        const AActor* Owner = Member ? Member->GetOwner() : nullptr;
        if (!Owner)
        {
            continue;
        }

        MaxDistance = FMath::Max(MaxDistance, FVector::Dist(Owner->GetActorLocation(), Center));
    }
    return MaxDistance;
}

void AHerdManager_Generic::DrawDebugHerd(const FHerdScanDataset& ScanDataset) const
{
    UWorld* World = GetWorld();
    if (!World)
    {
        return;
    }

    const FVector Center = GetHerdCenter();
    DrawDebugSphere(World, Center, MaxSpreadRadius, 32, FColor::Cyan, false, DebugDrawDuration, 0, 2.0f);

    for (const UHerdMemberComponent* Member : RegisteredMembers)
    {
        const AActor* Owner = Member ? Member->GetOwner() : nullptr;
        if (!Owner)
        {
            continue;
        }

        const FColor StateColor = Member->CurrentState == EHerdMemberState::Flee || Member->CurrentState == EHerdMemberState::Stampede ? FColor::Red : FColor::Green;
        DrawDebugLine(World, Center, Owner->GetActorLocation(), StateColor, false, DebugDrawDuration, 0, 1.0f);
        DrawDebugString(World, Owner->GetActorLocation() + FVector(0.0f, 0.0f, 120.0f), StaticEnum<EHerdMemberState>()->GetNameStringByValue(static_cast<int64>(Member->CurrentState)), nullptr, StateColor, DebugDrawDuration, true);
    }

    for (const FHerdScanItem& Threat : ScanDataset.NearbyThreats)
    {
        DrawDebugSphere(World, Threat.Location, 120.0f, 16, FColor::Red, false, DebugDrawDuration, 0, 3.0f);
    }

    for (const FHerdMigrationRoute& Route : MigrationRoutes)
    {
        for (int32 Index = 0; Index + 1 < Route.Waypoints.Num(); ++Index)
        {
            DrawDebugLine(World, Route.Waypoints[Index], Route.Waypoints[Index + 1], FColor::Yellow, false, DebugDrawDuration, 0, 2.0f);
        }
    }
}

bool AHerdManager_Generic::IsPredatoryDiet(const UDA_SpeciesData_Base* Species) const
{
    if (!Species)
    {
        return false;
    }

    return Species->DietType == EHerdDietType::Carnivore || Species->DietType == EHerdDietType::Scavenger || Species->DietType == EHerdDietType::Piscivore;
}
