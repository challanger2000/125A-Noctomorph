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


## Reused research leads from Mechamorph

These are not copied blindly from Mechamorph's release palette. They are re-evaluated specifically for Noctomorph's scene-building goal.

### Large industrial / hall environments

#### MECH-LEAD-001 — Factory_Ambience.wav
Source: https://freesound.org/people/Mortifreshman/sounds/368825/
Licence: CC0
Origin of lead: Mechamorph large-machine source sweep
Potential Noctomorph use:
- distant industrial WORLD bed
- low-level machine-hall depth
- transformed long-form ambience
Priority: HIGH

#### MECH-LEAD-002 — Large Warehouse/Factory Ambience.wav
Source: https://freesound.org/people/fimrod/sounds/278987/
Licence: CC0
Origin of lead: Mechamorph large-machine source sweep
Description: large factory/warehouse environment, loop-oriented.
Potential Noctomorph use:
- INDUSTRIAL / RUINS hall layer
- large-space background texture
Priority: VERY HIGH

#### MECH-LEAD-003 — Industrial factory working 03
Source: https://freesound.org/people/dersinnsspace/sounds/439401/
Licence: CC0
Origin of lead: Mechamorph large-machine source sweep
Description: hard industrial activity in a large naturally reverberant hall.
Potential Noctomorph use:
- sparse hard events inside a large-world bed
- natural-space reference for synthetic reverb design
Priority: HIGH

#### MECH-LEAD-004 — Factory Atmosphere
Source: https://freesound.org/people/RICHERlandTV/sounds/240134/
Licence: CC0
Origin of lead: Mechamorph large-machine source sweep
Potential Noctomorph use:
- rare background clangs
- distant industrial event layer
Priority: MEDIUM-HIGH

#### MECH-LEAD-005 — abandoned warehouse
Source: https://freesound.org/people/Kostrava/sounds/240895/
Licence: CC0
Origin of lead: Mechamorph large-machine source sweep
Description: metal squeeze, rumble, large empty industrial environment.
Potential Noctomorph use:
- RUINS / INDUSTRIAL world bed
- structural movement
- colossal-space atmosphere
Priority: VERY HIGH

### Structural metal / stress

#### MECH-LEAD-006 — Metallic Groan
Source: https://freesound.org/people/hinchinbrook/sounds/496836/
Licence: CC0
Origin of lead: Mechamorph large-machine source sweep
Description: low heavy steel/iron groan from a bunker-door source.
Potential Noctomorph use:
- structural stress event
- stretched low-frequency tension
- rare scene mutation cue
Rule:
- de-literalize; do not present as an obvious door/horror effect.
Priority: VERY HIGH

### Hydraulic / pressure motion

#### MECH-LEAD-007 — industrial_machine_hydraulic
Source: https://freesound.org/people/Kostrava/sounds/271328/
Licence: CC0
Origin of lead: Mechamorph large-machine source sweep
Potential Noctomorph use:
- low industrial bed
- pressure / load motion
- slow transformed scene movement
Priority: HIGH

#### MECH-LEAD-008 — Victorian steam escape valve
Source: https://freesound.org/people/pnwheeler/sounds/832100/
Licence: CC0
Origin of lead: Mechamorph pressure-source research
Description: real escape valve under a Victorian steam-driven beam engine.
Potential Noctomorph use:
- pressure-release event extraction
- hiss/chuff microtexture
Rule:
- keep non-literal and sparse; avoid “steampunk” identity.
Priority: MEDIUM-HIGH

### Long coherent mechanical-state recordings

#### MECH-LEAD-009 — Heidelberg printing press family
Representative source: https://bigsoundbank.com/heidelberg-printing-press-4-s3407.html
Licence: CC0/public-domain-equivalent on asset page
Origin of lead: Mechamorph deep source sweep
Potential Noctomorph use:
- not as a foreground machine
- mine low body resonance, room, spin-up/down, and cyclic distant motion
Priority: RESEARCH

#### MECH-LEAD-010 — 35mm cinema projector family
Representative source: https://bigsoundbank.com/35mm-cinema-projector-7-s0071.html
Licence: CC0/public-domain-equivalent on asset page
Origin of lead: Mechamorph deep source sweep
Potential Noctomorph use:
- subtle cyclic high-frequency motion
- start/stop transition texture
- transformed transport flutter
Priority: RESEARCH

## Noctomorph-specific reuse rule

A Mechamorph source is useful for Noctomorph only when at least one of these is true:
- it contributes environmental space rather than literal machine identity;
- it contains structural resonance / low-frequency mass;
- it yields sparse events that can be de-literalized;
- it has long coherent motion useful for scene evolution;
- it contains real-world complexity difficult to synthesize convincingly.

Reject or deprioritize:
- small obvious clockwork;
- foreground ratchets;
- clearly identifiable switches/camera clicks;
- anything that turns the scene into “a machine performance” rather than a dark world.


## Broad-search expansion — 2026-09-30

### Contact-mic / structural resonance

#### FS-STRUCT-001 — Post vibrating (contact microphone) in Illinois
Source: https://freesound.org/people/felix.blume/sounds/198736/
Creator: Felix Blume
Licence: CC0
Type: real contact-mic field recording
Specs: WAV, mono, 96 kHz / 24-bit, 1:30.420
Description: metal post / cable system vibrating and knocking under wind excitation.
Potential use:
- STRUCTURE layer
- TENSION resonance
- long-form material for pitch/time transformation
Priority: VERY HIGH

#### FS-STRUCT-002 — Wooden Bridge Contact Mic
Source: https://freesound.org/people/DanJGW/sounds/473953/
Creator: DanJGW
Licence: CC0
Type: real contact-mic field recording
Specs: WAV, mono, 44.1 kHz / 16-bit, 0:40.449
Description: bridge structure excited by wind and water, recorded through a metal bolt.
Potential use:
- structural resonator
- hybrid water / body texture
Priority: HIGH

#### FS-STRUCT-003 — Metal Distress, Barbed Wire in Windstorm
Source: https://freesound.org/people/CHallSmith/sounds/870790/
Creator: CHallSmith
Licence: CC0
Type: real contact-mic field recording
Specs: WAV, stereo, 48 kHz / 24-bit, 5:43.240
Description: large steel wire fence under wind load, with stress, hits, sway and vibration.
Potential use:
- WASTELAND / RUINS structural layer
- long evolving tension bed
- sparse metal events
Rule:
- avoid literal “spooky fence” presentation; mine the physical stress/resonance.
Priority: VERY HIGH

#### FS-STRUCT-004 — Metal wire - contact mic
Source: https://freesound.org/people/Salom%C3%A9_Lubczanski/sounds/733841/
Creator: Salomé Lubczanski
Licence: CC0
Type: real contact-mic field recording
Specs: WAV, stereo, 48 kHz / 24-bit, 0:35.762
Description: real metal-wire rustle, wobble and resonance through two contact microphones.
Potential use:
- microtexture
- spectral-grain source
- sparse tension details
Priority: HIGH

### Ice / cold physical worlds

#### FS-ICE-001 — frozen lake
Source: https://freesound.org/people/mentos987/sounds/818918/
Creator: mentos987
Licence: CC0
Type: real field recording
Specs: WAV, stereo, 48 kHz / 16-bit, 0:57.256
Description: naturally cracking frozen lake with cracks propagating and reverberating through water/ice.
Potential use:
- VOID / WASTELAND / NIGHT events
- resonant long-tail crack transformations
- scene evolution cues
Priority: VERY HIGH

#### FS-ICE-002 — Mixpre frozen puddle
Source: https://freesound.org/people/ventrapatte/sounds/843827/
Creator: ventrapatte
Licence: CC0
Type: real field recording
Specs: WAV, stereo, 48 kHz / 24-bit, 5:50.430
Description: prolonged creaking, bubbling, gurgling and cracking of frozen water.
Potential use:
- long evolving ice texture
- event mining
- granular / spectral source
Caution:
- distant road present; segment selectively.
Priority: HIGH

#### FS-ICE-003 — icefield contact-mic family
Representative sources:
- https://freesound.org/people/blaukreuz/sounds/52151/
- https://freesound.org/s/52144/
Creator: blaukreuz
Licence: CC0
Type: frozen-lake contact-mic recordings
Specs: WAV, stereo, 48 kHz / 24-bit
Description: stones/pebbles exciting a frozen lake recorded with contact microphones.
Potential use:
- resonant ice impulse/event bank
- extreme timestretch seeds
Priority: VERY HIGH

