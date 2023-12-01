#include "matrix_stack.h"
#include "glm/ext/matrix_transform.hpp"

MatrixStack::MatrixStack() {
    stack = std::stack<glm::mat4>();
    stack.push(glm::identity<glm::mat4>());
}

MatrixStack::MatrixStack(glm::mat4 r_mat) {
    stack = std::stack<glm::mat4>();
    stack.push(glm::identity<glm::mat4>());
    stack.push(r_mat);
}

auto MatrixStack::push() -> void {
    const glm::mat4& top = stack.top();
    stack.push(top);
}

auto MatrixStack::pop() -> void {
    stack.pop();
}

auto MatrixStack::operator*=(glm::mat4 m) -> void {
    auto& top = stack.top();
    top *= m;
}
