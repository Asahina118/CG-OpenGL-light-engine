#pragma once

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Camera.h"
#include "Shader.h"

class Scene {
public:
	Scene(GLFWwindow*, Camera&, int, int);
	Scene() = delete;
	void render();

private:
	GLFWwindow* window;
	Camera& camera;

	const int SCREEN_HEIGHT;
	const int SCREEN_WIDTH;

	const float nearPlane = 0.1f;
	const float farPlane = 500.0f;

	glm::mat4 view;
	glm::mat4 proj;

	std::string shaderDir = "src/Shaders/Advanced/";

	void initCube();
	unsigned int cubeVAO;
};