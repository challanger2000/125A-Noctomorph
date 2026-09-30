# Noctomorph Static GUI Design Baseline

Status: **LAYOUT BASELINE**
Date: 2026-09-30
Branch: `dev/prototype-v0.1`

This document defines geometry and hierarchy before any VSTGUI implementation.

## Canvas

- Base editor: **1120 x 620**
- Target zoom support later: 100 % / 150 %
- HiDPI assets required.
- No DAW chrome inside the product rendering.

## Visual direction

Noctomorph should read as:
- dark;
- architectural;
- cinematic;
- cold / Gothic-adjacent;
- industrial without becoming “factory control panel” cliché;
- premium and restrained.

Avoid:
- Halloween motifs;
- skulls / blood / horror props;
- bright sci-fi neon;
- generic waveform/spectrum decoration;
- fake VU meters with no functional purpose;
- overly busy labels.

Material language:
- near-black / graphite faceplate;
- subtly segmented panels;
- restrained metal edges;
- real depth/shadows;
- small 125A hardware-style screws;
- low-contrast cold highlights;
- one controlled accent state for active archetype/interaction.

## Layout hierarchy

### Header — 0..72 px

Left:
- 125A logo.

Center:
- NOCTOMORPH.

Right:
- small version text.
- no large marketing subtitle.

### Archetype rail — 82..150 px

Six equal selectors:

VOID · RUINS · INDUSTRIAL · WASTELAND · ABYSS · NOCTURNE

Behavior:
- one active at a time;
- active state obvious but not glowing excessively;
- equal widths;
- typography centered;
- no dropdown needed.

Suggested bounds:
- x = 90..1030
- total width = 940
- six cells ≈ 156 px each
- height ≈ 44 px

### Main deck — 176..548 px

Two control rows, each five columns.

Column centers:
- 132
- 346
- 560
- 774
- 988

Top row centers around y = 292:

1. FOUNDATION
2. WORLD
3. TEXTURE
4. BODY
5. SPACE

Bottom row centers around y = 462:

1. TENSION
2. MOTION
3. EVOLVE
4. EVENTS
5. OUTPUT

This gives a clean 5 x 2 geometry and keeps SPACE/OUTPUT as the right utility column.

## Grouping

Visually use panel segmentation rather than extra text wherever possible.

Top:
- first four controls = WORLD / MATERIAL family;
- SPACE separated by a subtle vertical panel break.

Bottom:
- first four controls = BEHAVIOR family;
- OUTPUT separated by the same right-column break.

The user should understand hierarchy from geometry before reading section labels.

## Knobs

Main scene knobs:
- FOUNDATION / WORLD / TEXTURE / BODY
- TENSION / MOTION / EVOLVE / EVENTS

Suggested visible diameter:
- 92–104 px

SPACE:
- same family but may be 8–12 % larger.

OUTPUT:
- slightly smaller than SPACE, utility role.

Knob behavior later:
- 0 % / 100 % clearly bounded;
- default position marked;
- CTRL + left click reset;
- automation-safe;
- no stepped fake detents for continuous parameters.

## Value display

Preferred:
- compact percentage below or immediately inside each control zone.
- do not permanently print excessive explanatory text.

ARCHETYPE does not need a numeric value.

## Labels

Uppercase, concise:
- FOUNDATION
- WORLD
- TEXTURE
- BODY
- SPACE
- TENSION
- MOTION
- EVOLVE
- EVENTS
- OUTPUT

No alternative marketing names until the actual engine semantics are frozen.

## Decorative scene element

A subtle central background motif is permitted only if it remains behind the controls.

Candidate direction:
- impossible dark architecture;
- distant stone / industrial silhouette;
- abstract monumental depth;
- barely visible cold atmospheric gradient.

It must not:
- contain literal ravens, skulls, church windows or obvious horror imagery;
- reduce label contrast;
- look like stock art pasted under controls.

The GUI should remain a professional instrument first.

## Interaction states

Required later:
- normal
- hover
- active/drag
- default/reset feedback
- archetype-selected
- disabled/bypass only if a bypass control is later justified

No animated decoration is required for v1.

## Geometry rule

Before VSTGUI coding:
1. produce static faceplate at 1120 x 620;
2. overlay exact control-center guides;
3. validate label width and spacing;
4. create one final knob family;
5. create archetype selector assets;
6. only then wire editor controls.

No rebuilding the faceplate around already-implemented widgets.
