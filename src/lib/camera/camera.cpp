#include "camera.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include <SDL3/SDL_events.h>
#include <iostream>

Camera::Camera(float fov, float nearZ, float farZ) {
  up = glm::vec3(0.0f, 1.0f, 0.0f);
  baseSpeed = 2.05f;
  speed = baseSpeed;
  this->front = glm::vec3(0.0f, 0.0f, -1.0f);
  this->pos = glm::vec3(0.0f, 0.0f, 3.0f);
  sensitivity = 0.1f;
  yaw = 0.0f;
  pitch = 0.0f;
  firstMouse = true;
  this->fov = fov;
  near = nearZ;
  far = farZ;
}

void Camera::SetPerspective(float a) {
  aspect = a;
  std::cout << "setting perspective\n";

  proj = glm::perspective(glm::radians(fov), aspect, near, far);
}

Camera::~Camera() { std::cout << "camera instance destroyed" << std::endl; }

const glm::mat4 Camera::View() { return view; }

glm::vec3 Camera::Pos() { return pos; }

void Camera::OnEvent(SDL_Event event) {
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

void Camera::Update(float deltaTime) {
  glm::vec3 f;
  f.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  f.y = sin(glm::radians(pitch));
  f.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  front = glm::normalize(f);

  right = glm::normalize(glm::cross(front, up));
  up = glm::normalize(glm::cross(right, front));
  view = glm::lookAt(pos, pos + front, up);

  if (input.w)
    MoveForward(deltaTime);
  if (input.s)
    MoveBack(deltaTime);
  if (input.a)
    MoveLeft(deltaTime);
  if (input.d)
    MoveRight(deltaTime);

  speed = input.l_shift ? baseSpeed * 2 : baseSpeed;
}

void Camera::look(float x, float y) {
  if (firstMouse) {
    firstMouse = false;
    return;
  }
  yaw += x * sensitivity;
  pitch -= y * sensitivity;

  if (pitch > 89.0f)
    pitch = 89.0f;
  if (pitch < -89.0f)
    pitch = -89.0f;

  if (yaw >= 360.0f)
    yaw -= 360.0f;
  if (yaw < 0.0f)
    yaw += 360.0f;
}

void Camera::MoveForward(float dt) { pos += front * speed * dt; }
void Camera::MoveBack(float dt) { pos -= front * speed * dt; }
void Camera::MoveLeft(float dt) { pos -= right * speed * dt; }
void Camera::MoveRight(float dt) { pos += right * speed * dt; }
