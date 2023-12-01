#include <glm/glm.hpp>
#include <vector>

using uint = unsigned int;

struct mesh_data {
  mesh_data(std::vector<glm::vec3> pos, std::vector<glm::vec3> norm,
            std::vector<glm::vec2> tex)
      : pos(pos), norm(norm), tex(tex){};
  std::vector<glm::vec3> pos, norm;
  std::vector<glm::vec2> tex;
};

struct mesh_handle {
  uint vao, vertex_count;
};

auto draw(const mesh_handle &) -> void;
auto load_obj(const std::string &filename) -> mesh_data;
auto from_mesh_data(const mesh_data &data) -> mesh_handle;
