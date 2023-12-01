#define TINYOBJLOADER_IMPLEMENTATION
#define TINYOBJLOADER_USE_MAPBOX_EARCUT
#include "mesh.h"
#include "tiny_obj_loader.h"
#include <iostream>

auto load_obj(const std::string &inputfile) -> mesh_data {

  std::vector<glm::vec3> positions;
  std::vector<glm::vec3> normals;
  std::vector<glm::vec2> texture;

  tinyobj::ObjReaderConfig reader_config;
  reader_config.mtl_search_path = "./models"; // Path to material files
  reader_config.vertex_color = false;
  reader_config.triangulation_method = "earcut";

  tinyobj::ObjReader reader;

  if (!reader.ParseFromFile(inputfile, reader_config)) {
    if (!reader.Error().empty()) {
      std::cerr << "TinyObjReader: " << reader.Error();
    }
    exit(1);
  }

  if (!reader.Warning().empty()) {
    std::cout << "TinyObjReader: " << reader.Warning();
  }

  auto &attrib = reader.GetAttrib();
  auto &shapes = reader.GetShapes();
  auto &materials = reader.GetMaterials();

  // Loop over shapes
  for (size_t s = 0; s < shapes.size(); s++) {
    // Loop over faces(polygon)
    size_t index_offset = 0;
    for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
      size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

      // Loop over vertices in the face.
      for (size_t v = 0; v < fv; v++) {
        // access to vertex
        tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
        tinyobj::real_t vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
        tinyobj::real_t vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
        tinyobj::real_t vz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];

        positions.push_back(glm::vec3(vx, vy, vz));

        // Check if `normal_index` is zero or positive. negative = no normal
        // data
        if (idx.normal_index >= 0) {
          tinyobj::real_t nx = attrib.normals[3 * size_t(idx.normal_index) + 0];
          tinyobj::real_t ny = attrib.normals[3 * size_t(idx.normal_index) + 1];
          tinyobj::real_t nz = attrib.normals[3 * size_t(idx.normal_index) + 2];

          normals.push_back(glm::vec3(nx, ny, nz));
        }

        // Check if `texcoord_index` is zero or positive. negative = no texcoord
        // data
        if (idx.texcoord_index >= 0) {
          tinyobj::real_t tx =
              attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
          tinyobj::real_t ty =
              attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];

          texture.push_back(glm::vec2(tx, ty));
        }
      }
      index_offset += fv;

      // per-face material
      shapes[s].mesh.material_ids[f];
    }
  }

  return mesh_data(positions, normals, texture);
}

// convert to use std::optional
auto from_mesh_data(const mesh_data &data) -> mesh_handle {
  // Generate a VAO
  uint vertex_array_object_id = 0;
  uint position_buffer_id = 0;
  uint normal_buffer_id = 0;
  uint texture_buffer_id = 0;

  uint vertex_count = data.pos.size() * 3;

  glGenVertexArrays(1, &vertex_array_object_id);
  glBindVertexArray(vertex_array_object_id);

  // Send the position array to the GPU
  glGenBuffers(1, &position_buffer_id);
  glBindBuffer(GL_ARRAY_BUFFER, position_buffer_id);
  glBufferData(GL_ARRAY_BUFFER, data.pos.size() * sizeof(glm::vec3),
               &data.pos[0], GL_STATIC_DRAW);
  glEnableVertexAttribArray(
      0); // Assuming 0 is the layout location for positions in the shader
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (const void *)0);

  // Send the normal array to the GPU
  glGenBuffers(1, &normal_buffer_id);
  glBindBuffer(GL_ARRAY_BUFFER, normal_buffer_id);
  glBufferData(GL_ARRAY_BUFFER, data.norm.size() * sizeof(glm::vec3),
               &data.norm[0], GL_STATIC_DRAW);
  glEnableVertexAttribArray(
      1); // Assuming 1 is the layout location for normals in the shader
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (const void *)0);

  // Send the texture array to the GPU
  glGenBuffers(1, &texture_buffer_id);
  glBindBuffer(GL_ARRAY_BUFFER, texture_buffer_id);
  glBufferData(GL_ARRAY_BUFFER, data.tex.size() * sizeof(glm::vec2),
               &data.tex[0], GL_STATIC_DRAW);
  glEnableVertexAttribArray(2); // Assuming 2 is the layout location for texture
                                // coordinates in the shader
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (const void *)0);

  // Unbind the VAO
  glBindVertexArray(0);

  mesh_handle h = {vertex_array_object_id, vertex_count};
  return h;
};

auto draw(const mesh_handle &handle) -> void {
  glBindVertexArray(handle.vao);
  glDrawArrays(GL_TRIANGLES, 0, handle.vertex_count);
  glBindVertexArray(0);
}
