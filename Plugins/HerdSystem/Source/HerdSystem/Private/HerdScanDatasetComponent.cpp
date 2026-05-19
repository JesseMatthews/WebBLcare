#include "HerdScanDatasetComponent.h"

#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "GameplayTagAssetInterface.h"
#include "HerdManager_Generic.h"
#include "HerdMemberComponent.h"
#include "Net/UnrealNetwork.h"
#include "SpeciesData_Base.h"
#include "UObject/ObjectKey.h"

UHerdScanDatasetComponent::UHerdScanDatasetComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.bStartWithTickEnabled = true;
    SetIsReplicatedByDefault(true);

    ObjectTypesToScan.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
    ObjectTypesToScan.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));

    ActorTagResourceMap.Add(TEXT("Herd.Food"), EHerdResourceType::Food);
    ActorTagResourceMap.Add(TEXT("Herd.Water"), EHerdResourceType::Water);
    ActorTagResourceMap.Add(TEXT("Herd.Shelter"), EHerdResourceType::Shelter);
    ActorTagResourceMap.Add(TEXT("Herd.Hazard"), EHerdResourceType::Hazard);

    ActorTagThreatMap.Add(TEXT("Herd.Threat"), EHerdThreatType::Unknown);
    ActorTagThreatMap.Add(TEXT("Herd.Predator"), EHerdThreatType::Predator);
    ActorTagThreatMap.Add(TEXT("Herd.Player"), EHerdThreatType::Player);
    ActorTagThreatMap.Add(TEXT("Herd.Fire"), EHerdThreatType::Fire);
    ActorTagThreatMap.Add(TEXT("Herd.Hazard"), EHerdThreatType::EnvironmentalHazard);
    ActorTagThreatMap.Add(TEXT("Herd.EnvironmentalHazard"), EHerdThreatType::EnvironmentalHazard);

    ActorTagResourceValues.Add(TEXT("Herd.Food"), 35.0f);
    ActorTagResourceValues.Add(TEXT("Herd.Water"), 40.0f);
    ActorTagResourceValues.Add(TEXT("Herd.Shelter"), 10.0f);

    ActorTagThreatLevels.Add(TEXT("Herd.Threat"), 4.0f);
    ActorTagThreatLevels.Add(TEXT("Herd.Predator"), 6.0f);
    ActorTagThreatLevels.Add(TEXT("Herd.Player"), 3.0f);
    ActorTagThreatLevels.Add(TEXT("Herd.Fire"), 8.0f);
    ActorTagThreatLevels.Add(TEXT("Herd.Hazard"), 6.0f);
    ActorTagThreatLevels.Add(TEXT("Herd.EnvironmentalHazard"), 6.5f);
}

void UHerdScanDatasetComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UHerdScanDatasetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    AActor* Owner = GetOwner();
    if (!Owner || !Owner->HasAuthority() || !bAutoScan)
    {
        return;
    }

    ScanTimer += DeltaTime;
    if (ScanTimer >= ScanInterval)
    {
        ScanTimer = 0.0f;
        Scan(Cast<AHerdManager_Generic>(Owner));
    }
}

void UHerdScanDatasetComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(UHerdScanDatasetComponent, ReplicatedScanDataset);
}

