#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HerdTypes.h"
#include "HerdManager_Generic.generated.h"

class UDA_SpeciesData_Base;
class UHerdMemberComponent;
class UHerdScanDatasetComponent;
class AHerdManager_Generic;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHerdCompositionChanged, AHerdManager_Generic*, HerdManager);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHerdStateChanged, AHerdManager_Generic*, HerdManager, EHerdMemberState, NewState);

UCLASS(BlueprintType, Blueprintable)
class HERDSYSTEM_API AHerdManager_Generic : public AActor
{
    GENERATED_BODY()

public:
    AHerdManager_Generic();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UHerdScanDatasetComponent> ScanDatasetComponent = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_AssignedSpecies, Category = "Herd")
    TArray<TObjectPtr<UDA_SpeciesData_Base>> AssignedSpecies;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Herd")
    int32 CurrentHerdSize = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Herd", meta = (ClampMin = "0.0"))
    float MaxSpreadRadius = 2500.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Herd")
    TArray<FHerdMigrationRoute> MigrationRoutes;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Herd", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float HerdLevelHungerThreshold = 0.65f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Herd", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float HerdLevelThirstThreshold = 0.7f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Herd", meta = (ClampMin = "0.0"))
    float DecisionInterval = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Herd")
    bool bAutoRegisterNearbyMembers = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Herd", meta = (ClampMin = "0.0"))
    float AutoRegisterRadius = 5000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Herd")
    bool bEnableDebugDraw = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Herd", meta = (ClampMin = "0.0"))
    float DebugDrawDuration = 0.1f;

    UPROPERTY(BlueprintReadOnly, Replicated, Category = "Herd")
    TArray<TObjectPtr<UHerdMemberComponent>> RegisteredMembers;

    UPROPERTY(BlueprintReadOnly, Replicated, Category = "Herd")
    TArray<FHerdRuntimeSpeciesEntry> SpeciesRuntime;

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HerdState, Category = "Herd")
    EHerdMemberState HerdState = EHerdMemberState::Idle;

    UPROPERTY(BlueprintReadOnly, Replicated, Category = "Migration")
    int32 CurrentMigrationRouteIndex = INDEX_NONE;

    UPROPERTY(BlueprintReadOnly, Replicated, Category = "Migration")
    int32 CurrentMigrationWaypointIndex = 0;

    UPROPERTY(BlueprintAssignable, Category = "Herd")
    FOnHerdCompositionChanged OnHerdCompositionChanged;

    UPROPERTY(BlueprintAssignable, Category = "Herd")
    FOnHerdStateChanged OnHerdStateChanged;

    UFUNCTION(BlueprintCallable, Category = "Herd")
    void RegisterMember(UHerdMemberComponent* Member, UDA_SpeciesData_Base* SpeciesOverride);

    UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Herd")
    void ServerRegisterMember(UHerdMemberComponent* Member, UDA_SpeciesData_Base* SpeciesOverride);

    UFUNCTION(BlueprintCallable, Category = "Herd")
    void UnregisterMember(UHerdMemberComponent* Member);

    UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Herd")
    void ServerUnregisterMember(UHerdMemberComponent* Member);

    UFUNCTION(BlueprintCallable, Category = "Herd")
    FHerdScanDataset ForceRescan();

    UFUNCTION(BlueprintCallable, Category = "Herd")
    void RebuildSpeciesRuntime();

    UFUNCTION(BlueprintCallable, Category = "Migration")
    bool StartMigrationRoute(int32 RouteIndex);

    UFUNCTION(BlueprintPure, Category = "Herd")
    FVector GetHerdCenter() const;

    UFUNCTION(BlueprintPure, Category = "Herd")
    UDA_SpeciesData_Base* GetDominantSpecies() const;

    UFUNCTION(BlueprintPure, Category = "Herd")
    TArray<UHerdMemberComponent*> GetMembersBySpecies(UDA_SpeciesData_Base* Species) const;

    UFUNCTION(BlueprintPure, Category = "Herd")
    UHerdMemberComponent* GetLeader() const;

    UFUNCTION(BlueprintPure, Category = "Herd")
    FHerdScanDataset GetCurrentScanDataset() const;

    UFUNCTION(BlueprintPure, Category = "Interactions")
    EHerdInteractionDisposition GetInteractionBetweenSpecies(const UDA_SpeciesData_Base* ObserverSpecies, const UDA_SpeciesData_Base* OtherSpecies) const;

    UFUNCTION(BlueprintPure, Category = "Interactions")
    EHerdThreatType GetThreatTypeBetweenSpecies(const UDA_SpeciesData_Base* ObserverSpecies, const UDA_SpeciesData_Base* OtherSpecies, EHerdThreatType FallbackThreatType) const;

    UFUNCTION(BlueprintPure, Category = "Interactions")
    float ComputeThreatLevelForScanItem(const FHerdScanItem& Item) const;

protected:
    UFUNCTION()
    void OnRep_AssignedSpecies();

    UFUNCTION()
    void OnRep_HerdState(EHerdMemberState PreviousState);

    void AutoRegisterNearbyMembers();
    void UpdateHerd(float DeltaSeconds);
    EHerdMemberState EvaluateHerdState(const FHerdScanDataset& ScanDataset) const;
    void ApplyHerdStateToMembers(const FHerdScanDataset& ScanDataset, EHerdMemberState NewHerdState);
    void SetHerdState(EHerdMemberState NewState);
    void AdvanceMigrationIfNeeded();
    FVector GetCurrentMigrationTarget() const;
    FVector GetThreatAvoidanceTarget(const FHerdScanDataset& ScanDataset) const;
    FVector GetDefensiveCircleTarget(int32 MemberIndex) const;
    bool HasAnySpeciesThatCanStampede() const;
    bool HasAnySpeciesWithDefensiveCircle() const;
    bool HasAnySpeciesWithMigration() const;
    float GetMaxDistanceFromCenter(FVector Center) const;
    void DrawDebugHerd(const FHerdScanDataset& ScanDataset) const;
    bool IsPredatoryDiet(const UDA_SpeciesData_Base* Species) const;

private:
    float DecisionTimer = 0.0f;
};
