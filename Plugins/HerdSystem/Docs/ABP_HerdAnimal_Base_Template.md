# ABP_HerdAnimal_Base Template

Unreal stores Animation Blueprints as binary `.uasset` files, so this plugin provides the C++ parent class and the exact graph contract.

## Asset

- Name: `ABP_HerdAnimal_Base`
- Parent Class: `UHerdAnimalAnimInstance_Base`
- Skeleton: any animal skeleton. For other skeletons, create child ABPs or retargeted copies using the same parent class.

## Exposed Variables

The parent class updates these every frame:

- `HerdMemberComponent`
- `SpeciesData`
- `HerdState`
- `LifeStage`
- `HerdRole`
- `HealthRatio`
- `HungerRatio`
- `ThirstRatio`
- `Panic`
- `Speed`
- `Direction`
- `bIsMoving`
- `CurrentStateAnimation`
- `CurrentStateAnimationName`

## Recommended Anim Graph

Use one of these patterns:

1. **Dynamic sequence player**
   - Read `CurrentStateAnimation`.
   - Feed it into a dynamic sequence/player node.
   - Blend with locomotion when `Speed > 3`.

2. **State machine**
   - Create states matching `EHerdMemberState`.
   - Transition from Any State when `HerdState == DesiredState`.
   - Each state uses the matching animation from `SpeciesData.AnimationSet`.

3. **Per-species child ABPs**
   - Make `ABP_HerdAnimal_Base` skeleton-agnostic.
   - Create children per skeleton/species and retarget animations.
   - Keep all behavior variables from the parent class.

## State Mapping

- `Idle`: idle loop
- `Walk`: walk loop
- `Run`: run loop
- `Flee`: flee or run loop
- `Scatter`: scatter or flee loop
- `Chase`: chase or run loop
- `Alert`: alert pose/loop
- `Graze`: graze loop
- `Defensive`: alert/defensive loop
- `Stampede`: flee/run loop
- `Regroup`: walk/run locomotion
- `Migrate`: walk locomotion
- `Attack`: attack montage or sequence
- `Drink`: drink loop
- `Rest`: rest loop
- `Dead`: death pose/sequence
