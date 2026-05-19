#include "HerdAnimalAnimInstance_Base.h"

#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h"
#include "HerdMemberComponent.h"
#include "SpeciesData_Base.h"

void UHerdAnimalAnimInstance_Base::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();
    HerdMemberComponent = ResolveHerdMemberComponent();
    RefreshFromHerdMember();
}

void UHerdAnimalAnimInstance_Base::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    if (!HerdMemberComponent)
    {
        HerdMemberComponent = ResolveHerdMemberComponent();
    }

    RefreshFromHerdMember();

    if (const USkeletalMeshComponent* MeshComponent = GetOwningComponent())
    {
        Speed = MeshComponent->GetComponentVelocity().Size2D();
        bIsMoving = Speed > 3.0f;
        Direction = CalculateDirection(MeshComponent->GetComponentVelocity(), MeshComponent->GetComponentRotation());
    }
}

void UHerdAnimalAnimInstance_Base::RefreshFromHerdMember()
{
    if (!HerdMemberComponent)
    {
        return;
    }

    SpeciesData = HerdMemberComponent->SpeciesData;
    HerdState = HerdMemberComponent->CurrentState;
    LifeStage = HerdMemberComponent->LifeStage;
    HerdRole = HerdMemberComponent->HerdRole;
    HealthRatio = HerdMemberComponent->GetHealthRatio();
    HungerRatio = HerdMemberComponent->GetHungerRatio();
    ThirstRatio = HerdMemberComponent->GetThirstRatio();
    Panic = HerdMemberComponent->Panic;
    CurrentStateAnimation = GetAnimationForState(HerdState);
    CurrentStateAnimationName = CurrentStateAnimation ? CurrentStateAnimation->GetFName() : NAME_None;
}

UAnimSequenceBase* UHerdAnimalAnimInstance_Base::GetAnimationForState(EHerdMemberState State) const
{
    return SpeciesData ? SpeciesData->GetAnimationForState(State) : nullptr;
}

UHerdMemberComponent* UHerdAnimalAnimInstance_Base::ResolveHerdMemberComponent() const
{
    if (AActor* Owner = GetOwningActor())
    {
        return Owner->FindComponentByClass<UHerdMemberComponent>();
    }

    return nullptr;
}
