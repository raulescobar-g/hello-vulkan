#include <string>
#include <glm/glm.hpp>

using uint = unsigned int;

struct phong_program {
	uint pid;
	int P, MV, iMV, ka, kd, ks, s, lightPos, lightColor;
};

auto new_phong_program(const std::string&, const std::string&) -> phong_program;
auto drop_phong_program(phong_program&) -> void;

auto bind(const phong_program&) -> void;
auto unbind(const phong_program&) -> void;