### Technical rooms / ventilation / infrastructure

#### FS-TECH-001 — Utility room front
Source: https://freesound.org/people/blaukreuz/sounds/212781/
Creator: blaukreuz
Licence: CC0
Type: real field recording
Specs: WAV, stereo, 48 kHz / 24-bit, 1:06.965
Description: computers, machinery and very loud air-conditioning hub.
Potential use:
- technical WORLD bed
- ventilation texture
- neutral dark sci-fi foundation
Priority: VERY HIGH

#### FS-TECH-002 — Utility room rear
Source: https://freesound.org/people/blaukreuz/sounds/212780/
Creator: blaukreuz
Licence: CC0
Type: real field recording
Specs: WAV, stereo, 48 kHz / 24-bit, 1:06.965
Description: complementary rear-channel capture of the same utility room.
Potential use:
- alternate spatial perspective
- source for layered / decorrelated technical worlds
Priority: HIGH

#### FS-TECH-003 — Dam / Hydro Plant 06
Source: https://freesound.org/people/gurek/sounds/234412/
Creator: gurek
Licence: CC0
Type: real hydro-plant interior field recording
Specs: WAV, stereo, 44.1 kHz / 16-bit, 0:58.746
Description: real inner hydro-plant ambience with hum/hiss/drone.
Potential use:
- FOUNDATION / WORLD
- massive infrastructure tone
Priority: VERY HIGH

#### FS-TECH-004 — Dam / Hydro Plant 05
Source: https://freesound.org/people/gurek/sounds/234413/
Creator: gurek
Licence: CC0
Type: real hydro-plant interior field recording
Specs: WAV, stereo, 44.1 kHz / 16-bit, 0:23.871
Description: alternate real hydro-plant interior ambience.
Potential use:
- infrastructure texture variation
Priority: HIGH

### Concrete spaces / impulse responses

#### FS-SPACE-001 — Parking Garage 0001 IR
Source: https://freesound.org/people/djericmark/sounds/724679/
Creator: djericmark
Licence: CC0
Type: real measured impulse response
Specs: WAV, 192 kHz / 24-bit, 6.0 s
Description: sine-sweep-derived IR from a large concrete parking garage.
Potential use:
- SPACE convolution/reference
- concrete-room benchmark
Priority: VERY HIGH

#### FS-SPACE-002 — LA Metro Garage 02 IR
Source: https://freesound.org/people/djericmark/sounds/724684/
Creator: djericmark
Licence: CC0
Type: real measured impulse response
Specs: WAV, stereo, 48 kHz / 24-bit, 1.927 s
Description: real large concrete garage IR.
Potential use:
- tighter concrete-space character
- reference against synthetic FDN designs
Priority: HIGH

### Harbor / shipyard / dock worlds

#### FS-HARB-001 — Shipyard Construction Ambience, Saint-Nazaire
Source: https://freesound.org/people/WattnotSounds/sounds/831634/
Creator: WattnotSounds
Licence: CC0
Type: real field recording
Specs: AIFF, stereo, 48 kHz / 24-bit, 2:08
Description: cruise-ship construction yard; engines, metallic clangs, steel cutting, dock acoustics.
Potential use:
- INDUSTRIAL / WASTELAND world
- long evolving industrial depth
Caution:
- faint shouts/footsteps; segment carefully.
Priority: VERY HIGH

#### FS-HARB-002 — St Nazaire Industrial Bulk Port without crane
Source: https://freesound.org/people/bruno.auzet/sounds/838021/
Creator: bruno.auzet
Licence: CC0
Type: real field recording
Specs: WAV, stereo, 48 kHz / 24-bit, 1:48.001
Description: comparatively quiet industrial unloading-dock ambience without active crane movement.
Potential use:
- clean industrial harbor bed
- subtle NDH underlay
Priority: VERY HIGH

#### FS-HARB-003 — Harbor Ambience 1
Source: https://freesound.org/people/clif_creates/sounds/254125/
Creator: clif_creates
Licence: CC0
Type: real field recording
Specs: WAV, mono, 48 kHz / 24-bit, 1:00.146
Description: docked boats, water, sail/flag movement.
Potential use:
- non-literal dock movement / water-body texture
- transformed distant maritime layer
Priority: MEDIUM-HIGH

#### FS-HARB-004 — AMB Harbor Waves
Source: https://freesound.org/people/cribbler/sounds/443018/
Creator: cribbler
Licence: CC0
Type: real night field recording
Specs: WAV, stereo, 96 kHz / 32-bit, 2:16.192
Description: night harbor / shore / dock water, high-resolution.
Potential use:
- very high-quality water-space transformation source
- dark exterior world
Priority: VERY HIGH

### Tunnel / city resonance

#### FS-TUN-001 — Tunnel ambience distant
Source: https://freesound.org/people/guidofm/sounds/839653/
Creator: guidofm
Licence: CC0
Type: real field recording
Specs: WAV, stereo, 96 kHz / 24-bit, 2:03.995
Description: traffic and distant tunnel resonance.
Potential use:
- large urban tunnel world
- high-resolution resonance extraction
Caution:
- people/bikes/cars may be identifiable; segmentation needed.
Priority: HIGH

#### FS-TUN-002 — Big Dig Tour 13
Source: https://freesound.org/people/alienistcog/sounds/123745/
Creator: alienistcog
Licence: CC0
Type: real field recording
Specs: AIFF, stereo, 44.1 kHz / 16-bit, 1:56.060
Description: huge echoing machine pings in an unfinished harbor tunnel.
Potential use:
- distant resonant EVENTS
- underground scene space
Caution:
- footsteps in a middle section.
Priority: HIGH

### Mining / heavy infrastructure

#### FS-MINE-001 — coal pit mine machinery / grabber
Source: https://freesound.org/people/be_a_hero_not_a_patriot/sounds/332535/
Creator: be_a_hero_not_a_patriot
Licence: CC0
Type: real MS field recording
Specs: WAV, stereo, 44.1 kHz / 24-bit, 1:28.127
Description: close open-pit coal-mine machinery.
Potential use:
- large-scale industrial motion
- low body / mechanism extraction
Priority: HIGH

#### FS-MINE-002 — demolition excavator
Source: https://freesound.org/people/Garuda1982/sounds/422081/
Creator: Garuda1982
Licence: CC0
Type: real field recording
Specs: WAV, stereo, 96 kHz / 24-bit, 2:41.501
Description: real demolition excavator with hydraulics.
Potential use:
- high-resolution hydraulic / load / body source
- de-literalized colossal motion
Priority: HIGH

#### FS-IND-004 — industrial_machine_tone
Source: https://freesound.org/people/Kostrava/sounds/434507/
Creator: Kostrava
Licence: CC0
Type: real factory field recording
Specs: WAV, stereo, 44.1 kHz / 32-bit, 1:50.054
Description: industrial machine tone / hydraulic ambience.
Potential use:
- dark industrial foundation
- tension bed
Priority: HIGH

## Search-direction conclusion

The strongest new direction is not “more dark ambience”.
It is **physical systems that naturally evolve**:
- wind-excited structures;
- frozen surfaces under stress;
- large technical infrastructure;
- resonant tunnels/concrete spaces;
- moving water coupled to structures;
- distant industrial/harbor systems.

These sources already contain complex non-periodic modulation and causal physical behavior. Noctomorph should preserve that complexity instead of flattening it into static loops.


## Dark-cinematic / Gothic expansion — 2026-09-30

### Cathedral / church spaces

#### FS-GOTH-001 — Lichfield Cathedral Interior
Source: https://freesound.org/people/JW_Audio/sounds/841923/
Creator: JW_Audio
Licence: CC0
Type: real cathedral field recording
Specs: WAV, stereo, 96 kHz / 24-bit, 4:06.282
Description: binaural interior recording of Lichfield Cathedral; highly varied natural acoustic.
Potential use:
- GOTHIC / RUINS spatial-world reference
- large sacred interior texture
- long-tail spectral extraction
Priority: VERY HIGH

#### FS-GOTH-002 — Large church ambience
Source: https://freesound.org/people/Zetheyo/sounds/475954/
Creator: Zetheyo
Licence: CC0
Type: real church field recording
Specs: WAV, stereo, 48 kHz / 24-bit, 4:53.062
Description: calm ambience inside a large church.
Potential use:
- large stone-room WORLD layer
- reverb/reference analysis
Priority: VERY HIGH

