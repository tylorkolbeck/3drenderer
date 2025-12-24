#include "camera.h"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include <SDL3/SDL_events.h>
#include <iostream>

Camera::Camera(glm::vec3 pos, glm::vec3 front) {
  up_ = glm::vec3(0.0f, 1.0f, 0.0f);
  front_ = glm::vec3(0.0f, 0.0f, -1.0f);
  baseSpeed = 2.05f;
  speed = baseSpeed;
  front_ = front;
  pos_ = pos;
  sensitivity = 0.1f;
  yaw_ = 0.0f;
  pitch_ = 0.0f;
  firstMouse = true;
}

Camera::~Camera() { std::cout << "camera instance destroyed" << std::endl; }

glm::mat4 Camera::view() { return view_; }

glm::vec3 Camera::pos() { return pos_; }

void Camera::onEvent(SDL_Event event) {
  switch (event.type) {
  case SDL_EVENT_MOUSE_MOTION:
    look((float)event.motion.xrel, (float)event.motion.yrel);
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
    if (event.key.key == SDLK_LSHIFT)
      input.l_shift = true;
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
    if (event.key.key == SDLK_LSHIFT)
      input.l_shift = false;
    break;
  }
}

void Camera::update(float deltaTime) {
  glm::vec3 f;
  f.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
  f.y = sin(glm::radians(pitch_));
  f.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
  front_ = glm::normalize(f);

  right_ = glm::normalize(glm::cross(front_, up_));
  up_ = glm::normalize(glm::cross(right_, front_));
  view_ = glm::lookAt(pos_, pos_ + front_, up_);

  if (input.w)
    move_forward(deltaTime);
  if (input.s)
    move_back(deltaTime);
  if (input.a)
    move_left(deltaTime);
  if (input.d)
    move_right(deltaTime);

  speed = input.l_shift ? baseSpeed * 2 : baseSpeed;
}

void Camera::look(float x, float y) {
  if (firstMouse) {
    firstMouse = false;
    return;
  }
  yaw_ += x * sensitivity;
  pitch_ -= y * sensitivity;

  if (pitch_ > 89.0f)
    pitch_ = 89.0f;
  if (pitch_ < -89.0f)
    pitch_ = -89.0f;

  if (yaw_ >= 360.0f)
    yaw_ -= 360.0f;
  if (yaw_ < 0.0f)
    yaw_ += 360.0f;
}

void Camera::move_forward(float dt) { pos_ += front_ * speed * dt; }
void Camera::move_back(float dt) { pos_ -= front_ * speed * dt; }
void Camera::move_left(float dt) { pos_ -= right_ * speed * dt; }
void Camera::move_right(float dt) { pos_ += right_ * speed * dt; }
