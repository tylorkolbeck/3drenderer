#include "stb_image_impl.h"
#include <stb_image.h>

unsigned char* stb::load(char const *filename, int *x, int *y, int *comp, int req_comp) {
  stbi_set_flip_vertically_on_load(true);
  return stbi_load(filename, x, y, comp, req_comp);
}

void stb::free(unsigned char* data) {
  stbi_image_free(data);
}
