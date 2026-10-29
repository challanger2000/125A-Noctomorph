# Noctomorph: unabhängiger Audit und Fix-Kandidat, 29.10.2026

**Status: lokale Regressionen bestanden; Windows/Host-Freigabe OFFEN. Kein FULL/FINAL QA PASS.**

## Herkunft und Umfang

- Repository: `challanger2000/125A-Noctomorph`; Version weiterhin **0.0.4**.
- Der anfangs vorhandene Checkout enthielt nur den Initial-Commit. Vor Produktänderungen wurde der ausdrücklich beauftragte Entwicklungsbranch abgerufen.
- Verifizierte Entwicklungsbasis: `dev/v4jb-gui-redesign-20261029`, **`a855d2d477f699326572646265e60d6728bd78f0`**.
- Separater Arbeitsbranch: **`audit/noctomorph-20261029`**. `main` bleibt bei **`bf59df92ad503ffeaee1fe8ce440e7064f46d8b5`**. Kein Release, kein Merge nach `main`.
- Hauptkorrektur: **`c447b08ecd1d3486ffa0a3f9437aa7c16f93749e`**.
- Finaler geprüfter Produkt-/Teststand: **`ea34765ebe607f72e1d789ea15647fa422b10b18`**. Der nachfolgende Berichtscommit verändert keinen Produktcode; seine SHA ist über die Branch-Historie verfügbar.
- Engineering-Einstieg und einschlägige Guardrails, DSP-/GUI-/Repository-/QA-Standards gelesen aus `125A-Engineering`, Commit `1b506cdd0115132196d48ea34b337035031dec0e`. `START-HERE.md` lag dort, nicht im Produktcheckout. Der initiale Audit-Bericht wurde vollständig gelesen und unabhängig geprüft.
- Originalressourcen: V4JB **`2b3bc12400ad3337ba6d8f2e37dc61672c39b951`**, alle **494** Factory-Presets und vier Factory-WAVs unverändert. JUCE 9.0.4: **`3fb5cbc0340758596dac39f7c8de47d562b4896e`**.
- `ParameterIDs.h` und `ParameterLayout.h` sind gegenüber der Entwicklungsbasis unverändert. PLAY, sämtliche ADVANCED-Seiten, Module, Presets und Parameter bleiben erhalten. Die Projucer-Produktbasis wurde nicht durch CMake ersetzt; CMake dient ausschließlich den Audit-Tests.

Statisch geprüft wurden Processor/Editor, alle `src/dsp`-Implementierungen, WaveAnalyzer, Presets, Parameterdefinitionen und alle GUI-Seiten einschließlich ihrer Callbacks. Build-/Hilfsskripte und bestehende Tests wurden hinsichtlich ihrer Aussagekraft geprüft. **Statische Durchsicht ist keine vollständige Laufzeitverifikation jeder Funktion.**

## Korrigierte Befunde

Zeilen beziehen sich auf den finalen Produktcommit. P1 = Speicher-/Stimmen-/Recall-Sicherheit, P2 = funktionaler Fehler, P3 = begrenzte Robustheit.

