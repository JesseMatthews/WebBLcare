#include "HerdMemberComponent.h"

#include "EngineUtils.h"
#include "HerdManager_Generic.h"
#include "Net/UnrealNetwork.h"
#include "SpeciesData_Base.h"

UHerdMemberComponent::UHerdMemberComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.bStartWithTickEnabled = true;
    SetIsReplicatedByDefault(true);
}

void UHerdMemberComponent::BeginPlay()
{
    Super::BeginPlay();

    if (GetOwner() && GetOwner()->HasAuthority())
    {
        if (SpeciesData)
        {
            InitializeFromSpecies(SpeciesData, HerdManager);
        }

        if (!HerdManager && bAutoRegisterWithNearbyManager)
        {
            AutoRegisterWithManager();
        }
    }
}

void UHerdMemberComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!GetOwner() || !GetOwner()->HasAuthority() || IsDead())
    {
        return;
    }

    ApplyNeeds(DeltaTime);
    ApplyStateStamina(DeltaTime);
    UpdateLifeStage(DeltaTime);
    ClearExpiredThreats();
    ClampVitals();

    DecisionTimer += DeltaTime;
}

void UHerdMemberComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UHerdMemberComponent, SpeciesData);
    DOREPLIFETIME(UHerdMemberComponent, HerdManager);
    DOREPLIFETIME(UHerdMemberComponent, CurrentState);
    DOREPLIFETIME(UHerdMemberComponent, Health);
    DOREPLIFETIME(UHerdMemberComponent, Stamina);
    DOREPLIFETIME(UHerdMemberComponent, Hunger);
    DOREPLIFETIME(UHerdMemberComponent, Thirst);
    DOREPLIFETIME(UHerdMemberComponent, Panic);
    DOREPLIFETIME(UHerdMemberComponent, Fear);
    DOREPLIFETIME(UHerdMemberComponent, Aggression);
    DOREPLIFETIME(UHerdMemberComponent, AgeDays);
    DOREPLIFETIME(UHerdMemberComponent, LifeStage);
    DOREPLIFETIME(UHerdMemberComponent, HerdRole);
    DOREPLIFETIME(UHerdMemberComponent, bPregnant);
    DOREPLIFETIME(UHerdMemberComponent, PregnancyElapsedDays);
    DOREPLIFETIME(UHerdMemberComponent, ThreatMemory);
    DOREPLIFETIME(UHerdMemberComponent, DesiredMoveLocation);
    DOREPLIFETIME(UHerdMemberComponent, DesiredFacingDirection);
}

void UHerdMemberComponent::InitializeFromSpecies(UDA_SpeciesData_Base* InSpeciesData, AHerdManager_Generic* InManager)
{
    if (!GetOwner() || !GetOwner()->HasAuthority())
    {
        return;
    }

    SpeciesData = InSpeciesData;
    HerdManager = InManager;

    if (!SpeciesData)
    {
        return;
    }

    Health = SpeciesData->MaxHealth;
    Stamina = SpeciesData->MaxStamina;
    Hunger = 0.0f;
    Thirst = 0.0f;
    Panic = 0.0f;
    Fear = SpeciesData->BaseFear;
    Aggression = FMath::Max(SpeciesData->BaseAggression, AggressionLevelOverride);
    HerdRole = HerdRole == EHerdRole::None ? SpeciesData->DefaultRole : HerdRole;
    AgeDays = FMath::FRandRange(SpeciesData->StartingAgeMinDays, SpeciesData->StartingAgeMaxDays);
    LifeStage = SpeciesData->GetLifeStageForAge(AgeDays);
    FleeThresholdHealth = SpeciesData->GetThreatResponse(EHerdThreatType::Predator).FleeThresholdHealth;
    DesiredMoveLocation = GetOwner()->GetActorLocation();
    DesiredFacingDirection = GetOwner()->GetActorForwardVector();
    ClampVitals();
}

void UHerdMemberComponent::SetHerdManager(AHerdManager_Generic* InManager)
{
    if (!GetOwner() || !GetOwner()->HasAuthority())
    {
        return;
    }

    HerdManager = InManager;
}

