# Archetype Behavior Families

Status: 2026-09-30
Branch: `dev/prototype-v0.1`

## Decision

The six Noctomorph archetypes are now **behavior families**, not preset labels.

They do not overwrite the user parameters. The public controls keep their
normal semantics:
- 0 % remains off / neutral where applicable;
- 100 % remains the user's requested maximum amount.

The archetype changes how the internal world interprets those amounts.

## Internal traits

| Archetype | Foundation | World | Texture | Body | Event rate | World motion | Texture motion |
|---|---:|---:|---:|---:|---:|---:|---:|
| VOID | 1.15 | 0.55 | 0.55 | 1.25 | 0.55 | 0.82 | 0.75 |
| RUINS | 0.75 | 1.00 | 1.10 | 0.90 | 0.75 | 0.95 | 0.85 |
| INDUSTRIAL | 0.70 | 1.15 | 1.20 | 0.95 | 1.20 | 1.00 | 1.00 |
| WASTELAND | 0.60 | 1.20 | 1.30 | 0.70 | 0.85 | 1.05 | 1.12 |
| ABYSS | 1.10 | 0.75 | 0.65 | 1.30 | 0.65 | 0.78 | 0.72 |
| NOCTURNE | 0.85 | 1.00 | 0.90 | 0.90 | 0.60 | 0.88 | 0.82 |

These are prototype calibration values, not public UI values.

## Semantic intent

### VOID
Deepest and sparsest. Foundation/BODY dominate, source motion is slow, event
rate is reduced.

### RUINS
Stone/architectural identity. Less foundation, more texture and physical-space
detail.

### INDUSTRIAL
Most machine-forward. WORLD/TEXTURE are emphasized and event activity is
highest.

### WASTELAND
Exposed, lean and textural. Strong moving WORLD/TEXTURE, reduced BODY mass.

### ABYSS
Heavy, low and physically impossible. FOUNDATION/BODY dominate with restrained
source motion.

### NOCTURNE
Dark, slow, Gothic-adjacent, with the accepted abstract formant Presence layer.
It remains less low-dominant than VOID/ABYSS.

## Regression rule

A dedicated core regression test renders all six archetypes with identical:
- seed;
- note;
- user parameters;
- duration.

All six output hashes must differ. This prevents a future refactor from
collapsing the archetypes back into cosmetic labels.

## Current listening evidence

Latest archetype listening QA after the behavior-family update:

- INDUSTRIAL centroid: ~285 Hz, <120 Hz energy ~31.4 %, stereo corr ~0.532
- RUINS centroid: ~628 Hz, <120 Hz energy ~10.6 %, stereo corr ~0.184
- NOCTURNE centroid: ~345 Hz, <120 Hz energy ~14.1 %, stereo corr ~0.311
- ABYSS centroid: ~132 Hz, <120 Hz energy ~46.2 %, stereo corr ~0.587
- WASTELAND centroid: ~405 Hz, <120 Hz energy ~7.9 %, stereo corr ~0.366
- VOID centroid: ~112 Hz, <120 Hz energy ~72.0 %, stereo corr ~0.547

The families are therefore separated in both implementation and measured output.
