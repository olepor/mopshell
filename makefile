C_COMPILER=clang
SRC=src/
OBJ=src/obj/
TESTS=test/
SRC_TESTS=$(wildcard $(TESTS)*.h)
OPTIMISATION_LEVEL=0

CFLAGS= -I $(SRC) -O$(OPTIMISATION_LEVEL)

parser: $(SRC)parser.c
	$(C_COMPILER) $(CFLAGS) -c -o $(OBJ)parser $<

mopshell: $(SRC)mopshell.c $(SRC)parser.c
	clang -I $(SRC) $(OBJ)parser -o $@ $(SRC)mopshell.c

.PHONY: test clean debug

debug: compile_debug
	lldb debug

compile_shell_debug:
	clang -g -o debug $(OBJ)parser src/mopshell.c

compile_debug: $(SRC)parser.c
	clang -g -o debug src/parser.c test/test_parser.c

test: compile_tests parser
	./testrunner

compile_tests: $(SRC_TESTS)
	$(C_COMPILER) $(wildcard $(OBJ)*) $(CFLAGS) -o testrunner $(wildcard $(TESTS)*.c)

clean:
	rm testrunner
	rm debug