#### FS-GOTH-003 — 1800s Church Room Tone
Source: https://freesound.org/people/composingatnight/sounds/697579/
Creator: composingatnight
Licence: CC0
Type: real church room tone
Specs: M4A, mono, 48 kHz, 1:01.354
Description: old church natural room tone with building noises.
Potential use:
- micro-detail / building texture
Caution:
- lossy source; use for texture/event mining rather than pristine bed.
Priority: MEDIUM

#### FS-GOTH-004 — Quiet Church Ambience
Source: https://freesound.org/people/hz37/sounds/792472/
Creator: hz37
Licence: CC0
Type: real large-church field recording
Specs: WAV, stereo, 48 kHz / 24-bit, 1:05.308
Description: large church ambience with some muffled human presence.
Potential use:
- sacred-room depth
Caution:
- segment around human sounds.
Priority: HIGH

### Bells / bourdon / distant ritual events

#### FS-BELL-001 — Isolated Bavarian Church Bell at Distance
Source: https://freesound.org/people/TSP-Talk/sounds/846302/
Creator: TSP-Talk
Licence: CC0
Type: real field recording
Specs: WAV, stereo, 48 kHz / 32-bit float, 0:25.507
Description: isolated bronze church bell about 300 m away with natural diffusion.
Potential use:
- rare GOTHIC event
- harmonic / modal extraction
- pitch-stretched metallic foundation
Priority: VERY HIGH

#### FS-BELL-002 — Distant Church Bells with Nature/Urban Ambience
Source: https://freesound.org/people/Julian_Eftei/sounds/830060/
Creator: Julian_Eftei
Licence: CC0
Type: real field recording
Specs: WAV, stereo, 44.1 kHz / 24-bit, 4:11
Description: soft distant bells across a natural/urban field.
Potential use:
- far-world event
- sparse evolving scene cue
Priority: HIGH

#### FS-BELL-003 — Largest Notre-Dame Bourdon
Source: https://freesound.org/people/Mxsmanic/sounds/139110/
Creator: Mxsmanic
Licence: CC0
Type: real cathedral bell
Specs: WAV, stereo, 96 kHz / 24-bit, 0:32.414
Description: Emmanuel bourdon bell of Notre-Dame ringing alone.
Potential use:
- very-low bell resonance
- spectral/modal decomposition
- transformed monumental event
Caution:
- background people present.
Priority: VERY HIGH

#### FS-BELL-004 — Notre-Dame Full Bell Set
Source: https://freesound.org/people/Mxsmanic/sounds/139109/
Creator: Mxsmanic
Licence: CC0
Type: real cathedral bells
Specs: WAV, stereo, 96 kHz / 24-bit, 1:04.228
Description: multiple Notre-Dame bells ringing together.
Potential use:
- modal cloud extraction
- grand but sparse scene mutation
Priority: HIGH

#### FS-BELL-005 — Gothic Church Bells
Source: https://freesound.org/people/Aeonemi/sounds/180330/
Creator: Aeonemi
Licence: CC0
Type: real church bell field recording
Specs: MP3, stereo, 44.1 kHz / 128 kbps, 0:48.348
Description: German Gothic church bell.
Potential use:
- aesthetic reference
- event-source fallback
Caution:
- lossy source; not preferred over high-resolution bell recordings.
Priority: REFERENCE

#### FS-BELL-006 — Bell Tower friction / wood support
Source: https://freesound.org/people/Dishings/sounds/795222/
Creator: Dishings
Licence: CC0
Type: real bell-tower recording
Specs: MP3, stereo, 48 kHz / 320 kbps, 0:08.036
Description: old bell tower, including audible wood-friction between strikes.
Potential use:
- bell-mechanism texture
- old-structure micro-event
Priority: MEDIUM

### Organ / sacred tonal material

#### FS-ORG-001 — Cathedral Organ, Stephansdom Vienna
Source: https://freesound.org/people/Breviceps/sounds/462340/
Creator: Breviceps
Licence: CC0
Type: real cathedral organ field recording
Specs: WAV, stereo, 48 kHz / 16-bit, 1:27.599
Description: pipe organ captured in the cathedral acoustic.
Potential use:
- harmonic/timbral reference
- resynthesis source
- long spectral-grain extraction
Rule:
- do not reproduce recognizable musical phrase literally; use only transformed/material-derived content.
Priority: HIGH, CONDITIONAL

#### FS-ORG-002 — Organ in Church / Madonna del Sasso
Source: https://freesound.org/people/BonnyOrbit/sounds/442541/
Creator: BonnyOrbit
Licence: CC0
Type: real church organ field recording
Specs: WAV, stereo, 48 kHz / 24-bit, 0:59.171
Potential use:
- tonal architecture reference
- transformed resonant cloud
Rule:
- deconstruct rather than replay musical content.
Priority: MEDIUM-HIGH

### Vocal / sacred-space reference

#### FS-VOX-001 — Singing in a Church
Source: https://freesound.org/people/MIKEJONESBONES/sounds/400930/
Creator: MIKEJONESBONES
Licence: CC0
Type: real vocal-in-church field recording
Specs: WAV, stereo, 96 kHz / 24-bit, 6:16.206
Potential use:
- reference for real voice-to-stone coupling
- spectral/vocal-resonance analysis
- possibly heavily deconstructed texture
Rule:
- do not expose recognizable sung material; treat as transformation/research source only unless a clean abstracted derivative is verified.
Priority: RESEARCH

### Ravens / crows — sparse event layer

#### FS-BIRD-001 — CrowOrRaven2 distant
Source: https://freesound.org/people/iwanPlays/sounds/512780/
Creator: iwanPlays
Licence: CC0
Type: real field recording
Specs: WAV, stereo, 96 kHz / 16-bit, 1.328 s
Description: distant crow/raven-type call, cleaned.
Potential use:
- very rare dark exterior event
Priority: HIGH

#### FS-BIRD-002 — CrowOrRaven1
Source: https://freesound.org/people/iwanPlays/sounds/512781/
Creator: iwanPlays
Licence: CC0
Type: real field recording
Specs: WAV, stereo, 96 kHz / 16-bit, 1.287 s
Potential use:
- alternate sparse event variation
Priority: HIGH

#### FS-BIRD-003 — Crow call field recording
Source: https://freesound.org/people/Garuda1982/sounds/418181/
Creator: Garuda1982
Licence: CC0
Type: real field recording
Specs: WAV, stereo, 96 kHz / 24-bit, 12.239 s
Potential use:
- high-resolution crow event pool
- distance / pitch / space transformations
Priority: VERY HIGH

#### FS-BIRD-004 — Ravens calling in rural Estonia
Source: https://freesound.org/people/inspire153/sounds/849152/
Creator: inspire153
Licence: CC0
Type: real field recording
Specs: WAV, mono, 48 kHz / 24-bit, 14.354 s
Description: two ravens with wingbeats.
Potential use:
- rare exterior event with organic movement
Priority: HIGH

### Cave / stone / subterranean spaces

#### FS-CAVE-001 — Ojo Guareña cave chamber
Source: https://freesound.org/people/nomadas/sounds/609161/
Creator: nomadas
Licence: CC0
Type: real cave field recording
Specs: WAV, stereo, 48 kHz / 16-bit, 0:47.569
Description: huge cave chamber recorded in total darkness with distributed drips.
Potential use:
- ABYSS / RUINS real stone-space layer
- stochastic event timing reference
Priority: VERY HIGH

#### FS-CAVE-002 — Sierra Mazateca cave water
Source: https://freesound.org/people/aurelien.leveque/sounds/417631/
Creator: aurelien.leveque
Licence: CC0
Type: real cave field recording
Specs: WAV, stereo, 48 kHz / 24-bit, 1:30.056
Description: medium cave, distant flow and water drops captured MS with Schoeps.
Potential use:
- high-quality stone/water spatial material
Priority: VERY HIGH

#### FS-CAVE-003 — Falun Mine underground waterfall
Source: https://freesound.org/people/blaukreuz/sounds/398830/
Creator: blaukreuz
Licence: CC0
Type: real underground mine field recording
Specs: WAV, stereo, 48 kHz / 24-bit, 0:27.041
Description: 60 m underground near subterranean waterfall.
Potential use:
- mine/cavern low-body ambience
Caution:
- distant tourists/voices.
Priority: HIGH, CONDITIONAL

