#!/bin/sh
testfiles=("asci.txt" "big.txt" "directory" "empty.txt" "error" "small.txt" "special.txt")
flags=("" "-b" "-n" "-s" "-e" "-E" "-t" "-T" "-v")
echo ---- Test s21_cat ----


for file in "${testfiles[@]}"
do
  echo
  echo --- testfile: "$file"
  echo --- no flag test  ---
  ./s21_cat ../tests/"$file"> cats21"$file"
  cat ../tests/"$file"> cat"$file"
  if cmp cat"$file" cats21"$file" ; then
   echo --   test successful --
  fi

  echo
  echo --- b flag test ---

  ./s21_cat -b ../tests/"$file"> cats21b"$file"
  cat -b ../tests/"$file"> catb"$file"
  if cmp catb"$file" cats21b"$file" ; then
   echo -- test successful --
  fi

  echo
  echo --- n flag test ---

  ./s21_cat -n ../tests/"$file"> cats21n"$file"
  cat -n ../tests/"$file"> catn"$file"
  if cmp catn"$file" cats21n"$file" ; then
   echo -- test successful --
  fi

  echo
  echo --- s flag test ---

  ./s21_cat -s ../tests/"$file"> cats21s"$file"
  cat -s ../tests/"$file"> cats"$file"
  if cmp cats"$file" cats21s"$file" ; then
   echo -- test successful --
  fi

  echo
  echo --- e flag test !GNU! ---

  ./s21_cat -E ../tests/"$file"> cats21E"$file"
  cat -e ../tests/"$file"> catE"$file"
  if cmp catE"$file" cats21E"$file" ; then
   echo -- test successful --
  fi

  echo
  echo --- e flag test  ---

  ./s21_cat -e ../tests/"$file"> cats21_e"$file"
  cat -e ../tests/"$file"> cat_e"$file"
  if cmp cat_e"$file" cats21_e"$file" ; then
   echo -- test successful --
  fi

  echo
  echo --- t flag test !GNU! ---

  ./s21_cat -T ../tests/"$file"> cats21T"$file"
  cat -t ../tests/"$file"> catTT"$file"
  if cmp catTT"$file" cats21T"$file" ; then
   echo -- test successful --
  fi

  echo
  echo --- t flag test ---

  ./s21_cat -t ../tests/"$file"> cats21ti"$file"
  cat -t ../tests/"$file"> catti"$file"
  if cmp catti"$file" cats21ti"$file" ; then
   echo -- test successful --
  fi

  echo
  echo --- v flag test ---
  ./s21_cat -v ../tests/"$file"> cats21v"$file"
  cat -v ../tests/"$file"> catv"$file"
  if cmp catv"$file" cats21v"$file" ; then
   echo -- test successful --
  fi

done

rm *.txt
rm *directory
rm *error