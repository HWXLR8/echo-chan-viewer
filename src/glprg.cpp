#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <vector>

#include <config.hpp>
#include <glprg.hpp>
#include <helper.hpp>
#include <resource_manager.hpp>
#include <renderer.hpp>

Renderer *renderer;

GLPRG::GLPRG() {
  // window/GL
  initGLFW();
  initGLAD();
  initGL();

  // hack GLFW to accept a c++ member function as a callback
  auto framebufferSizeCallbackAlias = [](GLFWwindow* window, int width, int height) {
    static_cast<GLPRG*>(glfwGetWindowUserPointer(window))->framebufferSizeCallback(window, width, height);
  };
  auto windowSizeCallbackAlias = [](GLFWwindow* window, int width, int height) {
    static_cast<GLPRG*>(glfwGetWindowUserPointer(window))->windowSizeCallback(window, width, height);
  };
  glfwSetFramebufferSizeCallback(window_, framebufferSizeCallbackAlias);
  glfwSetWindowSizeCallback(window_, windowSizeCallbackAlias);

  // main shader
  ResourceManager::loadShader("shader/vert.glsl", "shader/frag.glsl", nullptr, "static_image");
  glm::mat4 projection = glm::ortho(0.0f, Config::SCREEN_SIZE.x, Config::SCREEN_SIZE.y, 0.0f, -100.0f, 100.0f);
  ResourceManager::getShader("static_image").use().setInteger("image", 0);
  ResourceManager::getShader("static_image").setMatrix4("projection", projection);
  Shader shader = ResourceManager::getShader("static_image");
  renderer = new Renderer(shader);

  // input
  echo_chan_ = new EchoChan();
  input_view_ = new InputView(echo_chan_);
  input_ = new Input(window_, input_view_);
  tchan_ = std::thread(&EchoChan::run, echo_chan_);
  tchan_.detach();
}

GLPRG::~GLPRG() {
  delete echo_chan_;
  ResourceManager::clear();
  glfwTerminate();
}

void GLPRG::initGLFW() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  #ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  #endif
  glfwWindowHint(GLFW_RESIZABLE, false);
  window_ = glfwCreateWindow(Config::SCREEN_SIZE.x, Config::SCREEN_SIZE.y, Config::WINDOW_TITLE, nullptr, nullptr);
  glfwMakeContextCurrent(window_);
}

void GLPRG::initGLAD() {
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    throw std::runtime_error("Failed to initialize GLAD");
    return;
  }
}

void GLPRG::initGL() {
  glViewport(0, 0, Config::SCREEN_SIZE.x, Config::SCREEN_SIZE.y);
  // glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void GLPRG::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
  glm::vec2 new_screen_size {width, height};
  recalculateProjectionMatrix(new_screen_size);
}

void GLPRG::windowSizeCallback(GLFWwindow* window, int width, int height) {
  glm::vec2 new_screen_size {width, height};
  Config::calculateNewScale(new_screen_size);
  // recalculateProjectionMatrix(new_screen_size);
}

void GLPRG::recalculateProjectionMatrix(glm::vec2 new_screen_size) {
  glm::mat4 projection = glm::ortho(0.0f, new_screen_size.x, new_screen_size.y, 0.0f, -100.0f, 100.0f);
  ResourceManager::getShader("static_image").setMatrix4("projection", projection);
}

void GLPRG::update(double dt) {
  mouse_pos_ = input_->getMousePosition();
  input_->process();
  input_view_->update();
}

void GLPRG::render() {
  input_view_->render(renderer);
}

void GLPRG::run() {
  double dt = 0.0;
  double lastFrame = glfwGetTime();
  while (!glfwWindowShouldClose(window_)) {
    double currentFrame = glfwGetTime();
    dt = currentFrame - lastFrame;
    lastFrame = currentFrame;
    glfwPollEvents();

    update(dt);

    glClear(GL_COLOR_BUFFER_BIT);
    render();
    glfwSwapBuffers(window_);
  }
}