#### FS-CAVE-004 — Water into underground sinkhole
Source: https://freesound.org/people/hinchinbrook/sounds/552485/
Creator: hinchinbrook
Licence: CC0
Type: real field recording
Specs: WAV, stereo, 48 kHz / 16-bit, 0:22.833
Description: water entering sinkhole with deep underground chamber rumble.
Potential use:
- low-frequency cave body
- abstract subterranean motion
Priority: HIGH

### Wood / old-structure micro-events

#### FS-WOOD-001 — Wood Creak Single family
Representative source: https://freesound.org/people/Rudmer_Rotteveel/sounds/502505/
Creator: Rudmer_Rotteveel
Licence: CC0
Type: real foley/field source
Specs: WAV, stereo, 44.1 kHz / 16-bit
Description: clean isolated wooden creaks; multiple variants in pack.
Potential use:
- rare old-structure motion
- stretched tonal squeal
Rule:
- never use as generic haunted-house footsteps.
Priority: HIGH

#### FS-WOOD-002 — Old Armoire Wood Creak
Source: https://freesound.org/people/brunoboselli/sounds/478600/
Creator: brunoboselli
Licence: CC0
Type: real recording
Specs: WAV, stereo, 48 kHz / 24-bit, 18.349 s
Potential use:
- long wood-friction source
- old-building structural layer
Priority: HIGH

### Tonal / resonant object seeds

#### FS-TONAL-001 — Hurdy-gurdy drone string pluck
Source: https://freesound.org/people/clareboots/sounds/553961/
Creator: clareboots
Licence: CC0
Type: real string/resonant instrument source
Specs: WAV, stereo, 44.1 kHz / 24-bit, 12.532 s
Description: deep sonorous pluck of a hurdy-gurdy drone string.
Potential use:
- dark tonal impulse
- resonator excitation
- pitch-shifted foundation seed
Priority: HIGH

#### FS-TONAL-002 — Cello Drone
Source: https://freesound.org/people/carrieedick/sounds/465558/
Creator: carrieedick
Licence: CC0
Type: edited cello source
Specs: WAV, stereo, 44.1 kHz / 32-bit, 1:08.230
Potential use:
- timbral reference
- spectral-source candidate
Priority: RESEARCH

### Procedural / synthetic reference — architecture, not identity

#### REF-SYN-001 — MANTICE Resonant Cave Procedural Drone
Source: https://freesound.org/people/bassimat/sounds/856219/
Licence: CC0
Type: procedural synthetic render
Description: multi-layer procedural drone with per-layer motion/automation, FM/noise/grain concepts, spatial movement and reproducibility via seed.
Use:
- architecture / behavior reference for Noctomorph FOUNDATION generator.
Do not use as primary Noctomorph identity source.
Priority: ARCHITECTURE REFERENCE

#### REF-SYN-002 — MANTICE Low-Frequency Spectral Timbre Drone
Source: https://freesound.org/people/bassimat/sounds/856324/
Licence: CC0
Type: procedural synthetic render
Specs: WAV, stereo, 48 kHz / 24-bit, 5:00
Use:
- reference for deep evolving foundation behavior, automation and layer interaction.
Priority: ARCHITECTURE REFERENCE

#### REF-SYN-003 — Alien Drone / SoundScaper render
Source: https://freesound.org/people/bassimat/sounds/860238/
Licence: CC0
Type: generative synthetic render
Use:
- reference for glitch/rumble/space density balance in unreal worlds.
Priority: REFERENCE

## Source-selection principle after Gothic expansion

Noctomorph should not become a literal Gothic soundboard.

Use cathedral, bell, organ, raven and old-structure material as:
- sparse identity cues;
- resonant raw material;
- spectral/modal sources;
- spatial references;
- transformed event seeds.

The core still remains an original unreal-world generator, not a collage of recognizable Gothic tropes.


## Unreal-world raw material expansion — 2026-09-30

### Resonant metal bodies / pipes / tanks

#### FS-OBJ-001 — Resonant iron dome / tank hits
Source: https://freesound.org/people/kyles/sounds/637679/
Creator: kyles
Licence: CC0
Type: real resonant metal-object recording
Specs: FLAC, mono, 48 kHz / 24-bit, 14.222 s
Description: iron dome / sink / tank resonance.
Potential use:
- modal-resonator analysis
- impossible-body construction
- impact-to-drone transformation
Priority: VERY HIGH

#### FS-OBJ-002 — Metal Pipe resonant hits
Source: https://freesound.org/people/derjuli/sounds/824117/
Creator: derjuli
Licence: CC0
Type: real resonant metal-object recording
Specs: WAV, stereo, 48 kHz / 16-bit, 56.320 s
Potential use:
- pipe modal bank source
- tonal metallic body
- pitch/time transformation
Priority: HIGH

#### FS-OBJ-003 — Large hollow metal pipe
Source: https://freesound.org/people/hanasmusic/sounds/841476/
Creator: hanasmusic
Licence: CC0
Type: real resonant object recording
Specs: WAV, stereo, 48 kHz / 24-bit, 11 s
Description: large hollow pipe struck, recorded with AKG C414 XLII.
Potential use:
- clean resonant tone extraction
- modal analysis / convolution seed
Priority: VERY HIGH

#### FS-OBJ-004 — Metal-pipe whoosh
Source: https://freesound.org/people/Sadiquecat/sounds/855833/
Creator: Sadiquecat
Licence: CC0
Type: real air-through-pipe source
Specs: WAV, stereo, 48 kHz / 24-bit, 0.610 s
Potential use:
- spatial transition
- air-column / portal-like movement
Priority: MEDIUM-HIGH

### Abandoned / defense / empty structures

#### FS-ABAND-001 — Abandoned defense base cannon room
Source: https://freesound.org/people/vhio/sounds/791287/
Creator: vhio
Licence: CC0
Type: real field recording
Specs: WAV, stereo, 96 kHz / 24-bit, 8:29.487
Description: closed room in abandoned early-1900s defense base, distant traffic and wind coupling into room.
Potential use:
- RUINS / GOTHIC-INDUSTRIAL room body
- long evolving architectural noise
- large-scale granular/spectral mining
Priority: VERY HIGH

#### FS-ABAND-002 — Abandoned railway embankment
Source: https://freesound.org/people/Garuda1982/sounds/852236/
Creator: Garuda1982
Licence: CC0
Type: real field recording
Specs: WAV, stereo, 48 kHz / 16-bit, 2:35.997
Description: abandoned railway environment with subtle creaks/wind/urban distance.
Potential use:
- sparse exterior RUINS layer
- distant creak/event extraction
Priority: MEDIUM-HIGH

### Nocturnal animal events

#### FS-BIRD-005 — Distant Owl
Source: https://freesound.org/people/Sadiquecat/sounds/825822/
Creator: Sadiquecat
Licence: CC0
Type: real field recording
Specs: FLAC, mono, 48 kHz / 24-bit, 1:17.276
Description: distant owl in rural night setting.
Potential use:
- rare nocturnal event
Priority: HIGH

#### FS-BIRD-006 — Scops Owl in night silence
Source: https://freesound.org/people/darthbaul/sounds/266898/
Creator: darthbaul
Licence: CC0
Type: real field recording
Specs: WAV, stereo, 44.1 kHz / 16-bit, 1:21.662
Potential use:
- sparse night-event variant
Priority: MEDIUM-HIGH

#### FS-ANIM-001 — Distant wolves
Source: https://freesound.org/people/Sacha.Julien/sounds/753896/
Creator: Sacha.Julien
Licence: CC0
Type: real field recording
Specs: WAV, stereo, 96 kHz / 24-bit, 4.261 s
Description: adults/young wolves howling far away.
Potential use:
- extremely rare unreal/gothic exterior event
Rule:
- very low probability, distant placement, no “wolf pack preset”.
Priority: HIGH

### Neutral room bodies for transformation

#### FS-ROOM-001 — Empty library heavy building ambience
Source: https://freesound.org/people/kyles/sounds/635727/
Creator: kyles
Licence: CC0
Type: real room tone
Specs: FLAC, stereo, 48 kHz / 24-bit, 2:01.370
Potential use:
- neutral body/noise extraction
- hidden building-motion layer
Priority: MEDIUM

## Unreal-world design consequence

Noctomorph should be able to construct bodies that do not exist by cross-coupling:
- one source's excitation;
- another source's modal/resonant body;
- a third source's environment;
- synthetic sub/partial foundation;
- an independent evolution trajectory.

Example:
crow call transient -> large metal-pipe modal body -> cathedral IR -> slow detuned synthetic sub-field.

