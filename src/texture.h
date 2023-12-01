#include <string>

using uint = unsigned int;

struct texture {
  uint tid;
  uint unit;
};

auto new_texture(const std::string &, uint) -> texture;
auto bind(const texture &) -> void;
auto unbind(const texture &) -> void;
