#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HerdTypes.h"
#include "HerdMemberComponent.generated.h"

class AHerdManager_Generic;
class UDA_SpeciesData_Base;
class UHerdMemberComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHerdMemberStateChanged, UHerdMemberComponent*, Member, EHerdMemberState, NewState);

UCLASS(ClassGroup = (AI), BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent))
class HERDSYSTEM_API UHerdMemberComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UHerdMemberComponent();

    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_SpeciesData, Category = "Herd")
    TObjectPtr<UDA_SpeciesData_Base> SpeciesData = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Herd")
    TObjectPtr<AHerdManager_Generic> HerdManager = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_State, Category = "State")
    EHerdMemberState CurrentState = EHerdMemberState::Idle;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Survival", meta = (ClampMin = "0.0"))
    float Health = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Survival", meta = (ClampMin = "0.0"))
    float Stamina = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Survival", meta = (ClampMin = "0.0"))
    float Hunger = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Survival", meta = (ClampMin = "0.0"))
    float Thirst = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Emotion", meta = (ClampMin = "0.0", ClampMax = "10.0"))
    float Panic = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Emotion", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float Fear = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Emotion", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float Aggression = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Lifecycle", meta = (ClampMin = "0.0"))
    float AgeDays = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Lifecycle")
    EHerdLifeStage LifeStage = EHerdLifeStage::Adult;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Herd")
    EHerdRole HerdRole = EHerdRole::None;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Reproduction")
    bool bPregnant = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Reproduction", meta = (ClampMin = "0.0"))
    float PregnancyElapsedDays = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Threat Memory")
    TArray<FHerdThreatMemory> ThreatMemory;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Movement")
    FVector DesiredMoveLocation = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Movement")
    FVector DesiredFacingDirection = FVector::ForwardVector;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decision", meta = (ClampMin = "0.0"))
    float DecisionInterval = 0.35f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decision", meta = (ClampMin = "0.0"))
    float FleeThresholdHealth = 0.35f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decision", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float AggressionLevelOverride = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decision")
    bool bAllowManagerStateOverride = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decision")
    bool bAutoRegisterWithNearbyManager = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decision", meta = (ClampMin = "0.0"))
    float AutoRegisterRadius = 5000.0f;

    UPROPERTY(BlueprintAssignable, Category = "Herd")
    FOnHerdMemberStateChanged OnStateChanged;

    UFUNCTION(BlueprintCallable, Category = "Herd")
    void InitializeFromSpecies(UDA_SpeciesData_Base* InSpeciesData, AHerdManager_Generic* InManager);

    UFUNCTION(BlueprintCallable, Category = "Herd")
    void SetHerdManager(AHerdManager_Generic* InManager);

    UFUNCTION(BlueprintCallable, Category = "Decision")
    void EvaluateDecision(const FHerdScanDataset& ScanDataset);

    UFUNCTION(BlueprintCallable, Category = "Decision")
    void ApplyManagerState(EHerdMemberState NewState, FVector TargetLocation, float Priority);

    UFUNCTION(BlueprintCallable, Category = "State")
    void SetState(EHerdMemberState NewState);

    UFUNCTION(Server, Reliable, BlueprintCallable, Category = "State")
    void ServerSetState(EHerdMemberState NewState);

    UFUNCTION(BlueprintCallable, Category = "Survival")
    void ApplySurvivalDelta(float HealthDelta, float StaminaDelta, float HungerDelta, float ThirstDelta);

    UFUNCTION(BlueprintCallable, Category = "Threat Memory")
    void RememberThreat(AActor* ThreatActor, EHerdThreatType ThreatType, float ThreatLevel, float ExpireAfterSeconds);

    UFUNCTION(BlueprintCallable, Category = "Threat Memory")
    void ClearExpiredThreats();

    UFUNCTION(BlueprintPure, Category = "Decision")
    bool ShouldFleeFromThreat(const FHerdScanItem& ThreatItem, FHerdThreatResponse& OutResponse) const;

    UFUNCTION(BlueprintPure, Category = "Decision")
    bool ShouldAttackThreat(const FHerdScanItem& ThreatItem, FHerdThreatResponse& OutResponse) const;

    UFUNCTION(BlueprintPure, Category = "State")
    float GetHealthRatio() const;

    UFUNCTION(BlueprintPure, Category = "State")
    float GetHungerRatio() const;

    UFUNCTION(BlueprintPure, Category = "State")
    float GetThirstRatio() const;

    UFUNCTION(BlueprintPure, Category = "State")
    bool IsDead() const;

protected:
    UFUNCTION()
    void OnRep_State(EHerdMemberState PreviousState);

    UFUNCTION()
    void OnRep_SpeciesData();

    void ApplyNeeds(float DeltaSeconds);
    void ApplyStateStamina(float DeltaSeconds);
    void UpdateLifeStage(float DeltaSeconds);
    void AutoRegisterWithManager();
    void ClampVitals();

    FHerdScanItem FindHighestThreat(const FHerdScanDataset& ScanDataset, bool& bOutFoundThreat) const;
    float NormalizeHealthThreshold(float Threshold) const;

private:
    float DecisionTimer = 0.0f;
};
