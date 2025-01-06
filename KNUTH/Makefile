# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -std=c++17

# Source and object files
SOURCES = src/main.cpp src/parse/parse.cpp src/gi/gi.cpp src/ssa2/ssa2.cpp src/regroup/regroup.cpp src/config/config.cpp
OBJECTS = $(SOURCES:src/%.cpp=build/%.o)
TARGET = build/app

# Default target
all: $(TARGET)

# Create build directories if they don't exist
build:
	mkdir -p build/parse build/gi build/ssa2 build/regroup build/config build

# Compile object files from source files
build/%.o: src/%.cpp | build
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link the target executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(TARGET)

# Clean build
clean:
	rm -rf build/*.o build/parse/*.o build/gi/*.o build/ssa2/*.o build/regroup/*.o build/config/*.o

# Phony targets
.PHONY: all clean

