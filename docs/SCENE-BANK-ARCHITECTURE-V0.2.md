# Noctomorph Scene Bank Architecture v0.2

Status: implementation target
Date: 2026-09-30

## Why v0.1 is insufficient

The first host prototype proved VST3, state, automation, GUI and embedded-resource
handling, but it is not representative of the intended instrument.

The prototype bank contains only:
- 1 WORLD bed;
- 1 TEXTURE bed;
- 2 BODY exciters;
- 1 EVENT source.

That architecture necessarily causes:
- too much shared sonic identity between archetypes;
- audible repetition;
- recognizable bell/gong cues;
- a dominant synthetic drone/foundation;
- insufficient cinematic scale;
- insufficient long-form world evolution.

The researched source database is much larger than the embedded prototype bank.
v0.2 must use that research as actual production input.

## v0.2 scene model

Each archetype owns a dedicated scene bank.

Minimum per archetype:

- 3 WORLD beds
- 3 TEXTURE beds
- 2 BODY exciters
- 4 sparse EVENT sources
- 2 SPACE/IR choices or measured-space references
- synthetic FOUNDATION family specific to that archetype

Target initial production pool:
- 18+ WORLD clips
- 18+ TEXTURE clips
- 12+ BODY exciters
- 24+ EVENT clips
- multiple measured/hybrid spaces

Sources can be shared only when transformed/routed so that their role differs
substantially. No single recording may become the audible identity of all six
archetypes.

## Runtime architecture

WORLD:
- 3 concurrent stream voices;
- deterministic start positions;
- independent transport rates;
- 20–90 second crossfade times;
- slow gain redistribution driven by EVOLVE;
- no hard loop identity.

TEXTURE:
- 3 concurrent stream voices;
- different spectral bands/roles;
- independent motion;
- one source may drop out completely for long periods.

BODY:
- modal body remains synthetic/physical-modelled;
- 2-source excitation pool per archetype;
- recorded exciter is never directly audible;
- no automatic recognizable hit on Note-On.

EVENTS:
- pool of at least 4 per archetype;
- EVENTS=0 means none;
- low settings genuinely sparse;
- no event forced at note start;
- event type selection deterministic from seed/state;
- no globally shared bell signature.

SPACE:
- archetype-specific FDN calibration plus measured/hybrid space references;
- 100% remains true wet;
- space size/tail should contribute to identity, not merely decorate it.

FOUNDATION:
- per-archetype oscillator/resonator topology;
- reduced default dominance;
- should support the world rather than mask it;
- VOID/ABYSS may be low-heavy for different reasons;
- RUINS/NOCTURNE must not collapse to the same drone with a different EQ.

## Archetype source direction

### VOID
- sparse structural/contact-mic motion
- low/no literal environment bed
- spectral emptiness, long gaps, distant pressure
- no bell default

### RUINS
- cathedral/church/stone/abandoned architecture
- wood/stone/metal structural motion
- long natural resonances
- occasional distant structural event

### INDUSTRIAL
- large factory/warehouse
- transformer/EMF/hydraulic layers
- machine mass at distance
- denser but not Mechamorph-like foreground machinery

### WASTELAND
- structural wind, exposed metal/cable, harbor/distant exterior
- moving spectral texture
- less tonal mass, more air/motion/scale
- avoid generic weather-loop identity

### ABYSS
- deep body resonance, cave/mine/underground pressure
- frozen/structural sub events
- darkest low-frequency mass
- different from VOID: heavy and physically present, not empty

### NOCTURNE
- dark sacred/architectural night world
- subtle formant/organ-like/spectral presence
- cathedral/urban-night layers
- bell permitted only as rare optional event, never default onset

## Acceptance criteria before next user listening build

Do not ship another listening VST3 until:

1. At least 3 archetypes use genuinely different embedded WORLD source sets.
2. No archetype begins with a recognizable bell/gong by default.
3. Default EVENTS = 0.
4. Default FOUNDATION is subordinate to real/physical layers.
5. A/B renders of all six show clear differences in:
   - spectral centroid;
   - low-frequency energy;
   - stereo correlation;
   - temporal density;
   - event density;
   - source identity.
6. 30 s and 120 s renders of the same archetype remain recognizably the same
   world while changing internally.
7. At least one archetype already reads as a large cinematic environment
   without requiring the user to raise every knob.

This is the minimum bar for the next listening build.
