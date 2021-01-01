CC=gcc
CFLAGS=-Wall
LIBS=-lpng -lSDL2

image_test: image_test.c
	$(CC) $(CFLAGS) -o image_test image_test.c $(LIBS)

clean:
	rm -f image_test
