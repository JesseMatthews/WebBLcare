# Example Level: Generic Herd

Unreal levels are `.umap` assets, so this plugin includes `AHerdExampleLevelSpawner` to create the same result in any project.

## Build The Example

1. Create a new level named `L_GenericHerd_Example`.
2. Add a NavMesh if your animal actor classes use AI movement.
3. Drop `AHerdExampleLevelSpawner` into the level.
4. Create at least one `DA_SpeciesData_Base` asset.
5. In `SpawnGroups`, add entries:
   - `SpeciesData`: your data asset.
   - `AnimalActorClass`: an actor, pawn, or character class with `UHerdMemberComponent`.
   - `Count`: number to spawn.
   - `SpawnRadius`: initial herd radius.
   - `RoleWeights`: optional leader/sentinel/forager distribution.
6. Optionally add `ExampleMigrationRoutes` with waypoints.
7. Press Play. The spawner creates:
   - one `AHerdManager_Generic`
   - mixed species members
   - registered herd composition
   - scan-driven food/water/threat data
   - migration route state if configured

## Food, Water, And Threat Actors

The scanner is data-driven through tags and maps on `UHerdScanDatasetComponent`.

Default actor tags:

- `Herd.Food`
- `Herd.Water`
- `Herd.Shelter`
- `Herd.Hazard`
- `Herd.Threat`
- `Herd.Predator`
- `Herd.Player`
- `Herd.Fire`
- `Herd.EnvironmentalHazard`

Add those tags to actors in the level, or replace the maps with your own tags.

## Debug Widget

Create a UMG widget using `UHerdDebugWidget` as parent.

Bind your UI to:

- `HerdCompositionText`
- `ThreatSummaryText`
- `MigrationRouteText`
- `MemberRows`

Call `SetObservedManager` with the spawned or placed manager. The rows include health, hunger, thirst, life stage, role, panic/fear, and current animation name.
