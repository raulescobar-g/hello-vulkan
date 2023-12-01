#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

struct translation {
  glm::vec3 position;
};
struct scale {
  glm::vec3 scale;
};
struct rotation {
  glm::quat quaterion;
};
