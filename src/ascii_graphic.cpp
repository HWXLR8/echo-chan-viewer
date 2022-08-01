#include <ascii_graphic.hpp>

ASCIIGraphic::ASCIIGraphic(char character, std::string texturePath, glm::vec2 position, glm::vec2 size, bool transparent) : Graphic(texturePath, position, size, transparent, TEXT) {
  character_ = static_cast<int>(character);
}

void ASCIIGraphic::render(Renderer* renderer) {
  renderer->setAsciiOffset(character_);
  Graphic::render(renderer);
}

void ASCIIGraphic::setChar(char character) {
  character_ = static_cast<int>(character);
}
