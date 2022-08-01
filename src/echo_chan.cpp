#include <echo_chan.hpp>

#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include <fcntl.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>

EchoChan::EchoChan(std::string port) {
  device_ = open(port.c_str(), O_RDONLY | O_NOCTTY);

  if (device_ == -1) {
    throw std::runtime_error("Failed to connect to " + port);
  }
}

EchoChan::~EchoChan() {
  running_ = false;
  close(device_);
}

void EchoChan::run() {
  // previous states
  uint8_t prev_state_1 = -1;
  uint8_t prev_state_2 = -1;
  // rotation in gearing
  srand(time(0));
  int rgear1 = rand() % 1458;
  int rgear2 = rand() % 1458;

  running_ = true;

  while (running_) {
    extractStateFromPins();
    calculateRotation(spinner_state1_, prev_state_1, rgear1);
    calculateRotation(spinner_state2_, prev_state_2, rgear2);
    r1_ = rgear1 * 360.0 / 1458.0;
    r2_ = rgear2 * 360.0 / 1458.0;
  }
  std::cout << "THREAD DONE" << std::endl;
}

double EchoChan::getSpinnerRotation(int snum) {
  if (snum == 1) {
    return r1_;
  }
  return r2_;
}

void EchoChan::calculateRotation(uint8_t& state, uint8_t& prev_state, int& rgear) {
  if (state != prev_state) {
    if ((prev_state == 2 && state == 1) || // CW
	(prev_state == 1 && state == 0) ||
	(prev_state == 0 && state == 2)) {
      rgear = (rgear + 1) % 1458;
    } else if ((prev_state == 2 && state == 0) || // CCW
	       (prev_state == 0 && state == 1) ||
	       (prev_state == 1 && state == 2)) {
      rgear--;
      if (rgear == -1) {
	rgear = 1457;
      }
    }
    prev_state = state;
  }
}

void EchoChan::extractStateFromPins() {
  char buf[32];
  int n = read(device_, buf, sizeof(buf));

  // button state
  buttons_[UP1] = buf[0] - '0';
  buttons_[DOWN1] = buf[1] - '0';
  buttons_[LEFT1] = buf[2] - '0';
  buttons_[RIGHT1] = buf[3] - '0';
  buttons_[A1] = buf[4] - '0';
  buttons_[B1] = buf[5] - '0';
  buttons_[C1] = buf[6] - '0';
  buttons_[D1] = buf[7] - '0';
  buttons_[S1] = buf[16] - '0';

  buttons_[UP2] = buf[8] - '0';
  buttons_[DOWN2] = buf[9] - '0';
  buttons_[LEFT2] = buf[10] - '0';
  buttons_[RIGHT2] = buf[11] - '0';
  buttons_[A2] = buf[12] - '0';
  buttons_[B2] = buf[13] - '0';
  buttons_[C2] = buf[14] - '0';
  buttons_[D2] = buf[15] - '0';
  buttons_[S2] = buf[17] - '0';

  // spinner state
  spinner_state1_ = 0x0000;
  spinner_state2_ = 0x0000;
  // spinner 1
  spinner_state1_ |= buf[2] - '0';
  spinner_state1_ <<= 1;
  spinner_state1_ |= buf[3] - '0';
  if (spinner_state1_ == 3) {
    spinner_state1_ = 1;
  }
  // spinner 2
  spinner_state2_ |= buf[10] - '0';
  spinner_state2_ <<= 1;
  spinner_state2_ |= buf[11] - '0';
  if (spinner_state2_ == 3) {
    spinner_state2_ = 1;
  }
}

bool EchoChan::isButtonPressed(BUTTON_LABEL b) {
  if (buttons_.count(b) == 0) {
    return false;
  } else {
    return buttons_[b];
  }
}
