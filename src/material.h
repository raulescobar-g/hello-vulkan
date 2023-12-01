#include <glm/glm.hpp>

struct material {
	glm::vec3 ka, kd, ks;
	float s;
};

auto new_material() -> material;
