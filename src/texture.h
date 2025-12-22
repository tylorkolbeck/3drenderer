#pragma once

#include <cstddef>
class Texture {
public:
  unsigned int textureId;
  int width, height, nChannels;
  unsigned char *textureData;
  const char *file;
  Texture(const char *filePath);
  ~Texture() {
    --s_instance_count;
  }
  void loadTexture();
  void generateTextures();
  void setTextureParams();
  void bind();
  void init();

private:
  static std::size_t s_instance_count;
  int m_unit = 0;
};
