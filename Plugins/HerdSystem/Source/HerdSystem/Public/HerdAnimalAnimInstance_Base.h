#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "HerdTypes.h"
#include "HerdAnimalAnimInstance_Base.generated.h"

class UDA_SpeciesData_Base;
class UHerdMemberComponent;

UCLASS(BlueprintType, Blueprintable, Transient)
class HERDSYSTEM_API UHerdAnimalAnimInstance_Base : public UAnimInstance
{
    GENERATED_BODY()

public:
    virtual void NativeInitializeAnimation() override;
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;

    UPROPERTY(BlueprintReadOnly, Category = "Herd")
    TObjectPtr<UHerdMemberComponent> HerdMemberComponent = nullptr;

    UPROPERTY(BlueprintReadOnly, Category = "Herd")
    TObjectPtr<UDA_SpeciesData_Base> SpeciesData = nullptr;

    UPROPERTY(BlueprintReadOnly, Category = "Herd")
    EHerdMemberState HerdState = EHerdMemberState::Idle;

    UPROPERTY(BlueprintReadOnly, Category = "Herd")
    EHerdLifeStage LifeStage = EHerdLifeStage::Adult;

    UPROPERTY(BlueprintReadOnly, Category = "Herd")
    EHerdRole HerdRole = EHerdRole::None;

    UPROPERTY(BlueprintReadOnly, Category = "Herd")
    float HealthRatio = 1.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Herd")
    float HungerRatio = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Herd")
    float ThirstRatio = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Herd")
    float Panic = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Movement")
    float Speed = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Movement")
    float Direction = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Movement")
    bool bIsMoving = false;

    UPROPERTY(BlueprintReadOnly, Category = "Animation")
    TObjectPtr<UAnimSequenceBase> CurrentStateAnimation = nullptr;

    UPROPERTY(BlueprintReadOnly, Category = "Animation")
    FName CurrentStateAnimationName = NAME_None;

    UFUNCTION(BlueprintCallable, Category = "Herd")
    void RefreshFromHerdMember();

    UFUNCTION(BlueprintPure, Category = "Animation")
    UAnimSequenceBase* GetAnimationForState(EHerdMemberState State) const;

protected:
    UHerdMemberComponent* ResolveHerdMemberComponent() const;
};
