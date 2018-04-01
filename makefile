# Makefile
# 31/03/18 - 09:42
# By ..

CC = gcc
CFLAGS = -g -Wall -std=c99 -pedantic -Wno-format
TEST = -DTEST
EXEC = megaunit.exe

build: clean greetings megaunit.exe ok

build_test: clean greetings add_test megaunit.exe ok

test:
	@py megatest.py

.PHONY: clean add_test greetings

greetings: 
	@echo Compiling project

clean:
	@del $(EXEC) 2>nul

add_test:
	$(eval CFLAGS += $(TEST))
	@echo Test Enabled

megaunit.obj: megaunit.s
	@nasm -fwin64 $^

$(EXEC): megaunit.obj megaunit.c
	@$(CC) -o $@ $^ $(CFLAGS)

ok:
	@echo No errors