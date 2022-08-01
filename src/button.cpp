#include <button.hpp>

Button::Button(BUTTON_LABEL label, std::string texturePath, glm::vec2 position, glm::vec2 size) :
  Graphic(texturePath, position, size, true, GRAPHIC) {
  label_ = label;
}

void Button::toggleVisibility() {
  visible_ = !visible_;
  disabled_ = !disabled_;
}

bool Button::isDisabled() {
  return disabled_;
}

BUTTON_LABEL Button::getLabel() {
  return label_;
}
