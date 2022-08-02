#include <joystick.hpp>

#include <config.hpp>

Joystick::Joystick(EchoChan* echo_chan) {
  echo_chan_ = echo_chan;

  // p1 stick types
  stick_types_p1_.push_back(new Graphic("assets/guide_square.png", Config::STICK_POS_P1, Config::GATE_SIZE, true, GRAPHIC));
  stick_types_p1_.push_back(new Graphic("assets/guide_diamond.png", Config::STICK_POS_P1, Config::GATE_SIZE, true, GRAPHIC));
  stick_types_p1_.push_back(new Graphic("assets/guide_octo.png", Config::STICK_POS_P1, Config::GATE_SIZE, true, GRAPHIC));
  stick_types_p1_.push_back(new Graphic("assets/guide_circle.png", Config::STICK_POS_P1, Config::GATE_SIZE, true, GRAPHIC));
  stick_types_p1_.push_back(new Graphic("assets/guide_2way.png", Config::STICK_POS_P1, Config::GATE_SIZE, true, GRAPHIC));
  stick_types_p1_.push_back(new Graphic("assets/p1_spinner.png", Config::SPINNER_POS_P1, Config::SPINNER_SIZE, true, GRAPHIC));

  stick_positions_p1_.push_back(new Graphic("assets/p1_joy_1.png", Config::STICK_POS_P1, Config::STICK_SIZE, true, GRAPHIC));
  stick_positions_p1_.push_back(new Graphic("assets/p1_joy_2.png", Config::STICK_POS_P1, Config::STICK_SIZE, true, GRAPHIC));
  stick_positions_p1_.push_back(new Graphic("assets/p1_joy_3.png", Config::STICK_POS_P1, Config::STICK_SIZE, true, GRAPHIC));
  stick_positions_p1_.push_back(new Graphic("assets/p1_joy_4.png", Config::STICK_POS_P1, Config::STICK_SIZE, true, GRAPHIC));
  stick_positions_p1_.push_back(new Graphic("assets/p1_joy_5.png", Config::STICK_POS_P1, Config::STICK_SIZE, true, GRAPHIC));
  stick_positions_p1_.push_back(new Graphic("assets/p1_joy_6.png", Config::STICK_POS_P1, Config::STICK_SIZE, true, GRAPHIC));
  stick_positions_p1_.push_back(new Graphic("assets/p1_joy_7.png", Config::STICK_POS_P1, Config::STICK_SIZE, true, GRAPHIC));
  stick_positions_p1_.push_back(new Graphic("assets/p1_joy_8.png", Config::STICK_POS_P1, Config::STICK_SIZE, true, GRAPHIC));
  stick_positions_p1_.push_back(new Graphic("assets/p1_joy_9.png", Config::STICK_POS_P1, Config::STICK_SIZE, true, GRAPHIC));

  // p2 stick types
  stick_types_p2_.push_back(new Graphic("assets/guide_square.png", Config::STICK_POS_P2, Config::GATE_SIZE, true, GRAPHIC));
  stick_types_p2_.push_back(new Graphic("assets/guide_diamond.png", Config::STICK_POS_P2, Config::GATE_SIZE, true, GRAPHIC));
  stick_types_p2_.push_back(new Graphic("assets/guide_octo.png", Config::STICK_POS_P2, Config::GATE_SIZE, true, GRAPHIC));
  stick_types_p2_.push_back(new Graphic("assets/guide_circle.png", Config::STICK_POS_P2, Config::GATE_SIZE, true, GRAPHIC));
  stick_types_p2_.push_back(new Graphic("assets/guide_2way.png", Config::STICK_POS_P2, Config::GATE_SIZE, true, GRAPHIC));
  stick_types_p2_.push_back(new Graphic("assets/p2_spinner.png", Config::SPINNER_POS_P2, Config::SPINNER_SIZE, true, GRAPHIC));

  stick_positions_p2_.push_back(new Graphic("assets/p2_joy_1.png", Config::STICK_POS_P2, Config::STICK_SIZE, true, GRAPHIC));
  stick_positions_p2_.push_back(new Graphic("assets/p2_joy_2.png", Config::STICK_POS_P2, Config::STICK_SIZE, true, GRAPHIC));
  stick_positions_p2_.push_back(new Graphic("assets/p2_joy_3.png", Config::STICK_POS_P2, Config::STICK_SIZE, true, GRAPHIC));
  stick_positions_p2_.push_back(new Graphic("assets/p2_joy_4.png", Config::STICK_POS_P2, Config::STICK_SIZE, true, GRAPHIC));
  stick_positions_p2_.push_back(new Graphic("assets/p2_joy_5.png", Config::STICK_POS_P2, Config::STICK_SIZE, true, GRAPHIC));
  stick_positions_p2_.push_back(new Graphic("assets/p2_joy_6.png", Config::STICK_POS_P2, Config::STICK_SIZE, true, GRAPHIC));
  stick_positions_p2_.push_back(new Graphic("assets/p2_joy_7.png", Config::STICK_POS_P2, Config::STICK_SIZE, true, GRAPHIC));
  stick_positions_p2_.push_back(new Graphic("assets/p2_joy_8.png", Config::STICK_POS_P2, Config::STICK_SIZE, true, GRAPHIC));
  stick_positions_p2_.push_back(new Graphic("assets/p2_joy_9.png", Config::STICK_POS_P2, Config::STICK_SIZE, true, GRAPHIC));
}