| ID | Priorität / Stelle | Ursache und Korrektur | Nachweis |
|---|---|---|---|
| A01 | P1, `PluginProcessor.cpp:257,555`, `dsp/AudioReadSlot.h:13` | Zeitablauf erlaubte Freigabe noch gelesener WAV-Daten. Seq-cst-Hazard-Protokoll mit getrenntem spekulativem und bestätigtem Zeiger; Besitzer/Freigaben ausschließlich außerhalb Audio. Collector liest spekulativen vor bestätigtem Hazard, damit die Übergabe nicht übersehen wird. | Alter Processor verliert beim simulierten 11-s-Timer trotz gehaltenem Leser den letzten Besitzer: FAIL. Neuer Processor hält ihn über 1000 Publikationen. 10000 parallele Publikationen und Leser unter ASan/UBSan sowie TSan: PASS. |
| A02 | P1, `PluginProcessor.cpp:146,156` | Volle 31-Ereignis-Tastaturqueue verlor Note-Offs. Overflow setzt nun einen unabhängigen Panic-Marker. Audio konsumiert ihn vor dem Queue-Snapshot, verwirft den unvollständigen Batch und gibt Stimmen frei; ein neuer Overflow bleibt für den nächsten Callback erhalten. | Vorher acht gehaltene Stimmen und hörbare Ausgabe nach Releases; nachher keine gehaltene Stimme und leiser Ausklang: PASS. |
| A03 | P1, `PluginProcessor.cpp:270` | Mono wurde auf Scratch-Länge gekappt und der Rest genullt. Gemeinsame, begrenzte Render-Chunks für Mono/Stereo, MIDI-Zeitpunkte bleiben global; keine Scratch-Vergrößerung in Audio. | 30 Kombinationen: 44.1/48/96 kHz × 0,1,16,127,128,511,8192,8193,16384,32769 Frames. Mono gegen Stereo-Mittel, endliche Ausgabe, nicht abgeschnittener Schluss: vorher FAIL, nachher PASS. |
| A04 | P1, `dsp/Engine.h:146` | Nach Poly→Mono-Wechsel folgten alte Stimmen noch polyphoner Belegung, Note-Off aber bereits Mono-Stack-Semantik. Moduswechsel gibt alte Stimmen frei; sinkendes Poly-Limit gibt überzählige Stimmen frei. | Zwei Poly-Noten, Wechsel auf Mono, beide Note-Offs: vorher weiterhin gehaltene Stimme, nachher PASS. Bewusste Policy: Moduswechsel lässt alte Stimmen ausklingen. |
| A05 | P2, `PluginProcessor.cpp:242`, `dsp/TransportSafety.h`, `dsp/Engine.h:159` | Ungültiges Tempo erreichte Delay/Beatclock. Verifizierte PR-#3-Normalisierung übernommen, auch Engine-Eingang geschützt; nichtendliche PPQ wird nicht als spielende Position übernommen. | PR-Test plus echter Processor mit 0, negativ, Inf, NaN, winzigem BPM: vorher Beatclock Inf, nachher PASS. Policy: außerhalb 1..1000 BPM → 120; kein Anspruch auf Unterstützung extremer positiver Tempi außerhalb dieses Bereichs. |
| A06 | P2, `PluginProcessor.cpp:527`, `ui/WavePage.h:90` | Fehlgeschlagener Import verschwand ohne Rückmeldung. Fehlertext pro Slot, sichtbare Anzeige und UI-Version-Änderung; bisherige Wave bleibt unverändert. | Nicht vorhandene WAV: vorher keine Statusänderung, nachher Statusänderung und gleicher alter Wave-Besitzer: PASS. Keine modale Dialogprüfung behauptet. |
| A07 | P1, `PluginProcessor.cpp:585,604`, `WaveState.h` | Nur Dateipfade wurden gespeichert; fehlende Dateien veränderten den Sound. Neue States enthalten exakt die vom Synth verwendeten Analysedaten, versioniert und größenbegrenzt. Legacy-Dateien werden vor der Übernahme validiert. Defekte eingebettete Daten verwerfen den Recall vor Parameter-/Wave-Änderungen. Ältere Worker-Ergebnisse werden per Generation verworfen. | Neue Instanz restauriert Wave trotz fehlender Quelldatei; gleiche Analysefloats und Stereo-PCM innerhalb 1e-6; beschädigtes Payload verändert weder Wave noch geprüften Parameter: PASS. |
| A08 | P2, `PluginProcessor.cpp:397` | Delay addierte Echo zu unverändertem Dry. Korrigiert auf linearen Crossfade `(1-mix)*dry + mix*echo`. | Impuls bei 0/25/50/75/100%, unabhängige Direct-Gain-Referenz und vorhandene Echoausgabe: vorher FAIL, nachher PASS. |
| A09 | P2, `PluginProcessor.cpp:411` | Reverb hielt Dry bis 100% fest; zusätzlich skaliert JUCE `dryLevel` intern ×2. Jetzt `0.5*(1-mix)`, bestehende Wet-Netzwerk-Skalierung bleibt erhalten. | Gleicher Impulstest: vorher bei 0% doppelter Direct-Pegel, bei 100% voller Direct-Pegel; nachher unity/zero: PASS. |
| A10 | P2, `PluginProcessor.cpp:44,450` | Vier Dezimalstellen beschädigten Morph-Corners/Nodes beim Speichern. Wissenschaftliche Schreibweise mit genügend signifikanten Stellen für Float-Roundtrip. | 0.12345678 sowie 1e-12 bleiben erhalten; vorher FAIL, nachher PASS. |
| A11 | P2, `PluginProcessor.cpp:489` | Store Corner setzte auch den Offset von „Free“/gelockten Makros zurück, obwohl diese den gespeicherten Corner ignorieren. Diese Offsets bleiben jetzt erhalten. | Makro-Ausgabe vor/nach Store: vorher Sprung, nachher gleich: PASS. |
| A12 | P2, `ui/MorphPage.h:334` | Recall/Clear konnte eine Auswahl außerhalb des neuen Pfades lassen. Refresh verwirft ungültige Auswahl und Drag-Index. | Offener Editor, Auswahl 3, Pfad auf null Nodes: vorher stale Auswahl, nachher -1; drei Lebenszyklen: PASS. |
| A13 | P2, `ui/VersionCompare.h:29` | Lange Remote-Versionskomponente verursachte signed integer overflow. Vor Multiplikation/Addition prüfen und ungültigen Tag verwerfen. | Ursprünglicher Code mit UBSan: signed overflow, Exit 1. Korrigiert: PASS. |
| A14 | P2, `dsp/Morph.h:88` | Negativer PPQ-Preroll ergab negativen fmod-Rest und klemmte die Morph-Path-Position am Anfang fest. Positiver zyklischer Rest. | -2 Beats entspricht +6 Beats im 8-Beat-Pfad; vorher FAIL, nachher PASS. |
| A15 | P2, `PluginProcessor.cpp:290` | `MidiMessage`-Kopie allokierte für ignorierte lange SysEx-Nachrichten im Callback. Nicht unterstützte lange Nachrichten werden vor der Kopie übersprungen. | Linux-Heap-Wrapping von malloc/calloc/realloc sowie C++ new: vorher 1, nachher 0 im 32769-Frame-Callback mit SysEx/Note-On. Ein reiner new-Zähler hatte den Fehler zunächst NICHT erkannt. |
| A16 | P2, `PluginProcessor.cpp:15,604`, `WaveAnalyzer.h:57` | Malformed Morph-Daten wurden akzeptiert; Decoder-Leseerfolg und nichtendliche PCM wurden nicht geprüft. Morph-Struktur/Koordinaten/Segmente, State-Werte/-Größe und WAV-Header/Leseergebnis/PCM werden validiert. | Malformed Morph vorher angenommen, nachher zurückgewiesen. Echte FLOAT32-WAV mit endlichem Impuls akzeptiert, gleiche WAV mit NaN abgelehnt: PASS. Explizite Lese-I/O-Fehlerinjektion: NOT RUN. |

