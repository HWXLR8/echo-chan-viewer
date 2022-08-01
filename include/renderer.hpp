#pragma once

#include <glm/glm.hpp>

#include <shader.hpp>
#include <texture.hpp>

enum RENDER_TYPE {
  GRAPHIC,
  TEXT,
};

class Renderer {
 public:
  Renderer(Shader& shader);
  ~Renderer();

  void setShader(Shader& shader);
  void blit(Texture2D &texture, glm::vec2 position, glm::vec2 size, glm::vec3 rotation, glm::vec4 color, RENDER_TYPE render_type);
  void setAsciiOffset(float offset);

 private:
  Shader shader_;

  // VAO/VBO for standard graphic objects
  unsigned int VAO_graphic_;
  unsigned int VBO_graphic_;

  // VAO/VBO for text objects
  unsigned int VAO_text_;
  unsigned int VBO_text_;

  float graphic_verts_[24] = {
    // pos      // tex
    0.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f,

    0.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 0.0f
  };

  float text_verts_[24] = {
    // pos      // tex
    0.0f, 1.0f, 0, 1.0f,
    1.0f, 0.0f, 0 + (1.0f/128.0f), 0.0f,
    0.0f, 0.0f, 0, 0.0f,
    0.0f, 1.0f, 0, 1.0f,
    1.0f, 1.0f, 0 + (1.0f/128.0f), 1.0f,
    1.0f, 0.0f, 0 + (1.0f/128.0f), 0.0f
  };

  bool is_font_texture_bound_ = false;

  void initRenderData();
};
