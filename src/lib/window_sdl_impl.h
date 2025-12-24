#pragma once
#include <SDL3/SDL.h>

class Window {
public:
  Window();
  ~Window();
  bool Init();
  SDL_Window *Handle();
  int Width();
  int Height();
  float Aspect();
  SDL_GLContext glContext();
  float Time();
  void OnEvent(SDL_Event event);
  void Quit();
  void SwapBuffers();

private:
  int w;
  int h;
  float aspect;
  SDL_GLContext sdl_ctx;
  SDL_Window *sdl_window = nullptr;
  void setSize(int w, int h);
  void setAspect();
};
