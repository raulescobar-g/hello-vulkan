#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>

auto new_texture(const std::string &filename, uint unit) -> texture {
  // Load texture
  int w, h, ncomps;
  stbi_set_flip_vertically_on_load(true);
  unsigned char *data = stbi_load(filename.c_str(), &w, &h, &ncomps, 0);
  if (!data) {
    std::cerr << filename << " not found" << std::endl;
    exit(1);
  }
  if (ncomps != 3) {
    std::cerr << filename << " must have 3 components (RGB)" << std::endl;
    exit(1);
  }
  if ((w & (w - 1)) != 0 || (h & (h - 1)) != 0) {
    std::cerr << filename << " must be a power of 2" << std::endl;
    exit(1);
  }
  int width = w;
  int height = h;
  uint tid = 0;

  stbi_image_free(data);

  texture t = {tid, unit};
  return t;
}

auto bind(const texture &t) -> void { // change to not take handle
}

auto unbind(const texture &t) -> void {}
