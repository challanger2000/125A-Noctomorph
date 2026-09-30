# 125A Noctomorph — Sound Source Database v0.1

Status: initial web-research seed list
Branch: `research/sound-database-v0.1`
Date: 2026-09-30

## Licensing gate

Preferred:
1. CC0 / public domain.
2. Original 125A recordings.
3. Explicitly redistributable commercial sources with terms compatible with embedding audio inside a sample-based plugin.

Avoid by default:
- CC-BY-NC;
- unclear licenses;
- sources that permit sync use but prohibit redistribution / supply of the sound itself;
- “royalty-free” libraries whose EULA is intended for finished media projects rather than virtual instruments.

### Sonniss note

The current #GameAudioGDC licence allows commercial media-project use but prohibits distributing/supplying licensed sounds as sounds, including modified/redesigned sounds in sound libraries, sample packs, SDKs or similar products. Therefore do not treat Sonniss GDC material as a Noctomorph embedded-sample source without separate written permission.

## Source-quality / provenance fields

Every accepted asset should eventually record:
- internal asset ID;
- source URL;
- original title;
- creator/uploader;
- licence at acquisition date;
- acquisition date;
- original format / sample rate / bit depth / channels;
- original duration;
- source type: real field recording / electromagnetic field recording / synthetic;
- edits performed;
- derived-file checksum;
- target layer(s);
- archetype(s);
- loopability / event suitability;
- notes on identifiable voices/music/brands;
- legal-review status;
- audio-QA status.

## Seed candidates — CC0

### Industrial / electrical foundation

#### FS-IND-001 — Factory.wav
Source: https://freesound.org/people/XiiiSamples/sounds/382269/
Creator: XiiiSamples
Licence: CC0
Type: real field recording
Specs: WAV, stereo, 96 kHz / 24-bit, 1:20.912
Description: distant factory at night; industrial room tone + wind.
Potential use:
- INDUSTRIAL environment bed
- low-passed / stretched foundation
- mid/high isolated texture beds
Priority: HIGH

#### FS-IND-002 — assembly line factory atmo field recording
Source: https://freesound.org/people/Garuda1982/sounds/463999/
Creator: Garuda1982
Licence: CC0
Type: real field recording
Specs: WAV, stereo, 96 kHz / 24-bit, 1:00.916
Description: assembly line, compressed air, motors, steel/machinery.
Potential use:
- industrial scene activity
- sparse transformed events
- spectral texture extraction
Priority: MEDIUM-HIGH

#### FS-IND-003 — Factory Ambience
Source: https://freesound.org/people/JWS24/sounds/790753/
Creator: JWS24
Licence: CC0
Type: real field recording
Specs: WAV, stereo, 44.1 kHz / 16-bit, 1:15.325
Description: very large factory with distant and foreground activity.
Potential use:
- large industrial environment
- distant motion layer
Priority: MEDIUM

#### FS-ELEC-001 — Birmingham canal transformer
Source: https://freesound.org/people/keithpeter/sounds/107215/
Creator: keithpeter
Licence: CC0
Type: real field recording
Specs: WAV, stereo, 44.1 kHz / 16-bit, 1:12.220
Description: large electrical distribution transformer; magnetostriction beating.
Potential use:
- FOUNDATION drone
- TENSION beating partials
- electrical WORLD layer
Priority: VERY HIGH

#### FS-ELEC-002 — Power Transformer Waterloo Station London-2
Source: https://freesound.org/people/kiefspoon/sounds/144559/
Creator: kiefspoon
Licence: CC0
Type: real field recording
Specs: AIFF, stereo, 48 kHz / 16-bit, 4:58.714
Description: long real transformer hum near rail lines.
Potential use:
- persistent electrical foundation
- spectral / tonal extraction
Priority: HIGH

