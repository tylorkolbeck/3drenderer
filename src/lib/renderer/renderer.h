#pragma once

#include "camera/camera.h"
#include "shader.h"
#include <memory>
class Renderer {
public:
  Renderer();
  ~Renderer();
  bool Init();
  void RenderFrame(const Camera& camera);
  void OnEvent(SDL_Event event);
  void BeginFrame();
  void EndFrame();

private:
  std::unique_ptr<Shader> shader;
  std::unique_ptr<Shader> lightShader;
  void setup();
  int displayMode = 0;
}; 
