#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>

struct window {
public:
  auto operator*() -> GLFWwindow * { return win; }

  GLFWwindow *win;
  int width, height;
  std::string title;
};

auto init_window_lib() -> void;
auto init_glew() -> void;
auto drop_window_lib() -> void;

auto new_window(int, int, const std::string &) -> window;
auto drop_window(window &) -> void;
auto update_framesize(window &) -> void;
auto swap_buffers(window &) -> void;
auto get_unit_translation(window &) -> glm::vec3;
auto get_cursor_delta(window &) -> glm::vec2;

auto poll_events() -> void;
