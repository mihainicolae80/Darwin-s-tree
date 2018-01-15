
CC=gcc
CFLAGS=-Wall -g  -c -o $@
CC_DEP=$(CC) $^
SDL_FLAGS=-lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_gfx


run: main
	./main

build: main

clean:
	rm ./obj/*

main: ./obj/graphics.o ./obj/main.o ./obj/tree.o ./obj/tree_gfx.o ./obj/evolution.o ./obj/misc.o
	$(CC) -o main $^ $(SDL_FLAGS) -lm


./obj/main.o: ./src/main.c ./src/graphics.h ./src/conf_graphics.h \
	./src/tree.h ./src/tree_gfx.h
	$(CC) $(CFLAGS) ./src/main.c
./obj/graphics.o: ./src/graphics.h ./src/graphics.c ./src/conf_graphics.h
	$(CC) $(CFLAGS) ./src/graphics.c
./obj/tree.o: ./src/tree.h ./src/tree.c
	$(CC) $(CFLAGS) ./src/tree.c
./obj/tree_gfx.o: ./src/tree_gfx.h ./src/tree_gfx.c ./src/conf_tree_gfx.h
	$(CC) $(CFLAGS) ./src/tree_gfx.c
./obj/evolution.o: ./src/evolution.h ./src/evolution.c ./src/conf_evolution.h
	$(CC) $(CFLAGS) ./src/evolution.c
./obj/misc.o: ./src/misc.h ./src/misc.c
	$(CC) $(CFLAGS) ./src/misc.c
