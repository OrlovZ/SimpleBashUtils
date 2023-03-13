CC=gcc
CFLAGS=-c -std=c11 -Wall -Werror -Wextra

.PHONY: all clean rebuild tests

all: s21_cat style

s21_cat: common.o s21_cat.o
	$(CC) s21_cat.o common.o -o s21_cat

s21_cat.o:
	$(CC) $(CFLAGS) s21_cat.c

common.o:
	$(CC) -c ../common/common.c -DCAT_USAGE

tests: s21_cat
	./test_cat.sh

style:
	clang-format --style=google -i *.c *.h
	clang-format --style=google -i ../common/*.c ../common/*.h

rebuild:
	rm -rf *.o
	make

clean:
	rm -rf *.o
	rm -rf *.log
	rm -rf s21_cat
