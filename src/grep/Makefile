CC = gcc -Wall -Werror -Wextra

all: clean build

s21_grep:
	$(CC) s21_grep.c -o s21_grep

build: s21_grep

clean:
	rm -rf s21_grep

test:
	sh test_func_grep.sh

leaks:
	sh test_leak_grep.sh

check:
	cp ../../materials/linters/.clang-format ../grep
	clang-format -n s21_grep.c
	clang-format -n s21_grep.h
