CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude

SRC = src/main.c src/level.c src/logic.c src/render.c
OBJ = $(SRC:.c=.o)

sokoban: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(OBJ) sokoban
