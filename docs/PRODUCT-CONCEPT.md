# 125A Noctomorph — Product Concept / Research Baseline

Status: research baseline, not release specification
Branch: `research/sound-database-v0.1`

## Product intent

125A Noctomorph is an evolving cinematic scene instrument.

Primary goals:
- create dark, serious, cinematic environments that evolve over time;
- work as a complete standalone instrument for cinematic composers;
- also scale down naturally into subtle layers for NDH / industrial / modern-metal productions;
- prioritize real recorded source material and use synthesis only where it adds control or fills gaps;
- avoid Halloween, haunted-house, monster, jump-scare and generic horror-library clichés.

## Core sonic principles

- dark != horror;
- evolution should feel like one persistent world changing, not preset hopping;
- long-form movement should be multi-timescale and semi-deterministic;
- events should be sparse enough to retain impact;
- real recordings should be transformed, layered and recombined rather than simply looped;
- tonal/sub foundations may be synthetic or resynthesized where this improves playability;
- full-intensity settings may be cinematic and extreme;
- lower settings must remain mix-usable under guitars, bass, drums and vocals.

## Proposed scene layers

1. FOUNDATION
   - tonal drone
   - sub mass
   - low resonant beds

2. ENVIRONMENT / WORLD
   - exterior wind
   - distant urban night
   - industrial exterior
   - tunnels / underground rooms
   - large concrete / metal spaces

3. TEXTURE
   - electrical hum
   - metal resonance
   - scraping / friction
   - water / pipe / drainage textures
   - room-tone grain

4. MOTION
   - slow stereo drift
   - spectral movement
   - density changes
   - non-periodic modulation

5. TENSION
   - controlled dissonance
   - beating partials
   - resonant emphasis
   - unstable-but-bounded pitch/spectral behavior

6. EVENTS
   - distant rumbles
   - structural groans
   - isolated metal events
   - distant trains / machinery
   - wind surges
   - sparse thunder-like low events where they stay cinematic rather than literal

7. SPACE
   - large spaces and tails
   - 100% = meaningful full-wet spatial maximum where appropriate

8. EVOLVE
   - macro controlling amount/rate/depth of long-term scene mutation
   - 0%: relatively stable
   - 20–50%: musical working range
   - 50–75%: clearly evolving
   - 75–100%: strong/creative long-form transformation

## Architectural inheritance from Mechamorph

Candidate concepts to reuse after code audit:
- note lifecycle;
- Phrase / Continue concept if still musically appropriate;
- SCALE-related playability where suitable;
- deterministic state/recall infrastructure;
- realtime-safe sample handling;
- scene/archetype selection infrastructure;
- VST3 QA / editor lifecycle / automation discipline;
- Demo / Full infrastructure.

Do not copy Mechamorph machine-specific assets or assumptions into Noctomorph without explicit review.

## Preliminary archetype directions

Working names only:
- VOID
- RUINS
- INDUSTRIAL
- WASTELAND
- ABYSS
- NIGHT / NOCTURNE

These are behavior families, not simple presets. Each should eventually own:
- source pools;
- event probabilities;
- density limits;
- pitch/spectral ranges;
- spatial behavior;
- evolution constraints.

## Scaling target

No separate “song” vs “cinematic” engine is required by default.

Instead:
- low-to-moderate settings = usable background layer;
- middle settings = clearly audible atmosphere;
- upper settings = standalone cinematic scene;
- 100% = full intended creative maximum.

Any deviations from the 125A control-scaling standard must be documented technically.
