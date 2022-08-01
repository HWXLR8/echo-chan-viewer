#include <iostream>

#include <glprg.hpp>

int main(int argc, char** argv) {
  GLPRG* glprg = nullptr;
  try {
    glprg = new GLPRG();
    glprg->run();
  } catch (const std::exception& e) {
    std::cerr << "\033[1;31mERROR: \033[0m" << e.what() << std::endl;
  }
  // only delete glprg if it was successfully initialized
  if (glprg != nullptr) {
    delete glprg;
  }
  return 0;
}
