#include <cstdlib>
#include <entt.hpp>
#define GLM_FORCE_RADIANS
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

#include "camera.h"
#include "game_state.h"
#include "input.h"
#include "light.h"
#include "material.h"
#include "matrix_stack.h"
#include "mesh.h"
#include "phong_program.h"
#include "texture.h"
#include "texture_program.h"
#include "tick.h"
#include "transform.h"
#include "vulkan/vulkan.hpp"
#include "window.h"

auto DrawPbr(const phong_program &program, const entt::registry &registry,
             const camera &camera) -> void {
  bind(program);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glm::mat4 perspective_mat = perspective(camera);
  glm::mat4 view_mat = view(camera);

  MatrixStack p(perspective_mat);
  MatrixStack v(view_mat);
  MatrixStack m;

  auto light_view = registry.view<light, translation>();
  for (auto [entity, light, translation] : light_view.each()) {
    auto pos = v.stack.top() * glm::vec4(translation.position, 1.0f);
    glUniform3f(program.lightPos, pos.x, pos.y, pos.z);
    glUniform3f(program.lightColor, light.color.x, light.color.y,
                light.color.z);
  }

  auto component_view =
      registry.view<mesh_handle, translation, rotation, scale, material>();
  for (auto [entity, mesh_handle, translation, rotation, scale, material] :
       component_view.each()) {
    m.push();
    m *= glm::translate(glm::identity<glm::mat4>(), translation.position);
    m *= glm::scale(glm::identity<glm::mat4>(), scale.scale);
    m *= glm::toMat4(rotation.quaterion);
    auto mv = v.stack.top() * m.stack.top();
    auto imv = glm::transpose(glm::inverse(mv));
    glUniformMatrix4fv(program.P, 1, GL_FALSE, glm::value_ptr(p.stack.top()));
    glUniformMatrix4fv(program.MV, 1, GL_FALSE, glm::value_ptr(mv));
    glUniformMatrix4fv(program.iMV, 1, GL_FALSE, glm::value_ptr(imv));
    glUniform3f(program.ka, material.ka.x, material.ka.y, material.ka.z);
    glUniform3f(program.ks, material.ks.x, material.ks.y, material.ks.z);
    glUniform3f(program.kd, material.kd.x, material.kd.y, material.kd.z);
    glUniform1f(program.s, material.s);

    draw(mesh_handle);
    m.pop();
  }
  unbind(program);
}

auto Drawtextured(const texture_program &program, const texture &texture,
                  const entt::registry &registry, const camera &camera)
    -> void {
  bind(program);
  bind(texture);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glm::mat4 perspective_mat = perspective(camera);
  glm::mat4 view_mat = view(camera);

  MatrixStack p(perspective_mat);
  MatrixStack v(view_mat);
  MatrixStack m;

  auto component_view =
      registry.view<mesh_handle, translation, rotation, scale>();
  for (auto [entity, mesh_handle, translation, rotation, scale] :
       component_view.each()) {
    m.push();
    m *= glm::translate(glm::identity<glm::mat4>(), translation.position);
    m *= glm::scale(glm::identity<glm::mat4>(), scale.scale);
    m *= glm::toMat4(rotation.quaterion);
    auto mv = v.stack.top() * m.stack.top();
    auto imv = glm::transpose(glm::inverse(mv));

    glUniformMatrix4fv(program.P, 1, GL_FALSE, glm::value_ptr(p.stack.top()));
    glUniformMatrix4fv(program.MV, 1, GL_FALSE, glm::value_ptr(mv));
    glUniformMatrix4fv(program.iMV, 1, GL_FALSE, glm::value_ptr(imv));
    glUniform1i(program.texture0, texture.unit);

    draw(mesh_handle);
    m.pop();
  }
  unbind(texture);
  unbind(program);
}

auto SetupSystem(entt::registry &registry) -> void {
  init_window_lib();
  window win = new_window(1080, 1080, "fps shooter");
  auto window_entity = registry.create();
  registry.emplace<window>(window_entity, win);

  init_glew();

  auto player_entity = registry.create();
  registry.emplace<camera>(player_entity, 0.01f,
                           0.01f); // make this struct empty, and move the
                                   // members to a settings component
  registry.emplace<translation>(player_entity, glm::vec3(0.0f, 1.0f, -5.0f));
  registry.emplace<rotation>(player_entity, glm::identity<glm::quat>());
  registry.emplace<input>(player_entity, 0, glm::vec2(0.0f, 0.0f));

  phong_program phong_program =
      new_phong_program("./shaders/phong_vertex_shader.glsl",
                        "./shaders/phong_fragment_shader.glsl");
  texture_program texture_program =
      new_texture_program("./shaders/texture_vertex_shader.glsl",
                          "./shaders/texture_fragment_shader.glsl");
  texture texture = new_texture("./textures/texture.jpg", 0);

  mesh_data cube = load_obj("./models/cube.obj");
  mesh_handle cube_handle = from_mesh_data(cube);
  material cube_material = new_material();

  const auto entity = registry.create();
  registry.emplace<translation>(entity, glm::vec3(0.0f, 0.0f, 0.0f));
  registry.emplace<scale>(entity, glm::vec3(1.0f, 1.0f, 1.0f));
  registry.emplace<rotation>(entity, glm::identity<glm::quat>());
  registry.emplace<mesh_handle>(entity, cube_handle);
  registry.emplace<material>(entity, cube_material);

  const auto light_entity = registry.create();
  registry.emplace<light>(light_entity, glm::vec3(1.0f, 1.0f, 1.0f));
  registry.emplace<translation>(light_entity, glm::vec3(1.0f, 7.0f, 3.0f));

  const auto tick_entity = registry.create();
  registry.emplace<tick>(tick_entity, 0.0f, 1.0f / 60.0f);

  const auto game_state_entity = registry.create();
  registry.emplace<game_state>(game_state_entity, true);

  glfwSetTime(0.0);
}
auto CheckIfQuitSystem(const entt::registry &registry) -> bool {
  auto game_state_entity = registry.view<game_state>().front();
  auto state = registry.get<game_state>(game_state_entity);
  return !state.playing;
}

