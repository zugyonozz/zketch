# Makefile for zketch demo

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++14 -Wall -Wextra -I./include

# Platform detection
ifeq ($(OS),Windows_NT)
    PLATFORM = win32
    LDFLAGS = -lgdi32 -luser32
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        PLATFORM = linux
        LDFLAGS = -lX11
    endif
    ifeq ($(UNAME_S),Darwin)
        PLATFORM = macos
        LDFLAGS = -framework Cocoa
    endif
endif

# Source files
SRCS = src/style.cpp \
       src/element.cpp \
       src/ui_manager.cpp \
       src/zketch.cpp \
       src/main.cpp \
       platform/$(PLATFORM)/main.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Target executable
TARGET = zketch_demo

# Default target
all: $(TARGET)

# Linking
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Compilation
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean