CC = gcc
CFLAGS = -c -Wall -Werror -Wextra -std=c11
FLAGS_FOR_TEST = -lcheck -lm -lpthread

SRCS := $(wildcard s21_*.c) $(wildcard ./additional/*.c) # еще один wildcard

OBJS_PATHES = $(SRCS:.c=.o)
OBJS := $(subst ./additional/,,$(OBJS_PATHES)) # еще одну строчку с именем папки

OS := $(shell uname)
ifeq ($(OS),Linux) # определяем ОС
TEST_LIBS = -lcheck -lm -lsubunit
else
TEST_LIBS = -lcheck
endif

all : clean s21_decimal.a test gcov_report

s21_decimal.a :
	$(CC) $(CFLAGS) $(SRCS)
	ar rc s21_decimal.a $(OBJS)
	ranlib s21_decimal.a

%.o : %.c
	$(CC) $(CFLAGS) $< -o $@

test: s21_decimal.a
	$(CC) $(FLAGS) tests/s21_decimal_test.c s21_decimal.a -o s21_tests $(TEST_LIBS)
	./s21_tests

gcov_report: s21_decimal.a tests/s21_decimal_test.c
	$(CC) $(FLAGS) --coverage s21_decimal.a tests/s21_decimal_test.c -o gcov_report $(TEST_LIBS)
	./gcov_report
	lcov -t "./gcov_report" -o gcov_report.info --no-external -c -d .
	genhtml -o report gcov_report.info
	open ./report/index.html

check_linters:
	cp ../materials/linters/CPPLINT.cfg ./
	python3 ../materials/linters/cpplint.py --extensions=c *.c *.h additional/*.c additional/*.h
	cppcheck *.c *.h additional/*.c additional/*.h
	rm CPPLINT.cfg

cppcheck:
	cppcheck --enable=all  --check-config --suppress=missingIncludeSystem *.c *.h additional/*.c additional/*.h

clean :
	rm -rf *.o *.a gcov *.gch *.gcda *.gcno *.info *.out *.cfg test report .clang-format gcov_report
	rm -rf s21_tests

rebuild : clean all

style: clean
	cp ../materials/linters/.clang-format ../src/.clang-format
	clang-format -style=Google -i -n *.c *.h additional/*.c additional/*.h
	rm .clang-format

clang :
	clang-format -i --style=Google *.c *.h additional/*.c additional/*.h
	clang-format -n --style=Google *.c *.h additional/*.c additional/*.h