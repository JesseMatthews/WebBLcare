#include "SpeciesData_Base.h"

#include "Internationalization/Text.h"

UDA_SpeciesData_Base::UDA_SpeciesData_Base()
{
    DisplayName = NSLOCTEXT("HerdSystem", "GenericSpeciesDisplayName", "Generic Species");
    Description = NSLOCTEXT("HerdSystem", "GenericSpeciesDescription", "A reusable species data template for the generic herd system.");

    LifeStages =
    {
        {EHerdLifeStage::Calf, 0.0f, 180.0f, 0.45f, 0.45f, 0.65f, 0.0f, 0.4f, 0.02f},
        {EHerdLifeStage::Juvenile, 180.0f, 700.0f, 0.75f, 0.75f, 0.9f, 0.0f, 0.75f, 0.01f},
        {EHerdLifeStage::Adult, 700.0f, 5000.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.005f},
        {EHerdLifeStage::Elder, 5000.0f, 6500.0f, 0.95f, 0.85f, 0.8f, 0.25f, 0.8f, 0.015f},
        {EHerdLifeStage::Ancient, 6500.0f, 7300.0f, 0.9f, 0.7f, 0.6f, 0.0f, 0.65f, 0.03f}
    };

    ThreatResponses =
    {
        {EHerdThreatType::Predator, 0.15f, 0.55f, 0.2f, 2800.0f, 250.0f, 1.0f, false, true, false, true, 1.25f, 0, 0.6f},
        {EHerdThreatType::Player, 0.1f, 0.45f, 0.15f, 2200.0f, 250.0f, 0.85f, false, true, false, true, 0.9f, 0, 0.6f},
        {EHerdThreatType::EnvironmentalHazard, 0.0f, 1.0f, 0.0f, 3000.0f, 0.0f, 1.2f, false, true, false, true, 1.5f, 0, 1.0f},
        {EHerdThreatType::Prey, 0.2f, 0.25f, 0.0f, 800.0f, 200.0f, 0.35f, false, false, false, false, 0.0f, 0, 0.25f}
    };

    PreferredFoodTypes =
    {
        {EHerdFoodType::Grass, FGameplayTagContainer(), 35.0f, 25.0f, 8.0f, 200.0f, 0, TMap<FName, float>()}
    };

    WaterSources =
    {
        {EHerdWaterType::River, FGameplayTagContainer(), 40.0f, 30.0f, 6.0f, 180.0f, 0}
    };

    FoodValuePerType.Add(EHerdFoodType::Grass, 35.0f);
    FoodValuePerType.Add(EHerdFoodType::Leaf, 30.0f);
    FoodValuePerType.Add(EHerdFoodType::Fruit, 45.0f);
    FoodValuePerType.Add(EHerdFoodType::Meat, 70.0f);
    FoodValuePerType.Add(EHerdFoodType::Carrion, 50.0f);

    WaterValuePerType.Add(EHerdWaterType::River, 40.0f);
    WaterValuePerType.Add(EHerdWaterType::Lake, 35.0f);
    WaterValuePerType.Add(EHerdWaterType::Trough, 45.0f);

    EnvironmentalPreference.PreferredTerrains = {EHerdTerrainPreference::Plains, EHerdTerrainPreference::Forest};
    EnvironmentalPreference.AvoidedTerrains = {EHerdTerrainPreference::Urban};

    CarcassLoot.MeatAmount = 40.0f;
    CarcassLoot.HideAmount = 1.0f;
    CarcassLoot.BoneAmount = 12.0f;
    CarcassLoot.RotTimeByStageHours.Add(EHerdCarcassStage::Fresh, 2.0f);
    CarcassLoot.RotTimeByStageHours.Add(EHerdCarcassStage::Bloat, 12.0f);
    CarcassLoot.RotTimeByStageHours.Add(EHerdCarcassStage::ActiveDecay, 36.0f);
    CarcassLoot.RotTimeByStageHours.Add(EHerdCarcassStage::AdvancedDecay, 96.0f);
    CarcassLoot.RotTimeByStageHours.Add(EHerdCarcassStage::Dry, 240.0f);
}

