#define STB_IMAGE_IMPLEMENTATION
#include "camera/camera.h"
#include "renderer/renderer.h"
#include "shader.h"
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
static int display_mode = 0;
std::unique_ptr<Window> window;
std::unique_ptr<Camera> camera;
std::unique_ptr<Renderer> renderer;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// camera settings
float fov = 45.0f;
float nearZ = 0.1f;
float farZ = 100.0f;

void update(void) { camera->Update(deltaTime); }

void process_input(void) {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    // Let IMGUI HANDLE THE EVENT AS WELL
    // ImGui_ImplSDL3_ProcessEvent(&event);
    if (camera)
      camera->OnEvent(event);
    if (window)
      window->OnEvent(event);

    switch (event.type) {
      // System Events
    case SDL_EVENT_QUIT:
      running = false;
      break;
    case SDL_EVENT_KEY_DOWN:
      if (event.key.key == SDLK_ESCAPE) {
        running = false;
      }
      if (event.key.key == SDLK_M) {
        if (display_mode == 0) {
          display_mode = 1;
          glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        } else {
          display_mode = 0;
          glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
      }
      break;
    case SDL_EVENT_WINDOW_RESIZED:
        camera->SetPerspective(window->Aspect());
      break;
    }
  }
}

int main(void) {
  window = std::make_unique<Window>();
  camera = std::make_unique<Camera>(45.0f, 0.1f, 100.f);
  renderer = std::make_unique<Renderer>();
  if (!window->Init() || !renderer->Init()) {
    return 1;
  }

  while (running) {
    float currentFrame = SDL_GetPerformanceCounter();
    process_input();
    update();
    renderer->Render(*camera);
    SDL_GL_SwapWindow(window->Handle());
    deltaTime =
        (double)(currentFrame - lastFrame) / SDL_GetPerformanceFrequency();
    lastFrame = currentFrame;
  }

  return 0;
}
