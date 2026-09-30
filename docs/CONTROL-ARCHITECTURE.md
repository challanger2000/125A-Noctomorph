# Noctomorph Public Control Architecture

Status: **FROZEN FOR GUI PROTOTYPING**
Date: 2026-09-30
Branch: `dev/prototype-v0.1`

The control set is now complete enough to begin static GUI design.

## Public parameters

1. **ARCHETYPE**
   - VOID
   - RUINS
   - INDUSTRIAL
   - WASTELAND
   - ABYSS
   - NOCTURNE
   - behavior family, not preset selection.

2. **FOUNDATION**
   - tonal/sub foundation amount.
   - 0 % = no foundation layer.
   - upper range = strong cinematic tonal mass.

3. **WORLD**
   - real-world/environment body.
   - 0 % = no WORLD layer.
   - scales real architectural/industrial scene material.

4. **TEXTURE**
   - detail/friction/electrical/grain layer.
   - 0 % = no texture layer.
   - upper range = dense physical surface/detail.

5. **BODY**
   - impossible resonant-body response.
   - controls the modal/physical-body contribution.
   - real BODY sources are exciters, not parallel playback.

6. **TENSION**
   - inharmonicity / dissonance / unstable-but-bounded behavior.
   - 0 % = least tense valid scene.
   - upper range = strong spectral/pitch tension.

7. **MOTION**
   - immediate scene movement depth.
   - affects slow stereo drift, spectral motion, oscillator drift and source-motion variance.
   - 0 % = relatively stationary scene.
   - independent from EVOLVE.

8. **EVOLVE**
   - long-term scene mutation.
   - controls rate/depth of deterministic state evolution over long time.
   - 0 % = relatively stable state.
   - 20–50 % = musical working range.
   - 50–75 % = clearly evolving.
   - 75–100 % = strong long-form transformation.

9. **EVENTS**
   - sparse event probability / density.
   - 0 % = events disabled.
   - low values remain cinematic punctuation, not constant random triggering.
   - 100 % = densest intended event regime.

10. **SPACE**
    - spatial amount.
    - 0 % = dry.
    - 100 % = meaningful full-wet output.
    - later production path may combine algorithmic and measured/hybrid spaces.

11. **OUTPUT**
    - final output trim.
    - 0 % = silence.
    - 50 % = current nominal default.
    - 100 % = maximum intended output before safety ceiling.

## MOTION vs EVOLVE

These are deliberately separate.

**MOTION** answers:
> How much is the world moving right now?

Examples:
- slow stereo drift;
- spectral movement;
- source transport variance;
- oscillator drift depth.

**EVOLVE** answers:
> How strongly does this world change over tens of seconds to minutes?

Examples:
- deterministic-chaos trajectory speed;
- long-state mutation;
- event acceleration;
- long-term presence/resonance drift.

A scene can therefore be:
- still but slowly evolving;
- actively moving but structurally stable;
- both;
- neither.

## Archetype relationship

Archetypes apply internal behavior traits without overwriting public parameter values.

This preserves automation logic:
- 0 % remains off where applicable;
- parameter automation stays continuous;
- changing archetype changes the world's response, not the stored knob positions.

See `docs/ARCHETYPE-BEHAVIOR-FAMILIES.md`.

## State contract

Current state format: **v2**

- v1 stored 10 values.
- v2 adds MOTION as the 11th stored value.
- v1 loads are supported and receive MOTION = **35 %**.
- existing first ten parameter IDs and state ordering are preserved.
- MOTION uses new ParamID `3010`; no previous IDs moved.

Native state/recall QA confirms v1 -> v2 migration.

## GUI implications

Static GUI design should accommodate:

- one clearly visible ARCHETYPE selector;
- eight main scene-shaping controls:
  FOUNDATION / WORLD / TEXTURE / BODY / TENSION / MOTION / EVOLVE / EVENTS;
- SPACE as a spatial control;
- OUTPUT as a utility/final-level control.

Recommended grouping:

- **WORLD**: FOUNDATION, WORLD, TEXTURE, BODY
- **BEHAVIOR**: TENSION, MOTION, EVOLVE, EVENTS
- **SPACE / OUTPUT**: SPACE, OUTPUT
- **IDENTITY**: ARCHETYPE

Do not expose internal research parameters, chaos coordinates, modal ratios,
formant centers or source-file selection in the main GUI.
