# Define the build directory
BUILD_DIR := build

.PHONY: all build clean

# Default target is to build the project
all: build

# Target to configure and build the project
build:
	@echo "--- Configuring project with CMake and Ninja ---"
	@cmake -B $(BUILD_DIR) -G Ninja
	@echo "--- Building project with Ninja ---"
	@ninja -C $(BUILD_DIR)

# Target to clean the build directory
clean:
	@echo "--- Cleaning build directory ---"
	@rm -rf $(BUILD_DIR)