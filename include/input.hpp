#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <input_view.hpp>

enum MouseAction {
  LEFT_CLICK,
  RIGHT_CLICK,
  MIDDLE_CLICK,
  SCROLL_UP,
  SCROLL_DOWN,
  NONE,
};

class Input {
public:
  Input(GLFWwindow* window, InputView* input_view);
  glm::vec2 getMousePosition();
  void process();

private:
  bool keys_[1024] = {false};
  glm::vec2 mouse_pos_;
  MouseAction mouse_action_ = NONE;
  InputView* input_view_ = nullptr;

  // callbacks
  void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
  void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
  void framebufferSizeCallback(GLFWwindow* window, int width, int height);
  void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
  void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

  void runKeyCommand(int key);
};
