# Compiler settings - Can be customized.
CC = g++
CXXFLAGS = -Wall -Wextra -g

# Project directories
# Updated to match standard structure: .cpp in src, .h in include
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
BIN_DIR = bin

# Target executable name
TARGET = $(BIN_DIR)/app

# Find all .cpp files in the SRC_DIR
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)

# Create object file names based on source file names
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))

# The default rule to build the app
all: $(TARGET)

# Link the object files to create the executable
$(TARGET): $(OBJECTS) | $(BIN_DIR)
	$(CC) $(CXXFLAGS) -o $@ $^

# Compile source files into object files
# -I$(INC_DIR) tells the compiler to look for headers in the 'include' folder
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CC) $(CXXFLAGS) -I$(INC_DIR) -c $< -o $@

# Create the bin directory if it doesn't exist
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Create the obj directory if it doesn't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Clean up build files
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Phony targets avoid conflicts with files of the same name
.PHONY: all clean