Weitere begrenzte Absicherungen: `dsp/Modules.h:401` reduziert Phrase-PPQ vor Integer-Konvertierung; der Extreme-PPQ-Test besteht, reproduzierte aber im ursprünglichen optimierten Build keinen UBSan-Fehler. Deshalb **kein behaupteter Vorher-FAIL** dafür. Preset-/Wave-Metadaten werden zwischen Nicht-Audio-Aufrufern synchronisiert; Editor-Scale ist atomar. Ein kompletter JUCE-Processor-TSan-/Host-Recall-Stresstest bleibt offen.

### Letzte Kandidatenverfeinerung

Der erste Fix-Kandidat konnte bei einer Publikationskollision für einen Block ohne Wave zurückkehren. Der zusätzliche Kontinuitätstest brach gegen diesen Kandidaten mit Exit **134** ab. `ea34765` hält stattdessen die letzte sicher geschützte Wave bis zur nächsten erfolgreichen Übernahme. Kein Spin/Retry in Audio. Zwischen Callbacks bleibt ein bestätigter Hazard stehen; `releaseResources()` gibt ihn frei. Der Collector kann dadurch höchstens zwei **verschiedene** alte Objekte pro Slot schützen, statt unbegrenzt gestoppte Publikationen aufzubewahren. Der Eigentumsvertrag bleibt: Daten nach Veröffentlichung nicht verändern, genau ein Audio-Leser je Instanz, serialisierte Nicht-Audio-Publisher.

## Bewusste Klangänderung und Kompatibilität

Bei gleichem 0.1-Impuls und gleichem bestehenden Limiter betrug der Direct-Sample-Pegel:

| Mix | Delay vorher | Reverb vorher | Beide nachher |
|---:|---:|---:|---:|
| 0% | 0.163117 | 0.326235 | 0.163117 |
| 25% | 0.163117 | 0.285455 | 0.122338 |
| 50% | 0.163117 | 0.244676 | 0.0815586 |
| 75% | 0.163117 | 0.203897 | 0.0407793 |
| 100% | 0.163117 | 0.163117 | 0 |

