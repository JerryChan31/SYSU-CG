#include<glad\glad.h>
#include"shader.h"
#include<string>
#include<iostream>
#include<sstream>
#include<fstream>

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath) {
  // ------------- read shader code from file -----------
  // string & file
  std::string vertexSrc;
  std::string fragmentSrc;
  std::ifstream vShaderFile;
  std::ifstream fShaderFile;
  // exception
  vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try {
    // open file
    vShaderFile.open(vertexPath);
    fShaderFile.open(fragmentPath);
    std::stringstream vShaderStream, fShaderStream;
    // read file into bufer
    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();
    // close file
    vShaderFile.close();
    fShaderFile.close();
    vertexSrc = vShaderStream.str();
    fragmentSrc = fShaderStream.str();
  }
  catch (const std::exception&) {
    std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
  }
  const char* vShaderCode = vertexSrc.c_str();
  const char* fShaderCode = fragmentSrc.c_str();

  // --------------- compile shader ------------
  unsigned int vertex, fragment;
  int success;
  char compileLog[512];
  // compile vertex shader
  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vShaderCode, NULL);
  glCompileShader(vertex);
  glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertex, 512, NULL, compileLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << compileLog << std::endl;
  }
  // compile fragment shader
  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fShaderCode, NULL);
  glCompileShader(fragment);
  glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragment, 512, NULL, compileLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << compileLog << std::endl;
  }
  // shader program
  ID = glCreateProgram();
  glAttachShader(ID, vertex);
  glAttachShader(ID, fragment);
  glLinkProgram(ID);
  glGetProgramiv(ID, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(ID, 512, NULL, compileLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << compileLog << std::endl;
  }
  glDeleteShader(vertex);
  glDeleteShader(fragment);
}
void Shader::use() {
  glUseProgram(ID);
}
void Shader::setBool(const std::string &name, bool value) const{
  glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::setInt(const std::string &name, int value) const {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const std::string &name, float value) const {
  glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}