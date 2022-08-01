#include <shader.hpp>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

unsigned int Shader::getID() {
  return id_;
}

Shader& Shader::use() {
  glUseProgram(id_);
  return *this;
}

void Shader::compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource) {
  unsigned int sVertex, sFragment, gShader;
  // vertex shader
  sVertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(sVertex, 1, &vertexSource, NULL);
  glCompileShader(sVertex);
  checkCompileErrors(sVertex, "VERTEX");
  // fragment shader
  sFragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(sFragment, 1, &fragmentSource, NULL);
  glCompileShader(sFragment);
  checkCompileErrors(sFragment, "FRAGMENT");
  // geometry shader (optional)
  if (geometrySource != nullptr) {
    gShader = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(gShader, 1, &geometrySource, NULL);
    glCompileShader(gShader);
    checkCompileErrors(gShader, "GEOMETRY");
  }
  // link
  id_ = glCreateProgram();
  glAttachShader(id_, sVertex);
  glAttachShader(id_, sFragment);
  if (geometrySource != nullptr) {
    glAttachShader(id_, gShader);
  }
  glLinkProgram(id_);
  checkCompileErrors(id_, "PROGRAM");
  // delete the shaders since they have been linked
  glDeleteShader(sVertex);
  glDeleteShader(sFragment);
  if (geometrySource != nullptr) {
    glDeleteShader(gShader);
  }
}

void Shader::setFloat(const char* name, float value, bool useShader) {
  if (useShader) {
    use();
  }
  glUniform1f(glGetUniformLocation(id_, name), value);
}

void Shader::setInteger(const char* name, int value, bool useShader) {
  if (useShader) {
    use();
  }
  glUniform1i(glGetUniformLocation(id_, name), value);
}

void Shader::setVector2f(const char* name, float x, float y, bool useShader) {
  if (useShader) {
    use();
  }
  glUniform2f(glGetUniformLocation(id_, name), x, y);
}

void Shader::setVector2f(const char* name, const glm::vec2 &value, bool useShader) {
  if (useShader) {
    use();
  }
  glUniform2f(glGetUniformLocation(id_, name), value.x, value.y);
}

void Shader::setVector3f(const char* name, float x, float y, float z, bool useShader) {
  if (useShader) {
    use();
  }
  glUniform3f(glGetUniformLocation(id_, name), x, y, z);
}

void Shader::setVector3f(const char* name, const glm::vec3 &value, bool useShader) {
  if (useShader) {
    use();
  }
  glUniform3f(glGetUniformLocation(id_, name), value.x, value.y, value.z);
}

void Shader::setVector4f(const char* name, float x, float y, float z, float w, bool useShader) {
  if (useShader) {
    use();
  }
  glUniform4f(glGetUniformLocation(id_, name), x, y, z, w);
}

void Shader::setVector4f(const char* name, const glm::vec4 &value, bool useShader) {
  if (useShader) {
    use();
  }
  glUniform4f(glGetUniformLocation(id_, name), value.x, value.y, value.z, value.w);
}

void Shader::setMatrix4(const char* name, const glm::mat4 &matrix, bool useShader) {
  if (useShader) {
    use();
  }
  glUniformMatrix4fv(glGetUniformLocation(id_, name), 1, false, glm::value_ptr(matrix));
}

void Shader::checkCompileErrors(unsigned int object, std::string type) {
  int success;
  char infoLog[1024];
  if (type != "PROGRAM") {
    glGetShaderiv(object, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(object, 1024, NULL, infoLog);
      std::cout << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n"
		<< infoLog << std::endl;
    }
  } else {
    glGetProgramiv(object, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(object, 1024, NULL, infoLog);
      std::cout << "| ERROR::Shader: Link-time error: Type: " << type << "\n"
		<< infoLog << std::endl;
    }
  }
}
