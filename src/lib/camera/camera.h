#pragma once

#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float3.hpp"

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
  void forward(float dt);
  void back(float dt);
  void left(float dt);
  void right(float dt);

private:
  glm::vec3 pos_;
  glm::vec3 target_;
  glm::vec3 up_;
  glm::mat4 view_;
  glm::vec3 direction_;
  bool firstMouse;
  float camera_speed;
  float sensitivity;
  float yaw;
  float pitch;
  void set_view();
};
