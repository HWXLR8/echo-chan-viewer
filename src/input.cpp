#include <input.hpp>

#include <algorithm>

Input::Input(GLFWwindow* window, InputView* input_view) {
  input_view_ = input_view;

  // hack GLFW to accept a c++ member function as a callback
  glfwSetWindowUserPointer(window, this);
  auto keyCallbackAlias = [](GLFWwindow* window, int key, int scancode, int action, int mode) {
    static_cast<Input*>(glfwGetWindowUserPointer(window))->keyCallback(window, key, scancode, action, mode);
  };
  auto cursorPositionCallbackAlias = [](GLFWwindow* window, double xpos, double ypos) {
    static_cast<Input*>(glfwGetWindowUserPointer(window))->cursorPositionCallback(window, xpos, ypos);
  };
  auto scrollCallbackAlias = [](GLFWwindow* window, double xoffset, double yoffset) {
    static_cast<Input*>(glfwGetWindowUserPointer(window))->scrollCallback(window, xoffset, yoffset);
  };
  auto mouseButtonCallbackAlias = [](GLFWwindow* window, int button, int action, int mods) {
    static_cast<Input*>(glfwGetWindowUserPointer(window))->mouseButtonCallback(window, button, action, mods);
  };
  glfwSetKeyCallback(window, keyCallbackAlias);
  glfwSetCursorPosCallback(window, cursorPositionCallbackAlias);
  glfwSetScrollCallback(window, scrollCallbackAlias);
  glfwSetMouseButtonCallback(window, mouseButtonCallbackAlias);
}

void Input::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }

  if (key >= 0 && key < 1024) {
    if (action == GLFW_PRESS) {
	keys_[key] = true;
    } else if (action == GLFW_RELEASE) {
      keys_[key] = false;
    }
  }
}

void Input::cursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
  mouse_pos_.x = xpos;
  mouse_pos_.y = ypos;
}

void Input::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
  if (yoffset == -1) {
    mouse_action_ = SCROLL_DOWN;
  } else if (yoffset == 1) {
    mouse_action_ = SCROLL_UP;
  }
}

void Input::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
    mouse_action_ = LEFT_CLICK;
  }
}

glm::vec2 Input::getMousePosition() {
  return mouse_pos_;
}

void Input::process() {
  std::vector<int> functional_keys = {
    GLFW_KEY_SPACE,
    GLFW_KEY_R,
  };
  // key actions
  for (auto key : functional_keys) {
    if (keys_[key]) {
      runKeyCommand(key);
      keys_[key] = false;
    }
  }

  // mouse actions
  if (mouse_action_ == SCROLL_DOWN) {
    input_view_->nextStickType(1);
  } else if (mouse_action_ == SCROLL_UP) {
    input_view_->nextStickType(-1);
  } else if (mouse_action_ == LEFT_CLICK) {
    input_view_->toggleActiveButtonVisibility(mouse_pos_);
  }
  mouse_action_ = NONE;
}

void Input::runKeyCommand(int key) {
  switch (key) {
  case GLFW_KEY_SPACE:
    break;

  case GLFW_KEY_R:
    break;
  }
}
