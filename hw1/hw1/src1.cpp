#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"
#include "shader.h"
using namespace std;
#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 600

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

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

  // Setup ImGui bindings
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
  ImGui_ImplGlfwGL3_Init(window, true);
  ImGui::StyleColorsDark();

  // Triangle info
  ImVec4 tri1 = ImVec4(1.0f, 0.0f, 0.0f, 1.00f);
  ImVec4 tri2 = ImVec4(0.0f, 1.0f, 0.0f, 1.00f);
  ImVec4 tri3 = ImVec4(0.0f, 0.0f, 1.0f, 1.00f);
  float vertices[] = {
    -0.8f, -0.5f, 0.0f, tri1.x, tri1.y, tri1.z,
    -0.6f,  0.5f, 0.0f, tri2.x, tri2.y, tri2.z,
    -0.4f, -0.5f, 0.0f, tri3.x, tri3.y, tri3.z,
    -0.2f,  0.5f, 0.0f, tri1.x, tri1.y, tri1.z,
    -0.0f, -0.5f, 0.0f, tri2.x, tri2.y, tri2.z,
     0.2f,  0.5f, 0.0f, tri3.x, tri3.y, tri3.z,
     0.4f, -0.5f, 0.0f, tri1.x, tri1.y, tri1.z,
     0.6f,  0.5f, 0.0f, tri2.x, tri2.y, tri2.z,
     0.8f, -0.5f, 0.0f, tri3.x, tri3.y, tri3.z
  };
  unsigned int triangleIndices[] = {
    0, 1, 2, 
    3, 4, 5,
    6, 7, 8
  };
  unsigned int lineIndices[] = {
    0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8
  };
  ImVec4 lineVertex1 = ImVec4(1.5f, 1.5f, 0.0f, 1.0f);
  ImVec4 lineVertex2 = ImVec4(1.5f, 1.8f, 0.0f, 1.0f);

  unsigned int EBO;
  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangleIndices), triangleIndices, GL_STATIC_DRAW);

  unsigned int VBO;
  // generate VBO & bind to buffer
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // create shader with Shader class
  Shader shader("vShaderSrc.txt", "fShaderSrc.txt");
  shader.use();

  // create VAO
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  // Copy vertices to buffer
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
  // position
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  //color
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  int radioMark = 0;
  bool sameColor = true;

  // ---- render loop ----
  while (!glfwWindowShouldClose(window)) {
    // process input from keyboard/mouse/other
    processInput(window);
    glfwPollEvents();
    // init ImGui
    ImGui_ImplGlfwGL3_NewFrame();
    ImGui::SetWindowSize(ImVec2(300, 150));
    ImGui::Checkbox("SameColor", &sameColor);
    ImGui::RadioButton("Show Triangle", &radioMark, 0);
    ImGui::RadioButton("Show Line", &radioMark, 1);
    ImGui::RadioButton("Show Points", &radioMark, 2);
    if (!sameColor) {
      ImGui::ColorEdit3("Left", (float*)&tri1);
      ImGui::ColorEdit3("Right", (float*)&tri2);
      ImGui::ColorEdit3("Top", (float*)&tri3);
    } else {
      ImGui::ColorEdit3("All Vertices", (float*)&tri1);
      tri2 = tri1;
      tri3 = tri1;
    }
    float vertices[] = {
      -0.8f, -0.5f, 0.0f, tri1.x, tri1.y, tri1.z,
      -0.6f,  0.5f, 0.0f, tri2.x, tri2.y, tri2.z,
      -0.4f, -0.5f, 0.0f, tri3.x, tri3.y, tri3.z,
      -0.2f,  0.5f, 0.0f, tri1.x, tri1.y, tri1.z,
      -0.0f, -0.5f, 0.0f, tri2.x, tri2.y, tri2.z,
       0.2f,  0.5f, 0.0f, tri3.x, tri3.y, tri3.z,
       0.4f, -0.5f, 0.0f, tri1.x, tri1.y, tri1.z,
       0.6f,  0.5f, 0.0f, tri2.x, tri2.y, tri2.z,
       0.8f, -0.5f, 0.0f, tri3.x, tri3.y, tri3.z
    };
    // Copy vertices to buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(VAO);
    if (radioMark == 0) {
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangleIndices), triangleIndices, GL_STATIC_DRAW);
      glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
    } else if (radioMark == 1) {
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(lineIndices), lineIndices, GL_STATIC_DRAW);
      glDrawElements(GL_LINES, 16, GL_UNSIGNED_INT, 0);
    } else if (radioMark == 2) {
      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
      glDrawArrays(GL_POINTS, 0, 9); 
    }
    // check out triggerations & render
    
    ImGui::Render();
    ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);
  }
  // release sources
  ImGui_ImplGlfwGL3_Shutdown();
  ImGui::DestroyContext();
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