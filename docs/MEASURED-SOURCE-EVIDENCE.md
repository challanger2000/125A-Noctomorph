# Measured Source Evidence

## DATA-IR-001 — Aachen Cathedral, St. Nicholas Chapel

Status: **MEASURED — FULL DATASET**

Dataset facts:
- 46 measured room impulse responses;
- all 48 kHz;
- all 4-channel B-format;
- 26 files are 10.0 s, 19 files are 6.0 s, 1 file is 7.0 s;
- individual SHA-256 values are recorded in `measurements/aachen/DATA-IR-001-summary.csv`.

Measured comparison range:
- direct arrival: **7.71–111.48 ms**, mean **42.90 ms**
- decay to -20 dB: **0.716–1.361 s**, mean **1.098 s**
- decay to -30 dB: **1.424–2.166 s**, mean **1.822 s**
- decay to -60 dB: **4.483–9.965 s**, mean **7.546 s**
- per-channel -60 dB range across the dataset: **4.567–9.941 s**
- spectral centroid: **1.607–1.724 kHz**, mean **1.662 kHz**

Interpretation:
- source/receiver position materially changes the decay signature;
- this is much more useful than treating the chapel as one fixed reverb preset;
- shorter and longer coupled-volume states can be selected or morphed deliberately;
- B-format channels make it useful for later spatial decoding/research rather than only mono convolution;
- because the dataset is CC BY 4.0, production use requires attribution, licence link and modification disclosure.

Engineering note:
The repository's -60 dB figures are deterministic comparison landmarks from peak-aligned Schroeder-style energy decay. They are **not** presented as standards-compliant RT60/T30 values.

## DATA-IR-004 — OpenAIR R1 Nuclear Reactor Hall

Status: **MEASURED**

- format: mono WAV, 48 kHz / 24-bit
- duration: 19.939 s
- SHA-256: `02a0ba7c1075ec3bb336a907e99bddccae322b835bdba7787645b3eaeef24ef7`
- peak: -8.44 dBFS
- direct arrival: 42.56 ms
- decay to -10 dB: 0.512 s
- decay to -20 dB: 1.259 s
- decay to -30 dB: 2.120 s
- decay to -60 dB: 6.392 s
- early 0–80 ms / late energy: -0.46 dB
- spectral centroid: 1.256 kHz

Interpretation:
- strong long industrial hall tail;
- relatively substantial early-vs-late balance;
- useful candidate for monumental INDUSTRIAL / ABYSS spaces;
- production use still carries OpenAIR CC BY 4.0 attribution obligations.

## DATA-IR-005 — OpenAIR Terry's Factory Warehouse

Status: **MEASURED**

- format: mono WAV, 48 kHz / 24-bit
- duration: 22.669 s
- SHA-256: `2c0f61619e8a66ffa526f6b27bf76773e5896fe43d7408c413abf3a6f7b42cf0`
- peak: -10.67 dBFS
- direct arrival: 75.46 ms
- decay to -10 dB: 1.004 s
- decay to -20 dB: 2.512 s
- decay to -30 dB: 4.997 s
- decay to -60 dB: 20.506 s
- early 0–80 ms / late energy: -2.05 dB
- spectral centroid: 1.428 kHz

Interpretation:
- dramatically longer decay than R1;
- later field dominates more strongly;
- strong candidate for huge decaying warehouse / unreal industrial-space states;
- likely better as a sparse/hybrid long-tail layer than an always-on default room.

## Measurement note

The current decay figures are deterministic engineering measurements from the repository analysis utility. They are not being claimed as standards-compliant RT60/T30 values; they are peak-aligned Schroeder-style decay landmarks for source comparison.


## DATA-IR-006 — OpenAIR Maes Howe Tomb

Status: **MEASURED**

- format: 4-channel B-format WAV, 48 kHz / 24-bit
- duration: 1.000 s
- SHA-256: `0b68eb5ae862a7f0e45c97a1eeb4c332e9dc98dfd10f3444d7812254a94cdbb0`
- direct arrival: 42.98 ms
- decay to -20 dB: 0.108 s
- decay to -30 dB: 0.196 s
- decay to -60 dB: 0.528 s
- channel -60 dB range: 0.561–0.608 s
- early 0–80 ms / late energy: +16.50 dB
- spectral centroid: 3.968 kHz

Interpretation:
- very short, hard stone-space response rather than a conventional long reverb;
- excellent candidate for body coloration / stone resonator stages;
- useful contrast to the long cathedral and mausoleum spaces;
- likely more valuable as a physical coloration component than as the main SPACE tail.

## DATA-IR-007 — OpenAIR Hamilton Mausoleum

Status: **MEASURED**

- format: 4-channel B-format WAV, 48 kHz / 24-bit
- duration: 15.000 s
- SHA-256: `e58665dfd8a848e6ebcd8f1e982289aca187781502eeba6d5d8cd581bffb96c5`
- direct arrival: 11.85 ms
- decay to -20 dB: 2.648 s
- decay to -30 dB: 5.127 s
- decay to -60 dB: 14.569 s
- channel -60 dB range: 14.738–14.906 s
- early 0–80 ms / late energy: +3.49 dB
- spectral centroid: 1.135 kHz

Interpretation:
- genuinely monumental Gothic/stone tail;
- substantially longer than R1 and Aachen mean positions;
- strong candidate for upper-range SPACE / NOCTURNE / RUINS states;
- the B-format channels make later spatial decoding preferable to naive mono collapse.

