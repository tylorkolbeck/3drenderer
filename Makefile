CXX      := g++
CXXFLAGS := -Wall -Wextra -std=c++20 -O0 -g \
            $(shell pkg-config --cflags sdl3) \
            -Iinclude/glad/include

LDFLAGS  := $(shell pkg-config --libs sdl3) -lGL

SRC      := $(wildcard src/*.cpp) include/glad/src/glad.c
OUT      := dist/renderer

.PHONY: build run clean

build: $(OUT)

$(OUT): $(SRC)
	mkdir -p dist
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT) $(LDFLAGS)

run: build
	./$(OUT)

clean:
	rm -f $(OUT)
