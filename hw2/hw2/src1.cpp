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
vector<float> BresenhamLine(int x0, int y0, int x1, int y1);
vector<float> BresenhamCircle(int x, int y, int radius);

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

  int radioMark = 2;
  int cicleInput[3];
  cicleInput[0] = 0;
  cicleInput[1] = 0;
  cicleInput[2] = 100;
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
      ImGui::RadioButton("Triangle", &radioMark, 0);
      ImGui::RadioButton("Circle", &radioMark, 1);
      ImGui::RadioButton("Rasterize", &radioMark, 2);
      if (radioMark == 0) {
        triangle.push_back(BresenhamLine(0, 150, -100, -100));
        triangle.push_back(BresenhamLine(0, 150, 100, -100));
        triangle.push_back(BresenhamLine(-100, -100, 100, -100));
        shader.use();
        for (size_t i = 0; i < triangle.size(); i++) {
          for (size_t j = 0; j < triangle[i].size(); j += 2) {
            float vertices[] = { triangle[i][j], triangle[i][j + 1], 0.0f, 1.0f, 1.0f, 1.0f };
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
        }
      } else if (radioMark == 1) { 
        ImGui::InputInt3("X,Y,Radius", cicleInput, 0);
        vector<float> circle = BresenhamCircle(cicleInput[0], cicleInput[1], cicleInput[2]);
        for (size_t i = 0; i < circle.size(); i += 2) {
          float vertices[] = { circle[i], circle[i + 1], 0.0f, 1.0f, 1.0f, 1.0f };
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
      } else if (radioMark == 2) {
        triangle.push_back(BresenhamLine(0, 150, -100, -100));
        triangle.push_back(BresenhamLine(0, 150, 100, -100));
        triangle.push_back(BresenhamLine(-100, -100, 100, -100));
        shader.use();
        for (size_t i = 0; i < triangle.size(); i++) {
          for (size_t j = 0; j < triangle[i].size(); j += 2) {
            float vertices[] = { triangle[i][j], triangle[i][j + 1], 0.0f, 1.0f, 1.0f, 1.0f };
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
        }
        // Edge Walking
        for (size_t i = 1; i < triangle[0].size()+1; i+=2) {
          float xi = triangle[0][i-1];
          while (triangle[1][i-1] - xi > 0.00001) {
            float vertices[] = { xi, triangle[0][i], 0.0f, 1.0f, 1.0f, 1.0f };
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

            xi += (float)1.0 / (float)WINDOW_WIDTH;
          }
        }
      }
    } 
    // Copy vertices to buffer

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

vector<float> BresenhamLine(int x0, int y0, int x1, int y1) {
  vector<float> result;
  int deltaX = (x1 >= x0 ? x1 - x0 : x0 - x1);
  int deltaY = (y1 >= y0 ? y1 - y0 : y0 - y1);
  int p = 2 * deltaY - deltaX;
  int xi = x0, yi = y0;
  int xinc, yinc;
  xinc = (x1 >= x0 ? 1 : -1);
  yinc = (y1 >= y0 ? 1 : -1);
  result.push_back((float)xi / (float)WINDOW_WIDTH * 2);
  result.push_back((float)yi / (float)WINDOW_HEIGHT * 2);
  if (deltaX > deltaY) {
    while (xi != x1) {
      if (p <= 0) {
        p = p + 2 * deltaY;
      } else {
        yi += yinc;
        p = p + 2 * deltaY - 2 * deltaX;
      }
      xi += xinc;
      result.push_back((float)xi / (float)WINDOW_WIDTH * 2);
      result.push_back((float)yi / (float)WINDOW_HEIGHT * 2);
    }
  } else {
    while (yi != y1) {
      if (p <= 0) {
        p = p + 2 * deltaX;
      } else {
        xi += xinc;
        p = p + 2 * deltaX - 2 * deltaY;
      }
      yi += yinc;
      result.push_back((float)xi / (float)WINDOW_WIDTH * 2);
      result.push_back((float)yi / (float)WINDOW_HEIGHT * 2);
    }
  }
  //success
  return result;
}

vector<float> BresenhamCircle(int x, int y, int radius) {
  vector<float> result;
  int xi = 0, yi = radius, rec = 0;
  int d = 5 - 4 * radius;
  //1
  result.push_back((float)(xi + x) / (float)WINDOW_WIDTH * 2);
  result.push_back((float)(yi + y) / (float)WINDOW_HEIGHT * 2);
  //2
  result.push_back((float)(-xi + x) / (float)WINDOW_WIDTH * 2);
  result.push_back((float)(yi + y) / (float)WINDOW_HEIGHT * 2);
  //3
  result.push_back((float)(xi + x)/ (float)WINDOW_WIDTH * 2);
  result.push_back((float)(-yi + y) / (float)WINDOW_HEIGHT * 2);
  //4
  result.push_back((float)(-xi + x) / (float)WINDOW_WIDTH * 2);
  result.push_back((float)(-yi + y) / (float)WINDOW_HEIGHT * 2);
  //5
  result.push_back((float)(yi + x) / (float)WINDOW_WIDTH * 2);
  result.push_back((float)(xi + y) / (float)WINDOW_HEIGHT * 2);
  //6
  result.push_back((float)(-yi + x) / (float)WINDOW_WIDTH * 2);
  result.push_back((float)(xi + y) / (float)WINDOW_HEIGHT * 2);
  //7
  result.push_back((float)(yi + x) / (float)WINDOW_WIDTH * 2);
  result.push_back((float)(-xi + y) / (float)WINDOW_HEIGHT * 2);
  //8
  result.push_back((float)(-yi + x) / (float)WINDOW_WIDTH * 2);
  result.push_back((float)(-xi + y) / (float)WINDOW_HEIGHT * 2);
  while (xi <= yi) {
    if (d <= 0) {
      d += 8 * xi + 12;
    } else {
      d += 8 * (xi - yi) + 20;
      yi--;
    }
    xi++;
    //1
    result.push_back((float)(xi + x) / (float)WINDOW_WIDTH *2);
    result.push_back((float)(yi + y) / (float)WINDOW_HEIGHT *2);
    //2
    result.push_back((float)(-xi + x) / (float)WINDOW_WIDTH *2);
    result.push_back((float)(yi + y) / (float)WINDOW_HEIGHT *2);
    //3
    result.push_back((float)(xi + x) / (float)WINDOW_WIDTH *2);
    result.push_back((float)(-yi + y) / (float)WINDOW_HEIGHT *2);
    //4
    result.push_back((float)(-xi + x) / (float)WINDOW_WIDTH *2);
    result.push_back((float)(-yi + y) / (float)WINDOW_HEIGHT *2);
    //5
    result.push_back((float)(yi + x) / (float)WINDOW_WIDTH *2);
    result.push_back((float)(xi + y) / (float)WINDOW_HEIGHT *2);
    //6
    result.push_back((float)(-yi + x) / (float)WINDOW_WIDTH *2);
    result.push_back((float)(xi + y) / (float)WINDOW_HEIGHT *2);
    //7
    result.push_back((float)(yi + x) / (float)WINDOW_WIDTH *2);
    result.push_back((float)(-xi + y) / (float)WINDOW_HEIGHT *2);
    //8
    result.push_back((float)(-yi + x) / (float)WINDOW_WIDTH *2);
    result.push_back((float)(-xi + y) / (float)WINDOW_HEIGHT *2);
  }
  return result;
}