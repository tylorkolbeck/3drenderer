#include "renderer.h"
#include "shader.h"
#include "texture.h"
#include <SDL3/SDL_video.h>
#include <iostream>

Texture *texture1 = nullptr;
Texture *texture2 = nullptr;

float vertices[] = {
        -0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
        -0.5f,  0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 

        -0.5f, -0.5f,  0.5f, 
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f, 
        -0.5f, -0.5f,  0.5f, 

        -0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f,  0.5f, 
        -0.5f,  0.5f,  0.5f, 

         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  

        -0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f, -0.5f,  0.5f,  
        -0.5f, -0.5f,  0.5f, 
        -0.5f, -0.5f, -0.5f, 

        -0.5f,  0.5f, -0.5f, 
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f, -0.5f, 
    };
unsigned int VBO, VAO, EBO;
unsigned int lightVAO;
glm::vec3 lightPos = glm::vec3(1.2f, 1.0f, 2.0f);
Renderer::Renderer() {}

Renderer::~Renderer() { std::cout << "renderer destroyed\n"; }

void Renderer::RenderFrame(const Camera &camera) {
  shader->use();
  shader->setMat4("view", camera.ViewMatrix());
  shader->setMat4("proj", camera.Projection());

  shader->setVec3("objectColor", 1.0f, 0.5f, 0.31f);
  shader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);


  glBindVertexArray(VAO);
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
  shader->setMat4("model", model);
  glDrawArrays(GL_TRIANGLES, 0, 36);

  lightShader->use();
  lightShader->setVec3("objectColor", 1.0f, 0.5f, 0.31f);
  lightShader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);
  glBindVertexArray(lightVAO);

  lightShader->setMat4("view", camera.ViewMatrix());
  lightShader->setMat4("proj", camera.Projection());

  model = glm::mat4(1.0f);
  model = glm::translate(model, lightPos);
  model = glm::scale(model, glm::vec3(0.2f));
  lightShader->setMat4("model", model);
  glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Renderer::BeginFrame() {
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glClearColor(0.129f, 0.129f, 0.129f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::EndFrame() {}

void Renderer::OnEvent(SDL_Event event) {
  switch (event.type) {
  case SDL_EVENT_KEY_DOWN:
    if (event.key.key == SDLK_M) {
      if (displayMode == 0) {
        displayMode = 1;
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      } else {
        displayMode = 0;

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      }
    }
    break;
  }
}

void Renderer::setup() {}

bool Renderer::Init() {
  // Load OpenGL function pointers using SDL's loader
  if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
    std::fprintf(stderr, "gladLoadGL failed\n");
    return false;
  }

  shader = std::make_unique<Shader>("assets/shaders/vertex.glsl",
                                    "assets/shaders/fragment.glsl");

  lightShader = std::make_unique<Shader>("assets/shaders/lightvertex.glsl",
                                         "assets/shaders/lightfragment.glsl");

  texture1 = new Texture("assets/textures/container.jpg");
  texture1->init();
  texture2 = new Texture("assets/textures/awesomeface.png");
  texture2->init();
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  // glGenBuffers(1, &EBO);

  // bind the VAO first
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Vertices mapping
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // Color mapping
  // glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
  //                       (void *)(3 * sizeof(float)));
  // glEnableVertexAttribArray(1);

  // glBindBuffer(GL_ARRAY_BUFFER, 0);
  // glBindVertexArray(0);

  glGenVertexArrays(1, &lightVAO);
  glBindVertexArray(lightVAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glBindVertexArray(0);

  return true;
}
