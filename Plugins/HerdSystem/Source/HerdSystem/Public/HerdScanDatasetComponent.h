#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/EngineTypes.h"
#include "HerdTypes.h"
#include "HerdScanDatasetComponent.generated.h"

class AHerdManager_Generic;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHerdScanDatasetUpdated, FHerdScanDataset, Dataset);

UCLASS(ClassGroup = (AI), BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent))
class HERDSYSTEM_API UHerdScanDatasetComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UHerdScanDatasetComponent();

    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scan", meta = (ClampMin = "0.0"))
    float ScanRadius = 3000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scan", meta = (ClampMin = "0.0"))
    float ScanInterval = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scan")
    bool bAutoScan = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scan")
    bool bRequireLineOfSight = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scan")
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesToScan;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scan")
    TMap<FName, EHerdResourceType> ActorTagResourceMap;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scan")
    TMap<FName, EHerdThreatType> ActorTagThreatMap;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scan")
    TMap<FName, float> ActorTagResourceValues;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scan")
    TMap<FName, float> ActorTagThreatLevels;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scan")
    TArray<TObjectPtr<AActor>> ExplicitIgnoredActors;

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ScanDataset, Category = "Scan")
    FHerdScanDataset ReplicatedScanDataset;

    UPROPERTY(BlueprintAssignable, Category = "Scan")
    FOnHerdScanDatasetUpdated OnScanDatasetUpdated;

    UFUNCTION(BlueprintCallable, Category = "Scan")
    FHerdScanDataset Scan(AHerdManager_Generic* HerdManager);

    UFUNCTION(BlueprintPure, Category = "Scan")
    FHerdScanDataset GetScanDataset() const;

    UFUNCTION(BlueprintPure, Category = "Scan")
    bool GetHighestThreat(FHerdScanItem& OutThreat) const;

    UFUNCTION(BlueprintPure, Category = "Scan")
    bool FindNearestResource(EHerdResourceType ResourceType, FHerdScanItem& OutResource) const;

    UFUNCTION(BlueprintPure, Category = "Scan")
    bool HasLineOfSightToActor(AActor* Actor) const;

protected:
    UFUNCTION()
    void OnRep_ScanDataset();

    FHerdScanItem BuildScanItem(AActor* Actor, AHerdManager_Generic* HerdManager, const FVector& Origin) const;
    void ClassifyScanItem(FHerdScanItem& Item, AHerdManager_Generic* HerdManager) const;
    void AddItemToDataset(FHerdScanDataset& Dataset, const FHerdScanItem& Item) const;
    bool IsIgnoredActor(AActor* Actor) const;
    FGameplayTagContainer GetOwnedGameplayTags(AActor* Actor) const;

private:
    float ScanTimer = 0.0f;
};