Die Werte über 0.1 entstehen durch die unveränderte Limiter-Gain-Struktur. Der Test misst die Direktsignal-Komponente vor Eintreffen der verzögerten Antwort; er verlangt zusätzlich spätere Wet-Ausgabe. JUCEs Reverb-Wet-Skalierung ×3 wurde nicht umgestimmt. Die ursprüngliche Aussage „50% Dry bei 100% Reverb“ im Initialbericht bezog sich nur auf den übergebenen Parameter und übersah dessen interne ×2-Skalierung.

Die neue WAV-Headerprüfung akzeptiert 1..768000 Hz; Quellen außerhalb dieser
Range werden mit Importfehler zurückgewiesen. Das ist eine zusätzliche
Ressourcen-/Eingabepolicy, keine aus der Synthese abgeleitete Formatgrenze.
Auch innerhalb dieser Grenze kann die bisherige vollständige 120-s-Decodierung
viel RAM benötigen; Streaming-Analyse und Ressourcenstress sind nicht erledigt.

**Vorhandene Presets/DAW-Projekte mit aktiven Effekten klingen daher absichtlich anders.** Keine stille Äquivalenzbehauptung; keine neuen/umbenannten Parameter. Subjektive, pegelabgeglichene Abnahme bleibt NOT RUN. Neue States speichern Analysefloats, nicht die Original-WAV-Datei. Maximal zwei Analysen mit je 2048×256 Floats: ca. 4 MiB Nutzdaten plus Text-Encoding. Alte Plugin-Versionen können diese Erweiterung nicht ohne ihre ursprünglichen Quelldateien lesen. Alte Pfad-States mit fehlenden Dateien werden sichtbar abgelehnt und erhalten den vorherigen Sound; sie können nicht nachträglich verlorene Medien rekonstruieren.

## Unabhängige Prüfung PR #3 und #4

Beide GitHub-Seiten zeigten **DRAFT**; kein PR wurde gemergt.

