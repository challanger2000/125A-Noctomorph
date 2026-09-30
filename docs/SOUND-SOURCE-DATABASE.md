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
