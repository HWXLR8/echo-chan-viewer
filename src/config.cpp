#include <config.hpp>

const char* Config::WINDOW_TITLE = "ECHO CHAN INPUT VIEWER";
const float Config::FRAME_LENGTH = 1.0f/60.0f;
glm::vec2 Config::SCALE = {1.0, 1.0};
glm::vec2 Config::SCREEN_SIZE = {910, 220};

// stick/spinner/button sizes
glm::vec2 Config::SPINNER_SIZE = {150, 150};
glm::vec2 Config::BUTTON_SIZE = {60, 60};
glm::vec2 Config::BUTTON_SIZE_START = {50, 50};
glm::vec2 Config::GATE_SIZE = {150, 150};
glm::vec2 Config::STICK_SIZE = {150, 150};

// stick/spinner positions
glm::vec2 Config::STICK_POS_P1 = {20, 35};
glm::vec2 Config::SPINNER_POS_P1 = {20, 35};
glm::vec2 Config::STICK_POS_P2 = {520, 35};
glm::vec2 Config::SPINNER_POS_P2 = {520, 35};

// buttons group positions
glm::vec2 Config::BUTTON_POS_P1 = {185, 50};
glm::vec2 Config::BUTTON_POS_P2 = {675, 50};

// p1 button offsets
glm::vec2 Config::P1_AB = {65, -15};
glm::vec2 Config::P1_AC = {130, -5};
glm::vec2 Config::P1_AD = {-10, 65};
glm::vec2 Config::P1_AE = {55, 50};
glm::vec2 Config::P1_AF = {120, 60};
glm::vec2 Config::P1_S = {SPINNER_POS_P1.x + 355, 0.5 * (SPINNER_SIZE.y - BUTTON_SIZE_START.y)};

// p2 button offsets
glm::vec2 Config::P2_AB = {65, -20};
glm::vec2 Config::P2_AC = {130, -20};
glm::vec2 Config::P2_AD = {0, 65};
glm::vec2 Config::P2_AE = {65, 45};
glm::vec2 Config::P2_AF = {130, 45};
glm::vec2 Config::P2_S = {SPINNER_POS_P2.x - 580, 0.5 * (SPINNER_SIZE.y - BUTTON_SIZE_START.y)};

void Config::calculateNewScale(glm::vec2 new_screen_size) {
  SCALE = new_screen_size / SCREEN_SIZE;

  SCREEN_SIZE = new_screen_size;
}

glm::vec2 Config::getScale() {
  return SCALE;
}
