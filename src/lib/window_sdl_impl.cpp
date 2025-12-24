#include "window_sdl_impl.h"
#include "shader.h"
#include <SDL3/SDL_video.h>
#include <cstdio>
#include <iostream>

Window::Window() {}

Window::~Window() {
  if (sdl_window) {
    SDL_DestroyWindow(sdl_window);
    std::cout << "sdl_window destroyed" << std::endl;
  }
  std::cout << "window instance destroyed" << std::endl;
}

bool Window::init() {
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
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

  SDL_GLContext ctx = SDL_GL_CreateContext(sdl_window);
  if (!ctx) {
    std::fprintf(stderr, "SDL_GL_CreateContext failed: %s\n", SDL_GetError());
    SDL_Quit();
    return false;
  }

  SDL_SetWindowRelativeMouseMode(sdl_window, true);
  SDL_GL_SetSwapInterval(1);

  return true;
}

float Window::time() {
  return (float)SDL_GetPerformanceCounter() /
         (double)SDL_GetPerformanceFrequency();
}

SDL_GLContext Window::glContext() { return sdl_ctx; }

float Window::aspect() { return _aspect; }

void Window::setAspect() { _aspect = (h != 0) ? (float)w / (float)h : 1.0f; }

void Window::setSize(int w, int h) {
  this->w = w;
  this->h = h;
  setAspect();
}

void Window::onEvent(SDL_Event event) {
  switch (event.type) {
  case SDL_EVENT_WINDOW_RESIZED:
    int w, h;
    SDL_GetWindowSize(handle(), &w, &h);
    setSize(w, h);
    glViewport(0, 0, width(), height());
    break;
  }
}
int Window::width() { return w; }

int Window::height() { return h; }

SDL_Window *Window::handle() { return sdl_window; }
