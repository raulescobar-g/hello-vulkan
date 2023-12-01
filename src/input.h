#include <glm/glm.hpp>

enum class keycodes : uint8_t {
  w = 1 << 0,
  a = 1 << 1,
  s = 1 << 2,
  d = 1 << 3,
  space = 1 << 4,
  shift = 1 << 5,
  esc = 1 << 6
};

struct input {
  uint8_t input;
  glm::vec2 prev_cursor_pos;
  glm::vec2 cursor_pos;
};
