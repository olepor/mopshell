C_COMPILER=clang
SRC=src/
OBJ=src/obj/
TESTS=test/
SRC_TESTS=$(wildcard $(TESTS)*.h)
OPTIMISATION_LEVEL=0

CFLAGS= -I $(SRC) -O$(OPTIMISATION_LEVEL)

parser: $(SRC)parser.c
	$(C_COMPILER) $(CFLAGS) -c -o $(OBJ)parser $<

.PHONY: test clean debug

debug: compile_debug
	lldb debug

compile_debug: $(SRC)parser.c
	clang -g -o debug src/parser.c test/test_parser.c

test: compile_tests parser
	./testrunner

compile_tests: $(SRC_TESTS)
	$(C_COMPILER) $(wildcard $(OBJ)*) $(CFLAGS) -o testrunner $(wildcard $(TESTS)*.c)

clean:
	rm testrunner
