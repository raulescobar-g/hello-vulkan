#include "window.h"
#include "GLFW/glfw3.h"

auto init_window_lib() -> void {
  if (!glfwInit()) {
    perror("Error initializing glfw");
    exit(1);
  }
}
auto init_glew() -> void {
  glewExperimental = true;
  if (glewInit() != GLEW_OK) {
    perror("Error initializing glew");
    exit(1);
  }
}

auto new_window(int width, int height, const std::string &title) -> window {

  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

  GLFWwindow *win = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
  if (!win) {
    perror("glfw returned nullptr on create window");
    exit(1);
  }

  glfwMakeContextCurrent(win);
  glfwSwapInterval(0);

  window result = {win, width, height, title};
  return result;
}
auto drop_window_lib() -> void { glfwTerminate(); }
auto drop_window(window &win) -> void { glfwDestroyWindow(*win); }

auto update_framesize(window &win) -> void {
  int width, height;
  glfwGetFramebufferSize(*win, &width, &height);
  glViewport(0, 0, width, height);
}

auto swap_buffers(window &win) -> void { glfwSwapBuffers(*win); }

auto get_unit_translation(window &win) -> glm::vec3 {

  if (glfwGetKey(*win, GLFW_KEY_ESCAPE)) {
    glfwSetWindowShouldClose(*win, GLFW_TRUE);
  }

  auto sum = glm::vec3(0.0f);

  auto forward = glm::vec3(0.0f, 0.0f, 1.0f);
  auto backward = -forward;
  auto right = glm::vec3(1.0f, 0.0f, 0.0f);
  auto left = -right;
  auto up = glm::vec3(0.0f, 1.0f, 0.0f);
  auto down = -up;

  if (glfwGetKey(*win, GLFW_KEY_W) == GLFW_PRESS)
    sum += forward;
  if (glfwGetKey(*win, GLFW_KEY_S) == GLFW_PRESS)
    sum += backward;
  if (glfwGetKey(*win, GLFW_KEY_D) == GLFW_PRESS)
    sum += right;
  if (glfwGetKey(*win, GLFW_KEY_A) == GLFW_PRESS)
    sum += left;
  if (glfwGetKey(*win, GLFW_KEY_SPACE) == GLFW_PRESS)
    sum += up;
  if (glfwGetKey(*win, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    sum += down;

  sum = glm::length(sum) < 0.01f ? glm::vec3(0.0f) : glm::normalize(sum);
  return sum;
}

auto get_cursor_delta(window &win) -> glm::vec2 {
  double xpos, ypos;
  glfwGetCursorPos(*win, &xpos, &ypos);
  return glm::vec2((float)xpos, (float)ypos);
}

auto poll_events() -> void { glfwPollEvents(); }
