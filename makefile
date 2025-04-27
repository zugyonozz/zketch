# Compiler dan flags
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Iinclude -Iinclude/SDL3

# Direktori
SRC_DIR := src
MODULES_DIR := src/modules
OBJ_DIR := build
BIN := bin/zketch.exe

# File sumber dan object
SOURCES := $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(MODULES_DIR)/*.cpp)
OBJECTS := $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(SOURCES))

# Library
LIBS := -Llib -lSDL3 -lSDL3_image -lSDL3_ttf -lMsimg32 -lgdi32

# Target default
all: $(BIN)

# Compile .cpp ke .o dalam direktori build
$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Linking object files
$(BIN): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(BIN) $(LIBS)

# Jalankan program setelah build
run:
	$(MAKE) clean
	$(MAKE) all
	./$(BIN)

# Bersihkan build
clean:
	@echo Cleaning...
	@rm -rf build $(BIN)

.PHONY: all run clean