FHerdScanDataset UHerdScanDatasetComponent::Scan(AHerdManager_Generic* HerdManager)
{
    AActor* Owner = GetOwner();
    UWorld* World = GetWorld();
    if (!Owner || !World)
    {
        return ReplicatedScanDataset;
    }

    FHerdScanDataset NewDataset;
    NewDataset.ScanOrigin = Owner->GetActorLocation();
    NewDataset.ScanRadius = ScanRadius;
    NewDataset.ScanTime = World->GetTimeSeconds();

    FCollisionObjectQueryParams ObjectParams;
    for (const TEnumAsByte<EObjectTypeQuery>& ObjectType : ObjectTypesToScan)
    {
        ObjectParams.AddObjectTypesToQuery(UEngineTypes::ConvertToCollisionChannel(ObjectType));
    }

    if (ObjectTypesToScan.IsEmpty())
    {
        ObjectParams.AddObjectTypesToQuery(ECC_Pawn);
        ObjectParams.AddObjectTypesToQuery(ECC_WorldDynamic);
    }

    FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(HerdScanDataset), false, Owner);
    for (AActor* IgnoredActor : ExplicitIgnoredActors)
    {
        if (IgnoredActor)
        {
            QueryParams.AddIgnoredActor(IgnoredActor);
        }
    }

    TArray<FOverlapResult> Results;
    const FCollisionShape ScanShape = FCollisionShape::MakeSphere(ScanRadius);
    World->OverlapMultiByObjectType(Results, NewDataset.ScanOrigin, FQuat::Identity, ObjectParams, ScanShape, QueryParams);
    NewDataset.TotalActorsScanned = Results.Num();

    TSet<TObjectKey<AActor>> SeenActors;
    for (const FOverlapResult& Result : Results)
    {
        AActor* Actor = Result.GetActor();
        if (!Actor || Actor == Owner || IsIgnoredActor(Actor))
        {
            continue;
        }

        const TObjectKey<AActor> ActorKey(Actor);
        if (SeenActors.Contains(ActorKey))
        {
            continue;
        }

        SeenActors.Add(ActorKey);
        FHerdScanItem Item = BuildScanItem(Actor, HerdManager, NewDataset.ScanOrigin);
        if (bRequireLineOfSight && !Item.bLineOfSight)
        {
            continue;
        }
        AddItemToDataset(NewDataset, Item);
    }

    ReplicatedScanDataset = MoveTemp(NewDataset);
    OnScanDatasetUpdated.Broadcast(ReplicatedScanDataset);
    return ReplicatedScanDataset;
}

FHerdScanDataset UHerdScanDatasetComponent::GetScanDataset() const
{
    return ReplicatedScanDataset;
}

bool UHerdScanDatasetComponent::GetHighestThreat(FHerdScanItem& OutThreat) const
{
    float BestScore = 0.0f;
    bool bFound = false;
    for (const FHerdScanItem& Threat : ReplicatedScanDataset.NearbyThreats)
    {
        const float DistanceWeight = Threat.Distance > 0.0f ? 1.0f / FMath::Max(1.0f, Threat.Distance * 0.001f) : 1.0f;
        const float Score = Threat.ThreatLevel * DistanceWeight;
        if (Score > BestScore)
        {
            BestScore = Score;
            OutThreat = Threat;
            bFound = true;
        }
    }
    return bFound;
}

bool UHerdScanDatasetComponent::FindNearestResource(EHerdResourceType ResourceType, FHerdScanItem& OutResource) const
{
    const TArray<FHerdScanItem>* SourceArray = nullptr;
    if (ResourceType == EHerdResourceType::Food)
    {
        SourceArray = &ReplicatedScanDataset.NearbyFood;
    }
    else if (ResourceType == EHerdResourceType::Water)
    {
        SourceArray = &ReplicatedScanDataset.NearbyWater;
    }

    if (!SourceArray)
    {
        return false;
    }

    float BestDistance = TNumericLimits<float>::Max();
    bool bFound = false;
    for (const FHerdScanItem& Item : *SourceArray)
    {
        if (Item.Distance < BestDistance)
        {
            BestDistance = Item.Distance;
            OutResource = Item;
            bFound = true;
        }
    }
    return bFound;
}

bool UHerdScanDatasetComponent::HasLineOfSightToActor(AActor* Actor) const
{
    if (!Actor || !GetOwner() || !GetWorld())
    {
        return false;
    }

    FHitResult Hit;
    FCollisionQueryParams Params(SCENE_QUERY_STAT(HerdScanLineOfSight), false, GetOwner());
    Params.AddIgnoredActor(Actor);
    const FVector Start = GetOwner()->GetActorLocation();
    const FVector End = Actor->GetActorLocation();
    const bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);
    return !bHit;
}

void UHerdScanDatasetComponent::OnRep_ScanDataset()
{
    OnScanDatasetUpdated.Broadcast(ReplicatedScanDataset);
}