#### FS-ELEC-003 — solar inverter humming static noise
Source: https://freesound.org/people/DrNI/sounds/613916/
Creator: DrNI
Licence: CC0
Type: real field recording
Specs: FLAC, stereo, 48 kHz / 24-bit, 3:48.269
Description: static inverter hum with rhythmic modulation.
Potential use:
- subtle pulsing tension bed
- textural modulation source
Priority: HIGH

#### FS-ELEC-004 — Computer Monitor Power Supply Converter EMF
Source: https://freesound.org/people/bassimat/sounds/869579/
Creator: bassimat
Licence: CC0
Type: electromagnetic field recording
Specs: WAV, mono, 48 kHz / 32-bit, 0:16.146
Description: raw power-converter EMF capture.
Potential use:
- microscopic texture
- transformed electrical events
Priority: MEDIUM-HIGH

### Underground / concrete / enclosed worlds

#### FS-UND-001 — Underground metro station room tone
Source: https://freesound.org/people/kyles/sounds/450924/
Creator: kyles
Licence: CC0
Type: real field recording
Specs: FLAC, stereo, 48 kHz / 24-bit, 1:12.875
Description: large underground metro station; heavy ventilation and distant echo voices.
Potential use:
- large concrete WORLD
- ventilation body
Caution:
- distant voices are identifiable content risk; audition and potentially use only voice-free sections.
Priority: HIGH, CONDITIONAL

#### FS-UND-002 — ambientetunel agua.wav
Source: https://freesound.org/people/JOAQUIND/sounds/327935/
Creator: JOAQUIND
Licence: CC0
Type: real field recording
Specs: WAV, stereo, 44.1 kHz / 24-bit, 0:53.786
Description: abandoned sewer tunnel with substantial water.
Potential use:
- underground WATER environment
- spectral texture
Priority: HIGH

#### FS-UND-003 — Big Dig tour 12
Source: https://freesound.org/people/alienistcog/sounds/123744/
Creator: alienistcog
Licence: CC0
Type: real field recording
Specs: AIFF, stereo, 44.1 kHz / 16-bit, 0:47.806
Description: harbor tunnel under construction; machine pings, drainage water, hum.
Potential use:
- underground industrial texture
- isolated machine-ping events
Priority: HIGH

#### FS-UND-004 — Commons Tunnel.wav
Source: https://freesound.org/people/rawhiteman7/sounds/381160/
Creator: rawhiteman7
Licence: CC0
Type: real field recording
Specs: WAV, stereo, 44.1 kHz / 16-bit, 0:52.863
Description: rushing water in a tunnel followed by rocks bouncing off walls.
Potential use:
- tunnel world
- sparse stone/echo events
Priority: HIGH

### Wind / weather / exterior mass

#### FS-WIND-001 — Wind - Low Frequency, Warm and Strong
Source: https://freesound.org/people/bassimat/sounds/861757/
Creator: bassimat
Licence: CC0
Type: real field recording
Specs: WAV, stereo, 48 kHz / 32-bit, 1:15.418
Description: strong wind with deep low-frequency tones and sustained movement.
Potential use:
- WASTELAND / VOID environment
- evolving low-mid texture
Priority: VERY HIGH

#### FS-WIND-002 — Very high wind, debris and leaves thrown
Source: https://freesound.org/people/TheSurfboardingGiraffe/sounds/699674/
Creator: TheSurfboardingGiraffe
Licence: CC0
Type: real field recording
Specs: FLAC, stereo, 48 kHz / 16-bit, 16:10.575
Description: strong storm wind, debris, intermittent impacts.
Potential use:
- long-form exterior world
- event mining
Caution:
- birds present; avoid literal nature ambience where it weakens the dark-cinematic target.
Priority: MEDIUM-HIGH

#### FS-STORM-001 — Distant Thunder
Source: https://freesound.org/people/Beetlemuse/sounds/530133/
Creator: Beetlemuse
Licence: CC0
Type: real field recording
Specs: WAV, stereo, 44.1 kHz / 16-bit, 2:51.280
Description: distant storm approaching.
Potential use:
- low-frequency event extraction
- transformed tension swells
Rule:
- avoid obvious “thunder sample” usage; process toward distant cinematic mass.
Priority: HIGH

