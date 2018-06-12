#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"
#include "shader.h"
#include "camera.h"
#include "CImg.h"
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;
#define WINDOW_WIDTH  1600  
#define WINDOW_HEIGHT 800
Camera camera(glm::vec3(0.0f, 1.5f, 8.0f));
bool firstMouse = true;
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float lastX = (float)WINDOW_WIDTH / 2.0;
float lastY = (float)WINDOW_HEIGHT / 2.0;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
vector<vector<int>> readMap(const char *path);
vector<vector<int>> m_textures;
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
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad init
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}

	// Setup ImGui bindings
	/*
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	ImGui_ImplGlfwGL3_Init(window, true);
	ImGui::StyleColorsDark();
	*/
	Shader shader("shader/vShaderSrc.txt", "shader/fShaderSrc.txt");

	vector<vector<int>> map = readMap("texture/map3.bmp");
	int size = map.size() * map[0].size();
	float *vertices = new float[size * 8];
	for (int i = 0, j = 0, k = 0; i < size * 8; i += 8) {
		vertices[i] = (float)j / (float)map[0].size();
		vertices[i + 1] = (float)k / (float)map.size();
		vertices[i + 2] = (float)map[j][k] / 256.0f / 8;
		vertices[i + 3] = 1.0f;
		vertices[i + 4] = 1.0f;
		vertices[i + 5] = 1.0f;
		vertices[i + 6] = (float)j / (float)map[0].size();
		vertices[i + 7] = (float)k / (float)map.size();
		//cout << vertices[i] << "/" << vertices[i+1] << "/" << vertices[i+2] << "/" << vertices[i+3] << "/" << vertices[i+4] << "/" << vertices[i+5] << endl;
		k++;
		if (k >= map[0].size()) {
			k = 0;
			j++;
		}
	}
	unsigned int *triangleIndices = new unsigned int[size * 6];
	int count = 0;
	for (int i = 0; i < size - map[0].size(); i++) {
		if (i % map[0].size() == map[0].size() - 1) {
			continue;
		}
		triangleIndices[count] = i;
		triangleIndices[count + 1] = i + 1;
		triangleIndices[count + 2] = i + map[0].size();
		triangleIndices[count + 3] = i + 1;
		triangleIndices[count + 4] = i + map[0].size();
		triangleIndices[count + 5] = i + map[0].size() + 1;
		//cout << triangleIndices[count] << "/" << triangleIndices[count + 1] << "/" << triangleIndices[count + 2] << endl;
		count += 6;
	}
	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferStorage(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*size * 8, vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*(size * 6), triangleIndices, GL_STATIC_DRAW);
	// position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//texture
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	glm::mat4 view;
	bool depth = true;
	int taskMark = 1;
	int mark2 = 0;
	float temp = 0.0f;
	float save = 0.0f;
	int sign = 0;
	float trans[3] = { -1.5f, 0.5f, -1.5f };
	float perspec[4] = { 20.0f, 1.0f, 0.1f, 100.0f };
	float ortho[6] = { -2.0f, 2.0f, -2.0f, 2.0f, 1.0f, 100.0f };

	//ÌùÍ¼²¿·Ö
	int width, height, nrChannels;
	unsigned char *data = stbi_load("texture/sand_texture.jpg", &width, &height, &nrChannels, 0);

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	//glEnable(GL_DEPTH_TEST);
	// ---- render loop ----
	while (!glfwWindowShouldClose(window)) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glm::mat4 model(1);
		glm::mat4 view(1);
		glm::mat4 projection(1);

		// process input from keyboard/mouse/other
		processInput(window);

		// init ImGui
		//ImGui_ImplGlfwGL3_NewFrame();



		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);

		shader.use();
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		projection = glm::perspective(camera.Zoom, (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.01f, 100.0f);
		view = camera.GetViewMatrix();
		//projection = glm::perspective(180.0f, 1.0f, 0.01f, 100.0f);
		shader.setVec3("viewPos", camera.Position.x, camera.Position.y, camera.Position.z);
		int modelLoc = glGetUniformLocation(shader.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		int viewLoc = glGetUniformLocation(shader.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		int proLoc = glGetUniformLocation(shader.ID, "projection");
		glUniformMatrix4fv(proLoc, 1, GL_FALSE, glm::value_ptr(projection));

		

		//glDrawArrays(GL_TRIANGLES, 0, );
		glBindTexture(GL_TEXTURE_2D, texture);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, size * 10, GL_UNSIGNED_INT, 0);

		// check out triggerations & render

		//ImGui::Render();
		//ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
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
	float cameraSpeed = 2.5 * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.ProcessKeyboard(RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;
	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.ProcessMouseScroll(yoffset);
}


vector<vector<int>> readMap(const char *path) {
	cimg_library::CImg<unsigned char> img(path);
	//img.display();
	int width = img.width();
	int height = img.height();
	vector<int> row(height, 0);
	vector<vector<int>> map(width, row);
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			map[i][j] = (int)img(i, j, 0);
		}
	}
	return map;
}