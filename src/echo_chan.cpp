#include <echo_chan.hpp>

#include <bitset>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>

#include <fcntl.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>

EchoChan::EchoChan() {
  // scan Windows COM ports for device
  for (int i = 0; i < 256; ++i) {
    std::string port = "COM" + std::to_string(i);
    char byte[1] = { 0 };

    // attempt to open the device
    serial_ = CreateFile(port.c_str(), GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if (serial_ != INVALID_HANDLE_VALUE) {
      std::cout << "Attempting handshake with " << port << std::endl;

      // set speedy timeouts to scan quickly
      COMMTIMEOUTS timeout = { 0 };
      timeout.ReadIntervalTimeout = 10;
      timeout.ReadTotalTimeoutConstant = 10;
      timeout.ReadTotalTimeoutMultiplier = 1;
      timeout.WriteTotalTimeoutConstant = 10;
      timeout.WriteTotalTimeoutMultiplier = 1;
      SetCommTimeouts(serial_, &timeout);

      // send Data Terminal Ready (DTR) signal and attempt a handshake
      EscapeCommFunction(serial_, SETDTR);
      byte[0] = '6';
      WriteFile(serial_, &byte, 1, 0, NULL);
      ReadFile(serial_, &byte, 1, 0, NULL);
      EscapeCommFunction(serial_, CLRDTR);
      if (byte[0] == '9') {
        break;
      }

      // bad handshake
      CloseHandle(serial_);
    }

    if (i == 255) {
      throw std::runtime_error("Unable to find Echo Chan device");
    }
  }

  // set a large buffer too, just in case
  if (SetupComm(serial_, 32000, 32000) == 0) {
    throw std::runtime_error("Failed to configure buffer sizes");
  }
}

EchoChan::~EchoChan() {
  running_ = false;
  CloseHandle(serial_);
}

void EchoChan::run() {
  running_ = true;
  while (running_) {
    extractStateFromPins();
  }
}

double EchoChan::getSpinnerRotation(int snum) {
  if (snum == 1) {
    return r1_;
  }
  return r2_;
}

void EchoChan::extractStateFromPins() {
  char buf[11] = {0};
  char byte[1] = {0};
  int i = 0;

  while (i < 11 || byte[0] != '\n') { // >= 11 bytes, terminating with \n
    if (!ReadFile(serial_, &byte, 1, 0, NULL)) {
      std::cout << "Device stopped responding" << std::endl;
    } else if (i < 11) { // don't write past the buffer
      buf[i] = byte[0];
    }
    ++i;
  }

  if (i != 11) {
    std::cout << "Invalid byte line length: " << i << " detected" << std::endl;
    return;
  }

  // // debug display of all bytes
  // std::cout << "===\n";
  // std::bitset<8> b(buf[0]);
  // std::cout << "buttons: " << b << '\n';
  // std::bitset<8> j(buf[1]);
  // std::cout << "joysticks: " << j << '\n';
  // std::bitset<8> m(buf[2]);
  // std::cout << "misc: " << m << '\n';
  // std::bitset<8> k(buf[3]);
  // std::cout << "kick: " << k << '\n';
  // std::cout << "spinner 1 state: " << (((unsigned char)buf[4] << 2*CHAR_BIT) | ((unsigned char)buf[5] << CHAR_BIT) | ((unsigned char)buf[6])) << " / 1458\n";
  // std::cout << "spinner 2 state: " << (((unsigned char)buf[7] << 2*CHAR_BIT) | ((unsigned char)buf[8] << CHAR_BIT) | ((unsigned char)buf[9])) << " / 1458\n";

  // button states
  buttons_[UP1] = buf[1] & 0x01;
  buttons_[DOWN1] = buf[1] & 0x02;
  buttons_[LEFT1] = buf[1] & 0x04;
  buttons_[RIGHT1] = buf[1] & 0x08;
  buttons_[A1] = buf[0] & 0x01;
  buttons_[B1] = buf[0] & 0x02;
  buttons_[C1] = buf[0] & 0x04;
  buttons_[D1] = buf[0] & 0x08;
  buttons_[S1] = buf[2] & 0x01;

  buttons_[UP2] = buf[1] & 0x10;
  buttons_[DOWN2] = buf[1] & 0x20;
  buttons_[LEFT2] = buf[1] & 0x40;
  buttons_[RIGHT2] = buf[1] & 0x80;
  buttons_[A2] = buf[0] & 0x10;
  buttons_[B2] = buf[0] & 0x20;
  buttons_[C2] = buf[0] & 0x40;
  buttons_[D2] = buf[0] & 0x80;
  buttons_[S2] = buf[2] & 0x02;

  // spinner states
  int rgear1 = (((unsigned char)buf[4] << 2*CHAR_BIT) | ((unsigned char)buf[5] << CHAR_BIT) | ((unsigned char)buf[6]));
  int rgear2 = (((unsigned char)buf[7] << 2*CHAR_BIT) | ((unsigned char)buf[8] << CHAR_BIT) | ((unsigned char)buf[9]));
  r1_ = rgear1 * 360.0 / 1458.0;
  r2_ = rgear2 * 360.0 / 1458.0;

  fresh_ = true;
}

bool EchoChan::isButtonPressed(BUTTON_LABEL b) {
  if (buttons_.count(b) == 0) {
    return false;
  } else {
    return buttons_[b];
  }
}

bool EchoChan::getStateFresh() {
  return fresh_;
}

void EchoChan::setStateFresh(bool fresh) {
  fresh_ = fresh;
}