#### FS-STORM-002 — Midwest low booming thunder
Source: https://freesound.org/people/kvgarlic/sounds/847408/
Creator: kvgarlic
Licence: CC0
Type: real field recording
Specs: WAV, mono, 48 kHz / 24-bit, 8:31.524
Description: thunderstorm line with low booming/rumbling and no sharp cracks.
Potential use:
- ideal low-frequency EVENTS source
- long tension swells
Priority: VERY HIGH

### Urban night / distant movement

#### FS-URB-001 — traffic ambience at night.wav
Source: https://freesound.org/people/soundofsong/sounds/640635/
Creator: soundofsong
Licence: CC0
Type: real field recording
Specs: WAV, mono, 44.1 kHz / 16-bit, 1:00.375
Description: atmospheric suburban traffic at night; individual sources indistinct.
Potential use:
- distant night-world bed
- low-level NDH layer
Priority: HIGH

#### FS-URB-002 — City Night field recording
Source: https://freesound.org/people/LeoGuimaray99/sounds/590763/
Creator: LeoGuimaray99
Licence: CC0
Type: real field recording
Specs: WAV, stereo, 48 kHz / 24-bit, 8:32.759
Description: city-at-night ambience from a window.
Potential use:
- large long-form urban background
Priority: HIGH

#### FS-URB-003 — Downtown LA late-night semi-distant traffic
Source: https://freesound.org/people/janbezouska/sounds/330427/
Creator: janbezouska
Licence: CC0
Type: real field recording
Specs: WAV, stereo, 48 kHz / 24-bit, 3:44.640
Description: distant late-night city/traffic atmosphere.
Potential use:
- distant urban mass
- subtle WORLD layer
Priority: HIGH

#### FS-URB-004 — Berlin rooftop ambience
Source: https://freesound.org/people/danner/sounds/426894/
Creator: danner
Licence: CC0
Type: real field recording
Specs: WAV, stereo, 96 kHz / 24-bit, 5:45.521
Description: distant city, traffic, occasional people/aircraft, low motor rumble.
Potential use:
- high-resolution urban texture mining
Caution:
- voices / aircraft may need segmentation.
Priority: MEDIUM-HIGH

#### FS-URB-005 — suburban night, distant traffic, no bugs
Source: https://freesound.org/people/milesfg/sounds/685772/
Creator: milesfg
Licence: CC0
Type: real field recording
Specs: WAV, stereo, 48 kHz / 24-bit, 0:59.439
Description: clean stereo nighttime suburban background, explicitly no bugs/birds/sirens.
Potential use:
- clean neutral night bed
Priority: VERY HIGH

### Rail / distant mechanical travel

#### FS-RAIL-001 — Munich Ambience Night Train
Source: https://freesound.org/people/TSP-Talk/sounds/643045/
Creator: TSP-Talk
Licence: CC0
Type: real field recording
Specs: WAV, stereo, 192 kHz / 32-bit, 0:44.752
Description: Munich night ambience with train in background.
Potential use:
- high-resolution distant event / world movement
Priority: VERY HIGH

#### FS-RAIL-002 — Distant Train with Ambience
Source: https://freesound.org/people/azumarill/sounds/760524/
Creator: azumarill
Licence: CC0
Type: real field recording
Specs: WAV, stereo, 44.1 kHz / 16-bit, 24:23.700
Description: very long distant train environment with horn and motor.
Potential use:
- long environmental evolution source
Caution:
- horn is identifiable/literal; segment carefully.
Priority: MEDIUM-HIGH

### Metal / sparse events

