#include "camera.h"
#include "glm/ext/matrix_transform.hpp"
#include "matrix_stack.h"

auto rotate(camera &camera, glm::vec2 rotation) -> void {
  if (!camera.prev_mouse.has_value()) {
    camera.prev_mouse = std::make_optional(rotation);
    return;
  }

  glm::vec2 dm = (rotation - camera.prev_mouse.value()) * camera.sensitivity;

  camera.yaw -= dm.x;
  camera.pitch = std::clamp(camera.pitch - dm.y, -glm::pi<float>() / 3.0f,
                            glm::pi<float>() / 3.0f);

  camera.prev_mouse = rotation;
}

auto translate(camera &camera, glm::vec3 translation) -> void {
  glm::quat yaw_quaternion =
      glm::angleAxis(camera.yaw, glm::vec3(0.0f, 1.0f, 0.0f));
  glm::quat pitch_quaternion =
      glm::angleAxis(camera.pitch, glm::vec3(1.0f, 0.0f, 0.0f));
  glm::quat combined_rotation = yaw_quaternion * pitch_quaternion;

  translation *= glm::vec3(-1.0f, 1.0f, 1.0f);

  glm::vec3 local_translation = combined_rotation * translation;

  camera.position += local_translation * camera.movement_speed;
}

auto perspective(const camera &camera) -> glm::mat4 {
  return glm::perspective(glm::pi<float>() / 4.0f, 1.0f, 0.0001f, 1000.0f);
}
auto view(const camera &camera) -> glm::mat4 {
  return glm::lookAt(
      camera.position,
      camera.position +
          glm::normalize(glm::vec3(sin(camera.yaw) * cos(camera.pitch),
                                   sin(camera.pitch),
                                   cos(camera.yaw) * cos(camera.pitch))),
      glm::vec3(0.0f, 1.0f, 0.0f));
}
