# Compiler and flags
CC = gcc
CFLAGS = -Wall -g -fsanitize=address -fno-omit-frame-pointer `sdl2-config --cflags`
LDFLAGS = `sdl2-config --libs` -lSDL2_image -lSDL2_mixer -lSDL2_ttf -fsanitize=address

# Source and build directories
SRC_DIR = src
BUILD_DIR = build
OBJS = $(BUILD_DIR)/main.o $(BUILD_DIR)/logic.o $(BUILD_DIR)/level.o $(BUILD_DIR)/render.o

# Target
TARGET = mario

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Compile source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Link object files
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Normal build
all: $(TARGET)

# Clean
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

# Run normally
run: $(TARGET)
	./$(TARGET)

# Debug build with AddressSanitizer
debug: $(TARGET)
	@echo "Running with AddressSanitizer..."
	./$(TARGET)
