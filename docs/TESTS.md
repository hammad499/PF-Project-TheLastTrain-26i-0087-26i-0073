# Test suite

Every test is a text file of the keys a player would type (start floor, start room, then menu choices), ending with `9`. The expected results below were worked out from the cost table in the project PDF (door/stairs -2/-1, window -3/-2, lift -4/-3, inspect, pickup and use -1/-1, Energy Pack +25, Toolkit +15 Energy, Token +15 Time; invalid actions cost nothing) and then matched against the program output.

Tests that do not end in a win finish with Quit (menu 9), so their summary file says "defeated". For those tests the Energy, Time and Score values and the rejection messages are what matter.

## Run all tests

Build first, then from the project root:

- Windows: `run_tests.bat`
- Linux / macOS / Git Bash: `./run_tests.sh`

Each test runs twice. A test passes when both console outputs match each other, both summaries match each other, and the summary matches `data/expected_<test>.txt`. The last line shows the totals. Run outputs are kept in `out/test_runs/`.

## Tests

| Test | Input file | What it proves | Expected final result |
|---|---|---|---|
| win | `data/test_win.txt` | Full victory run, maximum score | Win, Energy 10, Time 4, Score 520 |
| loss_energy | `data/test_loss_energy.txt` | Mission fails the moment Energy reaches 0 (Observation Token first, then door moves back and forth) | Defeat, Energy 0, Time 14 |
| loss_time | `data/test_loss_time.txt` | Mission fails the moment Time reaches 0 (25 window moves) | Defeat, Energy 25, Time 0 |
| locked_door | `data/test_locked_door.txt` | Locked door rejected without Master Key at no cost, then works with the key | Energy 81, Time 39, Score 10; 1 rejection |
| wall | `data/test_wall.txt` | Wall between F4-R2 and F4-R3 is impassable; rejections cost nothing, window F4-R1/F4-R2 still works | Energy 94, Time 46; 2 rejections |
| lift1 | `data/test_lift1.txt` | Lift 1 between R2/R3 across floors and on the same floor, each -4/-3; F1-R2 to F2-R4 not connected | Energy 84, Time 38; 1 rejection |
| lift2_emergency | `data/test_lift2_emergency.txt` | Lift 2 rejected with no card; Emergency Card gives exactly 3 trips; 4th rejected | Energy 63, Time 26; 2 rejections |
| lift2_normal | `data/test_lift2_normal.txt` | After Fuse installed and Lift Pass collected, Lift 2 works 5 times with no card (more than 3) | Energy 45, Time 13, Score 50 |
| cabinet | `data/test_cabinet.txt` | Wrong code and early key collection rejected free; correct code 427 costs -1/-1 and +30; second unlock rejected | Energy 13, Time 6, Score 320; 3 rejections |
| cabinet_no_clues | `data/test_cabinet_no_clues.txt` | Cabinet stays locked until all clue digits are found | Energy 99, Time 49, Score 10; 3 rejections |
| no_rescue_passenger | `data/test_no_rescue_passenger.txt` | Passenger cannot be rescued or collected without the Medical Kit | Energy 100, Time 50, Score 0; 3 rejections |
| no_rescue_cat | `data/test_no_rescue_cat.txt` | Cat cannot be secured or collected without the Cat Carrier | Energy 100, Time 50, Score 0; 3 rejections |
| caps | `data/test_caps.txt` | Energy Packs and Emergency Toolkit clamp Energy at 100; third pack rejected (only two exist) | Energy 100, Time 41; 1 rejection |
| resume | `data/test_resume_part1.txt` then `part2` | Save mid-game (Fuse installed, Lift Pass held, 2 emergency trips left), reload in a new run, and finish the game | Identical to the uninterrupted win: Win, Energy 10, Time 4, Score 520 |

## Manual check: missing map file

Rename `data/building.txt`, run the game, and confirm it prints `Error: Could not load data/building.txt` and does not crash. Rename the file back afterwards.

## If a test fails

1. Open `out/test_runs/<test>_run1_summary.txt` and compare it with `data/expected_<test>.txt`.
2. If the code changed on purpose (costs, map, rules), recalculate the expected values by hand from the PDF before replacing the expected file. Do not just copy the program output.
3. If only line endings differ (the expected files use Windows line endings), regenerate that expected file on your own machine after checking its values against the table above.
