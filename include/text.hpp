#pragma once

#include <string>
#include <vector>

#include <glm/glm.hpp>

#include <ascii_graphic.hpp>
#include <graphic.hpp>

class Text {
public:
  Text(std::string text, glm::vec2 position, glm::vec2 letter_size, glm::vec2 spacing);
  void render(Renderer* renderer);
  void setPosition(glm::vec2 position);
  glm::vec2 getPosition();
  void update(double dt);
  // this method cannot change the length of the text string, only the
  // individual characters that already exist
  void setText(std::string text);
  std::string getText();
  void setColor(glm::vec4 color);
  void setVisibility(bool b);

private:
  std::string text_;
  std::vector<ASCIIGraphic*> letters_;
  glm::vec2 position_;
  glm::vec2 letter_size_;
  glm::vec2 scale_;

  void scale();
};
