#pragma once

#include <string>

class Texture2D {
public:
  Texture2D();
  void Generate(unsigned int width, unsigned int height, unsigned char* data, std::string name);
  void Bind() const;
  unsigned int getID();
  void setInternalFormat(unsigned int internal_format);
  void setImageFormat(unsigned int image_format);

  inline bool operator ==(Texture2D& rhs) {
    return name_ == rhs.name_;
  }
  inline bool operator !=(Texture2D& rhs) {
    return name_ != rhs.name_;
  }

private:
  unsigned int id_;
  unsigned int width_, height_;
  unsigned int internal_format_;
  unsigned int image_format_;
  unsigned int wrap_s_;
  unsigned int wrap_t_;
  unsigned int filter_min_;
  unsigned int filter_max_;
  std::string name_;
};
