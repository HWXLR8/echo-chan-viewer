#pragma once

#include <map>
#include <string>

#include <texture.hpp>
#include <shader.hpp>

class ResourceManager {
 public:
  static std::map<std::string, Shader> Shaders;
  static std::map<std::string, Texture2D> Textures;

  static Shader loadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name);
  static Shader getShader(std::string name);
  static Texture2D loadTexture(std::string path, bool alpha);
  static Texture2D getTexture(std::string path, bool alpha);
  static void clear();

 private:
  ResourceManager() {}
  static Shader loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile = nullptr);
  static Texture2D loadTextureFromFile(std::string path, bool alpha);
};
