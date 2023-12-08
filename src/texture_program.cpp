#include "texture_program.h"
#include "util.h"
#include <iostream>
#include <string>

auto new_texture_program(const std::string &vertex_src,
                         const std::string &fragment_src) -> texture_program {
  auto vertex_shader_source = read_file(vertex_src);
  auto fragment_shader_source = read_file(fragment_src);

  std::cout << "Successfully compiled shaders." << std::endl;

  std::string P_str = "P";
  std::string MV_str = "MV";
  std::string iMV_str = "iMV";
  std::string texture_str = "texture0";

  return texture_program{};
};
auto drop_texture_program(texture_program &p) -> void {}

auto bind(const texture_program &p) -> void {}
auto unbind(const texture_program &_) -> void {}
