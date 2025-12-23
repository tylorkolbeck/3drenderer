CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++20 -O0 -g \
  $(shell pkg-config --cflags sdl3) \
  -Iinclude/glad/include \
  -Iinclude/stb/include \
  -Iinclude/glm \
  -Isrc -Isrc/lib \
  -Iexternal/imgui \
  -Iexternal/imgui/backends \
  -DIMGUI_IMPL_OPENGL_LOADER_GLAD

LDFLAGS := $(shell pkg-config --libs sdl3) -lGL

OUT := dist/renderer
OBJDIR := dist/obj

IMGUI_DIR := external/imgui

PROJECT_SRC := $(shell find src -type f -name '*.cpp')
GLAD_SRC := include/glad/src/glad.c
IMGUI_SRC := \
  $(IMGUI_DIR)/imgui.cpp \
  $(IMGUI_DIR)/imgui_draw.cpp \
  $(IMGUI_DIR)/imgui_tables.cpp \
  $(IMGUI_DIR)/imgui_widgets.cpp \
  $(IMGUI_DIR)/backends/imgui_impl_sdl3.cpp \
  $(IMGUI_DIR)/backends/imgui_impl_opengl3.cpp

SRC := $(PROJECT_SRC) $(GLAD_SRC) $(IMGUI_SRC)

# Turn "path/to/file.cpp" into "dist/obj/path/to/file.cpp.o"
OBJ := $(patsubst %.cpp,$(OBJDIR)/%.cpp.o,$(filter %.cpp,$(SRC))) \
       $(patsubst %.c,$(OBJDIR)/%.c.o,$(filter %.c,$(SRC)))

.PHONY: build run clean

build: $(OUT)

$(OUT): $(OBJ)
	mkdir -p dist
	$(CXX) $(OBJ) -o $@ $(LDFLAGS)

# Compile C++
$(OBJDIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

# Compile C
$(OBJDIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

# Auto-include dependency files
DEPS := $(OBJ:.o=.d)
-include $(DEPS)

run: build
	./$(OUT)

clean:
	rm -rf dist
