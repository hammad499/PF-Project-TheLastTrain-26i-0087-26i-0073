#!/bin/bash
# Run from the project root after building:  ./run_tests.sh
# Each test is run twice (determinism) and its summary is compared with data/expected_<test>.txt
mkdir -p out/test_runs
pass=0; fail=0
same() { diff -q <(tr -d "\r" < "$1") <(tr -d "\r" < "$2") > /dev/null; }

for t in test_win test_loss_energy test_loss_time test_locked_door test_wall test_lift1 \
         test_lift2_emergency test_lift2_normal test_cabinet test_cabinet_no_clues \
         test_no_rescue_passenger test_no_rescue_cat test_caps; do
  ./last_train < data/$t.txt > out/test_runs/${t}_run1.txt
  cp out/game_summary.txt out/test_runs/${t}_run1_summary.txt
  ./last_train < data/$t.txt > out/test_runs/${t}_run2.txt
  cp out/game_summary.txt out/test_runs/${t}_run2_summary.txt
  if same out/test_runs/${t}_run1.txt out/test_runs/${t}_run2.txt \
     && same out/test_runs/${t}_run1_summary.txt out/test_runs/${t}_run2_summary.txt \
     && same out/test_runs/${t}_run1_summary.txt data/expected_$t.txt; then
    echo "PASS  $t"; pass=$((pass+1))
  else
    echo "FAIL  $t"; fail=$((fail+1))
  fi
done

# Save and resume: part 1 saves, part 2 loads and finishes the game
rm -f out/savegame.txt
./last_train < data/test_resume_part1.txt > out/test_runs/resume_p1_run1.txt
./last_train < data/test_resume_part2.txt > out/test_runs/resume_p2_run1.txt
cp out/game_summary.txt out/test_runs/resume_run1_summary.txt
rm -f out/savegame.txt
./last_train < data/test_resume_part1.txt > out/test_runs/resume_p1_run2.txt
./last_train < data/test_resume_part2.txt > out/test_runs/resume_p2_run2.txt
cp out/game_summary.txt out/test_runs/resume_run2_summary.txt
if same out/test_runs/resume_p2_run1.txt out/test_runs/resume_p2_run2.txt \
   && same out/test_runs/resume_run1_summary.txt out/test_runs/resume_run2_summary.txt \
   && same out/test_runs/resume_run1_summary.txt data/expected_test_resume.txt; then
  echo "PASS  test_resume (part1 + part2)"; pass=$((pass+1))
else
  echo "FAIL  test_resume (part1 + part2)"; fail=$((fail+1))
fi
echo "Passed: $pass   Failed: $fail"
