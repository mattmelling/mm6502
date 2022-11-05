CC=gcc
CFLAGS=-g -Werror -pedantic -I include -O1
LD=ld

OBJ=main.o mm6502.o memory.o repl.o dism.o via.o acia.o
HEADERS=*.h

default: mm6502

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

mm6502: lib/fake6502.o $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: clean
clean:
	rm -f *.o mm6502