FHerdScanItem UHerdScanDatasetComponent::BuildScanItem(AActor* Actor, AHerdManager_Generic* HerdManager, const FVector& Origin) const
{
    FHerdScanItem Item;
    Item.Actor = Actor;
    Item.ActorClass = Actor ? Actor->GetClass() : nullptr;
    Item.Location = Actor ? Actor->GetActorLocation() : FVector::ZeroVector;
    Item.DirectionFromScanner = (Item.Location - Origin).GetSafeNormal();
    Item.Distance = FVector::Dist(Item.Location, Origin);
    Item.LastSeenTime = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.0f;
    Item.bLineOfSight = HasLineOfSightToActor(Actor);
    Item.Tags = GetOwnedGameplayTags(Actor);

    if (Actor)
    {
        if (UHerdMemberComponent* HerdMember = Actor->FindComponentByClass<UHerdMemberComponent>())
        {
            Item.HerdMember = HerdMember;
            Item.SpeciesData = HerdMember->SpeciesData;
        }
    }

    ClassifyScanItem(Item, HerdManager);
    return Item;
}

void UHerdScanDatasetComponent::ClassifyScanItem(FHerdScanItem& Item, AHerdManager_Generic* HerdManager) const
{
    AActor* Actor = Item.Actor;
    if (!Actor)
    {
        return;
    }

    for (const TPair<FName, EHerdResourceType>& Pair : ActorTagResourceMap)
    {
        if (Actor->ActorHasTag(Pair.Key))
        {
            Item.ResourceType = Pair.Value;
            if (const float* Value = ActorTagResourceValues.Find(Pair.Key))
            {
                Item.ResourceValue = *Value;
            }
        }
    }

    for (const TPair<FName, EHerdThreatType>& Pair : ActorTagThreatMap)
    {
        if (Actor->ActorHasTag(Pair.Key))
        {
            Item.ThreatType = Pair.Value;
            if (const float* Level = ActorTagThreatLevels.Find(Pair.Key))
            {
                Item.ThreatLevel = *Level;
            }
        }
    }

    if (const APawn* Pawn = Cast<APawn>(Actor))
    {
        if (Pawn->IsPlayerControlled())
        {
            Item.ThreatType = EHerdThreatType::Player;
            Item.ThreatLevel = FMath::Max(Item.ThreatLevel, 3.0f);
        }
    }

    if (HerdManager && Item.SpeciesData)
    {
        Item.Relationship = HerdManager->GetInteractionBetweenSpecies(HerdManager->GetDominantSpecies(), Item.SpeciesData);
        Item.ThreatType = HerdManager->GetThreatTypeBetweenSpecies(HerdManager->GetDominantSpecies(), Item.SpeciesData, Item.ThreatType);
        Item.ThreatLevel = FMath::Max(Item.ThreatLevel, HerdManager->ComputeThreatLevelForScanItem(Item));
    }
}

void UHerdScanDatasetComponent::AddItemToDataset(FHerdScanDataset& Dataset, const FHerdScanItem& Item) const
{
    Dataset.AllItems.Add(Item);

    if (Item.ResourceType == EHerdResourceType::Food)
    {
        Dataset.NearbyFood.Add(Item);
    }
    else if (Item.ResourceType == EHerdResourceType::Water)
    {
        Dataset.NearbyWater.Add(Item);
    }

    if (Item.ThreatType != EHerdThreatType::None && Item.ThreatLevel > 0.0f)
    {
        Dataset.NearbyThreats.Add(Item);
    }

    if (Item.SpeciesData)
    {
        Dataset.NearbySpecies.Add(Item);
    }

    if (IsValid(Item.HerdMember))
    {
        Dataset.NearbyHerdMembers.Add(Item);
    }
}

bool UHerdScanDatasetComponent::IsIgnoredActor(AActor* Actor) const
{
    return !Actor || ExplicitIgnoredActors.Contains(Actor);
}

FGameplayTagContainer UHerdScanDatasetComponent::GetOwnedGameplayTags(AActor* Actor) const
{
    FGameplayTagContainer Tags;
    if (!Actor)
    {
        return Tags;
    }

    if (IGameplayTagAssetInterface* TagInterface = Cast<IGameplayTagAssetInterface>(Actor))
    {
        TagInterface->GetOwnedGameplayTags(Tags);
    }

    return Tags;
}
