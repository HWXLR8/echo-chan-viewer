#pragma once

#include <vector>

#include <button.hpp>
#include <echo_chan.hpp>
#include <graphic.hpp>
#include <joystick.hpp>
#include <renderer.hpp>

class InputView {
public:
  InputView(EchoChan* echo_chan);
  void update();
  void render(Renderer* renderer);
  void nextStickType(int direction);
  void toggleActiveButtonVisibility(glm::vec2 mouse_pos);

private:
  EchoChan* echo_chan_ = nullptr;
  Joystick* joystick_ = nullptr;
  std::vector<Button*> buttons_off_p1_;
  std::vector<Button*> buttons_on_p1_;
  std::vector<Button*> buttons_off_p2_;
  std::vector<Button*> buttons_on_p2_;
  std::vector<Button*> dummy_buttons_;
};