void UHerdMemberComponent::EvaluateDecision(const FHerdScanDataset& ScanDataset)
{
    if (!GetOwner() || !GetOwner()->HasAuthority() || !SpeciesData || IsDead())
    {
        return;
    }

    if (DecisionTimer < DecisionInterval)
    {
        return;
    }
    DecisionTimer = 0.0f;

    bool bFoundThreat = false;
    const FHerdScanItem ThreatItem = FindHighestThreat(ScanDataset, bFoundThreat);
    if (bFoundThreat && IsValid(ThreatItem.Actor))
    {
        RememberThreat(ThreatItem.Actor, ThreatItem.ThreatType, ThreatItem.ThreatLevel, SpeciesData->ThreatMemoryDuration);

        FHerdThreatResponse Response;
        if (ShouldFleeFromThreat(ThreatItem, Response))
        {
            const FVector Away = (GetOwner()->GetActorLocation() - ThreatItem.Location).GetSafeNormal();
            DesiredMoveLocation = GetOwner()->GetActorLocation() + Away * Response.FleeDistance;
            DesiredFacingDirection = Away;
            Panic = FMath::Clamp(Panic + Response.PanicContribution * SpeciesData->PanicMultiplier, 0.0f, 10.0f);
            SetState(Panic > SpeciesData->StampedePanicThreshold ? EHerdMemberState::Scatter : EHerdMemberState::Flee);
            return;
        }

        if (ShouldAttackThreat(ThreatItem, Response))
        {
            DesiredMoveLocation = ThreatItem.Location;
            DesiredFacingDirection = (ThreatItem.Location - GetOwner()->GetActorLocation()).GetSafeNormal();
            SetState(ThreatItem.Distance <= Response.AttackDistance ? EHerdMemberState::Attack : EHerdMemberState::Chase);
            return;
        }

        DesiredFacingDirection = (ThreatItem.Location - GetOwner()->GetActorLocation()).GetSafeNormal();
        SetState(EHerdMemberState::Alert);
        return;
    }

    const float HungerRatio = GetHungerRatio();
    const float ThirstRatio = GetThirstRatio();

    if (ThirstRatio >= SpeciesData->CriticalNeedsThreshold)
    {
        SetState(EHerdMemberState::Drink);
        return;
    }

    if (HungerRatio >= SpeciesData->CriticalNeedsThreshold)
    {
        SetState(SpeciesData->UsesPredatorHungerLogic() ? EHerdMemberState::Chase : EHerdMemberState::Graze);
        return;
    }

    if (Panic > 0.1f)
    {
        Panic = FMath::Max(0.0f, Panic - SpeciesData->CalmDownRatePerSecond * DecisionInterval);
    }

    SetState(Panic > 1.0f ? EHerdMemberState::Alert : EHerdMemberState::Idle);
}

void UHerdMemberComponent::ApplyManagerState(EHerdMemberState NewState, FVector TargetLocation, float Priority)
{
    if (!GetOwner() || !GetOwner()->HasAuthority() || !bAllowManagerStateOverride || IsDead())
    {
        return;
    }

    const bool bAlreadyCommittedToThreat = CurrentState == EHerdMemberState::Flee || CurrentState == EHerdMemberState::Chase || CurrentState == EHerdMemberState::Attack;
    if (bAlreadyCommittedToThreat && Priority < 1.0f)
    {
        return;
    }

    DesiredMoveLocation = TargetLocation;
    const FVector ToTarget = TargetLocation - GetOwner()->GetActorLocation();
    DesiredFacingDirection = ToTarget.IsNearlyZero() ? GetOwner()->GetActorForwardVector() : ToTarget.GetSafeNormal();
    SetState(NewState);
}

void UHerdMemberComponent::SetState(EHerdMemberState NewState)
{
    if (!GetOwner() || !GetOwner()->HasAuthority())
    {
        ServerSetState(NewState);
        return;
    }

    if (CurrentState == NewState)
    {
        return;
    }

    CurrentState = NewState;
    OnStateChanged.Broadcast(this, CurrentState);
}

void UHerdMemberComponent::ServerSetState_Implementation(EHerdMemberState NewState)
{
    SetState(NewState);
}

void UHerdMemberComponent::ApplySurvivalDelta(float HealthDelta, float StaminaDelta, float HungerDelta, float ThirstDelta)
{
    if (!GetOwner() || !GetOwner()->HasAuthority())
    {
        return;
    }

    Health += HealthDelta;
    Stamina += StaminaDelta;
    Hunger += HungerDelta;
    Thirst += ThirstDelta;
    ClampVitals();

    if (Health <= 0.0f)
    {
        SetState(EHerdMemberState::Dead);
    }
}

