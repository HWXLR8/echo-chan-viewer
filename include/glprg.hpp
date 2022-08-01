#pragma once

#include <GLFW/glfw3.h>

#include <thread>

#include <echo_chan.hpp>
#include <graphic.hpp>
#include <input.hpp>
#include <input_view.hpp>
#include <text.hpp>


class GLPRG {
 public:
  GLPRG();
  ~GLPRG();
  void run();

private:
  GLFWwindow* window_;
  Input* input_;
  EchoChan* echo_chan_ = nullptr;
  InputView* input_view_ = nullptr;
  std::thread tchan_;

  glm::vec2 mouse_pos_;

  // opengl
  void initGLFW();
  void initGLAD();
  void initGL();

  void update(double dt);
  void render();
  void framebufferSizeCallback(GLFWwindow* window, int width, int height);
  void windowSizeCallback(GLFWwindow* window, int width, int height);
  void recalculateProjectionMatrix(glm::vec2 new_screen_size);
};
