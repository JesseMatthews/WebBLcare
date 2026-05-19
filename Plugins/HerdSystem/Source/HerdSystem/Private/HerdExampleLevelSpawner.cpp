#include "HerdExampleLevelSpawner.h"

#include "Components/SkeletalMeshComponent.h"
#include "HerdManager_Generic.h"
#include "HerdMemberComponent.h"
#include "Net/UnrealNetwork.h"
#include "PhysicsEngine/PhysicsAsset.h"
#include "SpeciesData_Base.h"

AHerdExampleLevelSpawner::AHerdExampleLevelSpawner()
{
    PrimaryActorTick.bCanEverTick = false;
    bReplicates = true;
}

void AHerdExampleLevelSpawner::BeginPlay()
{
    Super::BeginPlay();

    if (HasAuthority() && bSpawnOnBeginPlay)
    {
        SpawnExampleHerd();
    }
}

void AHerdExampleLevelSpawner::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AHerdExampleLevelSpawner, SpawnedManager);
}

AHerdManager_Generic* AHerdExampleLevelSpawner::SpawnExampleHerd()
{
    UWorld* World = GetWorld();
    if (!World || !HasAuthority())
    {
        return SpawnedManager;
    }

    if (!SpawnedManager)
    {
        const TSubclassOf<AHerdManager_Generic> ManagerClass = HerdManagerClass ? HerdManagerClass : AHerdManager_Generic::StaticClass();
        FActorSpawnParameters ManagerParams;
        ManagerParams.Owner = this;
        SpawnedManager = World->SpawnActor<AHerdManager_Generic>(ManagerClass, GetActorTransform(), ManagerParams);
    }

    if (!SpawnedManager)
    {
        return nullptr;
    }

    SpawnedManager->MaxSpreadRadius = ExampleMaxSpreadRadius;
    SpawnedManager->MigrationRoutes = ExampleMigrationRoutes;

    bool bLeaderAssigned = false;
    for (const FHerdSpawnGroup& Group : SpawnGroups)
    {
        if (!Group.SpeciesData)
        {
            continue;
        }

        SpawnedManager->AssignedSpecies.AddUnique(Group.SpeciesData);
        const TSubclassOf<AActor> AnimalClass = Group.AnimalActorClass ? Group.AnimalActorClass : AActor::StaticClass();

        for (int32 Index = 0; Index < Group.Count; ++Index)
        {
            const FVector RandomOffset = FMath::VRand() * FMath::FRandRange(0.0f, Group.SpawnRadius);
            const FVector SpawnLocation = GetActorLocation() + FVector(RandomOffset.X, RandomOffset.Y, 0.0f);
            FActorSpawnParameters Params;
            Params.Owner = SpawnedManager;
            AActor* Animal = World->SpawnActor<AActor>(AnimalClass, SpawnLocation, GetActorRotation(), Params);
            if (!Animal)
            {
                continue;
            }

            Animal->SetReplicates(true);
            ApplySpeciesVisuals(Animal, Group.SpeciesData);

            UHerdMemberComponent* Member = EnsureHerdMemberComponent(Animal);
            if (!Member)
            {
                continue;
            }

            const bool bForceLeader = bAssignFirstMemberAsLeader && !bLeaderAssigned;
            Member->HerdRole = PickRole(Group, bForceLeader);
            bLeaderAssigned = bLeaderAssigned || Member->HerdRole == EHerdRole::Leader;
            Member->InitializeFromSpecies(Group.SpeciesData, SpawnedManager);
            SpawnedManager->RegisterMember(Member, Group.SpeciesData);
        }
    }

    if (ExampleMigrationRoutes.Num() > 0)
    {
        SpawnedManager->StartMigrationRoute(0);
    }

    SpawnedManager->RebuildSpeciesRuntime();
    SpawnedManager->ForceRescan();
    return SpawnedManager;
}

UHerdMemberComponent* AHerdExampleLevelSpawner::EnsureHerdMemberComponent(AActor* Actor) const
{
    if (!Actor)
    {
        return nullptr;
    }

    if (UHerdMemberComponent* Existing = Actor->FindComponentByClass<UHerdMemberComponent>())
    {
        return Existing;
    }

    UHerdMemberComponent* NewComponent = NewObject<UHerdMemberComponent>(Actor, UHerdMemberComponent::StaticClass(), TEXT("HerdMemberComponent"));
    if (!NewComponent)
    {
        return nullptr;
    }

    Actor->AddInstanceComponent(NewComponent);
    NewComponent->SetIsReplicated(true);
    NewComponent->RegisterComponent();
    return NewComponent;
}

void AHerdExampleLevelSpawner::ApplySpeciesVisuals(AActor* Actor, UDA_SpeciesData_Base* SpeciesData) const
{
    if (!Actor || !SpeciesData)
    {
        return;
    }

    if (USkeletalMeshComponent* MeshComponent = Actor->FindComponentByClass<USkeletalMeshComponent>())
    {
        if (SpeciesData->SkeletalMesh)
        {
            MeshComponent->SetSkeletalMesh(SpeciesData->SkeletalMesh);
        }

        if (SpeciesData->PhysicsAsset)
        {
            MeshComponent->SetPhysicsAsset(SpeciesData->PhysicsAsset);
        }

        if (SpeciesData->AnimBlueprintClass)
        {
            MeshComponent->SetAnimInstanceClass(SpeciesData->AnimBlueprintClass);
        }

        for (int32 Index = 0; Index < SpeciesData->MaterialOverrides.Num(); ++Index)
        {
            if (SpeciesData->MaterialOverrides[Index])
            {
                MeshComponent->SetMaterial(Index, SpeciesData->MaterialOverrides[Index]);
            }
        }

        const float Scale = FMath::FRandRange(SpeciesData->MinVisualScale, SpeciesData->MaxVisualScale);
        MeshComponent->SetRelativeScale3D(FVector(Scale));
        MeshComponent->SetRelativeLocation(SpeciesData->MeshRelativeOffset);
    }
}

EHerdRole AHerdExampleLevelSpawner::PickRole(const FHerdSpawnGroup& Group, bool bForceLeader) const
{
    if (bForceLeader)
    {
        return EHerdRole::Leader;
    }

    if (Group.RoleWeights.IsEmpty())
    {
        return Group.DefaultRole;
    }

    float TotalWeight = 0.0f;
    for (const TPair<EHerdRole, float>& Pair : Group.RoleWeights)
    {
        TotalWeight += FMath::Max(0.0f, Pair.Value);
    }

    if (TotalWeight <= 0.0f)
    {
        return Group.DefaultRole;
    }

    float Roll = FMath::FRandRange(0.0f, TotalWeight);
    for (const TPair<EHerdRole, float>& Pair : Group.RoleWeights)
    {
        Roll -= FMath::Max(0.0f, Pair.Value);
        if (Roll <= 0.0f)
        {
            return Pair.Key;
        }
    }

    return Group.DefaultRole;
}
