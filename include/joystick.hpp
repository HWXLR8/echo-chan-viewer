#pragma once

#include <vector>

#include <echo_chan.hpp>
#include <graphic.hpp>
#include <renderer.hpp>

// enum STICK_TYPE {
//   SQUARE,
//   DIAMOND,
//   OCTOGON,
//   CIRCLE,
//   TWO_WAY,
//   SPINNER,
// };

enum STICK_MODE {
  JOYSTICK,
  SPINNER,
};

class Joystick {
public:
  Joystick(EchoChan* echo_chan);
  void update();
  void render(Renderer* renderer);
  void nextStickType(int direction);
  void updateStick();
  void updateSpinner();

private:
  EchoChan* echo_chan_ = nullptr;
  Graphic* spinner2_ = nullptr;
  STICK_MODE stick_mode_ = JOYSTICK;
  std::vector<Graphic*> stick_positions_p1_;
  std::vector<Graphic*> stick_positions_p2_;
  std::vector<Graphic*> stick_types_p1_;
  std::vector<Graphic*> stick_types_p2_;
  int current_stick_type_ = 0;
  int current_stick_position_p1_ = 0;
  int current_stick_position_p2_ = 0;
};
