# Generic Herd System

Drop this plugin into a UE5 project's `Plugins` folder, enable **Generic Herd System**, regenerate project files, and compile.

## Runtime Classes

- `AHerdManager_Generic`: server-authoritative herd brain. Tracks mixed species, herd composition, migration routes, group hunger/thirst, threats, stampedes, regrouping, defensive circles, and scan-driven decisions.
- `UHerdMemberComponent`: attach to any animal actor. Replicates survival stats, emotion, lifecycle, role, pregnancy, threat memory, state, and desired movement.
- `UDA_SpeciesData_Base`: primary data asset with editable species parameters for visuals, movement, survival, age, reproduction, herd rules, threat responses, resources, interactions, carcass loot, environment, disease, genetics, and abilities.
- `UHerdScanDatasetComponent`: scans nearby food, water, threats, species, and herd members, producing `FHerdScanDataset`.
- `UHerdAnimalAnimInstance_Base`: Animation Blueprint parent class exposing herd state and current state animation.
- `UHerdDebugWidget`: UMG base widget that builds rows for herd composition, member state, vitals, animations, migration, and threats.
- `AHerdExampleLevelSpawner`: level helper that spawns a manager plus example herd members from data assets.

## Minimal Setup

1. Create one or more Data Assets using `DA_SpeciesData_Base`.
2. Set `SpeciesId`, diet, meshes, animation set, survival rates, lifecycle ranges, threat responses, food/water preferences, and interaction rules.
3. Attach `UHerdMemberComponent` to every animal actor class.
4. Place `AHerdManager_Generic` in the level and assign the species data assets.
5. Either manually register members, enable nearby auto-registration, or place `AHerdExampleLevelSpawner`.
6. Create an Animation Blueprint using `UHerdAnimalAnimInstance_Base` as the parent class and drive states from `HerdState`.

All decisions are data-driven. Species-specific behavior should live in data assets and interaction rules, not in C++ branches.