FPrimaryAssetId UDA_SpeciesData_Base::GetPrimaryAssetId() const
{
    const FName Id = SpeciesId.IsNone() ? GetFName() : SpeciesId;
    return FPrimaryAssetId(TEXT("SpeciesData"), Id);
}

FHerdThreatResponse UDA_SpeciesData_Base::GetThreatResponse(EHerdThreatType ThreatType) const
{
    for (const FHerdThreatResponse& Response : ThreatResponses)
    {
        if (Response.ThreatType == ThreatType)
        {
            return Response;
        }
    }

    return DefaultThreatResponse;
}

bool UDA_SpeciesData_Base::FindThreatResponse(EHerdThreatType ThreatType, FHerdThreatResponse& OutResponse) const
{
    for (const FHerdThreatResponse& Response : ThreatResponses)
    {
        if (Response.ThreatType == ThreatType)
        {
            OutResponse = Response;
            return true;
        }
    }

    OutResponse = DefaultThreatResponse;
    return false;
}

bool UDA_SpeciesData_Base::FindInteractionRule(const UDA_SpeciesData_Base* OtherSpecies, FHerdInteractionRule& OutRule) const
{
    if (!OtherSpecies)
    {
        return false;
    }

    const FString OtherPath = OtherSpecies->GetPathName();
    for (const FHerdInteractionRule& Rule : InterSpeciesInteractions)
    {
        if (Rule.OtherSpecies.IsNull())
        {
            continue;
        }

        if (Rule.OtherSpecies.Get() == OtherSpecies || Rule.OtherSpecies.ToSoftObjectPath().ToString() == OtherPath)
        {
            OutRule = Rule;
            return true;
        }
    }

    return false;
}

EHerdLifeStage UDA_SpeciesData_Base::GetLifeStageForAge(float AgeDays) const
{
    for (const FHerdLifeStageConfig& Config : LifeStages)
    {
        if (AgeDays >= Config.MinAgeDays && AgeDays < Config.MaxAgeDays)
        {
            return Config.Stage;
        }
    }

    return AgeDays >= MaxAgeDays ? EHerdLifeStage::Ancient : StartingLifeStage;
}

UAnimSequenceBase* UDA_SpeciesData_Base::GetAnimationForState(EHerdMemberState State) const
{
    switch (State)
    {
    case EHerdMemberState::Walk:
        return AnimationSet.Walk.Get();
    case EHerdMemberState::Run:
        return AnimationSet.Run.Get();
    case EHerdMemberState::Flee:
        return AnimationSet.Flee ? AnimationSet.Flee.Get() : AnimationSet.Run.Get();
    case EHerdMemberState::Scatter:
        return AnimationSet.Scatter ? AnimationSet.Scatter.Get() : AnimationSet.Flee.Get();
    case EHerdMemberState::Chase:
        return AnimationSet.Chase ? AnimationSet.Chase.Get() : AnimationSet.Run.Get();
    case EHerdMemberState::Alert:
        return AnimationSet.Alert.Get();
    case EHerdMemberState::Attack:
        return AnimationSet.Attack.Get();
    case EHerdMemberState::Graze:
        return AnimationSet.Graze.Get();
    case EHerdMemberState::Drink:
        return AnimationSet.Drink.Get();
    case EHerdMemberState::Rest:
        return AnimationSet.Rest.Get();
    case EHerdMemberState::Dead:
        return AnimationSet.Death.Get();
    case EHerdMemberState::Stampede:
        return AnimationSet.Flee ? AnimationSet.Flee.Get() : AnimationSet.Run.Get();
    case EHerdMemberState::Regroup:
    case EHerdMemberState::Migrate:
        return AnimationSet.Walk.Get();
    case EHerdMemberState::Defensive:
        return AnimationSet.Alert.Get();
    case EHerdMemberState::Idle:
    default:
        return AnimationSet.Idle.Get();
    }
}

bool UDA_SpeciesData_Base::UsesPredatorHungerLogic() const
{
    return bUseIndividualHunger || DietType == EHerdDietType::Carnivore || DietType == EHerdDietType::Scavenger || DietType == EHerdDietType::Piscivore;
}
