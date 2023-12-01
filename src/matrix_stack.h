#include "glm/glm.hpp"
#include <stack>

struct MatrixStack {
	MatrixStack();
	MatrixStack(glm::mat4 m);
	std::stack<glm::mat4> stack;

	auto push() -> void;
	auto pop() -> void;

	auto operator*=(glm::mat4) -> void;
};
