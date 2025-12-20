#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <stdbool.h>
#include <stdio.h>

SDL_Window *window = NULL;
static bool is_running = true;

SDL_AppResult initialize_window(void) {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
    SDL_Log("could not initialize SDL %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  int n = SDL_GetNumRenderDrivers();
  SDL_Log("Render drivers available: %d", n);
  for (int i = 0; i < n; i++) {
    SDL_Log("  %d: %s", i, SDL_GetRenderDriver(i));
  }

  // Create a SDL window
  window = SDL_CreateWindow("3D Renderer", 600, 600, SDL_WINDOW_FULLSCREEN);
  if (!window) {
    SDL_Log("could not create SDL window %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  // Create a renderer

  return SDL_APP_CONTINUE;
}

void setup(void) {}

void update(void) {}

void render(void) {
}

void process_input(void) {
  SDL_Event event;
  SDL_PollEvent(&event);

  switch (event.type) {
  case SDL_EVENT_QUIT:
    is_running = false;
    break;
  case SDL_EVENT_KEY_DOWN:
    if (event.key.key == SDLK_ESCAPE) {
      is_running = false;
    }
    break;
  }
}

int main(void) {
  if (initialize_window() != SDL_APP_CONTINUE) {
    is_running = false;
  }

  setup();

  while (is_running) {
    process_input();
    update();
    render();
  }

  return 0;
}
