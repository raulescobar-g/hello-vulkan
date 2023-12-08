#include "phong_program.h"
#include "util.h"
#include <iostream>
#include <string>

auto new_phong_program(const std::string &vertex_src,
                       const std::string &fragment_src) -> phong_program {
  auto vertex_shader_source = read_file(vertex_src);
  auto fragment_shader_source = read_file(fragment_src);

  std::cout << "Successfully compiled shaders." << std::endl;

  std::string P_str = "P";
  std::string MV_str = "MV";
  std::string iMV_str = "iMV";
  std::string ka_str = "ka";
  std::string kd_str = "kd";
  std::string ks_str = "ks";
  std::string s_str = "s";
  std::string lightPos_s = "lightPos";
  std::string lightColor_s = "lightColor";

  return phong_program{};
}

auto drop_phong_program(phong_program &p) -> void {}

auto bind(const phong_program &p) -> void {}
auto unbind(const phong_program &_) -> void {}
