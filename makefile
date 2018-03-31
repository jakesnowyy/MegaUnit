# Makefile
# 31/03/18 - 09:42
# By ..

CC = gcc
CFLAGS = -g -Wall -std=c99

build: clean greetings megaunit.exe

test: clean greetings add_test megaunit.exe

megaunit.exe: megaunit.c
	$(CC) -o $@ $? $(CFLAGS)

.PHONY: clean add_test greetings

greetings: 
	@echo "Compiling project"

clean: megaunit.exe
	@rm $^

add_test: CFLAGS += -DTEST