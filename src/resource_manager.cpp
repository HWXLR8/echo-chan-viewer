#include <glad/glad.h>
#include <stb/stb_image.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

#include <resource_manager.hpp>


std::map<std::string, Texture2D> ResourceManager::Textures;
std::map<std::string, Shader> ResourceManager::Shaders;

Shader ResourceManager::loadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name) {
  Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
  return Shaders[name];
}

Shader ResourceManager::getShader(std::string name) {
  if (Shaders.find(name) == Shaders.end()) {
    std::cout << "no such shader: " << name << std::endl;
  }
  return Shaders[name];
}

Texture2D ResourceManager::loadTexture(std::string path, bool alpha) {
  if (!std::filesystem::exists(path)) {
    throw std::runtime_error("no such file or directory: " + path);
  }
  Textures[path] = loadTextureFromFile(path, alpha);
  return Textures[path];
}

Texture2D ResourceManager::getTexture(std::string path, bool alpha) {
  // if the texture has never been loaded before
  if (Textures.find(path) == Textures.end()) {
    loadTexture(path, alpha);
  }
  return Textures[path];
}

void ResourceManager::clear() {
  for (auto shader : Shaders) {
    glDeleteProgram(shader.second.getID());
  }
  for (auto texture : Textures) {
    unsigned int tex_id = texture.second.getID();
    glDeleteTextures(1, &tex_id);
  }
}

Shader ResourceManager::loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile) {
  std::string vertexCode;
  std::string fragmentCode;
  std::string geometryCode;
  try {
    std::ifstream vertexShaderFile(vShaderFile);
    std::ifstream fragmentShaderFile(fShaderFile);
    std::stringstream vShaderStream, fShaderStream;
    vShaderStream << vertexShaderFile.rdbuf();
    fShaderStream << fragmentShaderFile.rdbuf();
    vertexShaderFile.close();
    fragmentShaderFile.close();
    vertexCode = vShaderStream.str();
    fragmentCode = fShaderStream.str();
    if (gShaderFile != nullptr) {
      std::ifstream geometryShaderFile(gShaderFile);
      std::stringstream gShaderStream;
      gShaderStream << geometryShaderFile.rdbuf();
      geometryShaderFile.close();
      geometryCode = gShaderStream.str();
    }
  }
  catch (std::exception& e) {
    std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
  }
  const char* vShaderCode = vertexCode.c_str();
  const char* fShaderCode = fragmentCode.c_str();
  const char* gShaderCode = geometryCode.c_str();
  Shader shader;
  shader.compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
  return shader;
}

Texture2D ResourceManager::loadTextureFromFile(std::string path, bool alpha) {
  // create texture object
  Texture2D texture;
  if (alpha) {
    texture.setInternalFormat(GL_RGBA);
    texture.setImageFormat(GL_RGBA);
  }
  // load image
  int width, height, nrChannels;
  unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
  // generate texture
  texture.Generate(width, height, data, path);
  // free image data
  stbi_image_free(data);
  return texture;
}