The result should no longer read as “crow + pipe + church”; it should read as one coherent unknown world-object.


## Internet-wide source landscape — discovery pools and research datasets

This section records broader discovery sources. Inclusion here does not mean an asset is cleared for embedding. Every individual file still requires license/provenance verification.

### BigSoundBank / La Sonothèque

Source: https://bigsoundbank.com/
Strengths:
- large CC0/public-domain-equivalent catalog;
- industrial machinery;
- old technology;
- rooms and ambiences;
- switches, presses, projectors, transport, environmental recordings.

Noctomorph policy:
- mine for real physical sources and unusual spaces;
- prefer original lossless downloads when available;
- avoid premade cinematic designs unless used only as reference.

### Wikimedia Commons

Source: https://commons.wikimedia.org/
Strengths:
- historic machinery;
- bells;
- organs;
- public spaces;
- transport;
- old recordings;
- scientific/acoustic media.

Noctomorph policy:
- verify each file individually;
- prefer Public Domain / CC0;
- CC-BY may be usable only if attribution and redistribution requirements are deliberately supported.

### Internet Archive

Source: https://archive.org/
Strengths:
- historical sound-effect collections;
- industrial/machinery archives;
- old field recordings;
- unusual obsolete technology.

Noctomorph policy:
- exact item/file rights must be verified;
- do not infer public-domain status from age or archive presence;
- potentially valuable for rare period machinery and spaces.

### OpenGameArt

Source: https://opengameart.org/
Strengths:
- CC0 sound packs;
- metal / wood / mechanical primitives;
- environmental and game-audio source material.

Noctomorph policy:
- use only assets with explicit compatible licensing;
- individual pack provenance should be archived.

### Freesound

Source: https://freesound.org/
Strengths:
- extremely broad field-recording coverage;
- contact-mic material;
- high-resolution CC0 uploads;
- industrial / natural / architectural / animal / object sources.

Noctomorph policy:
- CC0 preferred for embedded assets;
- retain uploader, source page, license, retrieval date and checksum;
- do not assume pack-level uniform licensing.

### Academic / research datasets

#### MIMII Dataset
Research reference:
https://arxiv.org/abs/1909.09347
Dataset:
https://zenodo.org/record/3384388
Content:
- real industrial valves;
- pumps;
- fans;
- slide rails;
- normal and anomalous machine states;
- real factory environments.

Noctomorph use:
- analysis/reference for real machine-state evolution;
- anomaly-driven spectral behavior research;
- not cleared as production sample source until exact dataset license and redistribution terms are reviewed.

#### STARSS22
Research reference:
https://arxiv.org/abs/2206.01948
Dataset:
https://zenodo.org/record/6387880
Content:
- spatial recordings of real scenes;
- first-order Ambisonics;
- tetrahedral microphone-array format;
- annotated moving/static sound events.

Noctomorph use:
- reference for spatial event behavior;
- testing scene localization / event-density ideas;
- not a production source until dataset licensing is reviewed.

## Broadened search axes

Continue systematic discovery across these semantic families:

### Dark architecture
- cathedrals
- churches
- crypt-like stone rooms
- bunkers
- forts
- defense structures
- underground stations
- abandoned factories
- mines
- tunnels
- silos
- tanks
- parking structures
- industrial stairwells
- bridges

### Structural excitation
- wind-loaded cable
- fences
- bridge members
- steel beams
- towers
- masts
- rails
- pipelines
- large sheets
- resonant doors
- structural groans
- contact-mic recordings

### Energy / infrastructure
- substations
- transformers
- hydro plants
- pump rooms
- server rooms
- HVAC
- ventilation shafts
- generators
- electric motors
- inverters
- electromagnetic-field recordings

### Geological / cold / subterranean
- frozen lakes
- ice plates
- glaciers
- cave drips
- underground rivers
- rocks / gravel / stone falls
- quarry resonance
- mine ventilation
- sinkholes
- deep shafts

### Gothic identity cues
- deep bells / bourdon
- bell mechanisms
- organ wind / pipe resonance
- old timber creaks
- ravens / crows
- owls
- distant dogs / wolves
- distant trains / horns
- storm rumbles
- sparse footsteps only where heavily de-literalized

### Impossible-body raw material
- giant pipes
- tanks
- hollow metal
- resonant glass
- ceramics
- bowed/scraped metal
- wire / cable
- springs
- plates
- string resonance
- contact transducers
- electromagnetic fields

### Transitional / event material
- pressure releases
- metal stress
- ice cracks
- distant impacts
- rail movement
- doors/gates at great distance
- structural settling
- low thunder without obvious crack
- air-column whooshes

## Search quality rule

The internet sweep is not complete when a large number of files is found.

It is complete only when:
- each required sonic role has several strong alternatives;
- high-priority roles have enough variation to avoid obvious repetition;
- production candidates have compatible licensing/provenance;
- the palette supports both subtle song-layer behavior and full cinematic scenes;
- the material can support real and unreal worlds without relying on generic premade dark drones.


## Deep sweep additions — curated pass

### High-value spatial / Gothic spaces

#### FS-SPACE-003 — Cathedral IR 5m Stereo
Source: https://freesound.org/people/Nox_Sound/sounds/648945/
Creator: Nox_Sound
Licence: CC0
Type: measured / recorded cathedral impulse response
Specs: WAV, stereo, 96 kHz / 24-bit, 2.614 s
Potential use:
- convolution benchmark
- sacred-space coloration
- hybrid FDN/convolution comparison
Priority: VERY HIGH

#### FS-GOTH-005 — Barcelona Cathedral del Mar interior
Source: https://freesound.org/people/Nimlos/sounds/524630/
Creator: Nimlos
Licence: CC0
Type: real cathedral field recording
Specs: WAV, stereo, 96 kHz / 24-bit, 55.667 s
Potential use:
- stone-space character
- crowd-free segmentation if possible
Caution:
- public-space content may include people; curate only clean sections.
Priority: HIGH

#### FS-GOTH-006 — Hallgrimskirkja large nave ambience
Source: https://freesound.org/people/NickTayloe/sounds/828537/
Creator: NickTayloe
Licence: CC0
Type: real church field recording
Specs: FLAC, stereo, 44.1 kHz / 24-bit, 18:03.529
Description: huge reverberant nave with long-form natural room behavior.
Potential use:
- spatial behavior reference
- long-room texture mining
Caution:
- contains walla/coughs/shuffling; not a clean bed by default.
Priority: RESEARCH / CONDITIONAL

### Bells / monumental metal

#### WC-BELL-001 — St. Petersglocke / Cologne Cathedral
Source: https://commons.wikimedia.org/wiki/File:200437_bigben12345_petersglocke.oga
Creator/uploader lineage: BIGBEN12345 / Wikimedia mirror
Licence: CC0
Type: real cathedral bell recording
Duration: ~60 s
Potential use:
- monumental low bell modal analysis
- rare GOTHIC event
- stretched/filtered resonance source
Caution:
- source quality is consumer-recording grade; use mainly for spectral/modal character, not pristine full-band foreground.
Priority: HIGH

#### WC-BELL-002 — Kapitelsglocke / Cologne Cathedral
Source: https://commons.wikimedia.org/wiki/File:198442_bigben12345_kapitelsglocke.wav
Licence: CC0
Type: real cathedral bell recording
Duration: ~38 s
Potential use:
- alternate bell modal family
- event variation / spectral extraction
Priority: HIGH

### Glass / brittle resonant bodies

#### FS-GLASS-001 — Glass resonating
Source: https://freesound.org/people/eoinot/sounds/622730/
Creator: eoinot
Licence: CC0
Type: real resonant glass recording
Specs: WAV, stereo, 44.1 kHz / 24-bit, 45.081 s
Description: finger-excited wine-glass resonance.
Potential use:
- glass modal-body extraction
- unreal resonator
- slowly beating partial source
Priority: VERY HIGH

#### FS-GLASS-002 — Crystal wine glass D# resonance
Source: https://freesound.org/people/Department64/sounds/544397/
Creator: Department64
Licence: CC0
Type: real resonant glass recording
Specs: WAV, stereo, 48 kHz / 16-bit, 8.863 s
Description: clean crystal glass tone.
Potential use:
- clean modal reference
- transposed resonator bank seed
Priority: HIGH

