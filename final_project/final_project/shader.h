#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


class Shader
{
public:
  // program ID
  unsigned int ID;

  // constructer & create program
  Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
  // 使用/激活程序
  void use();
  // uniform工具函数
  void setBool(const std::string &name, bool value) const;
  void setInt(const std::string &name, int value) const;
  void setFloat(const std::string &name, float value) const;
  void setVec3(const std::string &name, float value1, float value2, float value3) const;
  void setMat4(const std::string &name, const glm::mat4 &matrix) const;
};

#endif