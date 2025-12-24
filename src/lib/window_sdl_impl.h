#pragma once
#include <SDL3/SDL.h>

class Window {
public:
  Window();
  ~Window();
  bool init();
  void destroy();
  SDL_Window *handle();
  int width();
  int height();
  void setSize(int w, int h);
  void setAspect();
  float aspect();
  SDL_GLContext glContext();
  float time();
  void onEvent(SDL_Event event);

private:
  int w;
  int h;
  float _aspect;
  SDL_GLContext sdl_ctx;
  SDL_Window *sdl_window = nullptr;
};
