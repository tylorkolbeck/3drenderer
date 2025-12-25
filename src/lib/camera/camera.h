#pragma once

#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float3.hpp"
#include <SDL3/SDL_events.h>

struct InputState {
  bool w = false;
  bool a = false;
  bool s = false;
  bool d = false;
  bool l_shift = false;
};

class Camera {
public:
  Camera(float fov, float nearZ, float farZ);
  ~Camera();

  // getters
  glm::vec3 Position();
  glm::vec3 Target();
  // const glm::mat4 ViewMatrix();

  // Movement
  void MoveForward(float dt);
  void MoveBack(float dt);
  void MoveLeft(float dt);
  void MoveRight(float dt);
  void Update(float deltaTime);
  void OnEvent(SDL_Event event);
  void SetAspect(float aspect);
  void UpdatePerspective();
  void IncrementFov(int value);
  const glm::mat4 &Projection() const { return proj; };
  const glm::mat4 &ViewMatrix() const { return view; };

private:
  void look(float x, float y);
  glm::vec3 pos;
  glm::vec3 target;
  glm::vec3 worldUp;
  glm::mat4 view;
  glm::vec3 front;
  glm::vec3 right;
  InputState input;
  glm::mat4 proj;
  bool firstMouse;
  float baseSpeed;
  float speed;
  float sensitivity;
  float yaw;
  float pitch;

  // perspective members
  float fov;
  float aspect;
  float near;
  float far;
};