void UHerdMemberComponent::RememberThreat(AActor* ThreatActor, EHerdThreatType ThreatType, float ThreatLevel, float ExpireAfterSeconds)
{
    if (!ThreatActor || !GetOwner() || !GetOwner()->HasAuthority())
    {
        return;
    }

    for (FHerdThreatMemory& Memory : ThreatMemory)
    {
        if (Memory.ThreatActor == ThreatActor)
        {
            Memory.ThreatType = ThreatType;
            Memory.LastKnownLocation = ThreatActor->GetActorLocation();
            Memory.ThreatLevel = ThreatLevel;
            Memory.LastSeenTime = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.0f;
            Memory.ExpireAfterSeconds = ExpireAfterSeconds;
            return;
        }
    }

    FHerdThreatMemory NewMemory;
    NewMemory.ThreatActor = ThreatActor;
    NewMemory.ThreatType = ThreatType;
    NewMemory.LastKnownLocation = ThreatActor->GetActorLocation();
    NewMemory.ThreatLevel = ThreatLevel;
    NewMemory.LastSeenTime = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.0f;
    NewMemory.ExpireAfterSeconds = ExpireAfterSeconds;
    ThreatMemory.Add(NewMemory);
}

void UHerdMemberComponent::ClearExpiredThreats()
{
    const float Now = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.0f;
    ThreatMemory.RemoveAll([Now](const FHerdThreatMemory& Memory)
    {
        return !IsValid(Memory.ThreatActor) || Now - Memory.LastSeenTime > Memory.ExpireAfterSeconds;
    });
}

bool UHerdMemberComponent::ShouldFleeFromThreat(const FHerdScanItem& ThreatItem, FHerdThreatResponse& OutResponse) const
{
    if (!SpeciesData || ThreatItem.ThreatLevel <= 0.0f)
    {
        return false;
    }

    SpeciesData->FindThreatResponse(ThreatItem.ThreatType, OutResponse);
    if (!OutResponse.bCanFlee)
    {
        return false;
    }

    const float HealthRatio = GetHealthRatio();
    const float FleeHealth = NormalizeHealthThreshold(OutResponse.FleeThresholdHealth);
    const bool bHealthLow = HealthRatio <= FleeHealth || HealthRatio <= NormalizeHealthThreshold(FleeThresholdHealth);
    const bool bFearWins = Fear + ThreatItem.ThreatLevel * 0.1f >= Aggression + OutResponse.AggressionLevel;
    const bool bAlwaysFleeFromPlayer = ThreatItem.ThreatType == EHerdThreatType::Player && SpeciesData->bFleeFromPlayers;

    return bHealthLow || bFearWins || bAlwaysFleeFromPlayer;
}

bool UHerdMemberComponent::ShouldAttackThreat(const FHerdScanItem& ThreatItem, FHerdThreatResponse& OutResponse) const
{
    if (!SpeciesData || ThreatItem.ThreatLevel <= 0.0f)
    {
        return false;
    }

    SpeciesData->FindThreatResponse(ThreatItem.ThreatType, OutResponse);
    if (!OutResponse.bCanAttack && !OutResponse.bCanChase)
    {
        return false;
    }

    if (ThreatItem.ThreatType == EHerdThreatType::Player && !SpeciesData->bAttackPlayers)
    {
        return false;
    }

    if (ThreatItem.ThreatType == EHerdThreatType::Predator && !SpeciesData->bAttackPredators && OutResponse.MinGroupSizeToFight <= 0)
    {
        return false;
    }

    const float HealthRatio = GetHealthRatio();
    const float RequiredHealth = NormalizeHealthThreshold(OutResponse.HealthAttackThreshold);
    const float CombinedAggression = FMath::Max(Aggression, AggressionLevelOverride) + OutResponse.AggressionLevel;
    return HealthRatio >= RequiredHealth && CombinedAggression > Fear;
}

float UHerdMemberComponent::GetHealthRatio() const
{
    return SpeciesData && SpeciesData->MaxHealth > 0.0f ? Health / SpeciesData->MaxHealth : 0.0f;
}

float UHerdMemberComponent::GetHungerRatio() const
{
    return SpeciesData && SpeciesData->MaxHunger > 0.0f ? Hunger / SpeciesData->MaxHunger : 0.0f;
}

float UHerdMemberComponent::GetThirstRatio() const
{
    return SpeciesData && SpeciesData->MaxThirst > 0.0f ? Thirst / SpeciesData->MaxThirst : 0.0f;
}

bool UHerdMemberComponent::IsDead() const
{
    return CurrentState == EHerdMemberState::Dead || Health <= 0.0f;
}

void UHerdMemberComponent::OnRep_State(EHerdMemberState PreviousState)
{
    if (PreviousState != CurrentState)
    {
        OnStateChanged.Broadcast(this, CurrentState);
    }
}

void UHerdMemberComponent::OnRep_SpeciesData()
{
    if (SpeciesData)
    {
        Fear = SpeciesData->BaseFear;
        Aggression = FMath::Max(Aggression, SpeciesData->BaseAggression);
    }
}

