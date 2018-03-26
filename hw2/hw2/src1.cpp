#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"
#include "shader.h"
using namespace std;
#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 600

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
vector<float> Bresenham(int x0, int y0, int x1, int y1);

int main() {
  glfwInit();
  // OpenGL version & mode setting
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Create a window & context/viewpoint setting
  GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "My First OpenGL", NULL, NULL);
  if (window == NULL) {
    cout << "Failed to create GLFW window" << endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // glad init
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    cout << "Failed to initialize GLAD" << endl;
    return -1;
  }
  // shader init
  Shader shader("vShaderSrc.txt", "fShaderSrc.txt");
  // ======================================
  //// Setup ImGui bindings
  //ImGui::CreateContext();
  //ImGuiIO& io = ImGui::GetIO(); (void)io;
  //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
  //ImGui_ImplGlfwGL3_Init(window, true);
  //ImGui::StyleColorsDark();

  vector<float> points = Bresenham(-600, -300, 600, 300);
  //float* vertices = new float[points.size()];
  //memcpy(vertices, &points[0], points.size()*sizeof(float));

  unsigned int VBO;
  // generate VBO & bind to buffer
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // create shader with Shader class
  shader.use();

  // create VAO
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  // ---- render loop ----
  while (!glfwWindowShouldClose(window)) {
    // process input from keyboard/mouse/other
    processInput(window);
    glfwPollEvents();
    // init ImGui
    //ImGui_ImplGlfwGL3_NewFrame();
    //ImGui::SetWindowSize(ImVec2(300, 150));
    //ImGui::Checkbox("SameColor", &sameColor);
    //ImGui::RadioButton("Show Triangle", &radioMark, 0);
    //ImGui::RadioButton("Show Line", &radioMark, 1);
    //ImGui::RadioButton("Show Points", &radioMark, 2);
    //if (!sameColor) {
    //  ImGui::ColorEdit3("Left", (float*)&tri1);
    //  ImGui::ColorEdit3("Right", (float*)&tri2);
    //  ImGui::ColorEdit3("Top", (float*)&tri3);
    //} else {
    //  ImGui::ColorEdit3("All Vertices", (float*)&tri1);
    //  tri2 = tri1;
    //  tri3 = tri1;
    //}
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    shader.use();
    for (int i = 0; i < points.size(); i+=2) {
      float vertices[] = {points[i], points[i+1], 0.0f, 1.0f, 1.0f, 1.0f};
      shader.use();
      glBindVertexArray(VAO);
      glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
      // position
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
      glEnableVertexAttribArray(0);
      //color
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
      glEnableVertexAttribArray(1);
      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
      glDrawArrays(GL_POINTS, 0, 1);
    }
    // Copy vertices to buffer

    // check out triggerations & render
    
    //ImGui::Render();
    //ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);
  }
  // release sources
  //ImGui_ImplGlfwGL3_Shutdown();
  //ImGui::DestroyContext();
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

vector<float> Bresenham(int x0, int y0, int x1, int y1) {
  vector<float> result;
  int deltaX = x1 - x0;
  int deltaY = y1 - y0;
  int p = 2 * deltaY - deltaX;
  int xi = x0, yi = y0;
  result.push_back((float)xi / (float)WINDOW_WIDTH);
  result.push_back((float)yi / (float)WINDOW_HEIGHT);
  while (xi != x1 && yi != y1) {
    if (p <= 0) {
      xi += 1;
      p = p + 2 * deltaY;
      result.push_back((float)xi/(float)WINDOW_WIDTH);
      result.push_back((float)yi / (float)WINDOW_HEIGHT);
    } else {
      xi += 1;
      yi += 1;
      p = p + 2 * deltaY - 2 * deltaX;
      result.push_back((float)xi / (float)WINDOW_WIDTH*2);
      result.push_back((float)yi / (float)WINDOW_HEIGHT*2);
    }
  }
  return result;
}