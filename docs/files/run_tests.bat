@echo off
rem Run from the project root after building:  run_tests.bat
rem Each test is run twice (determinism) and its summary is compared with data\expected_<test>.txt
if not exist out\test_runs mkdir out\test_runs
set PASS=0
set FAIL=0

for %%T in (test_win test_loss_energy test_loss_time test_locked_door test_wall test_lift1 test_lift2_emergency test_lift2_normal test_cabinet test_cabinet_no_clues test_no_rescue_passenger test_no_rescue_cat test_caps) do call :runtest %%T

rem Save and resume: part 1 saves, part 2 loads and finishes the game
if exist out\savegame.txt del out\savegame.txt
last_train.exe < data\test_resume_part1.txt > out\test_runs\resume_p1_run1.txt
last_train.exe < data\test_resume_part2.txt > out\test_runs\resume_p2_run1.txt
copy /y out\game_summary.txt out\test_runs\resume_run1_summary.txt >nul
if exist out\savegame.txt del out\savegame.txt
last_train.exe < data\test_resume_part1.txt > out\test_runs\resume_p1_run2.txt
last_train.exe < data\test_resume_part2.txt > out\test_runs\resume_p2_run2.txt
copy /y out\game_summary.txt out\test_runs\resume_run2_summary.txt >nul
fc out\test_runs\resume_p2_run1.txt out\test_runs\resume_p2_run2.txt >nul && fc out\test_runs\resume_run1_summary.txt out\test_runs\resume_run2_summary.txt >nul && fc out\test_runs\resume_run1_summary.txt data\expected_test_resume.txt >nul && (echo PASS  test_resume & set /a PASS+=1) || (echo FAIL  test_resume & set /a FAIL+=1)

echo Passed: %PASS%   Failed: %FAIL%
goto :eof

:runtest
last_train.exe < data\%1.txt > out\test_runs\%1_run1.txt
copy /y out\game_summary.txt out\test_runs\%1_run1_summary.txt >nul
last_train.exe < data\%1.txt > out\test_runs\%1_run2.txt
copy /y out\game_summary.txt out\test_runs\%1_run2_summary.txt >nul
fc out\test_runs\%1_run1.txt out\test_runs\%1_run2.txt >nul && fc out\test_runs\%1_run1_summary.txt out\test_runs\%1_run2_summary.txt >nul && fc out\test_runs\%1_run1_summary.txt data\expected_%1.txt >nul && (echo PASS  %1 & set /a PASS+=1) || (echo FAIL  %1 & set /a FAIL+=1)
goto :eof
