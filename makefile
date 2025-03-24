# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -I.

# Define where to search for source files.
VPATH := .:data

# List of source files with paths (Make will use VPATH to find them)
SRC := main.cpp image.cpp

# Directory for object files
OBJ_DIR := bin

# Object files: we convert source filenames to object filenames in the bin folder.
OBJS := $(addprefix $(OBJ_DIR)/, $(SRC:.cpp=.o))

# Target executable
TARGET := executable

# Default target
all: $(TARGET)

# Link object files to create the executable.
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Pattern rule: compile .cpp file to .o file in the bin directory.
$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean target: remove the executable and the bin directory.
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

.PHONY: all clean
