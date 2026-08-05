#ifndef SHADER_H
#define SHADER_H

#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <glad/gl.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

class Shader {
public:
  unsigned int ID;

  Shader(const char *vertexPath, const char *fragmentPath) {
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vertexShaderFile;
    std::ifstream fragmentShaderFile;

    vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
      vertexShaderFile.open(vertexPath);
      fragmentShaderFile.open(fragmentPath);
      std::stringstream vertexShaderStream, fragmentShaderStream;

      vertexShaderStream << vertexShaderFile.rdbuf();
      fragmentShaderStream << fragmentShaderFile.rdbuf();

      vertexShaderFile.close();
      fragmentShaderFile.close();

      vertexCode = vertexShaderStream.str();
      fragmentCode = fragmentShaderStream.str();
    } catch (const std::ifstream::failure &error) {
      std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n" << error.what() << std::endl;
    }

    const char *vertexShaderCode = vertexCode.c_str();
    const char *fragmentShaderCode = fragmentCode.c_str();

    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexShaderCode, nullptr);
    glCompileShader(vertex);

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
      std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentShaderCode, nullptr);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
      std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);

    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(ID, 512, nullptr, infoLog);
      std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
  }

  void use() { glUseProgram(ID); }

  void setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), static_cast<int>(value));
  }

  void setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
  }

  void setFloat(const std::string &name, float value) const {
      glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
  }

  void setVec3(const std::string &name, const glm::vec3 &value) const {
      glUniform3f(glGetUniformLocation(ID, name.c_str()), value.x, value.y, value.z);
  }

  void setVec3(const std::string &name, float x, float y, float z) const {
      glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
  }

  void setMat4(const std::string &name, const glm::mat4 &value) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
  }
};

#endif
