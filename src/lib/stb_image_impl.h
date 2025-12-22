#pragma once
#define STB_IMAGE_IMPLEMENTATION

namespace stb {
unsigned char *load(char const *filename, int *x, int *y, int *comp, int req_comp);
void free(unsigned char *data);
} // namespace stb
