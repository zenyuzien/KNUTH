CXX = g++
CXXFLAGS = -Wall -std=c++17

SOURCES = src/main.cpp src/parse/parse.cpp src/gi/gi.cpp src/ssa2/ssa2.cpp src/regroup/regroup.cpp src/config/config.cpp
OBJECTS = $(SOURCES:src/%.cpp=build/%.o)
TARGET = build/app
all: $(TARGET)

build:
	mkdir -p build/parse build/gi build/ssa2 build/regroup build/config build

build/%.o: src/%.cpp | build
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(TARGET)
clean:
	rm -rf build/*.o build/parse/*.o build/gi/*.o build/ssa2/*.o build/regroup/*.o build/config/*.o

.PHONY: all clean
