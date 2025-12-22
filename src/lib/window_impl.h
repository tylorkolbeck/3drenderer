#pragma once
#include <SDL3/SDL.h>

class Window {
public:
  Window();
  ~Window();
  bool init();
  void destroy();
  void swapBuffer();
  SDL_Window *window();
  void render();
private:
  int w;
  int h;
  SDL_Window *sdl_window;
};
