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

  // Create a SDL window
  sdl_window = SDL_CreateWindow("Renderer", w, h,
                            SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
  if (!sdl_window) {
    std::fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
    SDL_Quit();
    return false;
  }

  return true;
}

void Window::render() {
  SDL_GL_SwapWindow(sdl_window);
}

void Window::destroy() {
  SDL_DestroyWindow(sdl_window);
}

void Window::swapBuffer() { SDL_GL_SwapWindow(sdl_window); }

SDL_Window *Window::window() { return sdl_window; }
