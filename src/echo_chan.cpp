#include <echo_chan.hpp>

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>

#include <fcntl.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>

EchoChan::EchoChan() {
  scanForChan();

  // set a large buffer too, just in case
  if (SetupComm(serial_, 32000, 32000) == 0) {
    throw std::runtime_error("Failed to configure buffer sizes");
  }
}

EchoChan::~EchoChan() {
  running_ = false;
  EscapeCommFunction(serial_, CLRDTR);
  CloseHandle(serial_);
}

void EchoChan::run() {
  running_ = true;
  while (running_) {
    extractStateFromPins();
  }
}

void EchoChan::scanForChan() {
  // in case the DTR pin was left high by another program
  EscapeCommFunction(serial_, CLRDTR);

  while (1) {
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
        if (byte[0] == '9') {
          return;
        }

        // bad handshake
        EscapeCommFunction(serial_, CLRDTR);
        CloseHandle(serial_);
      }

      if (i == 255) {
        std::cout << "Unable to find Echo Chan device" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
      }
    }
  }
}

double EchoChan::getSpinnerRotation(int snum) {
  if (snum == 1) {
    return r1_;
  }
  return r2_;
}

void EchoChan::extractStateFromPins() {
  char buf[11] = { 0 };
  char byte[1] = { 0 };
  uint32_t n_bytes_read = 0;
  int i = 0;

  // fetch 11 *or more* bytes, terminating with \n
  while (i < 11 || byte[0] != '\n') {

    // attempt to read a single byte repeatedly
    while (1) {
      n_bytes_read = 0;
      if (!ReadFile(serial_, &byte, 1, &n_bytes_read, NULL)) {
        std::cout << "Device stopped responding" << std::endl;
        EscapeCommFunction(serial_, CLRDTR); // our handshake is invalidated so clear the DTR bit
        scanForChan();
      } else if (n_bytes_read > 0) { // watch out, if the ReadFile timed out it returns 1 but with 0 bytes read
        break;
      }
      continue;
    }

    // add the byte to the buffer as long as it doesn't overflow
    if (i < 11) {
        buf[i] = byte[0];
    }

    ++i;
  }

  // if more than 11 bytes were read, reject the buffer
  if (i != 11) {
    std::cout << "Invalid byte line length: " << i << " detected" << std::endl;
    return;
  }
  // otherwise process the buffer

  // player 1 buttons
  buttons_[UP1] = buf[1] & 0x01;
  buttons_[DOWN1] = buf[1] & 0x02;
  buttons_[LEFT1] = buf[1] & 0x04;
  buttons_[RIGHT1] = buf[1] & 0x08;
  buttons_[A1] = buf[0] & 0x01;
  buttons_[B1] = buf[0] & 0x02;
  buttons_[C1] = buf[0] & 0x04;
  buttons_[D1] = buf[0] & 0x08;
  buttons_[S1] = buf[2] & 0x01;

  // player 2 buttons
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
}

bool EchoChan::isButtonPressed(BUTTON_LABEL b) {
  if (buttons_.count(b) == 0) {
    return false;
  } else {
    return buttons_[b];
  }
}