#### FS-GLASS-003 — Broken glass scrape/crack texture
Source: https://freesound.org/people/jhumbucker/sounds/250544/
Creator: jhumbucker
Licence: CC0
Type: real glass texture recording
Specs: WAV, mono, 96 kHz / 24-bit, 40.868 s
Potential use:
- brittle high-frequency texture
- granular / spectral tension source
Rule:
- use as material physics, not horror cliché.
Priority: HIGH

#### FS-GLASS-004 — Large glass plate scrape / shake family
Representative:
- https://freesound.org/people/RutgerMuller/sounds/104345/
- https://freesound.org/people/RutgerMuller/sounds/104347/
Creator: RutgerMuller
Licence: CC0
Type: real glass plate recording
Specs: AIFF, stereo, 48 kHz / 24-bit
Potential use:
- low glass rumble
- plate resonance
- scrape / flex texture
Priority: HIGH

#### FS-GLASS-005 — High-resolution glass/ceramic scrape micro-events
Representative:
https://freesound.org/people/Anthousai/sounds/447663/
Creator: Anthousai
Licence: CC0
Type: real foley
Specs: WAV, stereo, 96 kHz / 24-bit
Potential use:
- tiny brittle excitation grains
- granular layer seed
Priority: MEDIUM-HIGH

### Electrical infrastructure / wind-farm structures

#### FS-ELEC-005 — Wind-farm substation / structure settling
Source: https://freesound.org/people/theloniousdump/sounds/718985/
Creator: theloniousdump
Licence: CC0
Type: real night field recording
Specs: M4A, mono, 48 kHz, 18.069 s
Description: substation harmonics, overhead wires, large metallic structure settling, wind.
Potential use:
- electrical harmonic reference
- structure event mining
Caution:
- low-bitrate M4A; use as event/reference, not pristine foundation.
Priority: MEDIUM-HIGH

#### FS-ELEC-006 — Distant substation hum coupled to stream ambience
Source: https://freesound.org/people/JW_Audio/sounds/798042/
Creator: JW_Audio
Licence: CC0
Type: real field recording
Specs: WAV, stereo, 48 kHz / 24-bit, 1:33.384
Description: natural stream with distant electrical hum.
Potential use:
- isolate/learn distant-grid harmonic behavior
- layered unreal infrastructure worlds
Caution:
- birds/water dominate parts; not a direct foundation bed.
Priority: RESEARCH

### Tonal acoustic seeds

#### FS-TONAL-003 — Bowed cello note A3
Source: https://freesound.org/people/smoseson/sounds/48024/
Creator: smoseson
Licence: CC0
Type: real instrument sample
Specs: WAV, stereo, 44.1 kHz / 16-bit, 12.346 s
Potential use:
- clean bowed-string excitation
- resonator / spectral morph seed
- synthetic-foundation crossfade target
Priority: HIGH

#### FS-TONAL-004 — Rosined cello-bow friction, dark filtered texture
Source: https://freesound.org/people/Abolla/sounds/213914/
Creator: Abolla
Licence: CC0
Type: real bow-friction recording, filtered
Specs: WAV, mono, 48 kHz / 24-bit, 28.707 s
Potential use:
- dark organic friction bed
- low-passed granular texture
Priority: VERY HIGH

### Designed-source caution

#### REJECT-DESIGN-001 — Atmosphere_Scifi_Bunker_Loop_Stereo
Source: https://freesound.org/people/Nox_Sound/sounds/817225/
Licence: CC0
Decision: REFERENCE ONLY
Reason:
- already a designed dark/dystopian atmosphere;
- useful for competitive listening;
- embedding it would weaken Noctomorph's own identity.

#### REJECT-RIGHTS-001 — Cellos Destroyed and Droned with Paulstretch
Source: https://freesound.org/people/RutgerMuller/sounds/195850/
Page licence: CC0
Decision: DO NOT USE AS PRODUCTION SOURCE
Reason:
- description states it derives from a cello cloud from a Scelsi piece;
- uploader-level CC0 does not by itself establish rights in the underlying composition/performance;
- unnecessary chain-of-title risk.
Use:
- architecture/listening reference only.

## Curation rule refinement

For each production candidate, validate two separate things:

1. **File-page licence**
   - CC0 / public domain / explicit redistribution permission.

2. **Underlying-rights provenance**
   - uploader actually recorded/created the material, or
   - source chain is independently public-domain/cleared.

A CC0 badge on a derivative upload is not sufficient if the underlying recording, performance or composition may carry separate rights.


## Gumroad / indie-tool architecture references — 2026-09-30

These are **behavior and architecture references**, not sources to copy or redistribute.

### REF-GUM-001 — Ambiotica
Source: https://989477922824.gumroad.com/l/ambiotica
Type: regenerative ambient processor
Observed architecture:
- rolling looper;
- granular cloud;
- parallel micro-loop;
- modulated chord-tuned reverb;
- feedback/regeneration from the reverb tail back into the source;
- one macro (“Gravity”) coordinating the system.

Noctomorph takeaway:
- recursive transformation is useful for long-term evolution;
- separate time scales can coexist without sounding like one obvious loop;
- feedback must be bounded and state-aware to avoid runaway energy.
Priority: VERY HIGH ARCHITECTURE REFERENCE

### REF-GUM-002 — Grain Storm
Source: https://snakeshaky.gumroad.com/l/grainstorm
Type: Max for Live granular exploration device
Observed architecture:
- very slow or very fast buffer scrubbing;
- granular drone-to-percussive continuum;
- Decay / Ramp / Sample-and-Hold controls;
- freezable long reverb;
- smoothing filter.

Noctomorph takeaway:
- a single source can traverse sustained, textural and event-like states;
- granular playback position should be independently controllable from grain behavior;
- S&H-style state changes could help generate sparse “world events”.
Priority: HIGH ARCHITECTURE REFERENCE

### REF-GUM-003 — 7ewdResyntheis
Source: https://7ewd.gumroad.com/l/7ewdResyntheis
Type: granular resynthesis VST
Observed architecture:
- transforms live/input rhythm and contour through a separate sample corpus;
- real-time similar-grain search;
- CPU/GPU search modes;
- source folders and corpus-based matching.

Noctomorph takeaway:
- “behavior from one source, timbre from another” strongly matches the impossible-world concept;
- a lighter-weight corpus-selection idea may be valuable even without ML/GPU dependence.
Priority: VERY HIGH CONCEPT REFERENCE

### REF-GUM-004 — Abstructs
Source: https://sonusdept.gumroad.com/l/abstructs
Type: Max for Live experimental-device collection
Relevant concepts:
- dense evolving-spectrum synthesis;
- independent magnitude/phase spectral shifting;
- multi-line delays with probability;
- spectrum feature extraction;
- randomized sample-player variation;
- spectral magnitude/phase suppression/inversion;
- dedicated drone-generation devices.

Noctomorph takeaway:
- spectral state mutation can create unreal transitions that are not achievable by ordinary filtering;
- probability should alter event structure, not merely randomize every parameter.
Priority: HIGH RESEARCH REFERENCE

### REF-GUM-005 — Drone Liquifier
Source: https://tomcosm.gumroad.com/l/CSccE
Type: granular + long-reverb drone processor
Observed architecture:
- granular rearrangement;
- four LFOs;
- very long reverb;
- intended to preserve source timbre while converting it into evolving drone material.

Noctomorph takeaway:
- useful baseline for what a simple drone processor already does;
- Noctomorph must substantially exceed this by adding state evolution, multi-role layering, physical-source cross-coupling and event logic.
Priority: COMPETITIVE BASELINE

### REF-GUM-006 — Texture Loom 1.5
Source: https://s1gnsofl1fe.gumroad.com/l/texture-loom
Type: dual-source sample/granular instrument
Observed architecture:
- two independently controlled sample engines;
- conventional or granular mode per source;
- grain size, density, spray, width, window and pitch jitter;
- playhead travel independent from grain direction;
- frozen playhead state.

Noctomorph takeaway:
- independent source transport vs micro-grain motion is highly relevant;
- dual-source interaction can be a useful minimum building block for WORLD/TEXTURE transformations.
Priority: VERY HIGH ARCHITECTURE REFERENCE

### REF-GUM-007 — Panacousticon Series
Source: https://emilianopennisi.gumroad.com/l/dkgxa
Type: Max for Live instruments focused on cold / industrial / death-ambient language
Observed concepts:
- synthesis;
- granular fragmentation;
- tape memory;
- generative drone;
- organic noise;
- very high-density granular microscope;
- medium-density suspended texture;
- ultra-short locked fragments.

