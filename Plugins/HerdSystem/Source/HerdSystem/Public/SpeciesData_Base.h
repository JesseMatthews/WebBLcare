#pragma once

#include "CoreMinimal.h"
#include "Engine/PrimaryDataAsset.h"
#include "HerdTypes.h"
#include "SpeciesData_Base.generated.h"

class UAnimInstance;
class UMaterialInterface;
class UPhysicsAsset;
class USkeletalMesh;

UCLASS(BlueprintType, Blueprintable)
class HERDSYSTEM_API UDA_SpeciesData_Base : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UDA_SpeciesData_Base();

    virtual FPrimaryAssetId GetPrimaryAssetId() const override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "01 Identity")
    FName SpeciesId = TEXT("GenericSpecies");

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "01 Identity")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "01 Identity", meta = (MultiLine = "true"))
    FText Description;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "01 Identity")
    FGameplayTagContainer SpeciesTags;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "01 Identity")
    EHerdDietType DietType = EHerdDietType::Herbivore;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "01 Identity")
    EHerdRole DefaultRole = EHerdRole::Forager;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "01 Identity")
    bool bDocile = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "01 Identity")
    bool bTerritorial = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "01 Identity")
    bool bCanMixWithOtherSpecies = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "02 Visual")
    TObjectPtr<USkeletalMesh> SkeletalMesh = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "02 Visual")
    TObjectPtr<UPhysicsAsset> PhysicsAsset = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "02 Visual")
    TSubclassOf<UAnimInstance> AnimBlueprintClass = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "02 Visual")
    FHerdAnimationSet AnimationSet;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "02 Visual")
    TArray<TObjectPtr<UMaterialInterface>> MaterialOverrides;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "02 Visual", meta = (ClampMin = "0.01"))
    float MinVisualScale = 0.9f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "02 Visual", meta = (ClampMin = "0.01"))
    float MaxVisualScale = 1.1f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "02 Visual", meta = (ClampMin = "0.0"))
    float CapsuleRadius = 42.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "02 Visual", meta = (ClampMin = "0.0"))
    float CapsuleHalfHeight = 96.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "02 Visual")
    FVector MeshRelativeOffset = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "02 Visual")
    FName RetargetProfile = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "03 Movement", meta = (ClampMin = "0.0"))
    float WalkSpeed = 180.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "03 Movement", meta = (ClampMin = "0.0"))
    float RunSpeed = 420.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "03 Movement", meta = (ClampMin = "0.0"))
    float SprintSpeed = 600.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "03 Movement", meta = (ClampMin = "0.0"))
    float FleeSpeed = 650.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "03 Movement", meta = (ClampMin = "0.0"))
    float ChaseSpeed = 560.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "03 Movement", meta = (ClampMin = "0.0"))
    float Acceleration = 1200.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "03 Movement", meta = (ClampMin = "0.0"))
    float RotationRateDegrees = 360.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "03 Movement", meta = (ClampMin = "0.0"))
    float WanderRadius = 1600.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "03 Movement", meta = (ClampMin = "0.0"))
    float SeparationRadius = 220.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "03 Movement", meta = (ClampMin = "0.0"))
    float CohesionRadius = 900.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "03 Movement", meta = (ClampMin = "0.0"))
    float AlignmentRadius = 650.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "03 Movement", meta = (ClampMin = "0.0"))
    float LeaderFollowDistance = 550.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "03 Movement", meta = (ClampMin = "0.0"))
    float ObstacleAvoidanceRadius = 300.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "03 Movement", meta = (ClampMin = "0.0", ClampMax = "89.0"))
    float MaxWalkableSlopeDegrees = 45.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "03 Movement")
    float PreferredMinAltitude = -100000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "03 Movement")
    float PreferredMaxAltitude = 100000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "04 Survival", meta = (ClampMin = "0.0"))
    float MaxHealth = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "04 Survival", meta = (ClampMin = "0.0"))
    float MaxStamina = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "04 Survival", meta = (ClampMin = "0.0"))
    float MaxHunger = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "04 Survival", meta = (ClampMin = "0.0"))
    float MaxThirst = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "04 Survival", meta = (ClampMin = "0.0"))
    float HungerRatePerMinute = 0.35f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "04 Survival", meta = (ClampMin = "0.0"))
    float ThirstRatePerMinute = 0.55f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "04 Survival", meta = (ClampMin = "0.0"))
    float StaminaDrainRunPerSecond = 3.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "04 Survival", meta = (ClampMin = "0.0"))
    float StaminaDrainFleePerSecond = 6.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "04 Survival", meta = (ClampMin = "0.0"))
    float StaminaRecoveryPerSecond = 5.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "04 Survival", meta = (ClampMin = "0.0"))
    float HealthRegenPerSecond = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "04 Survival", meta = (ClampMin = "0.0"))
    float StarvationDamagePerSecond = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "04 Survival", meta = (ClampMin = "0.0"))
    float DehydrationDamagePerSecond = 1.5f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "04 Survival", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float ExhaustedSpeedMultiplier = 0.35f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "04 Survival", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float CriticalNeedsThreshold = 0.85f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "05 Lifecycle")
    EHerdLifeStage StartingLifeStage = EHerdLifeStage::Adult;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "05 Lifecycle", meta = (ClampMin = "0.0"))
    float StartingAgeMinDays = 900.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "05 Lifecycle", meta = (ClampMin = "0.0"))
    float StartingAgeMaxDays = 1800.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "05 Lifecycle", meta = (ClampMin = "0.0"))
    float MaxAgeDays = 7300.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "05 Lifecycle")
    TArray<FHerdLifeStageConfig> LifeStages;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "05 Lifecycle", meta = (ClampMin = "0.0"))
    float GrowthRateMultiplier = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "05 Lifecycle", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float ElderSpeedPenalty = 0.15f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "05 Lifecycle", meta = (ClampMin = "0.0", ClampMax = "10.0"))
    float AncientKnowledgeAlertBonus = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "06 Reproduction")
    bool bCanReproduce = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "06 Reproduction", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float Fertility = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "06 Reproduction", meta = (ClampMin = "0.0"))
    float PregnancyDurationDays = 180.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "06 Reproduction", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float BirthRate = 0.25f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "06 Reproduction", meta = (ClampMin = "0"))
    int32 LitterSizeMin = 1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "06 Reproduction", meta = (ClampMin = "0"))
    int32 LitterSizeMax = 1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "06 Reproduction", meta = (ClampMin = "0.0"))
    float GestationCooldownDays = 90.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "06 Reproduction", meta = (ClampMin = "0.0"))
    float MateSearchRadius = 1800.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "06 Reproduction", meta = (ClampMin = "0.0"))
    float MinAdultAgeDays = 700.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "06 Reproduction", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float ParentProtectiveness = 0.7f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "06 Reproduction", meta = (ClampMin = "0.0"))
    float OffspringFollowDurationDays = 120.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "06 Reproduction")
    bool bRequiresMate = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "06 Reproduction")
    bool bSeasonalBreeding = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "06 Reproduction")
    FGameplayTagContainer BreedingSeasonTags;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "07 Herd Behavior")
    bool bUseHerdLevelHunger = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "07 Herd Behavior")
    bool bUseIndividualHunger = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "07 Herd Behavior")
    bool bCanStampede = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "07 Herd Behavior")
    bool bDefensiveCircle = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "07 Herd Behavior")
    bool bMigrationEnabled = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "07 Herd Behavior", meta = (ClampMin = "0.0"))
    float AlertDistance = 2500.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "07 Herd Behavior", meta = (ClampMin = "0.0", ClampMax = "10.0"))
    float PanicMultiplier = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "07 Herd Behavior", meta = (ClampMin = "0.0"))
    float CalmDownRatePerSecond = 0.2f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "07 Herd Behavior", meta = (ClampMin = "0.0", ClampMax = "10.0"))
    float CohesionWeight = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "07 Herd Behavior", meta = (ClampMin = "0.0", ClampMax = "10.0"))
    float SeparationWeight = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "07 Herd Behavior", meta = (ClampMin = "0.0", ClampMax = "10.0"))
    float AlignmentWeight = 0.65f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "07 Herd Behavior", meta = (ClampMin = "0.0", ClampMax = "10.0"))
    float LeaderPreferenceWeight = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "07 Herd Behavior", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float HerdLevelHungerThreshold = 0.65f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "07 Herd Behavior", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float HerdLevelThirstThreshold = 0.65f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "07 Herd Behavior", meta = (ClampMin = "0.0"))
    float RegroupRadius = 2200.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "07 Herd Behavior", meta = (ClampMin = "0.0", ClampMax = "10.0"))
    float StampedePanicThreshold = 6.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "07 Herd Behavior", meta = (ClampMin = "0"))
    int32 StampedeMinMembers = 4;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "07 Herd Behavior", meta = (ClampMin = "0.0"))
    float DefensiveCircleRadius = 700.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "07 Herd Behavior", meta = (ClampMin = "0"))
    int32 MinPreferredHerdSize = 3;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "07 Herd Behavior", meta = (ClampMin = "0"))
    int32 MaxPreferredHerdSize = 25;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "07 Herd Behavior")
    EHerdMigrationMode MigrationMode = EHerdMigrationMode::Disabled;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "07 Herd Behavior", meta = (ClampMin = "0.0"))
    float MigrationWaypointAcceptanceRadius = 250.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "08 Threat Responses", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float BaseFear = 0.25f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "08 Threat Responses", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float BaseAggression = 0.1f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "08 Threat Responses", meta = (ClampMin = "0.0"))
    float ThreatMemoryDuration = 20.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "08 Threat Responses", meta = (ClampMin = "0.0"))
    float ThreatScanRadius = 3000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "08 Threat Responses", meta = (ClampMin = "0.0", ClampMax = "10.0"))
    float PlayerThreatWeight = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "08 Threat Responses", meta = (ClampMin = "0.0", ClampMax = "10.0"))
    float EnvironmentalThreatWeight = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "08 Threat Responses")
    bool bAttackPlayers = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "08 Threat Responses")
    bool bAttackPredators = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "08 Threat Responses")
    bool bFleeFromPlayers = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "08 Threat Responses")
    bool bProtectYoung = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "08 Threat Responses")
    TArray<FHerdThreatResponse> ThreatResponses;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "08 Threat Responses")
    TArray<TSoftObjectPtr<UDA_SpeciesData_Base>> PredatorSpeciesToFleeFrom;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "08 Threat Responses")
    TArray<TSoftObjectPtr<UDA_SpeciesData_Base>> PreySpeciesToChase;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "08 Threat Responses")
    TArray<TSoftObjectPtr<UDA_SpeciesData_Base>> NeutralSpecies;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "08 Threat Responses")
    TArray<TSoftObjectPtr<UDA_SpeciesData_Base>> FriendlySpecies;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "09 Food And Water")
    TArray<FHerdFoodPreference> PreferredFoodTypes;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "09 Food And Water")
    TMap<EHerdFoodType, float> FoodValuePerType;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "09 Food And Water")
    TArray<FHerdWaterPreference> WaterSources;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "09 Food And Water")
    TMap<EHerdWaterType, float> WaterValuePerType;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "09 Food And Water", meta = (ClampMin = "0.0"))
    float GrazingTime = 8.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "09 Food And Water", meta = (ClampMin = "0.0"))
    float DrinkingTime = 6.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "09 Food And Water", meta = (ClampMin = "0.0"))
    float ForageRadius = 1800.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "09 Food And Water", meta = (ClampMin = "0.0"))
    float DetectionRadius = 3000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "09 Food And Water", meta = (ClampMin = "0.0"))
    float FoodClaimRadius = 250.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "09 Food And Water", meta = (ClampMin = "0.0"))
    float WaterClaimRadius = 250.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "09 Food And Water")
    bool bCanGrazeWhileMoving = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "09 Food And Water")
    bool bCanScavenge = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "09 Food And Water")
    bool bPredatorsPrioritizeWeakPrey = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "10 Species Interactions")
    TArray<FHerdInteractionRule> InterSpeciesInteractions;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "10 Species Interactions")
    EHerdInteractionDisposition DefaultPredatorPredatorDisposition = EHerdInteractionDisposition::Avoid;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "10 Species Interactions")
    EHerdInteractionDisposition DefaultHerbivoreHerbivoreDisposition = EHerdInteractionDisposition::Friendly;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "10 Species Interactions")
    EHerdInteractionDisposition DefaultPredatorHerbivoreDisposition = EHerdInteractionDisposition::Predator;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "10 Species Interactions")
    bool bCanShareHerdWithFriendlySpecies = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "10 Species Interactions")
    bool bCompeteForSameFood = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "10 Species Interactions")
    bool bCompeteForSameWater = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "11 Carcass And Loot")
    FHerdCarcassLootConfig CarcassLoot;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "12 Environment")
    FHerdEnvironmentalPreference EnvironmentalPreference;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "12 Environment")
    TMap<FName, float> SeasonalMovementWeights;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "12 Environment")
    FGameplayTagContainer AvoidWeatherTags;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "13 Disease And Genetics")
    FHerdDiseaseConfig Disease;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "13 Disease And Genetics")
    FHerdGeneticsConfig Genetics;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "14 Special Abilities")
    TArray<FHerdSpecialAbilityConfig> SpecialAbilities;

    UFUNCTION(BlueprintPure, Category = "Species")
    FHerdThreatResponse GetThreatResponse(EHerdThreatType ThreatType) const;

    UFUNCTION(BlueprintPure, Category = "Species")
    bool FindThreatResponse(EHerdThreatType ThreatType, FHerdThreatResponse& OutResponse) const;

    UFUNCTION(BlueprintPure, Category = "Species")
    bool FindInteractionRule(const UDA_SpeciesData_Base* OtherSpecies, FHerdInteractionRule& OutRule) const;

    UFUNCTION(BlueprintPure, Category = "Species")
    EHerdLifeStage GetLifeStageForAge(float AgeDays) const;

    UFUNCTION(BlueprintPure, Category = "Species")
    UAnimSequenceBase* GetAnimationForState(EHerdMemberState State) const;

    UFUNCTION(BlueprintPure, Category = "Species")
    bool UsesPredatorHungerLogic() const;

private:
    FHerdThreatResponse DefaultThreatResponse;
};
