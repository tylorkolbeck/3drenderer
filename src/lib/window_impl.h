#pragma once
#include <SDL3/SDL.h>

class Window {
public:
  Window();
  ~Window();
  bool init();
  void destroy();
  SDL_Window *window();
  int width();
  int height();
  void setSize(int w, int h);
  void setAspect();
  float aspect();

private:
  int w;
  int h;
  float _aspect;
  SDL_Window *sdl_window;
};
