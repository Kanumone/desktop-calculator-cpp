#>>>>>>>>>>>     Makefile     <<<<<<<<<<<#
.PHONY: all clean install uninstall dvi tests leaks linter

#>>>>>>>>>>>     Exec file     <<<<<<<<<<<#
EXEC=C7_SmartCalc_v1.0-0

#>>>>>>>>>>>     Flags     <<<<<<<<<<<#
CC=gcc
CFLAGS=-Wall -Werror -Wextra -std=c11
CPPCHECKFLAG := --enable=all -std=c11
GFLAGS=-lcheck

OS=$(shell uname)
ifeq ($(OC), Linux)
	GFLAGS+= -lm -lpthread -lrt -lsubunit -D_GNU_SOURCE
	LEAKS=valgrind --leak-check=full ./$(EXEC)
else
	LEAKS=CK_FORK=no leaks -atExit -- ./$(EXEC)
endif

TESTFLAGS=-g -fprofile-arcs -ftest-coverage $(GFLAGS)

#>>>>>>>>>>>     Paths     <<<<<<<<<<<#
P_SRC := ./source
P_INC := ./include
P_OBJ := ./bin
P_LINT := ../materials/linters
P_TEST := ./tests

#>>>>>>>>>>>     Source files     <<<<<<<<<<<#
SRC=$(wildcard $(P_SRC)/*.c)
OBJ=$(SRC:.c=.o)
kk:
	echo $(OBJ)

#>>>>>>>>>>>     Style check     <<<<<<<<<<<#
linter:
	cp $(P_LINT)/CPPLINT.cfg .
	python3 $(P_LINT)/cpplint.py $(P_INC)/*.h $(P_SRC)/*.c $(P_SRC)/*.cpp
	rm -rf CPPLINT.cfg
	cppcheck $(CPPCHECKFLAG) $(P_INC)/*.h $(P_SRC)/*.c $(P_SRC)/*.cpp

#>>>>>>>>>>>     Leaks check     <<<<<<<<<<<#
leaks: $(EXEC)
	$(LEAKS)

#>>>>>>>>>>>     Creating C object files     <<<<<<<<<<<#
%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $< -I ./include

tests: $(OBJ)
	checkmk clean_mode=1 $(P_TEST)/calc_test.check >> $(P_TEST)/calc_test.c
	gcc $(CFLAGS) $(P_TEST)/calc_test.c $(OBJ) -L $(P_TEST) -o $(P_TEST)/calc_test -I $(P_INC) $(TESTFLAGS)
	$(P_TEST)/calc_test

gcov_report: tests
	cd $(P_TEST);
	gcovr -r .. --html-details -o gcov_report.html
	open gcov_report.html

clean: cleancov
	rm -f $(P_SRC)/*.o

cleancov:
	rm -f *.css *.html *.gcda *.gcno *.gcov
	rm -f $(P_TEST)/calc_test.c $(P_TEST)/calc_test

#>>>>>>>>>>>     Building     <<<<<<<<<<<#
Qmakefile:
	qmake Calculator.pro -o Qmakefile

install: Qmakefile
	make -f Qmakefile install INSTALL_ROOT=build

uninstall: Qmakefile
	make -f Qmakefile uninstall INSTALL_ROOT=build
	rm -rf build .qm
	rm Qmakefile .qmake.stash

run:
ifeq ($(OS), Darwin)
	./build/release/$(EXEC).app/Contents/MacOS/$(EXEC)
endif
ifeq ($(OS), Linux)
	./build/$(EXEC)
endif
