CC = gcc
CFLAGS = -Wall -I /opt/homebrew/include
OPTIMISATION_LVL = -O3
LIBS_DIR = -L /opt/homebrew/lib
LIBS = -lpng -lSDL2 -lm

sketcher: build_dir main image_operations utils
	$(CC) $(CFLAGS) $(LIBS_DIR) $(OPTIMISATION_LVL) -o build/sketcher build/main.o build/image_operations.o build/utils.o $(LIBS)

build_dir:
	mkdir build

main: src/main.c src/config.h
	$(CC) $(CFLAGS) -o build/main.o -c src/main.c

image_operations: src/image_operations.c src/image_operations.h
	$(CC) $(CFLAGS) -o build/image_operations.o -c src/image_operations.c

utils: src/utils.c src/utils.h
	$(CC) $(CFLAGS) -o build/utils.o -c src/utils.c

clean:
	rm -rf build
