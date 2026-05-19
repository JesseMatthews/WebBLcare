# DA_SpeciesData_Base Template

Create a Primary Data Asset using `DA_SpeciesData_Base`. The C++ class ships with defaults, then expose and tune these groups in the editor.

## Key Parameter Groups

- Identity: species id, display name, tags, diet, role, docile, territorial, mixed-herd support.
- Visual: skeletal mesh, physics asset, anim blueprint, idle/walk/run/flee/scatter/chase/alert/attack/graze/special/drink/rest/death animations, materials, scale, capsule, mesh offset, retarget profile.
- Movement: walk/run/sprint/flee/chase speeds, acceleration, rotation, wander, separation, cohesion, alignment, leader follow, avoidance, slope, altitude.
- Survival: health, stamina, hunger, thirst, rates, stamina drain/recovery, regen, starvation, dehydration, exhausted speed, critical threshold.
- Lifecycle: calf, juvenile, adult, elder, ancient ranges and multipliers.
- Reproduction: fertility, pregnancy duration, birth rate, litter size, cooldown, mate radius, adult age, protectiveness, follow duration, mate requirement, seasonal breeding.
- Herd behavior: herd hunger mode, individual hunger mode, stampede, defensive circle, migration, alert distance, panic, calm down, cohesion/separation/alignment/leader weights, thresholds, regroup, circle radius, herd size, migration mode.
- Threat responses: base fear/aggression, memory, scan radius, player/environment weights, attack/flee flags, predator/prey/friendly/neutral species lists, per-threat response rows.
- Food and water: preferred food rows, food values, water sources, water values, graze/drink time, forage/detection, claim radii, moving graze, scavenge, weak-prey preference.
- Interactions: per-species rules plus predator-predator, herbivore-herbivore, predator-herbivore defaults.
- Carcass and loot: meat, hide, bone, tusks, horns, pelt quality, rot stages, scavenger attraction, tags.
- Environment: activity cycle, preferred/avoided terrain, temperature, day/night weights, weather avoidance, seasonal availability.
- Disease and genetics: susceptibility, immunity, spread radius, infection panic, resistant diseases, mutation, inheritance, bloodline, trait weights.
- Special abilities: type, id, cooldown, range, power, required tags.

## Design Rule

Do not subclass C++ for a specific species unless you need custom animation or movement implementation. For behavior, prefer species data values, threat response rows, and inter-species interaction rules.
