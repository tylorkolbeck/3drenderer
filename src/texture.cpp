#include "texture.h"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <stb_image.h>

Texture::Texture(const char *filePath) : file(filePath) {
  glad_glGenTextures(1, &textureId);
  glBindTexture(GL_TEXTURE_2D, textureId);

  loadTexture();
  setTextureParams();
}

void Texture::init() {
  loadTexture();
  setTextureParams();
  stbi_image_free(textureData);
}

void Texture::loadTexture() {
  textureData = stbi_load(file, &width, &height, &nChannels, 0);
  if (textureData) {
    generateTextures();
  } else {
    std::cout << "ERROR::TEXTURE::FAILED_TO_LOAD_TEXTURE" << std::endl;
  }

}

void Texture::bind() {
  glBindTexture(GL_TEXTURE_2D, textureId);
}

void Texture::generateTextures() {
  glad_glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
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
