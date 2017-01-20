CC=gcc
CFLAGS= -g -std=c99
DEBUG= -DDEBUG=0
INCLUDE= -I./include/ -I./include/vm_context_util/
SRC=src/*.c src/loader/*.c src/vm_context_util/*.c src/refs_list/*.c
BUILD= -o build/main

all: 
	$(CC) $(DEBUG) $(INCLUDE) $(CFLAGS) $(SRC) $(BUILD)
