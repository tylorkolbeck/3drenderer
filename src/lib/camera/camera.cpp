#include "camera.h"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include <iostream>

Camera::Camera(glm::vec3 pos, glm::vec3 direction) {
  up_ = glm::vec3(0.0f, 1.0f, 0.0f);
  direction_ = glm::vec3(0.0f, 0.0f, -1.0f);
  camera_speed = 0.05f;
  set_direction(direction);
  set_pos(pos);
  set_view();

  // set_target(target);
}

void Camera::forward() { set_pos(pos_ + camera_speed * direction_); }
void Camera::back() { set_pos(pos_ - camera_speed * direction_); }
void Camera::left() { set_pos(pos_ - glm::normalize(glm::cross(direction_, up_)) * camera_speed); }
void Camera::right() { set_pos(pos_ + glm::normalize(glm::cross(direction_, up_)) * camera_speed); }

Camera::~Camera() { std::cout << "camera instance destroyed" << std::endl; }

void Camera::set_view() { view_ = glm::lookAt(pos_, pos_ + direction_, up_); }

void Camera::set_target(glm::vec3 target) { target_ = target; }

glm::mat4 Camera::view() {
  return view_ = glm::lookAt(pos_, pos_ + direction_, up_);
}

void Camera::set_pos(glm::vec3 pos) { pos_ = pos; }

glm::vec3 Camera::pos() { return pos_; }

void Camera::set_speed(float value) { camera_speed = value; }

void Camera::set_direction(glm::vec3 value) { direction_ = value; }
