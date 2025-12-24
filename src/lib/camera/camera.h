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
  Camera(glm::vec3 location, glm::vec3 target);
  ~Camera();

  // getters
  glm::vec3 pos();
  glm::vec3 target();
  glm::mat4 view();

  // Movement
  void move_forward(float dt);
  void move_back(float dt);
  void move_left(float dt);
  void move_right(float dt);

  void update(float deltaTime);
  void onEvent(SDL_Event event);

private:
  void look(float x, float y);
  glm::vec3 pos_;
  glm::vec3 target_;
  glm::vec3 up_;
  glm::mat4 view_;
  glm::vec3 front_;
  glm::vec3 right_;
  InputState input;
  bool firstMouse;
  float baseSpeed;
  float speed;
  float sensitivity;
  float yaw_;
  float pitch_;
};
