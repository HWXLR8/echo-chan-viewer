#include <input_view.hpp>

#include <config.hpp>

InputView::InputView(EchoChan* echo_chan) {
  echo_chan_ = echo_chan;
  joystick_ = new Joystick(echo_chan);

  // p1
  // buttons off
  buttons_off_p1_.push_back(new Button(echo_chan, A1, "assets/p1_button_off.png", Config::BUTTON_POS_P1, Config::BUTTON_SIZE));
  buttons_off_p1_.push_back(new Button(echo_chan, B1, "assets/p1_button_off.png", Config::BUTTON_POS_P1 + Config::P1_AB, Config::BUTTON_SIZE));
  buttons_off_p1_.push_back(new Button(echo_chan, C1, "assets/p1_button_off.png", Config::BUTTON_POS_P1 + Config::P1_AC, Config::BUTTON_SIZE));
  buttons_off_p1_.push_back(new Button(echo_chan, D1, "assets/p1_button_off.png", Config::BUTTON_POS_P1 + Config::P1_AD, Config::BUTTON_SIZE));
  buttons_off_p1_.push_back(new Button(echo_chan, S1, "assets/start_button_off.png", Config::SPINNER_POS_P1 + Config::P1_S, Config::BUTTON_SIZE_START));
  dummy_buttons_.push_back(new Button(echo_chan, E1, "assets/p1_button_off.png", Config::BUTTON_POS_P1 + Config::P1_AE, Config::BUTTON_SIZE));
  dummy_buttons_.push_back(new Button(echo_chan, F1, "assets/p1_button_off.png", Config::BUTTON_POS_P1 + Config::P1_AF, Config::BUTTON_SIZE));
  // buttons on
  buttons_on_p1_.push_back(new Button(echo_chan, A1, "assets/p1_button_on.png", Config::BUTTON_POS_P1, Config::BUTTON_SIZE));
  buttons_on_p1_.push_back(new Button(echo_chan, B1, "assets/p1_button_on.png", Config::BUTTON_POS_P1 + Config::P1_AB, Config::BUTTON_SIZE));
  buttons_on_p1_.push_back(new Button(echo_chan, C1, "assets/p1_button_on.png", Config::BUTTON_POS_P1 + Config::P1_AC, Config::BUTTON_SIZE));
  buttons_on_p1_.push_back(new Button(echo_chan, D1, "assets/p1_button_on.png", Config::BUTTON_POS_P1 + Config::P1_AD, Config::BUTTON_SIZE));
  buttons_on_p1_.push_back(new Button(echo_chan, S1, "assets/start_button_on.png", Config::SPINNER_POS_P1 + Config::P1_S, Config::BUTTON_SIZE_START));
  // make all "on" buttons default to invisible
  for (auto& button : buttons_on_p1_) {
    button->setVisibility(false);
  }

  // p2
  // buttons off
  buttons_off_p2_.push_back(new Button(echo_chan, A2, "assets/p2_button_off.png", Config::BUTTON_POS_P2, Config::BUTTON_SIZE));
  buttons_off_p2_.push_back(new Button(echo_chan, B2, "assets/p2_button_off.png", Config::BUTTON_POS_P2 + Config::P2_AB, Config::BUTTON_SIZE));
  buttons_off_p2_.push_back(new Button(echo_chan, C2, "assets/p2_button_off.png", Config::BUTTON_POS_P2 + Config::P2_AC, Config::BUTTON_SIZE));
  buttons_off_p2_.push_back(new Button(echo_chan, D2, "assets/p2_button_off.png", Config::BUTTON_POS_P2 + Config::P2_AD, Config::BUTTON_SIZE));
  buttons_off_p2_.push_back(new Button(echo_chan, S2, "assets/start_button_off.png", Config::SPINNER_POS_P2 + Config::P2_S, Config::BUTTON_SIZE_START));
  dummy_buttons_.push_back(new Button(echo_chan, E2, "assets/p2_button_off.png", Config::BUTTON_POS_P2 + Config::P2_AE, Config::BUTTON_SIZE));
  dummy_buttons_.push_back(new Button(echo_chan, F2, "assets/p2_button_off.png", Config::BUTTON_POS_P2 + Config::P2_AF, Config::BUTTON_SIZE));
  // buttons on
  buttons_on_p2_.push_back(new Button(echo_chan, A2, "assets/p2_button_on.png", Config::BUTTON_POS_P2, Config::BUTTON_SIZE));
  buttons_on_p2_.push_back(new Button(echo_chan, B2, "assets/p2_button_on.png", Config::BUTTON_POS_P2 + Config::P2_AB, Config::BUTTON_SIZE));
  buttons_on_p2_.push_back(new Button(echo_chan, C2, "assets/p2_button_on.png", Config::BUTTON_POS_P2 + Config::P2_AC, Config::BUTTON_SIZE));
  buttons_on_p2_.push_back(new Button(echo_chan, D2, "assets/p2_button_on.png", Config::BUTTON_POS_P2 + Config::P2_AD, Config::BUTTON_SIZE));
  buttons_on_p2_.push_back(new Button(echo_chan, S2, "assets/start_button_on.png", Config::SPINNER_POS_P2 + Config::P2_S, Config::BUTTON_SIZE_START));
  // make all "on" buttons default to invisible
  for (auto& button : buttons_on_p2_) {
    button->setVisibility(false);
  }
}

void InputView::update() {
  joystick_->update();

  std::vector<std::vector<Button*>> button_groups = {
    buttons_on_p1_,
    buttons_on_p2_,
  };

  for (auto& button_group : button_groups) {
    for (auto& button : button_group) {
      if (echo_chan_->isButtonPressed(button->getLabel()) && !button->isDisabled()) {
        button->setVisibility(true);
      } else {
        button->setVisibility(false);
      }
    }
  }
}

void InputView::render(Renderer* renderer) {
  joystick_->render(renderer);

  // p1
  for (auto& button : buttons_off_p1_) {
    button->render(renderer);
  }
  for (auto& button : buttons_on_p1_) {
    button->render(renderer);
  }

  // p2
  for (auto& button : buttons_off_p2_) {
    button->render(renderer);
  }
  for (auto& button : buttons_on_p2_) {
    button->render(renderer);
  }

  // dummy
  for (auto& button : dummy_buttons_) {
    button->render(renderer);
  }
}

void InputView::nextStickType(int direction) {
  joystick_->nextStickType(direction);
}

void InputView::toggleActiveButtonVisibility(glm::vec2 mouse_pos) {
  std::vector<std::vector<Button*>> all_buttons = {
    buttons_on_p1_,
    buttons_on_p2_,
    buttons_off_p1_,
    buttons_off_p2_,
    dummy_buttons_,
  };
  for (auto& button_group : all_buttons) {
    for (auto& button : button_group) {
      if (button->isActive(mouse_pos)) {
	button->toggleVisibility();
      }
    }
  }
}
