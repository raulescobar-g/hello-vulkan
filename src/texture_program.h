#include <string>
#include <glm/glm.hpp>

using uint = unsigned int;

struct texture_program {
	uint pid;
	int P, MV, iMV, texture0;
};

auto new_texture_program(const std::string&, const std::string&) -> texture_program;
auto drop_texture_program(texture_program&) -> void;

auto bind(const texture_program&) -> void;
auto unbind(const texture_program&) -> void;
