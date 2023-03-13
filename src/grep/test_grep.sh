#!/bin/bash

SUCCESS=0
FAIL=0
FAIL_LEAK=0
FAIL_FUNC=0
COUNTER=0
DIFF_RES=""

s21_command=(
    "s21_grep"
    )
sys_command=(
    "grep"
    )

declare -a tests=(
"s ../tests/greptest.txt VAR"
"for s21_grep.c s21_grep.h Makefile VAR"
"for s21_grep.c VAR"
"-e for -e ^int s21_grep.c s21_grep.h Makefile VAR"
"-e for -e ^int s21_grep.c VAR"
"-e regex -e ^print s21_grep.c VAR -f ../tests/test_ptrn_grep.txt"
"-e while -e void s21_grep.c Makefile VAR -f ../tests/test_ptrn_grep.txt"
)

declare -a extra=(
"-n for ../tests/test_1_grep.txt ../tests/test_2_grep.txt"
"-n for ../tests/test_1_grep.txt"
"-n -e ^\} ../tests/test_1_grep.txt"
"-c -e /\ ../tests/test_1_grep.txt"
"-ce ^int ../tests/test_1_grep.txt ../tests/test_2_grep.txt"
"-e ^int ../tests/test_1_grep.txt"
"-nivh = ../tests/test_1_grep.txt ../tests/test_2_grep.txt"
"-e"
"-ie INT ../tests/test_5_grep.txt"
"-echar ../tests/test_1_grep.txt ../tests/test_2_grep.txt"
"-ne = -e out ../tests/test_5_grep.txt"
"-iv int ../tests/test_5_grep.txt"
"-in int ../tests/test_5_grep.txt"
"-c -l aboba ../tests/test_1_grep.txt ../tests/test_5_grep.txt"
"-v ../tests/test_1_grep.txt -e ank"
"-noe ) ../tests/test_5_grep.txt"
"-l for ../tests/test_1_grep.txt ../tests/test_2_grep.txt"
"-o -e int ../tests/test_4_grep.txt"
"-e = -e out ../tests/test_5_grep.txt"
"-noe ing -e as -e the -e not -e is ../tests/test_6_grep.txt"
"-e ing -e as -e the -e not -e is ../tests/test_6_grep.txt"
# "-c -e . ../tests/test_1_grep.txt -e '.'"
"-l for ../tests/no_file.txt ../tests/test_2_grep.txt"
"-f ../tests/test_3_grep.txt ../tests/test_5_grep.txt"
)

testing()
{
    param=$(echo $@ | sed "s/VAR/$var/")
    ./"${s21_command[@]}" $param &> ${s21_command[@]}.log 2> tmp.log
    "${sys_command[@]}" $param &> ${sys_command[@]}.log 2> tmp.log
    DIFF_RES="$(diff -s ${s21_command[@]}.log ${sys_command[@]}.log)"
    leaks -quiet --atExit -- ./"${s21_command[@]}" $param &> "${s21_command[@]}"_leak.log 2> tmp.log
    leak=$(grep -o -A100000 "0 leaks for" ${s21_command[@]}_leak.log)
    (( COUNTER++ ))
    if [ "$leak" == "0 leaks for" ] && [ "$DIFF_RES" == "Files ${s21_command[@]}.log and ${sys_command[@]}.log are identical" ]
    then
      (( SUCCESS++ ))
      echo "\033[31m$FAIL\033[0m/\033[32m$SUCCESS\033[0m/$COUNTER \033[32mSuccess\033[0m ./${s21_command[@]} $param"
    else
        if [ "$DIFF_RES" != "Files ${s21_command[@]}.log and ${sys_command[@]}.log are identical" ]
        then
            (( FAIL_FUNC++ ))
            (( FAIL++ ))
            echo "\033[31m$FAIL\033[0m/\033[32m$SUCCESS\033[0m/$COUNTER \033[31mFail FUNC\033[0m ./${s21_command[@]} $param"
        fi
        if [ "$leak" != "0 leaks for" ]
        then
            (( FAIL_LEAK++ ))
            (( FAIL++ ))
            echo "$(grep -A100000 " leaks for" ${s21_command[@]}_leak.log)\033[0m"
            #grep -e "ERROR SUMMARY:" -e "LEAK SUMMARY:" -e "definitely lost:" -e "indirectly lost:" -e "possibly lost:" -e "still reachable:" -e "suppressed:" ${s21_command[@]}.log
            echo "\033[31m$FAIL\033[0m/\033[32m$SUCCESS\033[0m/$COUNTER \033[31mFail LEAK\033[0m ./${s21_command[@]} $param"
        fi
    fi
    rm -f *.log
}

clang-format --style=google -n *.c
clang-format --style=google -n *.h

# специфические тесты
for i in "${extra[@]}"
do
    var="-"
    testing $i
done

# 1 параметр
for var1 in v c l n h o
do
    for i in "${tests[@]}"
    do
        var="-$var1"
        testing $i
    done
done

# 2 параметра
for var1 in v c l n h o
do
    for var2 in v c l n h o
    do
        if [ $var1 != $var2 ]
        then
            for i in "${tests[@]}"
            do
                var="-$var1 -$var2"
                testing $i
            done
        fi
    done
done

# 3 параметра
for var1 in v c l n h o
do
    for var2 in v c l n h o
    do
        for var3 in v c l n h o
        do
            if [ $var1 != $var2 ] && [ $var2 != $var3 ] && [ $var1 != $var3 ]
            then
                for i in "${tests[@]}"
                do
                    var="-$var1 -$var2 -$var3"
                    testing $i
                done
            fi
        done
    done
done

# 2 сдвоенных параметра
for var1 in v c l n h o
do
    for var2 in v c l n h o
    do
        if [ $var1 != $var2 ]
        then
            for i in "${tests[@]}"
            do
                var="-$var1$var2"
                testing $i
            done
        fi
    done
done

# 3 строенных параметра
for var1 in v c l n h o
do
    for var2 in v c l n h o
    do
        for var3 in v c l n h o
        do
            if [ $var1 != $var2 ] && [ $var2 != $var3 ] && [ $var1 != $var3 ]
            then
                for i in "${tests[@]}"
                do
                    var="-$var1$var2$var3"
                    testing $i
                done
            fi
        done
    done
done

echo "\033[31mFAIL: $FAIL\033[0m"
echo "\033[31mFAIL FUNC: $FAIL_FUNC\033[0m"
echo "\033[31mFAIL LEAK: $FAIL_LEAK\033[0m"
echo "\033[32mSUCCESS: $SUCCESS\033[0m"
echo "ALL: $COUNTER"
