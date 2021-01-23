CC=gcc
CFLAGS=-Wall
LIBS=-lpng -lSDL2 -lm

sketcher: sketcher.c
	$(CC) $(CFLAGS) -O3 -o sketcher sketcher.c $(LIBS)

clean:
	rm -f sketcher
