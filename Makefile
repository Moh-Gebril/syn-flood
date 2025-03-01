# Makefile for SYN Flood Tool
# Author: Mohamed Gebril
# Date: March 2025

# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c11
LDFLAGS =

# Directories
SRC_DIR = .
OBJ_DIR = src
INC_DIR = headers
BIN_DIR = bin

# Source files explicitly listed
SRCS = main.c network_utils.c
OBJS = $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRCS))

# Target executable
TARGET = $(BIN_DIR)/syn_flood

# Phony targets
.PHONY: all clean format install uninstall

# Default target
all: directories $(TARGET)

# Create necessary directories
directories:
	@mkdir -p $(OBJ_DIR) $(BIN_DIR)

# Compile source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

# Link the executable
$(TARGET): $(OBJS)
	@echo "Linking $@..."
	$(CC) $(OBJS) -o $@ $(LDFLAGS)
	@rm -rf $(OBJ_DIR)/*.o
	@echo "Build completed successfully!"

# Install the tool (to /usr/local/bin)
install: $(TARGET)
	@echo "Installing to /usr/local/bin..."
	@sudo cp $(TARGET) /usr/local/bin/
	@echo "Installation completed!"

# Uninstall the tool
uninstall:
	@echo "Uninstalling from /usr/local/bin..."
	@sudo rm -f /usr/local/bin/$(notdir $(TARGET))
	@echo "Uninstallation completed!"

# Format code using clang-format
format:
	@echo "Formatting code..."
	@clang-format -i $(SRCS) $(INC_DIR)/*.h
	@echo "Code formatting completed!"

# Clean build artifacts
clean:
	@echo "Cleaning build artifacts..."
	@rm -rf $(BIN_DIR)
	@echo "Clean completed!"

# Run with root privileges (required for raw sockets)
run: $(TARGET)
	@echo "Running with root privileges..."
	@sudo $(TARGET)