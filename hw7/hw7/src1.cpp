#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"
#include "shader.h"
#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace std;
#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 600
int cursorPosX = 0;
int cursorPosY = 0;
int control_points_num = 0;
vector<glm::vec3> controlPoint;
bool holding = false;
int closestIndex = 0;
bool onDelete = false;
bool onClear = false;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void cursor_pos_callback(GLFWwindow* window, double x, double y);
void click_callback(GLFWwindow* window, int button, int action, int mods);
vector<glm::vec3> Bezier(glm::vec3 point0, glm::vec3 point1, glm::vec3 point2, glm::vec3 point3);
int closest_point(vector<glm::vec3> ctrlPoints, glm::vec3 clickPos);
glm::vec3 standardize(int x, int y);

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
  // Setup ImGui bindings
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
  ImGui_ImplGlfwGL3_Init(window, true);
  ImGui::StyleColorsDark();
  glfwSetCursorPosCallback(window, cursor_pos_callback);
  glfwSetMouseButtonCallback(window, click_callback);
  unsigned int VBO;
  // generate VBO & bind to buffer
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  // create shader with Shader class
  shader.use();

  // create VAO
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  float color[3] = { 1.0f, 1.0f, 1.0f };
  // ---- render loop ----
  while (!glfwWindowShouldClose(window)) {
    vector<vector<float>> triangle;
    // process input from keyboard/mouse/other
    processInput(window);
    glfwPollEvents();
    ImGui_ImplGlfwGL3_NewFrame();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    // init ImGui
    {
      ImGui::ColorEdit3("curve color", color, 0);
      ImGui::Checkbox("on delete", &onDelete);
      ImGui::Checkbox("clear", &onClear);
      ImGui::SetWindowSize(ImVec2(300,100));
      if (onClear) {
        control_points_num = 0;
        controlPoint.clear();
        onClear = false;
      }
      if (control_points_num == 4) {
        vector<glm::vec3> curve = Bezier(controlPoint[0], controlPoint[1], controlPoint[2], controlPoint[3]);
        for (size_t i = 0; i < curve.size(); i++) {
          float point[] = { curve[i].x, curve[i].y, curve[i].z, color[0], color[1], color[2] };
          glBindVertexArray(VAO);
          glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW);
          // position
          glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
          glEnableVertexAttribArray(0);
          //color
          glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
          glEnableVertexAttribArray(1);
          glBindBuffer(GL_ARRAY_BUFFER, VBO);
          glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW);
          glBindBuffer(GL_ARRAY_BUFFER, VBO);
          glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW);
          glPointSize(2.0f);
          glDrawArrays(GL_POINTS, 0, 1);
        }
      }
      for (size_t i = 0; i < controlPoint.size(); i++) {
        float point[] = {
          controlPoint[i].x, controlPoint[i].y, controlPoint[i].z, color[0], color[1], color[2],
        };
        glBindVertexArray(VAO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW);
        // position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        //color
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW);
        glPointSize(2.0f);
        glDrawArrays(GL_POINTS, 0, 4);
      }
    }    
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

void cursor_pos_callback(GLFWwindow* window, double x, double y) {
  bool hovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow);
  cursorPosX = x;
  cursorPosY = y;
  if (holding && control_points_num >= 4 && !hovered) {
    glm::vec3 clickPos = standardize(cursorPosX, cursorPosY);
    controlPoint[closestIndex] = clickPos;
  }
}

void click_callback(GLFWwindow* window, int button, int action, int mods) {
  bool hovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow);
  glm::vec3 clickPos = standardize(cursorPosX, cursorPosY);
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
    holding = true;
  }
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
    holding = false;
  }
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && control_points_num < 4 && !hovered && !onDelete) {
    controlPoint.push_back(standardize(cursorPosX, cursorPosY));
    control_points_num++;
  }
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && control_points_num >= 4 && !hovered && !onDelete) {
    closestIndex = closest_point(controlPoint, clickPos);
  }
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && control_points_num < 4 
      && control_points_num > 0 && !hovered && onDelete) {
    closestIndex = closest_point(controlPoint, clickPos);
    controlPoint.erase(controlPoint.begin()+closestIndex);
    control_points_num--;
  }
}

vector<glm::vec3> Bezier(glm::vec3 point0, glm::vec3 point1, glm::vec3 point2, glm::vec3 point3) {
  vector<glm::vec3> result;
  for (float t = 0; t <= 1; t+= 0.001) {
    float f0 = t * t;
    float f1 = f0 * t;
    float f2 = (1 - t);
    float f3 = f2 * (1 - t);
    float f4 = f3 * (1 - t);
    result.push_back(point0 * f4 + 3 * t * f3 * point1 + 3 * f0 * f2 * point2 + point3 * f1);
  }
  return result;
}

glm::vec3 standardize(int x, int y) {
  glm::vec3 result = glm::vec3((float(x) / float(WINDOW_WIDTH)*2.0)-1 , -((float(y) / float(WINDOW_HEIGHT)*2)-1), 0.0f);
  return result;
}

int closest_point(vector<glm::vec3> ctrlPoints, glm::vec3 clickPos) {
  int flag = -1;
  float minDistance = FLT_MAX;
  for (int i = 0; i < ctrlPoints.size(); i++) {
    float distance = glm::distance(ctrlPoints[i], clickPos);
    if (distance < minDistance) {
      flag = i;
      minDistance = distance;
    }
  }
  return flag;
}