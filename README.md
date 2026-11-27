# The Last Train

**CS-1002: Programming Fundamentals (AI & DS) – Semester Project**
Authors: `Hammad` (`26i-0087`) [, `Abdullah Ajaz` (`26i-0073`)]
Release tag: `v1.0.26i-0087`

A deterministic, text-based rescue game written in C++17 (procedural style, no classes). The player explores a 5-floor, 20-room building, rescues an injured passenger and a cat, finds three clue digits to open a cabinet containing the Train Key, restores Lift 2, and boards the train at F5-R2 before Energy or Time reaches 0.

---

## Project structure

```
include/   Header files: config, state, level, simulator, physics, switches, logger, util, viewer_iface
src/core/  Implementation: main_core, level, simulator, physics, switches, logger, util, viewer_iface_core
data/      building.txt (static map), initial-state.txt (start values), test input files
out/       savegame.txt and game_summary.txt (written at runtime; a completed sample run is committed)
docs/      Project_Diagram.jpg, Flowchart.png, ConsoleUI.png
```

| File | Role |
|---|---|
| `data/building.txt` | ASCII map and entity placement. Read on launch, never overwritten. |
| `data/initial-state.txt` | Starting values (Energy 100, Time 50, Score 0, empty inventory, all flags false). |
| `out/savegame.txt` | Snapshot of player position, resources, inventory, emergency trips, flags and room contents. |
| `out/game_summary.txt` | Final report written when the game ends (victory or defeat). |

All paths are relative, so run the program from the project root.

---

## Build

Requires `g++` with C++17 support.

```bash
g++ -std=c++17 -Wall -Wextra -Iinclude src/core/*.cpp -o last_train
```

The build produces no warnings. A `CMakeLists.txt` is also provided (`cmake -S . -B build && cmake --build build`).

---

## Run

```bash
./last_train          # Linux / macOS / Git Bash
last_train.exe        # Windows
```

1. Enter your starting **floor (1–5)** and **room (1–4)**.
2. Each turn shows the map, a status dashboard (Energy, Time, Score, inventory, emergency trips), and the contents of the current room. Then choose from the menu:

| Menu | Action | Input after choosing |
|---|---|---|
| 1 | Move | target floor and room, e.g. `3 2` |
| 2 | Inspect room (reveals clue digits) | – |
| 3 | Collect an item | its symbol, e.g. `<` |
| 4 | Use an item | its symbol, e.g. `*` |
| 5 | Unlock cabinet (F5-R1) | the 3-digit code |
| 6 | Show objectives checklist | – |
| 7 | Save to `out/savegame.txt` | – |
| 8 | Load from `out/savegame.txt` | – |
| 9 | Quit (counts as defeat) | – |

The game ends on victory, when Energy or Time reaches 0, or when you quit. A summary is then written to `out/game_summary.txt`.

---

## Game rules implemented

- **Resources** are clamped to 0–100. Start: Energy 100, Time 50. Rejected or invalid actions cost nothing.
- **Costs (Energy / Time):** door or stairs −2/−1, window −3/−2, lift −4/−3, inspect −1/−1, pickup −1/−1, item use or puzzle −1/−1.
- **Instant pickups:** Energy Pack +25 Energy (two available), Emergency Toolkit +15 Energy, Observation Token +15 Time. The pickup cost applies as well, so the net effect is, for example, +24 Energy and −1 Time for an Energy Pack.
- **Locked door** F3-R1 ↔ F3-R2 needs the Master Key. **Wall** between F4-R2 and F4-R3 is impassable.
- **Lift 1** connects the R2/R3 shaft of Floor 1 and Floor 2. It can be entered from either side, so crossing between R2 and R3 on the same floor also works as a lift journey.
- **Lift 2** connects F2-R4, F4-R4 and F5-R4. It needs the Fuse installed at the Control Panel (F3-R4) **and** the Lift Pass. Before that, the Emergency Access Card gives 3 emergency trips.
- **Cabinet code** is `427` (clue digits 4 in F3-R2, 2 in F4-R3, 7 in F5-R1). All three must be found first, and entering the correct code costs 1 Energy and 1 Time.
- **Scoring:** maximum 520 points (key 10, clues 30, cabinet 30, Train Key 10, passenger 100, cat 100, fuse 20, Lift Pass 20, train prepared 100, victory bonus 100).

---

## Verifying deterministic output

The game has no randomness. The same start room and the same command sequence always produce the same resource values, states and summary.

Test inputs are plain text files of the numbers you would type, one entry per line, **ending with `9` (Quit)** so the program exits. Place them in `data/`.

```bash
# Run 1
./last_train < data/<test_file>.txt > out/run1_console.txt
cp out/game_summary.txt out/run1_summary.txt

# Run 2 (identical input)
./last_train < data/<test_file>.txt > out/run2_console.txt
cp out/game_summary.txt out/run2_summary.txt

# Compare – no output means identical
diff out/run1_console.txt out/run2_console.txt
diff out/run1_summary.txt out/run2_summary.txt
```

On Windows (Command Prompt) use `fc` instead of `diff`, and `type` / `copy` in place of `cat` / `cp`.

### Included tests

| File | What it checks |
|---|---|
| `data/test_win.txt` | Full victory route from F4-R2 (rescues passenger and cat, installs fuse, unlocks cabinet `427`, boards train; Score: 520, Energy: 10, Time: 4; see `docs/test_win_walkthrough.md` and `data/expected_win_summary.txt`) |

---

## Notes and assumptions

- Menu and coordinate prompts expect numbers. Scripted runs should always end with menu option `9`.
- Quitting early is recorded as a defeat in the summary.
- `MAX_ENERGY` and `MAX_TIME` are both 100, as defined in `include/config.h`.
- Procedural design: state is held in primitive variables and fixed-size arrays (no classes); functions are small and take state by reference.
