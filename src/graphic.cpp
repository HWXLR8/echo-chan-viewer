#include <graphic.hpp>

#include <config.hpp>
#include <resource_manager.hpp>

Graphic::Graphic(std::string texturePath, glm::vec2 position, glm::vec2 size, bool transparent, RENDER_TYPE render_type) {
  if (texturePath != "") {
    texture_ = ResourceManager::getTexture(texturePath, transparent);
  }
  position_ = position;
  size_ = size;
  render_type_ = render_type;
  scale_ = Config::getScale();
}

void Graphic::update(double dt) {
  if (move_in_progress_) {
    move(move_dest_, std::nullopt, dt);
  }
  if (change_size_in_progress_) {
    changeSize(new_size_, growth_speed_, dt);
  }
  scale();
}

void Graphic::render(Renderer *renderer) {
  if (!visible_) {
    return;
  }
  renderer->blit(texture_, position_, size_, rotation_, color_, render_type_);
}

void Graphic::changeSize(glm::vec2 new_size, glm::vec2 speed, double dt) {
  // first animation step
  if (!change_size_in_progress_) {
    change_size_in_progress_ = true;
    new_size_ = new_size;
    growth_speed_ = speed;
    remaining_growth_ = {
      abs(new_size_.x - size_.x),
      abs(new_size_.y - size_.y)
    };
    // calculate whether speeds are positive or negative
    if (new_size_.x - size_.x < 0) {
      growth_speed_.x *= -1;
    } else {
      growth_speed_.x = abs(growth_speed_.x);
    }
    if (new_size_.y - size_.y < 0) {
      growth_speed_.y *= -1;
    } else {
      growth_speed_.y = abs(growth_speed_.x);
    }
  }

  // special case to skip animation
  if (dt == 0) {
    size_ = new_size_;
    change_size_in_progress_ = false;
    return;
  }

  growth_chunk_ = {
    growth_speed_.x * dt,
    growth_speed_.y * dt,
  };

  // are we done?
  if (remaining_growth_.x <= 0 && remaining_growth_.y <= 0) {
    size_ = new_size_;
    change_size_in_progress_ = false;
    return;
  }

  // are we done getting fatter?
  if ((remaining_growth_.x - fabs(growth_chunk_.x)) <= 0) {
    size_.x = new_size_.x;
    remaining_growth_.x = 0;
  } else {
    size_.x += growth_chunk_.x;
    remaining_growth_.x -= fabs(growth_chunk_.x);
  }

  // are we done getting taller?
  if ((remaining_growth_.y - fabs(growth_chunk_.y)) <= 0) {
    size_.y = new_size_.y;
    remaining_growth_.y = 0;
  } else {
    size_.y += growth_chunk_.y;
    remaining_growth_.y -= fabs(growth_chunk_.y);
  }
}

void Graphic::move(glm::vec2 dest, std::optional<double> a_time, double dt) {
  // first animation step
  if (!move_in_progress_) {
    move_in_progress_ = true;
    move_dest_ = dest;
    move_remaining_time_ = a_time.value();
    glm::vec2 unit_vec = calculateUnitVector(position_, dest);
    double mag = calculateVectorMagnitude(position_, dest);
    move_speed_ = {
      (unit_vec.x * mag) / a_time.value(),
      (unit_vec.y * mag) / a_time.value(),
    };
  }

  // special case to skip animation
  if (dt == 0) {
    position_ = move_dest_;
    move_remaining_time_ = 0;
    move_in_progress_ = false;
    return;
  }

  // are we done?
  if (move_remaining_time_ <= 0 || position_ == dest) {
    position_ = move_dest_;
    move_in_progress_ = false;
    return;
  }

  move_chunk_ = {
    move_speed_.x * dt,
    move_speed_.y * dt,
  };

  position_.x += move_chunk_.x;
  position_.y += move_chunk_.y;
  move_remaining_time_ -= dt;
}

bool Graphic::isActive(glm::vec2 mouse_pos) {
  return ((position_.x <= mouse_pos.x) &&
	  (mouse_pos.x <= position_.x + size_.x) &&
	  (position_.y <= mouse_pos.y) &&
	  (mouse_pos.y <= position_.y + size_.y));
}

glm::vec2 Graphic::getPosition() {
  return position_;
}

void Graphic::illuminate() {
  illuminated_ = true;
  setColor(glm::vec4{1.0f, 1.0f, 1.0f, 1.0f});
}

void Graphic::fade() {
  illuminated_ = false;
  setColor(glm::vec4{0.4f, 0.4f, 0.4f, 1.0f});
}

void Graphic::setColor(glm::vec4 color) {
  color_ = color;
}

bool Graphic::isVisible() {
  return visible_;
}

void Graphic::setVisibility(bool b) {
  visible_ = b;
}

void Graphic::setTexture(Texture2D new_tex) {
  texture_ = new_tex;
}

Texture2D Graphic::getTexture() {
  return texture_;
}

bool Graphic::isMoving() {
  return move_in_progress_;
}

void Graphic::stopMoving() {
  move_in_progress_ = false;
}

glm::vec2 Graphic::calculateUnitVector(glm::vec2 p1, glm::vec2 p2) {
  double mag = pow(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2), 0.5);
  glm::vec2 unit_vec {
    (p2.x - p1.x)/mag,
    (p2.y - p1.y)/mag
  };
  return unit_vec;
}

double Graphic::calculateVectorMagnitude(glm::vec2 p1, glm::vec2 p2) {
  double mag = pow(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2), 0.5);
  return mag;
}

void Graphic::steerMovement(glm::vec2 new_dest) {
  glm::vec2 unit_vec = calculateUnitVector(position_, new_dest);
  double mag = calculateVectorMagnitude(position_, new_dest);
  move_speed_ = {
    (unit_vec.x * mag) / move_remaining_time_,
    (unit_vec.y * mag) / move_remaining_time_,
  };
}

void Graphic::setSize(glm::vec2 new_size) {
  size_ = new_size;
}

void Graphic::setPosition(glm::vec2 position) {
  position_ = position;
}

void Graphic::scale() {
  glm::vec2 scale = Config::getScale();
  if (scale_ != scale) {
    scale_ = scale;
    size_ *= scale;
    position_ *= scale;
  }
}

void Graphic::setRotation(glm::vec3 new_rotation) {
  rotation_ = new_rotation;
}
