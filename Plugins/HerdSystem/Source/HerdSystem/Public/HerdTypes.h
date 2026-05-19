#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimSequenceBase.h"
#include "Engine/EngineTypes.h"
#include "GameplayTagContainer.h"
#include "HerdTypes.generated.h"

class AActor;
class UAnimInstance;
class UDA_SpeciesData_Base;
class UHerdMemberComponent;

UENUM(BlueprintType)
enum class EHerdMemberState : uint8
{
    Idle,
    Walk,
    Run,
    Flee,
    Scatter,
    Chase,
    Alert,
    Graze,
    Defensive,
    Stampede,
    Regroup,
    Migrate,
    Attack,
    Drink,
    Rest,
    Dead
};

UENUM(BlueprintType)
enum class EHerdLifeStage : uint8
{
    Calf,
    Juvenile,
    Adult,
    Elder,
    Ancient
};

UENUM(BlueprintType)
enum class EHerdRole : uint8
{
    None,
    Leader,
    Sentinel,
    Breeder,
    Parent,
    Juvenile,
    Elder,
    Scout,
    Defender,
    Forager,
    Migrator
};

UENUM(BlueprintType)
enum class EHerdThreatType : uint8
{
    None,
    Player,
    Predator,
    Prey,
    RivalPredator,
    RivalHerbivore,
    EnvironmentalHazard,
    Fire,
    Weather,
    Disease,
    Starvation,
    Dehydration,
    Projectile,
    Vehicle,
    Territorial,
    Unknown
};

UENUM(BlueprintType)
enum class EHerdCarcassStage : uint8
{
    Alive,
    Fresh,
    Bloat,
    ActiveDecay,
    AdvancedDecay,
    Dry,
    Skeletonized,
    Gone
};

UENUM(BlueprintType)
enum class EHerdDietType : uint8
{
    Herbivore,
    Carnivore,
    Omnivore,
    Scavenger,
    Piscivore,
    Insectivore,
    Nectarivore,
    Custom
};

UENUM(BlueprintType)
enum class EHerdFoodType : uint8
{
    Grass,
    Shrub,
    Leaf,
    Fruit,
    Grain,
    Root,
    Meat,
    Carrion,
    Fish,
    Insect,
    Nectar,
    Fungi,
    Custom
};

UENUM(BlueprintType)
enum class EHerdWaterType : uint8
{
    River,
    Lake,
    Pond,
    Pool,
    Trough,
    Rain,
    Snow,
    Custom
};

UENUM(BlueprintType)
enum class EHerdResourceType : uint8
{
    None,
    Food,
    Water,
    Shelter,
    SaltLick,
    NestingSite,
    Mineral,
    Medicine,
    Hazard,
    Custom
};

UENUM(BlueprintType)
enum class EHerdInteractionDisposition : uint8
{
    Friendly,
    Neutral,
    Avoid,
    Hostile,
    Predator,
    Prey,
    Ally,
    Ignore
};

UENUM(BlueprintType)
enum class EHerdActivityCycle : uint8
{
    Diurnal,
    Nocturnal,
    Crepuscular,
    Cathemeral,
    Seasonal,
    Custom
};

UENUM(BlueprintType)
enum class EHerdTerrainPreference : uint8
{
    Any,
    Plains,
    Forest,
    Wetland,
    Mountain,
    Desert,
    Tundra,
    Jungle,
    Savanna,
    Coast,
    Urban,
    Cave,
    Custom
};

UENUM(BlueprintType)
enum class EHerdMigrationMode : uint8
{
    Disabled,
    LoopRoute,
    OneWayRoute,
    SeasonalRoute,
    DynamicGoal
};

UENUM(BlueprintType)
enum class EHerdSpecialAbilityType : uint8
{
    None,
    Charge,
    Ram,
    Gore,
    Kick,
    Bite,
    Pounce,
    Venom,
    PackCall,
    Camouflage,
    Burrow,
    Climb,
    Swim,
    Fly,
    Custom
};

USTRUCT(BlueprintType)
struct HERDSYSTEM_API FHerdAnimationSet
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations")
    TObjectPtr<UAnimSequenceBase> Idle = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations")
    TObjectPtr<UAnimSequenceBase> Walk = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations")
    TObjectPtr<UAnimSequenceBase> Run = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations")
    TObjectPtr<UAnimSequenceBase> Flee = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations")
    TObjectPtr<UAnimSequenceBase> Scatter = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations")
    TObjectPtr<UAnimSequenceBase> Chase = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations")
    TObjectPtr<UAnimSequenceBase> Alert = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations")
    TObjectPtr<UAnimSequenceBase> Attack = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations")
    TObjectPtr<UAnimSequenceBase> Graze = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations")
    TObjectPtr<UAnimSequenceBase> SpecialAttack = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations")
    TObjectPtr<UAnimSequenceBase> Drink = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations")
    TObjectPtr<UAnimSequenceBase> Rest = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations")
    TObjectPtr<UAnimSequenceBase> Death = nullptr;
};

