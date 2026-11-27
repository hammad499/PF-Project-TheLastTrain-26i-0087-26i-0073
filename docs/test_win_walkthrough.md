# Test: full victory run (test_win)

**Input file:** `data/test_win.txt`  |  **Expected result:** `data/expected_win_summary.txt`

Starts in F4-R2 and finishes with the maximum score of **520**, Energy **10** and Time **4**.

Route in words: Medical Kit and Emergency Card (F4-R2) -> rescue passenger (F4-R1) -> Cat Carrier (F5-R3) and Observation Token (F5-R4) -> Lift 2 emergency trip down to F2 -> Master Key (F1-R2) -> Fuse and clue 4 (F3-R2) -> Lift Pass and install Fuse (F3-R4) -> secure Cat (F3-R3) -> back up via Lift 2 -> clue 2 (F4-R3) -> clue 7, cabinet and Train Key (F5-R1) -> board train (F5-R2).

Each row shows the state **after** that command. Check several rows yourself against the cost table in the project PDF (door -2/-1, window -3/-2, lift -4/-3, pickup -1/-1, inspect -1/-1, use -1/-1).

| # | Command typed | Action | Energy | Time | Score |
|---|---|---|---|---|---|
| – | (start F4-R2) | Initial state | 100 | 50 | 0 |
| 1 | `3 %` | Collect % | 99 | 49 | 0 |
| 2 | `3 $` | Collect $ | 98 | 48 | 0 |
| 3 | `1 4 1` | Move to F4-R1 | 95 | 46 | 0 |
| 4 | `4 %` | Use % | 94 | 45 | 100 |
| 5 | `1 5 1` | Move to F5-R1 | 92 | 44 | 100 |
| 6 | `1 5 2` | Move to F5-R2 | 90 | 43 | 100 |
| 7 | `1 5 3` | Move to F5-R3 | 88 | 42 | 100 |
| 8 | `3 @` | Collect @ | 87 | 41 | 100 |
| 9 | `1 5 4` | Move to F5-R4 | 84 | 39 | 100 |
| 10 | `3 &` | Collect & | 83 | 53 | 100 |
| 11 | `1 2 4` | Move to F2-R4 | 79 | 50 | 100 |
| 12 | `1 2 3` | Move to F2-R3 | 77 | 49 | 100 |
| 13 | `1 1 2` | Move to F1-R2 | 73 | 46 | 100 |
| 14 | `3 <` | Collect < | 72 | 45 | 110 |
| 15 | `1 1 1` | Move to F1-R1 | 69 | 43 | 110 |
| 16 | `1 2 1` | Move to F2-R1 | 67 | 42 | 110 |
| 17 | `1 3 1` | Move to F3-R1 | 65 | 41 | 110 |
| 18 | `1 3 2` | Move to F3-R2 | 63 | 40 | 110 |
| 19 | `2` | Inspect | 62 | 39 | 120 |
| 20 | `3 *` | Collect * | 61 | 38 | 120 |
| 21 | `1 3 3` | Move to F3-R3 | 58 | 36 | 120 |
| 22 | `1 3 4` | Move to F3-R4 | 56 | 35 | 120 |
| 23 | `3 (` | Collect ( (Lift Pass) | 55 | 34 | 140 |
| 24 | `4 *` | Use * | 54 | 33 | 160 |
| 25 | `1 3 3` | Move to F3-R3 | 52 | 32 | 160 |
| 26 | `4 @` | Use @ | 51 | 31 | 260 |
| 27 | `1 3 2` | Move to F3-R2 | 48 | 29 | 260 |
| 28 | `1 3 1` | Move to F3-R1 | 46 | 28 | 260 |
| 29 | `1 2 1` | Move to F2-R1 | 44 | 27 | 260 |
| 30 | `1 2 2` | Move to F2-R2 | 42 | 26 | 260 |
| 31 | `1 2 3` | Move to F2-R3 | 38 | 23 | 260 |
| 32 | `1 2 4` | Move to F2-R4 | 36 | 22 | 260 |
| 33 | `1 4 4` | Move to F4-R4 | 32 | 19 | 260 |
| 34 | `1 4 3` | Move to F4-R3 | 30 | 18 | 260 |
| 35 | `2` | Inspect | 29 | 17 | 270 |
| 36 | `1 4 4` | Move to F4-R4 | 27 | 16 | 270 |
| 37 | `1 5 4` | Move to F5-R4 | 23 | 13 | 270 |
| 38 | `1 5 3` | Move to F5-R3 | 20 | 11 | 270 |
| 39 | `1 5 2` | Move to F5-R2 | 18 | 10 | 270 |
| 40 | `1 5 1` | Move to F5-R1 | 16 | 9 | 270 |
| 41 | `2` | Inspect | 15 | 8 | 280 |
| 42 | `5 427` | Unlock cabinet 427 | 14 | 7 | 310 |
| 43 | `3 ^` | Collect ^ | 13 | 6 | 320 |
| 44 | `1 5 2` | Move to F5-R2 | 11 | 5 | 320 |
| 45 | `4 ^` | Use ^ | 10 | 4 | 520 |

Notes:
- Row 10 (`3 &`, Observation Token): the pickup costs -1 Energy and -1 Time, and the token adds +15 Time, so Time rises by 14 overall.
- The final `9` in the input file is never read, because the game ends on victory in row 45.
