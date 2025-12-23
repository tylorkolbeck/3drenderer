#include "window_impl.h"
#include <SDL3/SDL_video.h>
#include <cstdio>

Window::Window() {}

Window::~Window() {}

bool Window::init() {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    std::fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
    return false;
  }

  sdl_window = SDL_CreateWindow(
      "Renderer", w, h,
      SDL_WINDOW_OPENGL | SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_RESIZABLE);
  if (!sdl_window) {
    std::fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
    SDL_Quit();
    return false;
  }

  return true;
}

float Window::aspect() {
  return _aspect;
}

void Window::setAspect() { _aspect = (h != 0) ? (float)w / (float)h : 1.0f; }

void Window::setSize(int w, int h) {
  this->w = w;
  this->h = h;
  setAspect();
}
void Window::render() { SDL_GL_SwapWindow(sdl_window); }

void Window::destroy() { SDL_DestroyWindow(sdl_window); }

int Window::width() { return w; }

int Window::height() { return h; }

void Window::swapBuffer() { SDL_GL_SwapWindow(sdl_window); }

SDL_Window *Window::window() { return sdl_window; }
