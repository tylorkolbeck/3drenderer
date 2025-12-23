#include "camera.h"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include <iostream>

Camera::Camera(glm::vec3 pos, glm::vec3 direction) {
  up_ = glm::vec3(0.0f, 1.0f, 0.0f);
  direction_ = glm::vec3(0.0f, 0.0f, -1.0f);
  camera_speed = 2.05f;
  set_direction(direction);
  set_pos(pos);
  set_view();
  sensitivity = 0.1f;
  yaw = 0.0f;
  pitch = 0.0f;
  firstMouse = true;
  // set_target(target);
}

Camera::~Camera() { std::cout << "camera instance destroyed" << std::endl; }

void Camera::set_view() { view_ = glm::lookAt(pos_, pos_ + direction_, up_); }

void Camera::set_target(glm::vec3 target) { target_ = target; }

glm::mat4 Camera::view() {
  glm::vec3 direction;
  direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  direction.y = sin(glm::radians(pitch));
  direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  direction = glm::normalize(direction);

  return view_ = glm::lookAt(pos_, pos_ + direction, up_);
}

void Camera::set_pos(glm::vec3 pos) { pos_ = pos; }

glm::vec3 Camera::pos() { return pos_; }

void Camera::set_speed(float value) { camera_speed = value; }

void Camera::set_direction(glm::vec3 value) { direction_ = value; }

void Camera::look(float x, float y) {
  if (firstMouse) {
    firstMouse= false;
    return;
  }
  yaw += x * sensitivity;
  pitch -= y * sensitivity;

  if (pitch > 89.0f) pitch = 89.0f;
  if (pitch < -89.0f) pitch = -89.0f;

  if (yaw >= 360.0f) yaw -= 360.0f;
  if (yaw < 0.0f) yaw += 360.0f;
}

void Camera::forward(float dt) {
  set_pos(pos_ + camera_speed * direction_ * dt);
}
void Camera::back(float dt) { set_pos(pos_ - camera_speed * direction_ * dt); }
void Camera::left(float dt) {
  set_pos(pos_ -
          glm::normalize(glm::cross(direction_, up_)) * camera_speed * dt);
}
void Camera::right(float dt) {
  set_pos(pos_ +
          glm::normalize(glm::cross(direction_, up_)) * camera_speed * dt);
}
