#pragma once

class Texture {
public:
  unsigned int textureId;
  int width, height, nChannels;
  unsigned char *textureData;
  const char *file;
  Texture(const char *filePath);
  void loadTexture();
  void generateTextures();
  void setTextureParams();
  void bind();
  void init();

private:
};
