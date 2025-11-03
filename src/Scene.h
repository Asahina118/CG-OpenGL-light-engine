#pragma once

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Camera.h"
#include "Shader.h"
#include "Mesh.h"
#include "InputHandler.h"

class Scene {
public:
	Scene(GLFWwindow*, Camera&, int, int);
	Scene() = delete;
	void render();

private:
	GLFWwindow* window;
	Camera& camera;
	InputHandler input = InputHandler(window, camera);

	const int SCREEN_HEIGHT;
	const int SCREEN_WIDTH;

	const float nearPlane = 0.1f;
	const float farPlane = 500.0f;

	glm::mat4 view;
	glm::mat4 proj;

	std::string shaderDir = "src/Shaders/Advanced/";
	std::string resourceDir = "src/Resources/";


	// render loop
	void renderMeshes();

	// Meshes
	void initMeshes();

	// metal cube
	void initMetalCube();
	void renderMetalCube();
	Mesh metalCube;

	// marble plane
	void initMarblePlane();
	void renderMarblePlane();
	Mesh marblePlane;

	// boilerplates
	void startFrame();
	void endFrame();
	void updateImGuiConfig();
	void initRender();
};