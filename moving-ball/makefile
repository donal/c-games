
OBJECTS=
CFLAGS = -g -Wall -O0
LDLIBS=-lncurses
LDFLAGS=
CC=gcc

$(P): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(P).c $(LDLIBS) -o bin/$(P)

# default for compiling a .o object file from a .c source file:
# $(CC) $(CFLAGS) $(LDFLAGS) -o $@ $*.c
#
# and default for linking:
# $(CC) $(LDFLAGS) .o $(LDLIBS)

.PHONY: dump
dump: ; $(info $$@ is [${@}], $$< is [${<}], $$^ is [${^}] )echo Hello world