USTRUCT(BlueprintType)
struct HERDSYSTEM_API FHerdLifeStageConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lifecycle")
    EHerdLifeStage Stage = EHerdLifeStage::Adult;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lifecycle", meta = (ClampMin = "0"))
    float MinAgeDays = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lifecycle", meta = (ClampMin = "0"))
    float MaxAgeDays = 3650.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lifecycle", meta = (ClampMin = "0.01"))
    float SizeScale = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lifecycle", meta = (ClampMin = "0.01"))
    float MaxHealthMultiplier = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lifecycle", meta = (ClampMin = "0.0"))
    float SpeedMultiplier = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lifecycle", meta = (ClampMin = "0.0"))
    float FertilityMultiplier = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lifecycle", meta = (ClampMin = "0.0"))
    float AggressionMultiplier = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lifecycle", meta = (ClampMin = "0.0"))
    float MortalityRiskPerDay = 0.0f;
};

USTRUCT(BlueprintType)
struct HERDSYSTEM_API FHerdThreatResponse
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Threat")
    EHerdThreatType ThreatType = EHerdThreatType::Predator;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Threat", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float AggressionLevel = 0.25f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Threat", meta = (ClampMin = "0.0"))
    float FleeThresholdHealth = 0.35f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Threat", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float DefensiveAbility = 0.25f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Threat", meta = (ClampMin = "0.0"))
    float FleeDistance = 2500.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Threat", meta = (ClampMin = "0.0"))
    float AttackDistance = 350.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Threat", meta = (ClampMin = "0.0", ClampMax = "10.0"))
    float AlertWeight = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Threat")
    bool bCanAttack = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Threat")
    bool bCanFlee = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Threat")
    bool bCanChase = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Threat")
    bool bCanCallForHelp = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Threat", meta = (ClampMin = "0.0", ClampMax = "10.0"))
    float PanicContribution = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Threat", meta = (ClampMin = "0"))
    int32 MinGroupSizeToFight = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Threat", meta = (ClampMin = "0.0"))
    float HealthAttackThreshold = 0.5f;
};

USTRUCT(BlueprintType)
struct HERDSYSTEM_API FHerdInteractionRule
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    TSoftObjectPtr<UDA_SpeciesData_Base> OtherSpecies;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    EHerdInteractionDisposition Disposition = EHerdInteractionDisposition::Neutral;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    EHerdThreatType ThreatType = EHerdThreatType::Unknown;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float AggressionOverride = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float FearOverride = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction", meta = (ClampMin = "0.0", ClampMax = "10.0"))
    float ThreatWeight = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    bool bCanShareResources = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    bool bCanJoinSameHerd = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction", meta = (ClampMin = "0"))
    int32 Priority = 0;
};

USTRUCT(BlueprintType)
struct HERDSYSTEM_API FHerdFoodPreference
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Food")
    EHerdFoodType FoodType = EHerdFoodType::Grass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Food")
    FGameplayTagContainer SourceTags;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Food", meta = (ClampMin = "0.0"))
    float FoodValue = 35.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Food", meta = (ClampMin = "0.0"))
    float HungerReduction = 25.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Food", meta = (ClampMin = "0.0"))
    float GrazingTime = 8.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Food", meta = (ClampMin = "0.0"))
    float PreferredRadius = 200.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Food", meta = (ClampMin = "0"))
    int32 Priority = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Food")
    TMap<FName, float> SeasonalAvailability;
};

USTRUCT(BlueprintType)
struct HERDSYSTEM_API FHerdWaterPreference
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Water")
    EHerdWaterType WaterType = EHerdWaterType::River;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Water")
    FGameplayTagContainer SourceTags;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Water", meta = (ClampMin = "0.0"))
    float WaterValue = 35.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Water", meta = (ClampMin = "0.0"))
    float ThirstReduction = 25.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Water", meta = (ClampMin = "0.0"))
    float DrinkingTime = 6.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Water", meta = (ClampMin = "0.0"))
    float PreferredRadius = 180.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Water", meta = (ClampMin = "0"))
    int32 Priority = 0;
};

