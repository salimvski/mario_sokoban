CC = gcc
CFLAGS = -Wall -Wextra -g -O1 -fsanitize=address `sdl2-config --cflags`
LDFLAGS = `sdl2-config --libs` -lSDL2_image -lSDL2_mixer -lSDL2_ttf -fsanitize=address

SRC_CLIENT = src/client.c src/level.c src/render.c
SRC_SERVER = src/server.c src/level.c src/logic.c

TARGET_CLIENT = sokoban
TARGET_SERVER = server

all: $(TARGET_CLIENT) $(TARGET_SERVER)

$(TARGET_CLIENT): $(SRC_CLIENT)
	$(CC) $(CFLAGS) $(SRC_CLIENT) -o $(TARGET_CLIENT) $(LDFLAGS)

$(TARGET_SERVER): $(SRC_SERVER)
	$(CC) -Wall -Wextra -g -fsanitize=address $(SRC_SERVER) -o $(TARGET_SERVER)

clean:
	rm -f $(TARGET_CLIENT) $(TARGET_SERVER)
