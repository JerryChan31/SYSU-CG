#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
using namespace std;

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void drawTriangle();

int main() {
  glfwInit();
  // OpenGL version setting
  // Major version
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  // Minor version
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  // Core mode setting
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Create a window
  GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "My First OpenGL", NULL, NULL);
  // check if successfully created
  if (window == NULL) {
    cout << "Failed to create GLFW window" << endl;
    glfwTerminate();
    return -1;
  }
  // major context setting
  glfwMakeContextCurrent(window);
  // set viewport when window size changed
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // glad init
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    cout << "Failed to initialize GLAD" << endl;
    return -1;
  }

  // render loop
  // render until close the windows
  while (!glfwWindowShouldClose(window)) {
    // process input from keyboard/mouse/other
    processInput(window);

    // code
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // draw in a function
    drawTriangle();

    // swap color buffer to show image
    glfwSwapBuffers(window);
    // check out triggerations
    glfwPollEvents();
  }

  // release sources
  glfwTerminate();
  return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
  // press ESC to close
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

void drawTriangle() {
  float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
    -0.0f,  0.5f, 0.0f,
  };
  unsigned int VBO;
  // generate VBO & bind to buffer
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

}