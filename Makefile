CXX      := g++
CXXFLAGS := -Wall -Wextra -std=c++20 -O0 -g \
            $(shell pkg-config --cflags sdl3) \
            -Iinclude/glad/include \
			-Iinclude/stb/include \
			-Isrc -Isrc/lib \
			-Iinclude/glm

LDFLAGS  := $(shell pkg-config --libs sdl3) -lGL
SRC := $(shell find src -type f -name '*.cpp') include/glad/src/glad.c
# SRC      := $(wildcard src/*.cpp)  \
# 			$(wildcard src/lib/*.cpp) \
# 			include/glad/src/glad.c
OUT      := dist/renderer

.PHONY: build run clean

build: $(OUT)

$(OUT): $(SRC)
	mkdir -p dist
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT) $(LDFLAGS)

run: build
	./$(OUT)

all: clean build run

clean:
	rm -f $(OUT)
