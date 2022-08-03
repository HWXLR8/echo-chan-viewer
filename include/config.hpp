#pragma once

#include <vector>
#include <string>

#include <glm/glm.hpp>

class Config {
public:
  static const char* WINDOW_TITLE;
  static const float FRAME_LENGTH;
  static glm::vec2 SCALE;
  static glm::vec2 SCREEN_SIZE;

  // stick/spinner/button sizes
  static glm::vec2 SPINNER_SIZE;
  static glm::vec2 BUTTON_SIZE;
  static glm::vec2 BUTTON_SIZE_START;
  static glm::vec2 GATE_SIZE;
  static glm::vec2 STICK_SIZE;

  // stick/spinner positions
  static glm::vec2 STICK_POS_P1;
  static glm::vec2 STICK_POS_P2;
  static glm::vec2 SPINNER_POS_P1;
  static glm::vec2 SPINNER_POS_P2;

  // buttons group positions
  static glm::vec2 BUTTON_POS_P1;
  static glm::vec2 BUTTON_POS_P2;

  // p1 button offsets
  static glm::vec2 P1_AB;
  static glm::vec2 P1_AC;
  static glm::vec2 P1_AD;
  static glm::vec2 P1_AE;
  static glm::vec2 P1_AF;
  static glm::vec2 P1_S;

  // p2 button offsets
  static glm::vec2 P2_AB;
  static glm::vec2 P2_AC;
  static glm::vec2 P2_AD;
  static glm::vec2 P2_AE;
  static glm::vec2 P2_AF;
  static glm::vec2 P2_S;

  static void calculateNewScale(glm::vec2 new_screen_size);
  static glm::vec2 getScale();
};