#### FS-MET-001 — Metal Scrape
Source: https://freesound.org/people/magnuswaker/sounds/530075/
Creator: magnuswaker
Licence: CC0
Type: real recorded / designed metal source
Specs: WAV, stereo, 96 kHz / 24-bit, 0:01.229
Description: shrill resonant metal-panel scrape.
Potential use:
- rare transformed metallic event
- granular tail seed
Rule:
- do not use as horror sting; keep sparse and environmental.
Priority: HIGH

#### FS-MET-002 — Underground parking elevator mechanical ambience
Source: https://freesound.org/people/JoanCalsina/sounds/867596/
Creator: JoanCalsina
Licence: CC0
Type: real field recording
Specs: M4A, mono, 48 kHz, 0:52.159
Description: heavy doors, motor hum, cable tension, deep metal resonances in concrete shaft.
Potential use:
- resonance/event mining
- industrial/underground archetypes
Caution:
- lossy M4A source; prioritize for event extraction, not pristine full-band bed.
Priority: HIGH

### Water / pipes

#### FS-WATER-001 — water_running_pipe_loop.wav
Source: https://freesound.org/people/eardeer/sounds/443868/
Creator: eardeer
Licence: CC0
Type: real field recording
Specs: WAV, stereo, 48 kHz / 24-bit, 0:26.015
Description: drainage pipe, seamless water ambience.
Potential use:
- underground texture layer
- filtered / granular movement
Priority: MEDIUM-HIGH

## Derived / transformed CC0 candidates

Use only after provenance of the original source chain is checked.

#### FS-DER-001 — Deep Electromagnetic Low-Frequency Drone
Source: https://freesound.org/people/bassimat/sounds/869334/
Creator: bassimat
Licence: CC0
Type: transformed electromagnetic field recording
Specs: WAV, stereo, 48 kHz / 24-bit, 2:03.349
Description: transformed EMF into dense low-frequency drone.
Potential use:
- benchmark/reference for the desired direction;
- possible production source if provenance chain remains CC0.
Priority: RESEARCH FIRST

#### FS-DER-002 — Deep LF Drone with Glitch Impacts from EMF
Source: https://freesound.org/people/bassimat/sounds/869201/
Creator: bassimat
Licence: CC0
Type: transformed electromagnetic field recording
Specs: WAV, stereo, 48 kHz / 24-bit, 2:00.896
Description: deep drone plus electrical pulses/impacts.
Potential use:
- reference for event-density behavior;
- source only after original-pack provenance check.
Priority: RESEARCH FIRST

## Rejection / caution examples

- recordings containing obvious music: generally reject or segment away;
- recordings dominated by identifiable speech: reject unless clean non-speech portions can be isolated;
- horror-specific designed screams, monster voices, heartbeat clichés, haunted-house chains: reject as aesthetic mismatch;
- obvious cinematic braams/risers from third-party libraries: reject unless we synthesize/record our own;
- generic premade dark drones: use mainly as reference; prioritize real-source derivation so Noctomorph has its own identity.

## Next search queues

1. large empty concrete halls / bunkers / parking structures, CC0;
2. bridge resonance / structural wind / cable movement, CC0;
3. shipyard / dock / harbor industrial night, CC0;
4. HVAC / ventilation / server rooms / power rooms, CC0;
5. distant heavy machinery without speech, CC0;
6. metal flex / groan / friction, high-sample-rate CC0;
7. ice / rock / earth / gravel sources that read as texture rather than “nature ambience”;
8. low wind through apertures / structures;
9. long clean room tones suitable for loop/evolution analysis;
10. original 125A recordings to replace weak/noisy external candidates.

## Processing research ideas

Candidate transformations to test, measure and compare:
- multi-band decomposition;
- extreme high-quality resampling / pitch shifting;
- transient vs sustained separation;
- granular event re-orchestration;
- long-window spectral freeze/morph for selected layers;
- deterministic random event scheduling;
- statistical texture variation rather than naive looping;
- FDN / velvet-noise derived spaces;
- dynamic filtering tied to scene evolution;
- low-frequency synthesis layered underneath real recordings only where needed.

Any irreversible production processing should preserve the original source and metadata.
