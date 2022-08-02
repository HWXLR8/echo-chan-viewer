#pragma once

#include <string>

#include <echo_chan.hpp>
#include <graphic.hpp>

class Button : public Graphic{
public:
  Button(EchoChan* echo_chan, BUTTON_LABEL label, std::string texturePath, glm::vec2 position, glm::vec2 size);
  void toggleVisibility();
  bool isDisabled();
  BUTTON_LABEL getLabel();

private:
  EchoChan* echo_chan_ = nullptr;
  BUTTON_LABEL label_;
  bool disabled_ = false;
};
