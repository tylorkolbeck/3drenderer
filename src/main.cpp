#define STB_IMAGE_IMPLEMENTATION
#include "camera/camera.h"
#include "renderer/renderer.h"
#include "window_sdl_impl.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_video.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_sdl3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <memory>
#include <stdbool.h>

static bool running = true;
std::unique_ptr<Window> window;
std::unique_ptr<Camera> cameraPerspective;
std::unique_ptr<Renderer> renderer;

void process_input(void) {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    cameraPerspective->OnEvent(event);
    window->OnEvent(event);
    renderer->OnEvent(event);

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
      cameraPerspective->SetAspect(window->Aspect());
      break;
    }
  }
}

int main(void) {
  window = std::make_unique<Window>();
  cameraPerspective = std::make_unique<Camera>(45.0f, 0.1f, 100.f);
  renderer = std::make_unique<Renderer>();
  if (!window->Init() || !renderer->Init()) {
    return 1;
  }

  uint64_t last = SDL_GetPerformanceCounter();

  while (running) {
    // Start frame
    uint64_t now = SDL_GetPerformanceCounter();
    double dt = (double)(now - last) / (double)SDL_GetPerformanceFrequency();
    last = now;
    
    process_input();

    cameraPerspective->Update(dt);
    renderer->BeginFrame();
    renderer->RenderFrame(*cameraPerspective);
    renderer->EndFrame();

    window->SwapBuffers();
    // End frame
  }

  return 0;
}
