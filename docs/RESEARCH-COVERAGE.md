# Noctomorph Research Coverage Matrix

Status date: 2026-09-30

This document answers one question: **do we now have enough research material to begin architecture/prototyping without obvious source-family blind spots?**

Legend:
- **STRONG** — several credible candidates / references exist.
- **ADEQUATE** — enough to prototype, but not necessarily final production selection.
- **REFERENCE ONLY** — research exists, but licensing or identity rules prohibit direct embedding.
- **OPEN** — meaningful gap remains.

## 1. Physical source coverage

| Role | Status | Current evidence |
|---|---|---|
| Industrial interiors / machinery | STRONG | factories, hydraulic/mine machinery, utility rooms, hydro plants, printing press/projector leads, OpenGameArt/BigSoundBank pools |
| Electrical / electromagnetic | STRONG | transformer, inverter, converter EMF, substation harmonics, utility infrastructure |
| Structural metal under stress | STRONG | cable/post, fences, barbed wire, wire bundles, gates, large plates/tanks, contact mics |
| Large hollow bodies | STRONG | pipes, iron domes, oil/water/propane tanks, cages, large metal objects |
| Rail / transport resonance | STRONG | ICE contact-mic vibration, stereo/contact train source, freight-train structural pass, distant horns |
| Ice / cold physical systems | STRONG | frozen lakes, puddle cracks, contact-mic icefield family |
| Underground / tunnel / cave | STRONG | sewers, Big Dig tunnel recordings, cave chambers, mine, sinkhole, underground water |
| Stone / ruins microtexture | STRONG | shell/stone scrapes, cave/stone spaces, heritage IRs |
| Water / hydrophone | STRONG | drainage, sink hydrophone, long dock/plumbing hydrophone, cave/subterranean water |
| Glass / brittle resonators | STRONG | wine glass, crystal glass, glass plate, glass scrape/ceramic micro-events |
| Wood / old structure | ADEQUATE | old furniture/wood creaks, building noises, bell-tower friction |
| Bowed / frictional metal | STRONG | singing saw, bowed-cymbal texture, metal-on-metal contact sources |
| Tonal acoustic seeds | STRONG | organ one-shots, cello/bow sources, hurdy-gurdy string, gong/tam-tam families |
| Animal / nocturnal sparse events | STRONG | ravens/crows, owls, distant wolves, distant dog |
| Human-presence sparse events | ADEQUATE | tunnel footsteps / distant human traces; deliberately not core material |
| Weather / nature | STRONG but DE-EMPHASIZED | wind/storm/rain/water exist only as supporting raw material, not product identity |

## 2. Spatial / room coverage

| Role | Status | Current evidence |
|---|---|---|
| Cathedral / church | STRONG | CC0 cathedral IR + Aachen Cathedral B-format dataset + multiple church field recordings |
| Gothic stone / mausoleum / tomb | STRONG | OpenAIR Hamilton Mausoleum, Maes Howe, cathedral/chapel sources |
| Industrial hall / warehouse | STRONG | OpenAIR R1 Reactor Hall, Terry's Factory Warehouse, real factory/warehouse recordings |
| Concrete / garage | STRONG | parking-garage IRs and concrete/underground recordings |
| Tunnel / enclosed infrastructure | STRONG | Big Dig, city tunnel and underground-source pools |
| Ambisonic / spatial research | STRONG | Aachen B-format, OpenAIR formats, STARSS research |
| Unreal-space construction | STRONG RESEARCH BASIS | hybrid convolution + FDN, IR morphing, early/late decoupling, synthetic late tail hypotheses |

## 3. Synthetic / engine architecture coverage

| Engine concept | Status | References / hypothesis |
|---|---|---|
| Synthetic drone/foundation | STRONG | multi-oscillator / FM / noise / sub references; procedural drone baselines |
| Granular transformation | STRONG | Texture Loom, Grain Storm, Oi Grandad!, Glacier, Panacousticon |
| Multi-timescale evolution | STRONG | separate macro/meso/micro time regimes documented |
| Modal body synthesis | STRONG | modal synth, RipplerX, resonator-bank references |
| Waveguide / physical model | STRONG | Oi Grandad!, MechanOdd, DaisySP references |
| Coupled impossible bodies | STRONG HYPOTHESIS | exciter -> body A -> coupled body B -> space architecture |
| Spectral freeze / memory | STRONG | HISSTools Freeze + spectral-memory hypothesis |
| Spectral morph / partial control | STRONG | Abstructs / freeze-morph / partial-retention references |
| Abstract vocal / phantom presence | STRONG HYPOTHESIS | Pareidolia/formant-grain approach; avoids literal choir dependency |
| Deterministic-chaos modulation | STRONG HYPOTHESIS | Lorenz/Rössler/coupled-system references |
| State-space evolution | STRONG | constrained trajectory / map-based evolution concept |
| Recursive regeneration | STRONG | Ambiotica-style bounded regeneration concept |
| Behavior/timbre decoupling | STRONG | resynthesis/corpus concepts + real-source/synthetic-body design |
| Sparse event logic | STRONG | state-dependent probability / non-repeating event pool |
| Long-term recall / determinism | STRONG REQUIREMENT | seeded evolution, state recall, non-periodic but repeatable trajectories |

## 4. Product-identity coverage

| Requirement | Status |
|---|---|
| Dark always present | STRONG / LOCKED |
| Cinematic scale | STRONG / LOCKED |
| Gothic influence without cliché | STRONG / LOCKED |
| Industrial reality | STRONG / LOCKED |
| Unreal / impossible worlds | STRONG / LOCKED |
| Not a weather/nature generator | STRONG / LOCKED |
| Not a horror soundboard | STRONG / LOCKED |
| Not merely a drone generator | STRONG / LOCKED |
| Subtle song-layer behavior | STRONG DESIGN TARGET |
| Full standalone cinematic intensity | STRONG DESIGN TARGET |
| Long evolution without obvious loop | STRONG DESIGN TARGET |
| Real sources first where they add complexity | STRONG / LOCKED |
| Synthesis where control or impossibility requires it | STRONG / LOCKED |

## 5. Licensing coverage

### Production-preferred
- CC0 / Public Domain.
- Original 125A recordings.
- Explicitly redistributable material compatible with embedding.

### Production-possible with obligations
- CC BY 4.0, only where attribution/licence/change notices are deliberately shipped and tracked.

### Research/reference only
- CC BY-NC / NC-SA.
- unclear/custom restrictions.
- sources with questionable underlying-rights chain.
- already-designed cinematic/horror/drone material when it would dilute Noctomorph's identity.

### Rejected principle
A file is not considered safe merely because its upload page says CC0 if the underlying performance/composition/recording chain may still be protected.

## 6. Remaining work before audio prototyping

There is **no major source-family blind spot left**.

Remaining work is now curation and implementation preparation:

1. Select a smaller production shortlist from the database.
2. Acquire/download only the shortlist.
3. Store provenance/checksums.
4. Measure spectral range, noise floor, transient density, modal decay, stereo correlation and loop suitability.
5. Segment long sources into useful regions.
6. Build derived assets.
7. Prototype the minimum engine:
   - FOUNDATION;
   - WORLD/TEXTURE;
   - IMPOSSIBLE BODY;
   - EVENT layer;
   - SPACE;
   - EVOLVE.
8. Compare generated 30 s / 2 min / 5 min outputs for repetition and identity stability.

## Conclusion

**Research coverage is sufficient to stop broad discovery and begin controlled curation/prototyping.**

Continuing generic internet search from this point has sharply diminishing returns. New sources should now be added only when they clearly beat an existing candidate or fill a newly discovered implementation need.
