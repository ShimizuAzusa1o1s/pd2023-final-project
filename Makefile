# Build the game with "make" and run it with "make run".
#
# Windows (MinGW): uses the bundled SDL2 development files in sdl2-mingw/.
# Linux / WSL:     uses the system SDL2 found by pkg-config
#                  (install it with "sudo apt install libsdl2-dev").
#
# Override SDL2_CFLAGS or SDL2_LIBS to use another SDL2 installation.

CXX ?= g++
CXXFLAGS ?= -O2
SOURCE = main.cpp

ifeq ($(OS),Windows_NT)
    TARGET = main.exe
    SDL2_CFLAGS ?= -I sdl2-mingw/include
    SDL2_LIBS ?= -L sdl2-mingw/lib -lmingw32 -lSDL2main -lSDL2
    RUN = $(TARGET)
    REMOVE = del /Q
else
    TARGET = main
    SDL2_CFLAGS ?= $(shell pkg-config --cflags sdl2)
    SDL2_LIBS ?= $(shell pkg-config --libs sdl2)
    RUN = ./$(TARGET)
    REMOVE = rm -f
endif

# Every source file is included by main.cpp, so any change rebuilds the game.
HEADERS = $(wildcard *.h *.cpp ecs/*.h ecs/*.cpp)

all: $(TARGET)

$(TARGET): $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SDL2_CFLAGS) -o $(TARGET) $(SOURCE) $(SDL2_LIBS)

# Pass options with ARGS, for example: make run ARGS=--fullscreen
run: $(TARGET)
	$(RUN) $(ARGS)

clean:
	$(REMOVE) $(TARGET)

.PHONY: all run clean
