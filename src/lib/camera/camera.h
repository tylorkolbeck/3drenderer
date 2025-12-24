#pragma once

#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float3.hpp"
#include <SDL3/SDL_events.h>

struct InputState {
  bool w = false;
  bool a = false;
  bool s = false;
  bool d = false;
};

class Camera {
public:
  Camera(glm::vec3 location, glm::vec3 target);
  ~Camera();

  void set_pos(glm::vec3 loc);
  glm::vec3 pos();
  void set_target(glm::vec3 target);
  glm::vec3 target();
  glm::mat4 view();
  void set_speed(float value);
  void set_direction(glm::vec3 value);
  void look(float x, float y);

  // Movement
  void move_forward(float dt);
  void move_back(float dt);
  void move_left(float dt);
  void move_right(float dt);

  void update(float deltaTime);
  void onEvent(SDL_Event event);

private:
  glm::vec3 pos_;
  glm::vec3 target_;
  glm::vec3 up_;
  glm::mat4 view_;
  glm::vec3 front_;
  glm::vec3 right_;
  InputState input;
  bool firstMouse;
  float camera_speed;
  float sensitivity;
  float yaw_;
  float pitch_;
  void set_view();
};
