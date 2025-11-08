#include <iostream>

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "Dependencies/stb_image.h"
#endif

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Camera.h"
#include "Model.h"
#include "Scene.h"
#include "SceneTemplate.h"

#include "lighting.h"

// model loading
#include "renderModel.h"

const int WINDOW_WIDTH = 1920;
const int WINDOW_HEIGHT = 1080;

Camera camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
static float lastX = WINDOW_WIDTH / 2;
static float lastY = WINDOW_HEIGHT / 2;

float dTime = 0.0f;
float lastFrame = 0.0f;

static void mouse_callback(GLFWwindow* window, double xPos, double yPos) 
{

	float dx = xPos - lastX;
	float dy = lastY - yPos; // reversed for OpenGL / GLFW mismatch

	lastX = xPos;
	lastY = yPos;

	camera.processMouseMovement(dx, dy);
}

void windowFocus_callback(GLFWwindow* window, int focused) 
{
	if (focused) camera.firstMouse = true;
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset) 
{
	if (camera.stopUpdating) return;
	camera.FOV -= 3.0f*(float)yOffset;
	if (camera.FOV < 1.0f) camera.FOV = 1.0f;
	//if (camera.FOV > 90.0f) camera.FOV = 90.0f;
}

GLFWwindow* glfwWindowInit(const char* name) 
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, name, NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return NULL;
	}
	glfwMakeContextCurrent(window);

	// Initialize function pointer for OpenGL from GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return NULL;
	}

	return window;
}

int main() 
{
	int OPTION = 4;

	GLFWwindow* window = glfwWindowInit("Init");
	if (!window) {
		std::cerr << "[FAILURE] window initialzation failed" << std::endl;
		return -1;
	}

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetWindowFocusCallback(window, windowFocus_callback);    
	glfwSetScrollCallback(window, scroll_callback);

	if (OPTION == 1) {
		Lighting* engine = new Lighting(window, camera);
		engine->render();
	}
	else if (OPTION == 2) {
		renderModel renderModel(window, camera, WINDOW_HEIGHT, WINDOW_WIDTH);
		renderModel.render();
	}
	else if (OPTION == 3) {
		Scene scene(window, camera, WINDOW_HEIGHT, WINDOW_WIDTH);
		scene.render();
	}
	else if (OPTION == 4) {
		SceneTemplate scene(window, camera, WINDOW_HEIGHT, WINDOW_WIDTH);
		scene.render();
	}
	
}