- [PR #3](https://github.com/challanger2000/125A-Noctomorph/pull/3), Head **`803748424e8a36543ba963cb32b4347c1068a41a`**, eigentliche Korrektur **`f2b594bba315171e481d0991c37f42386c89cc66`**: Header/Test aus dem PR separat extrahiert, C++17, NDEBUG, Werror, ASan/UBSan: PASS. Grenzen 1..1000 sind eine bewusste Policy. Der Test prüft nur einen Helper; erst der zusätzliche echte Processor-Test belegt die Einbindung. Header/Test übernommen, Routing unabhängig ergänzt. PR wurde nicht blind gecherry-pickt.
- [PR #4](https://github.com/challanger2000/125A-Noctomorph/pull/4), Head **`99ca2beebcf0091fa2e5a01e83b0005498bd1987`**: separater Policy-/Thread-Test unter ASan/UBSan: PASS. Entfernen des Zeit-Fallbacks ist richtig. Sein Test beweist jedoch weder den echten Processor noch begrenzte Speicherhaltung bei ausbleibenden Callbacks. Daher **nicht als vollständige WAV-Lösung übernommen**, sondern durch getestete Hazard-Ownership ersetzt. Grundannahme einer serialisierten Audio-Instanz bleibt ausdrücklich dokumentiert.

## Ausgeführte Tests und Grenzen

Umgebung: Linux x86_64, GNU g++ **14.2.0**, C++17, Debug, JUCE 9.0.4, ASan + UBSan + float-cast-overflow + LeakSanitizer. AMD EPYC 9V74, fünf sichtbare CPUs. Tests außerhalb der ptrace-Sandbox, da LeakSanitizer dort nicht arbeiten kann; keine abgeschaltete Leak-Erkennung. Keine Windows- oder Studio-One-Ausführung in dieser Maschine.

**Finale Suite: 18/18 PASS**, Rohprotokolle unter [`audit-evidence`](audit-evidence), Wiederholanleitung in [`tests/README.md`](../tests/README.md).

| Test / Vertrag | Beobachtung | Status / Grenze |
|---|---|---|
| `engine_check` | 18 Spektralalgorithmen; Modul-Extrema, Waves, Voice-Stealing, Release, Morph-Bilerp. Chord RMS 0.171, Peak 0.587, Tail 0.00000, wie ursprünglicher Test. | PASS; keine vollständige Klangreferenz für jeden Parameter |
| `version_check` | Bestehende Versionsvergleiche | PASS |
| `processor_blocks` | 30 reale Mono-/Stereo-/Samplerate-/Block-Kombinationen, unabhängiges Stereo-Mittel | PASS |
| `processor_keys` | Voller FIFO, verlorene Releases, tatsächlich keine gehaltenen Stimmen/kein Dauerton | PASS |
| `processor_wet` | 10 Impulsfälle, Direct-Gain-Orakel und Wet-Ausgabe | PASS |
| `processor_tempo` | Ungültige BPM durch echten PlayHead/Processor | PASS |
| `processor_state` | Alle Parameter, Morph-Präzision, fehlende WAV-Quelle, frische Instanz und PCM, Payload-Rollback, Free/Store, malformed Morph | PASS; kein DAW-Projekt |
| `processor_gui` | Drei Editor-Lebenszyklen, 10 Slot-/5 FX-Callbacks, 233 physische Parameter-Widgets für 224 eindeutige IDs sowie beide XY-Parameter, beidseitige Attachments/XY-Ereignisse | PASS im Harness; keine DAW-Maus-/Popup-/Dialog-/Zoom-Abnahme |
| `processor_waves` | Gestallter Leser/1000 Swaps, Importfehler behält alte Wave, FLOAT32-Impuls/NaN-Datei | PASS; keine vollständige Host-Thread-Matrix |
| `processor_realtime` | Eine SysEx-/Note-/32769-Frame-Verarbeitung: 0 beobachtete Heap-Allokationen | PASS für diesen Pfad; kein allgemeines RT-/Deadline-PASS |
| `processor_presets` | Alle 494 originalen Presets geparst, Parameter-IDs geprüft, geladen, kurze Note-On/All-Notes-Off-Blöcke auf endliche Ausgabe geprüft | PASS; kein vollständiger musikalischer Render jedes Presets |
| `processor_midi` | Note-On-Offset ohne vorzeitige Ausgabe, Pitchbend-Frequenz, CC1, Channel Pressure, Note-Off-Ausklang | PASS; kein Sustain/MPE-/DAW-Gesamttest |
| `transport_safety_check` | Verifizierter PR-#3-Helper mit Grenzwerten | PASS |
| `audio_read_slot_check` | Gestallter Leser und 10000 echte Publikationen mit parallelen geschützten Lesezugriffen; kein temporär verlorener gültiger Wave-Zeiger | PASS unter ASan/UBSan; separat TSan PASS mit 19904 geschützten Lesezugriffen |
| `dsp_version`, `dsp_preroll`, `dsp_ppq`, `dsp_voices` | Overflow, negativer Morph-Loop, extremes Phrase-PPQ, Poly→Mono | 4× PASS; PPQ wie oben ohne behaupteten Vorher-FAIL |

Testadäquanz wurde selbst geprüft: boolesche Parameter wurden anfangs mit 0.37 angesteuert, was deren kanonischer State-Darstellung widerspricht. Diese 20 zusätzlichen Abweichungen sind **kein behaupteter Plugin-Defekt**; der Test verwendet gültige Bool-Endpunkte. Die erste GUI-Inventur übersah JUCEs abgehängte, nicht ausgewählte Tab-Inhalte; die Inventur wurde auf alle Seiten erweitert, nicht die fehlenden IDs aus der Anforderung gestrichen. Der pure new-Zähler übersah SysEx-malloc und wurde durch Linker-Wrapping ergänzt. Alle Checks bleiben unter NDEBUG aktiv.

## GUI-/Funktions-Traceability

[`control-inventory.tsv`](audit-evidence/control-inventory.tsv) führt jedes im Harness angetriebene Parameter-Widget auf. **224 eindeutige Parameter-IDs**, neun zusätzliche Darstellungen bestehender Parameter auf PLAY/FX, plus die zwei separat getesteten Morph-X/Y-Parameter ergeben den vollständigen 226-Parameterbestand. Die Widget-IDs dienen stabiler Zuordnung, nicht neuen Host-Parametern. Für jeden Eintrag gilt: Widget → JUCE-Attachment → vorhandener Parameter und zurück **PASS**; individuelle akustische Konsequenz, native DAW-Mausbedienung, Automation/Recall jeder einzelnen Darstellung **NOT RUN**. Das ist ausdrücklich kein pauschales „jedes GUI-Element end-to-end PASS“.

| Interaktive Befehle / Bereich | Tatsächliche Route / erwartete Funktion | Evidenz und fehlender Teil |
|---|---|---|
| PLAY, ADVANCED, SOUND, MODULATION, MORPH, WAVES | JUCE TabbedComponent → vorhandene Seiten; Header folgt Timer | Konstruktion aller Seiten, kein verlorener Parameter. Native Tab-Klick-/Fokus-/Zoom-Matrix NOT RUN |
| Header Preset, Kategorien/Einträge, `<`, `>` | `showPresetMenu`, `stepPreset` → `loadPreset` → APVTS/Morph/Waves | Alle Presets am Processor getestet; echte Menü-/Pfeilklicks NOT RUN |
| SAVE, Save/Cancel, Presetordner | `savePresetDialog`/`saveUserPreset`, `Presets::userDir` | Statische Route vorhanden; Datei-/Dialog-/Abbruch-/Rechtefehlerkette NOT RUN, siehe R04 |
| GENERATE: New / Vary | `showRandomMenu` → `randomize(true/false)` → bestehende Parameter/Module | Statische Route vorhanden; klanglicher Diversity-/Menütest NOT RUN |
| SOURCE / FX & MASTER | `EditPage::setTop` → Sichtbarkeit Spectral/FxCard | Code geprüft; tatsächliche Mausnavigation NOT RUN |
| Slots 02..11, FX-Seiten DRIVE/MOD/DELAY/REVERB/MASTER | jeweilige `onClick` → `setSlot`/`setEffect` | Jeder Auswahlcallback und Zielindex im echten Editor PASS; Maus/Popup nicht behauptet |
| Jeder Slot: Modultyp, MAIN, P1..P4, MOD, Env/LFO/MIDI Source+Amount | APVTS; `kModuleInfo` bestimmt sichtbare Regler; Engine `processModule` liest passende Felder | Alle Attachments PASS, alle Module im Engine-Extremtest. Jede Modulation/Reglerposition einzeln im PCM NOT RUN |
| Vier ADSR-/Velocity-Karten, drei LFO-Karten | APVTS → Engine Env/Lfo → Slot-Routen/Amp | Attachments PASS; einzelne Zeit-/Wellenform-Orakel NOT RUN |
| Morphpad/Corners A-D | Maus → Parameter-Gesten; Corner-Klick → `recallCorner`; Audio → `Morph::advance/macros` | XY-Ereignisse und Bilerp PASS; alle vier echten Corner-Mausklicks NOT RUN |
| Store A-D, 11 Offsets/Free, Mode/Time/Snap/User Slot | `storeCorner`, APVTS, Macro-Mapping | Free/Store-Regression und Attachments PASS; alle Store-Buttons/Lock-Kombinationen in DAW NOT RUN |
| Edit Path, Nodes Add/Drag/Delete, Segment, Clear Path | Atomare Nodes/Len/Count; `select`, `deleteNode`, Morph-Interpolation | Statische Route, Preroll-/Invalid-Selection-Tests PASS; vollständige Maus-/Gesture-/Recall-Interleavings NOT RUN |
| Save Points / Save Path / Load / Cancel | `fileAction` → Morph-Datei-Funktionen | Malformed-Datei wird zurückgewiesen; native Dialog-/Abbruch-/Fehlerrückmeldung NOT RUN |
| Waves A/B Load → Factory/File/Cancel, Clear, Start/End/Speed, Balance | FileChooser → Worker/Generation → `publishWave`; Clear nullt Slot; APVTS → Module-Region | Analyse/Fehler/Ownership/Recall und Attachments PASS. Datei-Browser-/Clear-Maus-/Reverse-/Freeze-PCM-Matrix NOT RUN |
| Standalone-Piano C2..C6 | KeyboardState Listener → `pushKey` → SPSC → Engine | Queue-/Overflow-Pfade PASS; Standalone-Maus/Editor-Schließen während gehaltener Taste NOT RUN |
| About öffnen/außerhalb schließen, Check, Releases, Close | `contentMouseUp`, AboutBox; Hintergrundthread → MessageManager/SafePointer | Statisch geprüft; tatsächliche Netzwerk-/Popup-/Close-Stressprüfung NOT RUN; Updateziel bleibt upstream V4JB |
| Resize-Griff und Regler-Defaultgesten | JUCE Constrainer/Transform, SliderAttachment-Default | Unveränderte Grundarchitektur, konstruktionserhaltender Editor-Test. Native 40–150%/Ctrl-Reset-Matrix NOT RUN |

## Offene Defekte und technische Risiken

| ID | Priorität / Stelle | Offener Punkt / notwendige nächste Prüfung |
|---|---|---|
| R01 | P1, `PluginProcessor.cpp:704,719,604` | Host-Programmwechsel lädt XML/APVTS und gegebenenfalls Legacy-WAV synchron unter Nicht-Audio-Lock. Der Host darf diesen Pfad nicht in einer Echtzeit-Callback-Kette ausführen. Vor einer RT-Freigabe: vorgefertigte Preset-/Wave-Snapshots und geprüfte RT-Übergabe, tatsächlicher VST3-Program-Change-/Automation-Stress. Kein bloßes asynchrones Verschieben mit stiller Offline-Render-Verzögerung eingebaut. |
| R02 | P2, `PluginProcessor.h:33`, `dsp/Engine.h` Release, `PluginProcessor.cpp` Delay-FB | Pauschale Tail-Meldung 20 s ist bei langen Releases/hohem Feedback nicht konservativ. Beispiel: Delay 2 s, Feedback .95 benötigt ca. 269 s allein für -60 dB; Amp-Release kann durch Makro auf 20 s Zeitkonstante skaliert werden und später idle werden. Noch keine tail-konforme Meldung und kein tatsächlicher Host-Tail-Test. |
| R03 | P1 / unbewiesener Gesamtkontrakt, `PluginProcessor.cpp:604`, `dsp/Morph.h` | Atomare Einzelwerte und Writer-Lock garantieren keinen kohärenten gesamten Parameter-/Corner-/Path-/A/B-Snapshot während parallel laufender Audio-/UI-Änderungen. Getestet wurde Recall nach abgeschlossener Übernahme, nicht jede live überlappende Transaktion. Weiterer Host-/Thread-Stress und ggf. zusammenhängende Snapshot-Übergabe nötig. |
| R04 | P2, `PluginProcessor.cpp:730`, `PluginEditor.cpp:364`, `ui/MorphPage.h:310` | Preset-Speicherfehler verändert vorher bereits den angezeigten Presetnamen; Save-/Morph-Dateidialoge ignorieren Rückgabefehler. WAV-Importfehler wurden separat behoben, allgemeine Save/Load-Dialog-Fehlerkette bleibt offen. Rechtefehler-/Rollback-/Cancel-Integrationstest erforderlich. |
| R05 | P2, `PluginProcessor.cpp:704`, State-/UI-Preset-Routen | Host-Program-Index wird vor erfolgreichem Presetladen gesetzt und beim UI-/State-Presetwechsel nicht zuverlässig aus dem Preset rekonstruiert. PCM-/Parameter-Recall-Test ist kein Program-Index-/Host-Anzeige-Test. |
| R06 | P2, `PluginProcessor.cpp:296` | CC120 All Sound Off und CC123 All Notes Off verwenden denselben Release-Pfad. Sofortiges Stummschalten einschließlich FX-Tails bei CC120 ist nicht bewiesen; MIDI-Compliancetest und spezifizierte Panic-Policy fehlen. |
| R07 | P1 / NOT VERIFIED, `PluginProcessor.cpp:327`, `dsp/Engine.h:146` | Dichte Automation/FX-Ein-/Ausschalten, neue Modultypen und Presetwechsel sind nicht vollständig auf Klicks, maximale Callback-Zeit und atomaren Klangübergang geprüft. Kein Windows-ASIO-p95/p99/max-/Deadline-PASS. Tanh-/Drive-Aliasing und subjektive Qualität wurden nicht als behoben behauptet. |
| R08 | P2 / NOT VERIFIED, Host-/GUI-Grenze | Echter VST3-Bundle-Test, Studio One Save/Close/Reopen, Editor-Maus/Zoom/Fokus/Popup/Cancel, Standalone-Piano-Schließen, mehrere simultan klingende Instanzen, alle Controller-/Sustain-/MPE-Verträge, reales Offline/Realtime, jede einzelne GUI→PCM-Route: NOT RUN. |

Der unabhängige Audit hat damit substanzielle konkrete Fehler behoben, aber **keine vollständige technische Freigabe** erzeugt. Die offenen Punkte werden nicht durch einen Build oder die 18 grünen Tests abgedeckt. Insbesondere begrenzter Heap-Test ≠ harte Echtzeitsicherheit; UI-Attachment-Test ≠ alle Controls akustisch/hostseitig korrekt.

## GitHub Actions / externe Blocker

- Historischer, unabhängig über GitHub gelesener Lauf **[#8 / 40800652521](https://github.com/challanger2000/125A-Noctomorph/actions/runs/40800652521)**: SUCCESS, anderer Branch `ci/gui-render-20261029`, Commit **`fae7d839146457b1b12f2684ccdeb006d2744b64`**. Kein Nachweis für diesen Audit-Kandidaten; Metadaten im Evidence-Verzeichnis.
- Der manuelle Workflow wurde konsolidiert um die Processor-/GUI-Suite und den offiziellen Steinberg-Validator **3.8.1**, SDK **`3cdf9ca5d1f5b1b21e0a86832aa4abe55607bd96`**, ergänzt. Keine zusätzlichen Push-/PR-Trigger.
- Validator lokal kompiliert; dessen eigene **51 Selftests PASS**. **Keine Noctomorph-VST3-Validierung** damit behauptet. Die Windows-Workflow-Erweiterung ist **NOT RUN**.
- Git-Proxy-Lesen und Audit-Branch-Push funktionieren. GitHub-API-Verbindungen werden jedoch bereits beim CONNECT vom Proxy mit **403** abgelehnt. Eine `GH_TOKEN`-Variable ist vorhanden; ihr Wert wurde nicht ausgegeben, ihre API-Nutzbarkeit ist hinter der Sperre unbekannt. Der einmalige Dispatch-Versuch schlug vor dem Start fehl: **keine neue Run-ID, keine behaupteten Windows-Ergebnisse**.
- Umgebungsentwurf: ausschließlich **`network.allowed_domains += api.github.com`** gespeichert, vorherige Custom-Liste war leer; Presets bleiben erhalten. Speicherung bestätigt, aber nicht im laufenden Netz angewendet/publiziert. Die Umgebungsoberfläche verlangt Prüfung/Speichern und Veröffentlichung, bevor diese Änderung aktiv wird. Kein zusätzlicher Token wurde angefordert.

Nach Aktivierung des API-Zugriffs den konsolidierten manuellen Workflow auf dem Audit-Branch genau einmal starten und seine tatsächlichen Ergebnisse prüfen. Die Berechtigung des vorhandenen Tokens und die Verfügbarkeit des manuellen Workflows auf GitHub bleiben bis dahin ungetestet. Kein Umgehen der Netzpolicy, keine Änderung von `main` zur Aktivierung eines Workflows.

## Geänderte Dateien

Produkt: `V4JB.jucer`; `src/PluginProcessor.{h,cpp}`; `src/PluginEditor.cpp`;
`src/WaveAnalyzer.h`; neu `src/WaveState.h`, `src/dsp/AudioReadSlot.h`,
`src/dsp/TransportSafety.h`; `src/dsp/{Engine,Modules,Morph}.h`;
`src/ui/{EditPage,MorphPage,Theme,VersionCompare,WavePage}.h`.

QA/Build: `.github/workflows/windows-vst3.yml`; neu `tests/CMakeLists.txt`,
`tests/README.md`, `tests/processor_check.cpp`, `tests/audio_read_slot_check.cpp`,
`tests/dsp_safety_check.cpp`, `tests/transport_safety_check.cpp`.

Dokumentation: `docs/ARCHITECTURE.md`, `docs/DSP_NOTES.md`, dieser Bericht und
`docs/audit-evidence/{baseline-ctest.log.txt,baseline-extended.log.txt,fixed-ctest.log.txt,fixed-details.log.txt,control-inventory.tsv,historical-actions.json,validator-selftest.log.txt}`.
Keine Parameterdeklarationen, Factory-Ressourcen, Logo-Master, Lizenzen oder Lockfiles verändert.

## Studio-One-Einschätzung

**Noch nicht technisch für einen abschließenden Studio-One-Abnahmetest freigegeben.**
Der Branch ist ein nachvollziehbarer, lokal geprüfter Reparaturkandidat für die
nächste Windows-QA. Zuerst die offenen Programm-/Recall-/Tail-/RT-Verträge
klären und den konsolidierten Windows-/VST3-Lauf verifizieren. Danach gezielter
Studio-One-Praxistest einschließlich der bewusst veränderten Wet/Dry-Klänge.
Keine Aussage „vollständig funktionsfähig“, „release-ready“ oder DAW-PASS.
