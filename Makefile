# Compiler
CC = gcc
CFLAGS = -Wall -Wextra -O2 -g

# SDL flags (for client)
SDL_CFLAGS  = $(shell sdl2-config --cflags)
SDL_LIBS    = $(shell sdl2-config --libs) -lSDL2_image -lSDL2_mixer

# Sources
SERVER_SRC = src/server.c src/level.c src/logic.c
CLIENT_SRC = src/client.c src/level.c src/logic.c src/render.c

# Targets
SERVER_BIN = server
CLIENT_BIN = client

# Default target
all: $(SERVER_BIN) $(CLIENT_BIN)

# Build server (no SDL)
$(SERVER_BIN): $(SERVER_SRC)
	$(CC) $(CFLAGS) $^ -o $@

# Build client (with SDL)
$(CLIENT_BIN): $(CLIENT_SRC)
	$(CC) $(CFLAGS) $(SDL_CFLAGS) $^ -o $@ $(SDL_LIBS)

# Clean build
clean:
	rm -f $(SERVER_BIN) $(CLIENT_BIN) *.o

.PHONY: all clean