USTRUCT(BlueprintType)
struct HERDSYSTEM_API FHerdCarcassLootConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Carcass", meta = (ClampMin = "0.0"))
    float MeatAmount = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Carcass", meta = (ClampMin = "0.0"))
    float HideAmount = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Carcass", meta = (ClampMin = "0.0"))
    float BoneAmount = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Carcass", meta = (ClampMin = "0.0"))
    float TusksAmount = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Carcass", meta = (ClampMin = "0.0"))
    float HornAmount = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Carcass", meta = (ClampMin = "0.0"))
    float PeltQuality = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Carcass")
    TMap<EHerdCarcassStage, float> RotTimeByStageHours;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Carcass", meta = (ClampMin = "0.0", ClampMax = "10.0"))
    float ScavengerAttraction = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Carcass")
    FGameplayTagContainer LootTags;
};

USTRUCT(BlueprintType)
struct HERDSYSTEM_API FHerdEnvironmentalPreference
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
    EHerdActivityCycle ActivityCycle = EHerdActivityCycle::Diurnal;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
    TArray<EHerdTerrainPreference> PreferredTerrains;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
    TArray<EHerdTerrainPreference> AvoidedTerrains;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment", meta = (ClampMin = "-100.0", ClampMax = "100.0"))
    float MinComfortTemperatureC = -10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment", meta = (ClampMin = "-100.0", ClampMax = "100.0"))
    float MaxComfortTemperatureC = 35.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float DayActivityWeight = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float NightActivityWeight = 0.25f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment", meta = (ClampMin = "0.0", ClampMax = "10.0"))
    float WeatherAvoidanceWeight = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
    TMap<FName, float> SeasonalFoodAvailability;
};

USTRUCT(BlueprintType)
struct HERDSYSTEM_API FHerdDiseaseConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Disease", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float DiseaseSusceptibility = 0.1f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Disease", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float NaturalImmunity = 0.2f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Disease", meta = (ClampMin = "0.0"))
    float DiseaseSpreadRadius = 350.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Disease", meta = (ClampMin = "0.0", ClampMax = "10.0"))
    float InfectionPanicMultiplier = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Disease")
    TArray<FName> ResistantDiseases;
};

USTRUCT(BlueprintType)
struct HERDSYSTEM_API FHerdGeneticsConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Genetics", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float MutationChance = 0.01f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Genetics", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float InheritanceStrength = 0.7f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Genetics", meta = (ClampMin = "0.0", ClampMax = "10.0"))
    float BloodlineValue = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Genetics")
    FName BloodlineId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Genetics")
    TMap<FName, float> TraitWeights;
};

USTRUCT(BlueprintType)
struct HERDSYSTEM_API FHerdSpecialAbilityConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
    EHerdSpecialAbilityType AbilityType = EHerdSpecialAbilityType::None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
    FName AbilityId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability", meta = (ClampMin = "0.0"))
    float Cooldown = 5.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability", meta = (ClampMin = "0.0"))
    float Range = 300.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability", meta = (ClampMin = "0.0"))
    float Power = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
    FGameplayTagContainer RequiredTags;
};

USTRUCT(BlueprintType)
struct HERDSYSTEM_API FHerdThreatMemory
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory")
    TObjectPtr<AActor> ThreatActor = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory")
    EHerdThreatType ThreatType = EHerdThreatType::Unknown;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory")
    FVector LastKnownLocation = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory", meta = (ClampMin = "0.0", ClampMax = "10.0"))
    float ThreatLevel = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory")
    float LastSeenTime = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory", meta = (ClampMin = "0.0"))
    float ExpireAfterSeconds = 20.0f;
};

USTRUCT(BlueprintType)
struct HERDSYSTEM_API FHerdScanItem
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Scan")
    TObjectPtr<AActor> Actor = nullptr;

    UPROPERTY(BlueprintReadOnly, Category = "Scan")
    TSubclassOf<AActor> ActorClass = nullptr;

    UPROPERTY(BlueprintReadOnly, Category = "Scan")
    TObjectPtr<UHerdMemberComponent> HerdMember = nullptr;

    UPROPERTY(BlueprintReadOnly, Category = "Scan")
    TObjectPtr<UDA_SpeciesData_Base> SpeciesData = nullptr;

    UPROPERTY(BlueprintReadOnly, Category = "Scan")
    EHerdResourceType ResourceType = EHerdResourceType::None;

    UPROPERTY(BlueprintReadOnly, Category = "Scan")
    EHerdThreatType ThreatType = EHerdThreatType::None;

    UPROPERTY(BlueprintReadOnly, Category = "Scan")
    EHerdInteractionDisposition Relationship = EHerdInteractionDisposition::Neutral;

    UPROPERTY(BlueprintReadOnly, Category = "Scan")
    FVector Location = FVector::ZeroVector;

    UPROPERTY(BlueprintReadOnly, Category = "Scan")
    FVector DirectionFromScanner = FVector::ZeroVector;

    UPROPERTY(BlueprintReadOnly, Category = "Scan")
    float Distance = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Scan")
    float ThreatLevel = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Scan")
    float ResourceValue = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Scan")
    bool bIsReachable = true;

    UPROPERTY(BlueprintReadOnly, Category = "Scan")
    bool bLineOfSight = true;

    UPROPERTY(BlueprintReadOnly, Category = "Scan")
    float LastSeenTime = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Scan")
    FGameplayTagContainer Tags;
};

