
CC=gcc
CFLAGS=-Wall -g $^ -o $@
CC_DEP=$(CC) $^


build: ./obj/graphics.o ./obj/main.o
	$(CC) $^ -o main -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

./obj/main.o: ./src/main.c
	$(CC) $(CFLAGS)
./obj/graphics.o: ./src/graphics.h ./src/graphics.c ./src/conf_graphics.h
	$(CC) $(CFLAGS)
