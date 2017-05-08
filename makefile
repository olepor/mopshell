C_COMPILER=clang
SRC=src/
OBJ=src/obj/
TESTS=test/
SRC_TESTS=$(wildcard $(TESTS).c)

CFLAGS= -I $(wildcard )

.PHONY: test


test:
	$(C_COMPILER) 
