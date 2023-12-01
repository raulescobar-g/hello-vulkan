#include "material.h"

auto new_material() -> material {
    material mat = {
        glm::vec3(0.5),
        glm::vec3(0.5),
        glm::vec3(0.5),
        0.5
    };
    return mat;
}
