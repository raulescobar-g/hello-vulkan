#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

struct camera {
  float sensitivity;
  float movement_speed;
};

auto translate(camera &, glm::vec3) -> void;
auto rotate(camera &, glm::vec2) -> void;

auto perspective(const camera &) -> glm::mat4;
auto view(const camera &) -> glm::mat4;
