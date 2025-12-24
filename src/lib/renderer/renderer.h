#pragma once

#include "camera/camera.h"
#include "shader.h"
#include <memory>
class Renderer {
public:
  Renderer();
  ~Renderer();
  bool Init();
  void Render(const Camera& camera);

private:
  std::unique_ptr<Shader> shader;
  void setup();
}; 
