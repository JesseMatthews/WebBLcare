#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HerdTypes.h"
#include "HerdExampleLevelSpawner.generated.h"

class AHerdManager_Generic;
class UDA_SpeciesData_Base;
class UHerdMemberComponent;

UCLASS(BlueprintType, Blueprintable)
class HERDSYSTEM_API AHerdExampleLevelSpawner : public AActor
{
    GENERATED_BODY()

public:
    AHerdExampleLevelSpawner();

    virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Example")
    bool bSpawnOnBeginPlay = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Example")
    bool bAssignFirstMemberAsLeader = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Example")
    TSubclassOf<AHerdManager_Generic> HerdManagerClass = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Example")
    TArray<FHerdSpawnGroup> SpawnGroups;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Example")
    TArray<FHerdMigrationRoute> ExampleMigrationRoutes;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Example", meta = (ClampMin = "0.0"))
    float ExampleMaxSpreadRadius = 2500.0f;

    UPROPERTY(BlueprintReadOnly, Replicated, Category = "Example")
    TObjectPtr<AHerdManager_Generic> SpawnedManager = nullptr;

    UFUNCTION(BlueprintCallable, Category = "Example")
    AHerdManager_Generic* SpawnExampleHerd();

protected:
    UHerdMemberComponent* EnsureHerdMemberComponent(AActor* Actor) const;
    void ApplySpeciesVisuals(AActor* Actor, UDA_SpeciesData_Base* SpeciesData) const;
    EHerdRole PickRole(const FHerdSpawnGroup& Group, bool bForceLeader) const;
};
