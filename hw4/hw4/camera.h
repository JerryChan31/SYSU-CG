#pragma once
#include<glad\glad.h>
#include<GLFW\glfw3.h>

class Camera {
public:
  void moveForward(GLfloat const distance);
  void moveBack(GLfloat const distance);
  void moveRight(GLfloat const distance);
  void moveLeft(GLfloat const distance);
  void rotate(GLfloat const pitch, GLfloat const yaw);

private:
  GLfloat pfov, pratio, pnear, pfar;
  GLfloat cameraPosX, cameraPosY, cameraPosZ;
  GLfloat cameraFrontX, cameraFrontY, cameraFrontZ;
  GLfloat cameraRightX, cameraRightY, cameraRightZ;
  GLfloat cameraUpX, cameraUpY, cameraUpZ;
};