void Joystick::update() {
  if (stick_mode_ == JOYSTICK) {
    updateStick();
  } else if (stick_mode_ == SPINNER) {
    updateSpinner();
  }
}

void Joystick::updateSpinner() {
  stick_types_p1_[5]->setRotation(glm::vec3{0, 0, echo_chan_->getSpinnerRotation(1)});
  stick_types_p2_[5]->setRotation(glm::vec3{0, 0, echo_chan_->getSpinnerRotation(2)});
}

void Joystick::updateStick() {
  // p1
  // up combos
  if (echo_chan_->isButtonPressed(UP1)) {
    if (echo_chan_->isButtonPressed(RIGHT1)) {
      current_stick_position_p1_ = 8;
    } else if (echo_chan_->isButtonPressed(LEFT1)) {
      current_stick_position_p1_ = 6;
    } else {
      current_stick_position_p1_ = 7;
    }
  // down combos
  } else if (echo_chan_->isButtonPressed(DOWN1)) {
    if (echo_chan_->isButtonPressed(RIGHT1)) {
      current_stick_position_p1_ = 2;
    } else if (echo_chan_->isButtonPressed(LEFT1)) {
      current_stick_position_p1_ = 0;
    } else {
      current_stick_position_p1_ = 1;
    }
  // everything else
  } else if (echo_chan_->isButtonPressed(LEFT1)) {
    current_stick_position_p1_ = 3;
  } else if (echo_chan_->isButtonPressed(RIGHT1)) {
    current_stick_position_p1_ = 5;
  } else {
    current_stick_position_p1_ = 4;
  }

  // p2
  // up combos
  if (echo_chan_->isButtonPressed(UP2)) {
    if (echo_chan_->isButtonPressed(RIGHT2)) {
      current_stick_position_p2_ = 8;
    } else if (echo_chan_->isButtonPressed(LEFT2)) {
      current_stick_position_p2_ = 6;
    } else {
      current_stick_position_p2_ = 7;
    }
  // down combos
  } else if (echo_chan_->isButtonPressed(DOWN2)) {
    if (echo_chan_->isButtonPressed(RIGHT2)) {
      current_stick_position_p2_ = 2;
    } else if (echo_chan_->isButtonPressed(LEFT2)) {
      current_stick_position_p2_ = 0;
    } else {
      current_stick_position_p2_ = 1;
    }
  // everything else
  } else if (echo_chan_->isButtonPressed(LEFT2)) {
    current_stick_position_p2_ = 3;
  } else if (echo_chan_->isButtonPressed(RIGHT2)) {
    current_stick_position_p2_ = 5;
  } else {
    current_stick_position_p2_ = 4;
  }
}

void Joystick::render(Renderer* renderer) {
  stick_types_p1_[current_stick_type_]->render(renderer);
  stick_types_p2_[current_stick_type_]->render(renderer);
  if (stick_mode_ == JOYSTICK) {
    stick_positions_p1_[current_stick_position_p1_]->render(renderer);
    stick_positions_p2_[current_stick_position_p2_]->render(renderer);
  }
}

void Joystick::nextStickType(int direction) {
  echo_chan_->setStateFresh(true);
  current_stick_type_ += direction;
  if (current_stick_type_ > 5) {
    current_stick_type_ = 0;
  }
  if (current_stick_type_ < 0) {
    current_stick_type_ = 5;
  }
  if (current_stick_type_ == 5) {
    stick_mode_ = SPINNER;
  } else {
    stick_mode_ = JOYSTICK;
  }
}