Noctomorph takeaway:
- grain density itself can define distinct perceptual regimes;
- “one algorithm with different density” can become multiple scene behaviors;
- cold/dark identity can arise from process language, not premade horror samples.
Priority: VERY HIGH AESTHETIC / ARCHITECTURE REFERENCE

### REF-GUM-008 — Noise Map
Source: https://remodevico.gumroad.com/l/noisemap
Type: Max for Live sample/noise soundscape device
Observed architecture:
- 2D morphing field;
- white noise, pink noise, tones and sample source;
- smooth interpolation across a control map;
- automatable path through the space.

Noctomorph takeaway:
- state-space navigation may be more musically useful than exposing dozens of independent random modulators;
- an EVOLVE trajectory could travel through a constrained multidimensional scene state.
Priority: HIGH CONCEPT REFERENCE

### REF-GUM-009 — LIRA•8 digital interpretation
Source: https://mikemorenodsp.gumroad.com/l/lira-8
Type: digital drone-synth interpretation
Observed concepts:
- multiple tunable voices;
- cross-FM;
- dual delay;
- slow modulation / hyper-LFO;
- distortion.

Noctomorph takeaway:
- cross-coupled oscillators can provide unstable but coherent synthetic foundations;
- any similar idea should be original and generic DSP, not an imitation of a specific commercial hardware implementation.
Priority: SYNTHESIS REFERENCE

## Gumroad-derived engine hypotheses

The indie-tool sweep suggests several high-value Noctomorph experiments:

1. **Recursive regeneration**
   - transformed tail re-enters a controlled earlier stage;
   - energy-limited, band-limited, state-aware.

2. **Multi-timescale granular system**
   - macro transport: seconds to minutes;
   - meso grains: 100 ms–seconds;
   - micro grains: milliseconds;
   - sparse event extraction as a fourth regime.

3. **Behavior/timbre decoupling**
   - one source provides timing/dynamics;
   - another provides spectral body;
   - synthetic foundation supplies pitch/sub continuity.

4. **State-space evolution**
   - EVOLVE traverses constrained scene states rather than applying arbitrary LFOs;
   - each archetype owns a valid region and transition graph.

5. **Spectral mutation**
   - selective magnitude/phase manipulation;
   - partial retention/suppression;
   - spectral freezing / cross-morphing;
   - always bounded to avoid metallic aliasing/noise collapse.

6. **Independent transport and grain motion**
   - source position, grain direction, density, spread and pitch jitter evolve separately.

7. **Density regimes**
   - sparse particles;
   - suspended mid-density texture;
   - dense spectral cloud;
   - frozen micro-fragment pressure.

These are research hypotheses, not committed product features. Each must be prototyped, measured and auditioned before entering the final architecture.


## Advanced synthesis / physical-model / generative references — 2026-09-30

These references are for DSP architecture and behavior study. Do not copy implementation where licensing is incompatible with the 125A codebase.

### REF-ADV-001 — Oi, Grandad! V2
Source: https://github.com/publicsamples/Oi-Grandad
Licence: GPL-3.0
Type: open-source granular synthesizer
Relevant architecture:
- four independent granular voices;
- up to four playheads per voice;
- complex/multistage modulation;
- per-voice waveguide resonator;
- crossfade / round-robin behaviors.

Noctomorph takeaway:
- waveguide resonance after granular decomposition can turn recorded matter into coherent “impossible bodies”;
- multiple playheads are useful only if constrained by scene logic rather than exposed as random chaos.
Priority: VERY HIGH ARCHITECTURE REFERENCE

### REF-ADV-002 — Modal Synthesiser
Source: https://github.com/crispinha/modal-synth
Licence: GPL-3.0-or-later / JUCE-related copyleft caveats
Type: modal-synthesis instrument
Relevant architecture:
- banks of resonators;
- parametrically controlled spectra;
- material-like timbres: wood / metal / glass.

Noctomorph takeaway:
- modal banks are a strong candidate for constructing non-existent resonant objects;
- material identity can be parameterized by mode ratios, decay, damping and excitation spectrum.
Priority: VERY HIGH DSP REFERENCE

### REF-ADV-003 — JoepVanlier Partials
Source: https://github.com/JoepVanlier/JSFX
Type: modal resonator effect
Relevant architecture:
- audio excites banks of resonators;
- multiple fundamentals can coexist;
- reverb-like or instrument-like behavior.

Noctomorph takeaway:
- WORLD/TEXTURE sources could excite tonal bodies without becoming conventional pitched samples;
- multiple fundamentals may create controlled dark clusters.
Priority: HIGH DSP REFERENCE

### REF-ADV-004 — MechanOdd
Source: https://github.com/odoare/MechanOdd
Type: physical-modeling synthesizer
Relevant architecture:
- simulated strings;
- plates;
- membranes;
- beams;
- feedback matrix between resonators;
- modulation and effects.

Noctomorph takeaway:
- cross-coupling multiple physical bodies is highly aligned with “unreal worlds”;
- a bounded feedback matrix can create structures that behave physically but cannot exist in reality.
Priority: VERY HIGH CONCEPT / DSP REFERENCE

### REF-ADV-005 — DaisySP physical-model toolbox
Source: https://github.com/electro-smith/DaisySP
Type: DSP library
Relevant facilities:
- Karplus-Strong;
- resonators;
- modal synthesis;
- granular player;
- fractal / particle / clocked noise;
- FM / subtractive synthesis.

Noctomorph takeaway:
- useful algorithmic reference map for original implementations;
- particle/fractal noise could provide non-looping exciters for physical bodies.
Priority: HIGH RESEARCH REFERENCE

### REF-ADV-006 — HISSTools Freeze
Source: https://github.com/AlexHarker/HISSTools_Freeze
Type: spectral freeze/morph plugin
Relevant architecture:
- multiple freeze/morph modes;
- output evolution control;
- randomized multiband movement.

Noctomorph takeaway:
- spectral freeze should not be a binary “hold FFT frame” gimmick;
- morph state and band-selective motion can preserve identity while extending time indefinitely.
Priority: VERY HIGH SPECTRAL REFERENCE

### REF-ADV-007 — Pareidolia
Source: https://github.com/thorinside/pareidolia
Licence: MIT
Type: spectral/granular “phantom choir” experiment
Relevant architecture:
- grain source can be noise/input/resonator;
- formant center and drift;
- input tracking;
- coherence control;
- spectral/voice-like illusion without literal choir playback.

Noctomorph takeaway:
- formant-resonant motion can add “presence” or quasi-vocal dark character without using recognizable vocal samples;
- valuable for Gothic atmosphere if kept abstract and non-human enough.
Priority: VERY HIGH CONCEPT REFERENCE

### REF-ADV-008 — math-sonify
Source: https://github.com/Mattbusel/math-sonify
Type: generative synthesizer driven by dynamical systems
Relevant architecture:
- Lorenz;
- Rössler;
- double pendulum;
- Kuramoto;
- three-body;
- hyperchaotic systems;
- mappings into granular / spectral / FM / AM / waveguide / resonator synthesis.

Noctomorph takeaway:
- chaotic/dynamical systems may be better long-form modulators than stacked LFOs;
- deterministic chaos can produce repeatable but non-obvious evolution;
- especially promising for EVOLVE trajectories and sparse-event scheduling.
Priority: EXTREMELY HIGH EVOLUTION REFERENCE

### REF-ADV-009 — RipplerX
Source: https://github.com/tiagolr/ripplerx
Type: open-source physically modeled/modal synth
Relevant architecture:
- modal bodies;
- inharmonicity;
- model ratios;
- mallet/exciter concepts;
- serial physical coupling references.

Noctomorph takeaway:
- explicit inharmonicity control is essential for dark/unreal bodies;
- controlled departure from harmonic mode ratios can move from “instrument” to “architecture”.
Priority: VERY HIGH DSP REFERENCE

### REF-ADV-010 — ShadowScape Generator
Source: https://tekengine-audio.itch.io/shadowscape-generator
Type: standalone drone / texture generator
Relevant architecture:
- three classic oscillators;
- detune beating;
- noise beds;
- granular user-sample engine;
- procedural rumble / metallic / mechanical layers.

Noctomorph takeaway:
- useful baseline for the feature set we must exceed;
- Noctomorph should avoid becoming merely oscillators + noise + granular + reverb.
Priority: COMPETITIVE BASELINE

