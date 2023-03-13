CC=gcc
CFLAGS=-c -std=c11 -Wall -Werror -Wextra

.PHONY: all clean rebuild tests

all: s21_grep style

s21_grep: s21_grep.o common.o
	$(CC) s21_grep.o common.o -o s21_grep

s21_grep.o:
	$(CC) $(CFLAGS) s21_grep.c

common.o:
	$(CC) $(CFLAGS) ../common/common.c -DGREP_USAGE

tests: s21_grep
	sh test_grep.sh

style:
	clang-format --style=google -i *.c *.h
	clang-format --style=google -i ../common/*.c ../common/*.h

rebuild:
	rm -rf *.o
	make

clean:
	rm -rf *.o
	rm -rf *.log	
	rm -rf s21_grep
