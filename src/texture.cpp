#include "texture.h"
#include <iostream>
#include <glad/glad.h>
#include "stb_image_impl.h"

std::size_t Texture::s_instance_count = 0;

Texture::Texture(const char *filePath) : file(filePath) {
  glad_glGenTextures(1, &textureId);
  glBindTexture(GL_TEXTURE_2D, textureId);
  m_unit = s_instance_count++;
}

void Texture::init() {
  loadTexture();
  setTextureParams();
  stb::free(textureData);
}

void Texture::loadTexture() {
  textureData = stb::load(file, &width, &height, &nChannels, 0);
  if (textureData) {
    std::cout << "INFO::TEXTURE::TEXTURE_IMAGE_LOADED" << std::endl;
    generateTextures();
  } else {
    std::cout << "ERROR::TEXTURE::FAILED_TO_LOAD_TEXTURE" << std::endl;
  }
}

void Texture::bind() {
  glActiveTexture(GL_TEXTURE0 + m_unit);
  glBindTexture(GL_TEXTURE_2D, textureId);
}

void Texture::generateTextures() {
  GLenum format = GL_RGB;
  switch (nChannels) {
  case 1:
    format = GL_RED;
    break;
  case 3:
    format = GL_RGB;
    break;
  case 4:
    format = GL_RGBA;
  }
  glad_glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                    GL_UNSIGNED_BYTE, textureData);
  glad_glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::setTextureParams() {
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
