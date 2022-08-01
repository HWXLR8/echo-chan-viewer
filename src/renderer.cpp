#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include <renderer.hpp>

Renderer::Renderer(Shader &shader) {
  shader_ = shader;
  initRenderData();
}

Renderer::~Renderer() {
  glDeleteVertexArrays(1, &VAO_graphic_);
  glDeleteVertexArrays(1, &VAO_text_);
}

void Renderer::setShader(Shader& shader) {
  shader_ = shader;
}

void Renderer::blit(Texture2D &texture, glm::vec2 position, glm::vec2 size, glm::vec3 rotation, glm::vec4 color, RENDER_TYPE render_type) {
  shader_.use();
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(position, 0.0f));

  model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
  model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
  model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
  model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
  model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

  model = glm::scale(model, glm::vec3(size, 1.0f));

  shader_.setMatrix4("model", model);
  shader_.setVector4f("spriteColor", color);
  shader_.setFloat("time", glfwGetTime());

  glActiveTexture(GL_TEXTURE0);

  if (render_type == GRAPHIC) {
    texture.Bind();
    is_font_texture_bound_ = false;
    glBindVertexArray(VAO_graphic_);
    shader_.setFloat("ascii_offset", 0.0);
  } else if (render_type == TEXT) {
    // only bind the font texture once!
    if (!is_font_texture_bound_) {
      texture.Bind();
      is_font_texture_bound_ = true;
    }
    glBindVertexArray(VAO_text_);
  }
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);
}

void Renderer::initRenderData() {
  // graphic VAO/VBO
  glGenVertexArrays(1, &VAO_graphic_);
  glBindVertexArray(VAO_graphic_);
  glGenBuffers(1, &VBO_graphic_);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_graphic_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(graphic_verts_), graphic_verts_, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // text VBO
  glGenVertexArrays(1, &VAO_text_);
  glBindVertexArray(VAO_text_);
  glGenBuffers(1, &VBO_text_);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_text_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(text_verts_), text_verts_, GL_DYNAMIC_DRAW);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void Renderer::setAsciiOffset(float offset) {
  shader_.setFloat("ascii_offset", offset);
}
