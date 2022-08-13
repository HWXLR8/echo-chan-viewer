#pragma once

#include <map>
#include <string>

#include <windows.h>

enum BUTTON_LABEL {
  UP1, DOWN1, LEFT1, RIGHT1, A1, B1, C1, D1, E1, F1, S1,
  UP2, DOWN2, LEFT2, RIGHT2, A2, B2, C2, D2, E2, F2, S2,
};

class EchoChan {
public:
  EchoChan();
  ~EchoChan();
  void scanForChan();
  void run();
  void extractStateFromPins();
  bool isButtonPressed(BUTTON_LABEL b);

private:
  HANDLE serial_;
  // device state
  std::map<BUTTON_LABEL, bool> buttons_;
  // spinner rotation in degrees
  double r1_;
  double r2_;
  bool running_ = false;
};
