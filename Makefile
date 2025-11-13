# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g -O1
ASAN_FLAGS = -fsanitize=address

# SDL2 flags
SDL_CFLAGS = $(shell sdl2-config --cflags)
SDL_LIBS   = $(shell sdl2-config --libs) -lSDL2_mixer -lSDL2_image

# Source files
SERVER_SRCS = src/server.c src/level.c src/logic.c
CLIENT_SRCS = src/client.c src/level.c src/render.c

# Output binaries
SERVER_BIN = server
CLIENT_BIN = sokoban

# Valgrind command
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes

# Default target
all: $(SERVER_BIN) $(CLIENT_BIN)

# Compile server
$(SERVER_BIN): $(SERVER_SRCS)
	$(CC) $(CFLAGS) $(SERVER_SRCS) -o $(SERVER_BIN)

# Compile client with ASan
$(CLIENT_BIN): $(CLIENT_SRCS)
	$(CC) $(CFLAGS) $(ASAN_FLAGS) $(CLIENT_SRCS) -o $(CLIENT_BIN) $(SDL_CFLAGS) $(SDL_LIBS)

# Run server with valgrind and optional arguments
run-server:
	$(VALGRIND) ./$(SERVER_BIN) $(ARGS)

# Run client with optional arguments
run-client:
	./$(CLIENT_BIN) $(ARGS)

# Clean binaries
clean:
	rm -f $(SERVER_BIN) $(CLIENT_BIN)

.PHONY: all run-server run-client clean
