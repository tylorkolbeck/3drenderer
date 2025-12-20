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

static SDL_Window *window = NULL;
static bool running = true;
static int w = 900;
static int h = 600;

SDL_AppResult initialize_window(void) {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    std::fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  // Create a SDL window
  window = SDL_CreateWindow("Renderer", w, h, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
  if (!window) {
    std::fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
    SDL_Quit();
    return SDL_APP_FAILURE;
  }

  return SDL_APP_CONTINUE;
}

bool setup(void) {
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

  SDL_GLContext ctx = SDL_GL_CreateContext(window);
  if (!ctx) {
    std::fprintf(stderr, "SDL_GL_CreateContext failed: %s\n", SDL_GetError());
    SDL_DestroyWindow(window);
    SDL_Quit();
    return false;
  }

  // Load OpenGL function pointers using SDL's loader
  if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
    std::fprintf(stderr, "gladLoadGL failed\n");
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
  }

  SDL_GL_SetSwapInterval(1);

  return true;
}

void update(void) {}

void render(void) {
  glViewport(0, 0, w, h);

  glClearColor(0.196f, 0.2f, 0.302f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  SDL_GL_SwapWindow(window);
}

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
    case SDL_EVENT_WINDOW_RESIZED:
        SDL_GetWindowSize(window, &w, &h);
        std::printf("WINDOW RESIZE EVENT %i, %i\n", w, h);
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
