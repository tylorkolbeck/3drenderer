#pragma once

#include "glm/ext/matrix_float4x4.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <fstream>
#include <glad/glad.h>
#include <iostream>
#include <sstream>
#include <string>

class Shader {
public:
  unsigned int id;
  Shader(const char *vertexPath, const char *fragmentPath);

  void use();

  void setInt(const std::string &name, int value) const;
  void setBool(const std::string &name, bool value) const;
  void setFloat(const std::string &name, float value) const;
  void setVec4(const std::string &name, float v1, float v2, float v3,
               float v4) const;
  void setVec3(const std::string &name, float v1, float v2, float v3) const;
  void setMat4(const std::string &name, glm::mat4 value) const;

private:
  const char *vShaderPath;
  const char *fShaderPath;

  std::string vShaderSource;
  std::string fShaderSource;
  std::ifstream vShaderFile;
  std::ifstream fShaderFile;

  unsigned int vertexShader;
  unsigned int fragmentShader;

  void loadShaders();
  void compileShaders();
  void linkShaders();
};

inline void Shader::setVec4(const std::string &name, float v1, float v2,
                            float v3, float v4) const {
  glUniform4f(glGetUniformLocation(id, name.c_str()), v1, v2, v3, v4);
}

inline void Shader::setInt(const std::string &name, int value) const {
  glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

inline void Shader::setBool(const std::string &name, bool value) const {
  glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}

inline void Shader::setFloat(const std::string &name, float value) const {
  glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

inline void Shader::setMat4(const std::string &name, glm::mat4 value) const {
  glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE,
                     glm::value_ptr(value));
}
inline void Shader::setVec3(const std::string &name, float v1, float v2, float v3) const {
  glUniform3f(glGetUniformLocation(id, name.c_str()), v1, v2, v3 );
}

inline void Shader::use() { glUseProgram(id); }

inline void Shader::linkShaders() {
  id = glCreateProgram();
  glAttachShader(id, vertexShader);
  glAttachShader(id, fragmentShader);
  glLinkProgram(id);
  int success;
  char infoLog[512];
  glGetProgramiv(id, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(id, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER_LINKER::LINK_FAILED\n" << infoLog << std::endl;
  } else {
    std::cout << "INFO::SHADER_LINKER::LINK_SUCCESS" << std::endl;
  }
  // Clean up shaders after linking
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

inline void Shader::compileShaders() {
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  const char *vsrx = vShaderSource.c_str();
  glShaderSource(vertexShader, 1, &vsrx, NULL);
  glCompileShader(vertexShader);

  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }
  std::cout << "INFO::SHADER::VERTEX::COMPILATION_SUCCESS\n";

  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  const char *fsrc = fShaderSource.c_str();
  glShaderSource(fragmentShader, 1, &fsrc, NULL);
  glCompileShader(fragmentShader);

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }
  std::cout << "INFO::SHADER::FRAGMENT::COMPILATION_SUCCESS\n";
}

inline Shader::Shader(const char *vertexPath, const char *fragmentPath) {
  vShaderPath = vertexPath;
  fShaderPath = fragmentPath;
  std::cout << "INITIALIZING SHADER SOURCE" << std::endl;
  loadShaders();
  compileShaders();
  linkShaders();
}

inline void Shader::loadShaders() {
  try {
    vShaderFile.open(vShaderPath);
    if (!vShaderFile.is_open()) {
      std::cout << "ERROR::SHADER::COULD_NOT_OPEN_VERTEX_SHADER_FILE"
                << std::endl;
    }
    fShaderFile.open(fShaderPath);
    if (!fShaderFile.is_open()) {
      std::cout << "ERROR::SHADER::COULD_NOT_OPEN_FRAGMENT_SHADER_FILE"
                << std::endl;
    }

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    std::stringstream vShaderStream, fShaderStream;

    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();

    vShaderFile.close();
    fShaderFile.close();

    vShaderSource = vShaderStream.str();
    fShaderSource = fShaderStream.str();
    std::cout << "INFO::SHADER::SHADER_SOURCES_LOADED\n" << std::endl;
  } catch (std::exception &e) {
    std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ\n"
              << e.what() << std::endl;
  }
}
