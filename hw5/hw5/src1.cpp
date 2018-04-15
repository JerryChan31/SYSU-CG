#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"
#include "shader.h"
using namespace std;
#define WINDOW_WIDTH  600
#define WINDOW_HEIGHT 600

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
glm::vec3 lightPosition(2.0f, 1.0f, 2.0f);

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

  Shader shader("vShaderSrc.txt", "fShaderSrc.txt");
  Shader lightShader("vShaderSrc.txt", "lightfShaderSrc.txt");
  // Triangle info
  ImVec4 tri1 = ImVec4(1.0f, 0.0f, 0.0f, 1.00f);
  ImVec4 tri2 = ImVec4(0.0f, 1.0f, 0.0f, 1.00f);
  ImVec4 tri3 = ImVec4(0.0f, 0.0f, 1.0f, 1.00f);
  ImVec4 tri4 = ImVec4(0.5f, 0.f, 0.5f, 1.00f);
  float vertices[] = {
    -0.2f, -0.2f, -0.2f,
    -0.2f, -0.2f,  0.2f, 
    -0.2f,  0.2f, -0.2f, 
    -0.2f,  0.2f,  0.2f, 
     0.2f, -0.2f, -0.2f, 
     0.2f, -0.2f,  0.2f, 
     0.2f,  0.2f, -0.2f, 
     0.2f,  0.2f,  0.2f
  };
  unsigned int triangleIndices[] = {
    0, 1, 3, 
    0, 3, 2,
    0, 1, 5, 
    0, 5, 4,
    3, 2, 7,
    2, 7, 6,
    3, 1, 5,
    3, 5, 7,
    0, 2, 6,
    0, 4, 6,
    5, 4, 7,
    4, 7, 6
  };

  unsigned int cubeVAO, VBO, EBO, lightVAO;
  // cube VAO&VBO
  glGenVertexArrays(1, &cubeVAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(cubeVAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangleIndices), triangleIndices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // light VAO &VBO
  glGenVertexArrays(1, &lightVAO);
  glBindVertexArray(lightVAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangleIndices), triangleIndices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  

  glm::mat4 view;
  bool depth = true;
  int radioMark = 0;
  float temp = 0.0f;
  float save = 0.0f;
  int sign = 1;
  // ---- render loop ----
  while (!glfwWindowShouldClose(window)) {
    // process input from keyboard/mouse/other
    processInput(window);
    glfwPollEvents();
    // init ImGui
    ImGui_ImplGlfwGL3_NewFrame();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    shader.use();
    shader.setVec3("objectColor", 1.0f, 0.0f, 0.0f);
    shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);

    shader.use();
    shader.setVec3("objectColor", 1.0f, 0.0f, 0.0f);
    shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);

    glm::mat4 model(1);
    glm::mat4 view(1);
    glm::mat4 projection(1);
    //model = glm::rotate(model, glm::radians(3500.0f), glm::vec3(0.0f, 1.0f, 1.0f));
    //view = glm::translate(view, glm::vec3(0.0f, 0.0f, -50.0f));
    view = glm::lookAt(glm::vec3(5.0f, 2.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    projection = glm::perspective(45.0f, 1.0f, 0.1f, 1000.0f);

    int modelLoc = glGetUniformLocation(shader.ID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    int viewLoc = glGetUniformLocation(shader.ID, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    int proLoc = glGetUniformLocation(shader.ID, "projection");
    glUniformMatrix4fv(proLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 39, GL_UNSIGNED_INT, 0);
    
    lightShader.use();
    model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
    model = glm::translate(model, glm::vec3(1.0f, 1.0f, -1.0f));
    modelLoc = glGetUniformLocation(lightShader.ID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    viewLoc = glGetUniformLocation(lightShader.ID, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    proLoc = glGetUniformLocation(lightShader.ID, "projection");
    glUniformMatrix4fv(proLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glBindVertexArray(lightVAO);
    glDrawElements(GL_TRIANGLES, 39, GL_UNSIGNED_INT, 0);


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