USTRUCT(BlueprintType)
struct HERDSYSTEM_API FHerdScanDataset
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Scan")
    TArray<FHerdScanItem> NearbyFood;

    UPROPERTY(BlueprintReadOnly, Category = "Scan")
    TArray<FHerdScanItem> NearbyWater;

    UPROPERTY(BlueprintReadOnly, Category = "Scan")
    TArray<FHerdScanItem> NearbyThreats;

    UPROPERTY(BlueprintReadOnly, Category = "Scan")
    TArray<FHerdScanItem> NearbySpecies;

    UPROPERTY(BlueprintReadOnly, Category = "Scan")
    TArray<FHerdScanItem> NearbyHerdMembers;

    UPROPERTY(BlueprintReadOnly, Category = "Scan")
    TArray<FHerdScanItem> AllItems;

    UPROPERTY(BlueprintReadOnly, Category = "Scan")
    FVector ScanOrigin = FVector::ZeroVector;

    UPROPERTY(BlueprintReadOnly, Category = "Scan")
    float ScanRadius = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Scan")
    float ScanTime = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Scan")
    int32 TotalActorsScanned = 0;
};

USTRUCT(BlueprintType)
struct HERDSYSTEM_API FHerdMigrationRoute
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Migration")
    FName RouteName = TEXT("DefaultRoute");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Migration")
    TArray<FVector> Waypoints;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Migration")
    bool bLoop = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Migration", meta = (ClampMin = "0.0"))
    float DesiredSpeed = 350.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Migration", meta = (ClampMin = "0.0"))
    float ArrivalRadius = 250.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Migration")
    FGameplayTagContainer SeasonTags;
};

USTRUCT(BlueprintType)
struct HERDSYSTEM_API FHerdRuntimeSpeciesEntry
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Runtime")
    TObjectPtr<UDA_SpeciesData_Base> SpeciesData = nullptr;

    UPROPERTY(BlueprintReadOnly, Category = "Runtime")
    int32 CurrentCount = 0;

    UPROPERTY(BlueprintReadOnly, Category = "Runtime")
    float AverageHealth = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Runtime")
    float AverageHunger = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Runtime")
    float AverageThirst = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Runtime")
    float AveragePanic = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Runtime")
    TArray<TObjectPtr<UHerdMemberComponent>> Members;
};

USTRUCT(BlueprintType)
struct HERDSYSTEM_API FHerdSpawnGroup
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
    TObjectPtr<UDA_SpeciesData_Base> SpeciesData = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
    TSubclassOf<AActor> AnimalActorClass = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn", meta = (ClampMin = "0"))
    int32 Count = 8;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn", meta = (ClampMin = "0.0"))
    float SpawnRadius = 1200.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
    EHerdRole DefaultRole = EHerdRole::Forager;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
    TMap<EHerdRole, float> RoleWeights;
};

USTRUCT(BlueprintType)
struct HERDSYSTEM_API FHerdDebugMemberRow
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Debug")
    TObjectPtr<AActor> Actor = nullptr;

    UPROPERTY(BlueprintReadOnly, Category = "Debug")
    FName SpeciesId = NAME_None;

    UPROPERTY(BlueprintReadOnly, Category = "Debug")
    EHerdMemberState State = EHerdMemberState::Idle;

    UPROPERTY(BlueprintReadOnly, Category = "Debug")
    EHerdLifeStage LifeStage = EHerdLifeStage::Adult;

    UPROPERTY(BlueprintReadOnly, Category = "Debug")
    EHerdRole Role = EHerdRole::None;

    UPROPERTY(BlueprintReadOnly, Category = "Debug")
    float Health = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Debug")
    float Hunger = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Debug")
    float Thirst = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Debug")
    float Panic = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Debug")
    float Fear = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Debug")
    FName CurrentAnimation = NAME_None;
};