auto CaptureInputSystem(const entt::registry &registry) -> void {
  auto input_entity = registry.view<input>().front();
  auto in = registry.get<input>(input_entity);

  auto window_entity = registry.view<window>().front();
  auto win = registry.get<window>(window_entity);

  // reset inputs from last frame
  in.input = 0;
  glfwPollEvents();

  if (glfwGetKey(*win, GLFW_KEY_ESCAPE)) {
    in.input |= (int)keycodes::esc;
  }

  if (glfwGetKey(*win, GLFW_KEY_W) == GLFW_PRESS)
    in.input |= (int)keycodes::w;
  if (glfwGetKey(*win, GLFW_KEY_S) == GLFW_PRESS)
    in.input |= (int)keycodes::s;
  if (glfwGetKey(*win, GLFW_KEY_D) == GLFW_PRESS)
    in.input |= (int)keycodes::d;
  if (glfwGetKey(*win, GLFW_KEY_A) == GLFW_PRESS)
    in.input |= (int)keycodes::a;
  if (glfwGetKey(*win, GLFW_KEY_SPACE) == GLFW_PRESS)
    in.input |= (int)keycodes::space;
  if (glfwGetKey(*win, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    in.input |= (int)keycodes::shift;

  double xpos, ypos;
  glfwGetCursorPos(*win, &xpos, &ypos);
  in.prev_cursor_pos = in.cursor_pos;
  in.cursor_pos = glm::vec2((float)xpos, (float)ypos);
}

auto HandleInputSystem(const entt::registry &registry) -> void {
  auto input_entity = registry.view<input>().front();
  auto in = registry.get<input>(input_entity);

  auto game_state_entity = registry.view<game_state>().front();
  auto state = registry.get<game_state>(game_state_entity);

  if ((in.input & (uint8_t)keycodes::esc) == (uint8_t)keycodes::esc) {
    state.playing = false;
  }

  auto camera_entity = registry.view<camera, translation, rotation>().front();
  auto camera_translation = registry.get<translation>(camera_entity);

  glm::vec3 dx = glm::vec3(0.0f);
  glm::vec3 forward = glm::vec3(0.0f, 0.0f, -1.0f);
  glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);
  glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

  auto camera_rotation = registry.get<rotation>(camera_entity);

  if ((in.input & (uint8_t)keycodes::w) == (uint8_t)keycodes::w)
    dx += camera_rotation.quaterion * forward; // like this ????
  if ((in.input & (uint8_t)keycodes::s) == (uint8_t)keycodes::w)
    dx += camera_rotation.quaterion * -forward;
  if ((in.input & (uint8_t)keycodes::d) == (uint8_t)keycodes::w)
    dx += camera_rotation.quaterion * right;
  if ((in.input & (uint8_t)keycodes::a) == (uint8_t)keycodes::w)
    dx += camera_rotation.quaterion * -right;
  if ((in.input & (uint8_t)keycodes::space) == (uint8_t)keycodes::w)
    dx += camera_rotation.quaterion * up;
  if ((in.input & (uint8_t)keycodes::shift) == (uint8_t)keycodes::w)
    dx += camera_rotation.quaterion * -up;

  auto cam = registry.get<camera>(camera_entity);
  camera_translation.position += dx.length() > 0.1f
                                     ? glm::normalize(dx) * cam.movement_speed
                                     : glm::vec3(0.0f);
  // handle camera look
  glm::vec2 dm = (in.cursor_pos - in.prev_cursor_pos) * cam.sensitivity;

  // camera.yaw -= dm.x;
  // camera.pitch = std::clamp(camera.pitch - dm.y, -glm::pi<float>() / 3.0f,
  //                           glm::pi<float>() / 3.0f);
}

auto UpdateSystem(const entt::registry &registry) -> void {
} // simulate and tick
auto RenderSystem(const entt::registry &registry) -> void {
} // draw normal and textured
auto PrepareNextFrame(const entt::registry &registry) -> void {
  auto win_entity = registry.view<window>().front();
  auto win = registry.get<window>(win_entity);

  glfwSwapBuffers(*win);
}

auto CleanUpSystem(const entt::registry &registry) -> void {
  auto win_entity = registry.view<window>().front();
  auto win = registry.get<window>(win_entity);

  drop_window(win);
  drop_window_lib();
}

auto main() -> int {
  entt::registry registry;
  SetupSystem(registry);

  // maybe put in program bind code
  glClearColor(0.3f, 0.3f, 0.7f, 1.0f);
  while (CheckIfQuitSystem(registry)) {
    PrepareNextFrame(registry);
    CaptureInputSystem(registry);
    HandleInputSystem(registry);
    UpdateSystem(registry);
    RenderSystem(registry);
  }

  CleanUpSystem(registry);
  return EXIT_SUCCESS;
}
