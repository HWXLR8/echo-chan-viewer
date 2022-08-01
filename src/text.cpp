#include <text.hpp>

#include <config.hpp>

Text::Text(std::string text, glm::vec2 position, glm::vec2 letter_size, glm::vec2 spacing) {
  text_ = text;
  position_ = position;
  letter_size_ = letter_size;
  scale_ = Config::getScale();

  int c = 0;
  for (auto ch : text) {
    glm::vec2 pos = {position.x + letter_size.x * c, position.y};
    ASCIIGraphic* letter = new ASCIIGraphic(ch, "assets/ascii.gif", pos, letter_size, true);
    letters_.push_back(letter);
    c++;
  }
}

void Text::render(Renderer* renderer) {
  for (auto& letter : letters_) {
    letter->render(renderer);
  }
}

void Text::setPosition(glm::vec2 position) {
  int c = 0;
  for (auto& letter : letters_) {
    glm::vec2 pos = {position.x + letter_size_.x * c, position.y};
    letter->setPosition(pos);
    c++;
  }
}

glm::vec2 Text::getPosition() {
  return position_;
}

void Text::update(double dt) {
  for (auto& letter : letters_) {
    letter->update(dt);
  }
  scale();
}

void Text::scale() {
  glm::vec2 scale = Config::getScale();
  if (scale_ != scale) {
    scale_ = scale;
    letter_size_ *= scale;
  }
}

void Text::setText(std::string text) {
  int c = 0;
  for (auto ch : text) {
    // if we are trying to change the value of a character that does
    // not exist
    if (c == letters_.size()) {
      return;
    }
    letters_[c]->setChar(ch);
    c++;
  }
}

std::string Text::getText() {
  return text_;
}

void Text::setColor(glm::vec4 color) {
  for (auto letter : letters_) {
    letter->setColor(color);
  }
}

void Text::setVisibility(bool b) {
  for (auto letter : letters_) {
    letter->setVisibility(b);
  }
}
