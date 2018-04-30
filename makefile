options = -O2
os = linux
obj_ext = .o
ASMFLAGS = -felf64 -dlinux
src_c = $(wildcard *.c)
src_asm = $(wildcard *.s)
obj = $(src_asm:.s=$(obj_ext))
proj = megaunit
CC = gcc
CFLAGS = $(options) -Wall -std=c99 -pedantic -Wno-format -D$(os)
EXEC = $(proj)$(exe_ext)
TEMPS = $(obj) $(EXEC)

build: clean greetings $(EXEC) ok

.PHONY: clean greetings ok

greetings: 
	@echo Compiling project $(proj)

$(obj): $(src_asm)
	@nasm $(ASMFLAGS) $^

$(EXEC): $(obj) $(src_c)
	@$(CC) -o $@ $^ $(CFLAGS)

ok:
	@echo No errors

clean:
	@rm -f $(TEMPS)