### REF-ADV-011 — GLACIER
Source: https://tekengine-audio.itch.io/glacier
Type: three-layer granular synthesizer
Relevant architecture:
- three independent source/layer chains;
- harmonic-series grain scheduling;
- multiple grain windows.

Noctomorph takeaway:
- harmonic-ratio grain scheduling is worth testing as a way to preserve coherence while clouds evolve;
- compare against random and state-driven scheduling.
Priority: HIGH GRANULAR REFERENCE

### REF-ADV-012 — Revelation Drone
Source: https://morkshmork.itch.io/revelation-drone
Type: sustained drone instrument
Relevant architecture:
- supersaw/sub/noise foundation;
- slow swelling;
- granular unison drift;
- random-walk pitch behavior;
- tape echo / wow / flutter.

Noctomorph takeaway:
- useful baseline for conventional “living drone” behavior;
- random-walk pitch can work but must be bounded by archetype and tonal role.
Priority: COMPETITIVE REFERENCE

### REF-ADV-013 — RITUAL Audio Engine
Source: https://tekengine-audio.itch.io/ritual-audio-engine
Type: dark-ambient granular instrument
Relevant architecture:
- three sample layers;
- vocal/instrument/noise source library;
- continuously evolving textures.

Noctomorph takeaway:
- confirms market appetite for dark evolving engines;
- Noctomorph should differentiate through real-world cross-coupling, physical bodies, state evolution and non-literal Gothic cues rather than “ritual sample library” identity.
Priority: MARKET / AESTHETIC REFERENCE

## New engine hypotheses from advanced sweep

### A. Impossible Body Engine
Exciter -> modal/waveguide body -> optional coupled second body -> space.

Candidate parameters:
- BODY SIZE
- MATERIAL
- INHARMONICITY
- DAMPING
- COUPLING
- EXCITER TYPE / SOURCE
- BODY MOTION

Goal:
Create coherent resonant structures that sound physically plausible but could not exist.

### B. Deterministic-chaos EVOLVE engine
Use bounded dynamical systems instead of ordinary periodic LFOs for macro evolution.

Properties:
- deterministic/repeatable with seed/state recall;
- non-periodic over musically useful durations;
- map separate state dimensions to density, spectral centroid, body damping, event probability, spatial motion and regeneration;
- clamp and smooth all mappings;
- preserve scene identity.

Candidate systems to prototype:
- Lorenz;
- Rössler;
- coupled oscillators / Kuramoto-inspired phase relationships;
- slow double-pendulum-like trajectories.

### C. Abstract Presence / Phantom Voice layer
Not a choir sampler.

Possible ingredients:
- noise / real texture / resonator grains;
- broad formant banks;
- very slow formant drift;
- coherence parameter;
- pitch tracking optional;
- spectral freeze/morph.

Goal:
A dark “presence” that can suggest voices/ritual/sacred space without exposing literal words, melodies or recognisable choir recordings.

### D. Spectral-memory layer
Instead of freezing one FFT frame:
- capture weighted spectral history;
- decay bins independently;
- cross-morph between history states;
- retain selected partial families;
- slowly mutate magnitude and phase coherence.

Goal:
Allow a world to remember earlier events and let them haunt later states without obvious repetition.

### E. Physical-source + synthetic-body decoupling
A real recording need not remain recognizable.

Examples:
- ice crack excites 40 m virtual plate;
- transformer buzz excites impossible glass/steel hybrid;
- raven transient excites subterranean pipe body;
- distant bell modes seed synthetic coupled resonators.

This should be a central Noctomorph design principle.


## Heritage acoustics / structural excitation expansion — 2026-09-30

### Cathedral acoustics datasets

#### DATA-IR-001 — Aachen Cathedral, St. Nicholas Chapel
Source: https://zenodo.org/records/22207913
Creators: Martin Zerwas, Selin Kayku
Licence: CC BY 4.0
Type: measured room-acoustic dataset
Contents:
- 46 measured room impulse responses;
- 4-channel B-format (FuMa);
- 48 kHz WAV;
- 45 source-receiver combinations plus repeat measurement;
- omnidirectional dodecahedron source;
- Sennheiser Ambeo / IRIS 3D receiver;
- exponential sine-sweep capture;
- ISO 3382-1 measurement procedure;
- late-Gothic coupled two-storey chapel;
- approx. 3050 m³;
- mid-frequency reverberation time approx. 4.6 s.
Commercial-use status:
- permitted under CC BY 4.0 with attribution, licence link and indication of changes.
Potential use:
- production convolution source if attribution obligations are included in product documentation;
- reference for coupled-volume decay;
- B-format spatial research;
- benchmark for hybrid FDN/convolution space design.
Priority: EXTREMELY HIGH

#### DATA-IR-002 — York Minster Chapter House
Source: https://zenodo.org/records/4040994
Licence: CC BY-NC-SA 4.0
Type: measured and simulated B-format room impulse-response dataset
Potential use:
- RESEARCH / ACOUSTIC REFERENCE ONLY
Reason:
- NonCommercial restriction is incompatible with commercial Noctomorph embedding.
Priority: HIGH REFERENCE

#### DATA-IR-003 — Ely Cathedral Lady Chapel
Source: https://zenodo.org/records/5150020
Licence: CC BY-NC-SA 4.0
Type: measured and simulated B-format room impulse-response dataset
Potential use:
- RESEARCH / ACOUSTIC REFERENCE ONLY
Reason:
- NonCommercial restriction is incompatible with commercial Noctomorph embedding.
Priority: HIGH REFERENCE

### Structural / contact-mic additions

#### FS-STRUCT-005 — Metal post / cable vibration, contact mic
Source: https://freesound.org/people/felix.blume/sounds/476742/
Creator: Felix Blume
Licence: CC0
Type: real structural contact-mic recording
Specs: WAV, mono, 96 kHz / 24-bit, 4:04.352
Description:
- vibration of a metal post;
- cable knocking;
- wind-driven structural motion;
- captured with H2a Aquarian contact mic.
Potential use:
- long non-periodic structural layer;
- spectral/body extraction;
- impossible-body excitation;
- transformed metallic-world foundation.
Priority: EXTREMELY HIGH

#### FS-STRUCT-006 — Contact mic metal scrape, yard objects
Source: https://freesound.org/people/ilmari_freesound/sounds/585517/
Creator: ilmari_freesound
Licence: CC0
Type: real contact-mic recording
Specs: WAV, mono, 96 kHz / 24-bit, 45.198 s
Potential use:
- micro-grain excitation;
- friction texture;
- resonator drive;
- spectral-smear source.
Priority: VERY HIGH

#### FS-STRUCT-007 — Contact mic metal scrape variant
Source: https://freesound.org/people/ilmari_freesound/sounds/585528/
Creator: ilmari_freesound
Licence: CC0
Type: real contact-mic recording
Specs: WAV, mono, 96 kHz / 24-bit, 13.050 s
Potential use:
- alternate friction/excitation source.
Priority: HIGH

#### FS-STRUCT-008 — Keys scraped against metal gate, contact mic #1
Source: https://freesound.org/people/JarredGibb/sounds/219059/
Creator: JarredGibb
Licence: CC0
Type: real contact-mic metal-on-metal recording
Specs: WAV, stereo, 48 kHz / 24-bit, 21.645 s
Potential use:
- aggressive friction grain source;
- high-frequency metallic excitation;
- transformed industrial event.
Priority: HIGH

#### FS-STRUCT-009 — Keys scraped against metal gate, contact mic #2
Source: https://freesound.org/people/JarredGibb/sounds/219060/
Creator: JarredGibb
Licence: CC0
Type: real contact-mic metal-on-metal recording
Specs: WAV, stereo, 48 kHz / 24-bit, 16.661 s
Potential use:
- alternate scrape/event pool.
Priority: HIGH

## Heritage-acoustics policy

Measured heritage-space impulse responses are valuable because they provide:
- real multi-rate decay;
- direction-dependent early energy;
- physically plausible late-field behavior;
- unusual coupled-volume signatures.

But production use requires exact licence classification:
- CC0 / Public Domain: preferred;
- CC BY: acceptable only if attribution and modification notices are deliberately shipped;
- CC BY-NC / NC-SA: reference only for a commercial product;
- unclear/custom terms: do not embed until cleared.

Noctomorph should use heritage acoustics as real-world anchors, then extend them into unreal spaces through:
- IR morphing;
- filtered / frequency-dependent decay remapping;
- early/late field decoupling;
- synthetic late tails;
- modal-body injection;
- spatial motion after convolution.
