#include <button.hpp>

Button::Button(EchoChan* echo_chan, BUTTON_LABEL label, std::string texturePath, glm::vec2 position, glm::vec2 size) :
  Graphic(texturePath, position, size, true, GRAPHIC) {
  echo_chan_ = echo_chan;
  label_ = label;
}

void Button::toggleVisibility() {
  echo_chan_->setStateFresh(true);
  visible_ = !visible_;
  disabled_ = !disabled_;
}

bool Button::isDisabled() {
  return disabled_;
}

BUTTON_LABEL Button::getLabel() {
  return label_;
}