void UHerdMemberComponent::ApplyNeeds(float DeltaSeconds)
{
    if (!SpeciesData)
    {
        return;
    }

    Hunger += SpeciesData->HungerRatePerMinute * (DeltaSeconds / 60.0f);
    Thirst += SpeciesData->ThirstRatePerMinute * (DeltaSeconds / 60.0f);

    if (GetHungerRatio() >= SpeciesData->CriticalNeedsThreshold)
    {
        Health -= SpeciesData->StarvationDamagePerSecond * DeltaSeconds;
    }

    if (GetThirstRatio() >= SpeciesData->CriticalNeedsThreshold)
    {
        Health -= SpeciesData->DehydrationDamagePerSecond * DeltaSeconds;
    }

    if (Health > 0.0f && Health < SpeciesData->MaxHealth)
    {
        Health += SpeciesData->HealthRegenPerSecond * DeltaSeconds;
    }

    if (Health <= 0.0f)
    {
        SetState(EHerdMemberState::Dead);
    }
}

void UHerdMemberComponent::ApplyStateStamina(float DeltaSeconds)
{
    if (!SpeciesData)
    {
        return;
    }

    switch (CurrentState)
    {
    case EHerdMemberState::Run:
    case EHerdMemberState::Chase:
    case EHerdMemberState::Stampede:
        Stamina -= SpeciesData->StaminaDrainRunPerSecond * DeltaSeconds;
        break;
    case EHerdMemberState::Flee:
    case EHerdMemberState::Scatter:
        Stamina -= SpeciesData->StaminaDrainFleePerSecond * DeltaSeconds;
        break;
    default:
        Stamina += SpeciesData->StaminaRecoveryPerSecond * DeltaSeconds;
        break;
    }
}

void UHerdMemberComponent::UpdateLifeStage(float DeltaSeconds)
{
    if (!SpeciesData)
    {
        return;
    }

    AgeDays += (DeltaSeconds / 86400.0f) * SpeciesData->GrowthRateMultiplier;
    LifeStage = SpeciesData->GetLifeStageForAge(AgeDays);

    if (bPregnant)
    {
        PregnancyElapsedDays += DeltaSeconds / 86400.0f;
    }
}

void UHerdMemberComponent::AutoRegisterWithManager()
{
    UWorld* World = GetWorld();
    if (!World || !GetOwner())
    {
        return;
    }

    for (TActorIterator<AHerdManager_Generic> It(World); It; ++It)
    {
        AHerdManager_Generic* Manager = *It;
        if (Manager && FVector::DistSquared(Manager->GetActorLocation(), GetOwner()->GetActorLocation()) <= FMath::Square(AutoRegisterRadius))
        {
            Manager->RegisterMember(this, SpeciesData);
            return;
        }
    }
}

void UHerdMemberComponent::ClampVitals()
{
    if (!SpeciesData)
    {
        Health = FMath::Max(0.0f, Health);
        Stamina = FMath::Max(0.0f, Stamina);
        Hunger = FMath::Max(0.0f, Hunger);
        Thirst = FMath::Max(0.0f, Thirst);
        return;
    }

    Health = FMath::Clamp(Health, 0.0f, SpeciesData->MaxHealth);
    Stamina = FMath::Clamp(Stamina, 0.0f, SpeciesData->MaxStamina);
    Hunger = FMath::Clamp(Hunger, 0.0f, SpeciesData->MaxHunger);
    Thirst = FMath::Clamp(Thirst, 0.0f, SpeciesData->MaxThirst);
    Panic = FMath::Clamp(Panic, 0.0f, 10.0f);
    Fear = FMath::Clamp(Fear, 0.0f, 1.0f);
    Aggression = FMath::Clamp(Aggression, 0.0f, 1.0f);
}

FHerdScanItem UHerdMemberComponent::FindHighestThreat(const FHerdScanDataset& ScanDataset, bool& bOutFoundThreat) const
{
    bOutFoundThreat = false;
    FHerdScanItem BestThreat;
    float BestScore = 0.0f;

    for (const FHerdScanItem& Item : ScanDataset.NearbyThreats)
    {
        const float DistanceWeight = Item.Distance > 0.0f ? 1.0f / FMath::Max(1.0f, Item.Distance * 0.001f) : 1.0f;
        const float Score = Item.ThreatLevel * DistanceWeight;
        if (Score > BestScore)
        {
            BestScore = Score;
            BestThreat = Item;
            bOutFoundThreat = true;
        }
    }

    return BestThreat;
}

float UHerdMemberComponent::NormalizeHealthThreshold(float Threshold) const
{
    if (!SpeciesData || SpeciesData->MaxHealth <= 0.0f)
    {
        return Threshold <= 1.0f ? Threshold : 0.0f;
    }

    return Threshold <= 1.0f ? Threshold : Threshold / SpeciesData->MaxHealth;
}
