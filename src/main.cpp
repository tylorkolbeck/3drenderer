#include "camera/camera.h"
#include "glm/ext/matrix_transform.hpp"
#include <SDL3/SDL_mouse.h>
#include <memory>
#define STB_IMAGE_IMPLEMENTATION
#include "shader.h"
#include "texture.h"
#include "window_sdl_impl.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_video.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_sdl3.h>
#include <cstdio>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <stdbool.h>

void compileShaders();
void linkShaders();

// static SDL_Window *window = NULL;
static bool running = true;
static int display_mode = 0;

Shader *shader = nullptr;
Texture *texture1 = nullptr;
Texture *texture2 = nullptr;
std::unique_ptr<Window> window;
std::unique_ptr<Camera> camera;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float lastX = 0, lastY = 0;

bool showDemo = true;

struct InputState {
  bool w = false;
  bool a = false;
  bool s = false;
  bool d = false;
};

InputState input;

// Fake camera settings
float fov = 45.0f;

unsigned int VBO, VAO, EBO;

float vertices[] = {
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,
    0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
    -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

    -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

    -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,

    0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
    0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,
    0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,
    0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

    -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f};

glm::vec3 cubePositions[] = {
    glm::vec3(0.0f, 0.0f, -2.0f),   glm::vec3(2.0f, 5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
    glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};

unsigned char *textureData;

bool setup(void) {
  // Load OpenGL function pointers using SDL's loader
  if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
    std::fprintf(stderr, "gladLoadGL failed\n");
    SDL_Quit();
    return 1;
  }

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui::StyleColorsDark();

  ImGui_ImplSDL3_InitForOpenGL(window->handle(), window->glContext());
  ImGui_ImplOpenGL3_Init("#version 330");

  glEnable(GL_DEPTH_TEST);

  // Shader setup
  shader =
      new Shader("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl");
  texture1 = new Texture("assets/textures/container.jpg");
  texture1->init();
  texture2 = new Texture("assets/textures/awesomeface.png");
  texture2->init();

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  // bind the VAO first
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Vertice mapping
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // Color mapping
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // unbind the VBO and VAO
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA,
              GL_ONE_MINUS_SRC_ALPHA); // Common for standard alpha blending

  return true;
}

void update(void) {
  if (input.w)
    camera->forward(deltaTime);
  if (input.s)
    camera->back(deltaTime);
  if (input.a)
    camera->left(deltaTime);
  if (input.d)
    camera->right(deltaTime);
}

void process_input(void) {
  // TODO: future implementation
  // if (camera) camera->onEvent(e);
  //   if (ui) ui->onEvent(e);
  //   if (window) window->onEvent(e);

  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    // Let IMGUI HANDLE THE EVENT AS WELL
    ImGui_ImplSDL3_ProcessEvent(&event);
    switch (event.type) {
    case SDL_EVENT_QUIT:
      running = false;
      break;
    case SDL_EVENT_KEY_DOWN:
      if (event.key.repeat)
        break; // optional: ignore OS repeat
      if (event.key.key == SDLK_W)
        input.w = true;
      if (event.key.key == SDLK_S)
        input.s = true;
      if (event.key.key == SDLK_A)
        input.a = true;
      if (event.key.key == SDLK_D)
        input.d = true;
      if (event.key.key == SDLK_ESCAPE) {
        running = false;
      }
      if (event.key.key == SDLK_M) {
        if (display_mode == 0) {
          display_mode = 1;
          glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        } else {
          display_mode = 0;
          glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
      }
      break;

    case SDL_EVENT_KEY_UP:
      if (event.key.key == SDLK_W)
        input.w = false;
      if (event.key.key == SDLK_S)
        input.s = false;
      if (event.key.key == SDLK_A)
        input.a = false;
      if (event.key.key == SDLK_D)
        input.d = false;
      break;
    case SDL_EVENT_MOUSE_MOTION:
      camera->look((float)event.motion.xrel, (float)event.motion.yrel);
      break;
    case SDL_EVENT_WINDOW_RESIZED:
      int w, h;
      SDL_GetWindowSize(window->handle(), &w, &h);
      window->setSize(w, h);
      glViewport(0, 0, window->width(), window->height());
      std::printf("WINDOW RESIZE EVENT %i, %i\n", w, h);
      break;
    }
  }
}

void render(void) {
  SDL_HideCursor();
  glClearColor(0.129f, 0.129f, 0.129f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // START IMGUI FRAME
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL3_NewFrame();
  ImGui::NewFrame();
  ImGui::ShowDemoWindow(&showDemo);

  shader->use();
  shader->setInt("texture1", 0);
  shader->setInt("texture2", 1);

  texture1->bind();
  texture2->bind();

  glm::mat4 proj =
      glm::perspective(glm::radians(fov), window->aspect(), 0.1f, 100.0f);

  shader->setMat4("view", camera->view());
  shader->setMat4("proj", proj);

  glBindVertexArray(VAO);
  for (unsigned int i = 0; i < 10; i++) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, cubePositions[i]);
    float angle = window->time() * 5 * i;
    model =
        glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
    shader->setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
  }

  // RENDER IMGUI FRAME
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  // SWAP THE FRONT AND BACK BUFFER
  SDL_GL_SwapWindow(window->handle());
}
int main(void) {
  window = std::make_unique<Window>();
  camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 3.0f),
                                    glm::vec3(0.0f, 0.0f, -1.0f));

  if (!window->init()) {
    return 1;
  }

  setup();

  while (running) {
    float currentFrame = SDL_GetPerformanceCounter();
    process_input();
    update();
    render();

    deltaTime =
        (double)(currentFrame - lastFrame) / SDL_GetPerformanceFrequency();
    lastFrame = currentFrame;
  }
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL3_Shutdown();
  ImGui::DestroyContext();

  SDL_Quit();

  return 0;
}
