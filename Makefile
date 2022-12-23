CC=gcc
CFLAGS=-g -Werror -pedantic -I include -O1 $(shell pkg-config --libs --cflags --static fake65c02)
LD=ld

OBJ=main.o mm6502.o memory.o repl.o dism.o via.o acia.o
HEADERS=*.h

default: mm6502

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

mm6502: $(OBJ)
	$(CC) $(CFLAGS) -l:fake6502.o $^ -o $@

.PHONY: clean
clean:
	rm -f *.o mm6502
