#include "camera.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include <SDL3/SDL_events.h>
#include <iostream>

Camera::Camera(float f, float nearZ, float farZ) {
  worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
  baseSpeed = 2.05f;
  speed = baseSpeed;
  front = glm::vec3(0.0f, 0.0f, -1.0f);
  pos = glm::vec3(0.0f, 0.0f, 10.0f);
  sensitivity = 0.1f;
  yaw = -90.0f;
  pitch = 0.0f;
  firstMouse = true;
  fov = f;
  near = nearZ;
  far = farZ;
}

void Camera::SetAspect(float a) {
  aspect = a;
  UpdatePerspective(); 
}

void Camera::UpdatePerspective() {
  proj = glm::perspective(glm::radians(fov), aspect, near, far);
}

void Camera::IncrementFov(int value) {
  fov -= value;
  if (fov < 1.0f) fov = 1.0f;
  if (fov > 45.0f) fov = 45.0f;
  UpdatePerspective();
}

Camera::~Camera() { std::cout << "camera instance destroyed" << std::endl; }

// const glm::mat4 Camera::ViewMatrix() const { return view; }

glm::vec3 Camera::Position() { return pos; }

void Camera::OnEvent(SDL_Event event) {
  switch (event.type) {
  case SDL_EVENT_MOUSE_MOTION:
    look((float)event.motion.xrel, (float)event.motion.yrel);
    break;
  case SDL_EVENT_MOUSE_WHEEL:
      IncrementFov(event.wheel.integer_y);
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
  front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  front.y = sin(glm::radians(pitch));
  front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  front = glm::normalize(front);
  right = glm::normalize(glm::cross(front, worldUp));
  worldUp = glm::normalize(glm::cross(right, front));
  view = glm::lookAt(pos, pos + front, worldUp);

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
