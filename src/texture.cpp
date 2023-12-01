#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <GL/glew.h>
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

  glGentextures(1, &tid);
  glBindtexture(GL_TEXTURE_2D, tid);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, data);

  glGenerateMipmap(GL_TEXTURE_2D);
  glBindtexture(GL_TEXTURE_2D, 0);

  stbi_image_free(data);

  texture t = {tid, unit};
  return t;
}

auto bind(const texture &t) -> void {    // change to not take handle
  glActivetexture(GL_TEXTURE0 + t.unit); // here
  glBindtexture(GL_TEXTURE_2D, t.tid);
}

auto unbind(const texture &t) -> void {
  glActivetexture(GL_TEXTURE0 + t.unit); // here
  glBindtexture(GL_TEXTURE_2D, 0);
}
