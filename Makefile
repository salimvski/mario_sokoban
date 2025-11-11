CC = gcc
CFLAGS = -Wall -g `sdl2-config --cflags`
LIBS = `sdl2-config --libs` -lSDL2_image -lSDL2_mixer

SRC_DIR = src
BUILD_DIR = build
OBJS = $(BUILD_DIR)/main.o $(BUILD_DIR)/logic.o $(BUILD_DIR)/level.o $(BUILD_DIR)/render.o
TARGET = sokoban

all: $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LIBS)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

run: $(TARGET)
	./$(TARGET) levels/level1.txt
