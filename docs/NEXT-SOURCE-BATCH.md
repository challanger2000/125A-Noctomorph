# Noctomorph — Next Source Batch

Status: acquisition plan
Date: 2026-09-30
Branch: `dev/prototype-v0.1`

## Goal

Expand beyond the current 25-source bank with material that strengthens
VOID / RUINS / ABYSS / NOCTURNE rather than adding more foreground machinery.

Priority is source diversity and cinematic identity, not raw file count.

## Acquisition classes

### A. CI-ready direct-download sources

#### OGA-SHOP-001 — LEGIT Audio “The Shop” free WAV subset
Source page:
https://opengameart.org/content/the-shop

Direct WAV archive:
https://opengameart.org/sites/default/files/legit_audio_-_the_shop_free_sfx_wav.zip

Licence:
CC0 for the free files distributed through OpenGameArt.

Use direction:
- interior room tone
- appliance/fan drone
- neutral enclosed-space movement
- transformed NOCTURNE / RUINS background layers

Rule:
Do not use obvious retail/shop identity. Segment only neutral room, fan, motor
or pressure-like intervals that survive transformation into an abstract world.

#### DATA-IR-001 — Aachen Cathedral St. Nicholas Chapel
Source:
https://zenodo.org/records/22207913

Direct archive:
https://zenodo.org/records/22207913/files/impulse_responses.zip?download=1

Licence:
CC BY 4.0

Measured contents already verified in this repository:
- 46 measured 4-channel B-format impulse responses
- 48 kHz
- late-Gothic coupled chapel
- long measured decay

Use direction:
- RUINS / NOCTURNE / VOID / ABYSS spatial identity
- hybrid SPACE calibration
- late-tail extraction
- early/late energy variants

Rule:
Do not embed the full 271.5 MB archive blindly. Select representative positions
and/or derive compact production IRs after measured comparison.

### B. Rights-verified but not yet CI-ready

These Freesound sources are verified CC0 at their public source pages, but the
normal download path requires account/API access. They remain production
candidates until an authenticated acquisition path or alternate mirror is
available.

#### FS-WIND-001 — Wind - Low Frequency, Warm and Strong
Source:
https://freesound.org/people/bassimat/sounds/861757/

Verified:
- CC0
- stereo
- 48 kHz / 32-bit
- 1:15.418
- strong low-frequency sustained wind

Use:
VOID / WASTELAND pressure and moving low-mid texture.
Avoid literal weather-loop identity.

#### FS-STORM-001 — Distant Thunder
Source:
https://freesound.org/people/Beetlemuse/sounds/530133/

Verified:
- CC0
- stereo
- 44.1 kHz / 16-bit
- 2:51.280

Use:
VOID / ABYSS long low-frequency mass and distant transformed events.
Avoid obvious thunder playback.

#### FS-UND-004 — Commons Tunnel
Source:
https://freesound.org/people/rawhiteman7/sounds/381160/

Verified:
- CC0
- stereo
- 44.1 kHz / 16-bit
- 0:52.863
- rushing tunnel water + rock impacts / tunnel echoes

Use:
RUINS / ABYSS WORLD and sparse structural-event extraction.

#### FS-UND-001 — Underground metro station room tone
Source:
https://freesound.org/people/kyles/sounds/450924/

Licence:
CC0

Use:
RUINS / ABYSS large concrete-room WORLD material.

Caution:
Distant echo voices are present. Only voice-free or sufficiently abstracted
segments may be considered for production.

## Next production target

After the current 25-source bank passes:

1. Add 6–10 non-industrial sources first.
2. At least:
   - 2 architectural / room sources
   - 2 underground / pressure sources
   - 1 low-frequency environmental mass source
   - 1 genuinely different sparse event family
3. Add at least two distinct production SPACE identities from measured IR data.
4. Measure each new source and derived segment before embedding.
5. Re-run:
   - Core QA
   - real-source render QA
   - archetype listening QA
   - long-form repetition/identity QA
   - VST3 host / validator QA

## Memory / asset-size policy

Do not reject a useful source merely to minimize RAM.

Optimize without sonic loss:
- remove unused source regions;
- keep mono assets mono where stereo carries no useful spatial information;
- retain stereo for WORLD / room / spatially meaningful material;
- avoid duplicate buffers and duplicate derived files;
- use compact, measured production segments rather than full raw recordings
  when long unused regions provide no additional identity.

A larger memory footprint is acceptable when it produces measurable source
diversity and audible scene differentiation.
