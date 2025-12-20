#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <cstdio>
#include <glad/glad.h>
#include <stdbool.h>
#include <stdio.h>

SDL_Window *window = NULL;
SDL_GLContext gl_ctx = NULL;
static bool running = true;

SDL_AppResult initialize_window(void) {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    std::fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
    return SDL_APP_FAILURE;
  }
  

  // SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  // SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  // SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
  // SDL_GL_CONTEXT_PROFILE_CORE);

  // int n = SDL_GetNumRenderDrivers();
  // SDL_Log("Render drivers available: %d", n);
  // for (int i = 0; i < n; i++) {
  //   SDL_Log("  %d: %s", i, SDL_GetRenderDriver(i));
  // }

  // Create a SDL window
  window = SDL_CreateWindow("Renderer", 600, 600, 0);
  if (!window) {
    std::fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
    SDL_Quit();
    return SDL_APP_FAILURE;
  }

  return SDL_APP_CONTINUE;
}

void setup(void) {}

void update(void) {}

void render(void) {}

void process_input(void) {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_EVENT_QUIT:
      running = false;
      break;
    case SDL_EVENT_KEY_DOWN:
      if (event.key.key == SDLK_ESCAPE) {
        running = false;
      }
      break;
    }
  }
}

int main(void) {
  if (initialize_window() != SDL_APP_CONTINUE) {
    return 1;
  }

  setup();

  while (running) {
    process_input();
    update();
    render();
  }
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
