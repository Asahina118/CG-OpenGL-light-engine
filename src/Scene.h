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
	std::string vertexDir = shaderDir + "vertex.vs";

	// shaders
	Shader highlightShader = Shader(vertexDir, shaderDir + "singleColor.fs");

	// render
	void simpleRender();
	void renderHighlightObject();
	bool highlightObject = false;
	bool highlightBorderOnly = true;
	glm::vec3 highlightColor = glm::vec3(0.42f, 0.26f, 0.72f);

	// Meshes
	void initMeshes();

	// metal cube
	void initCube();
	void renderCube();
	Mesh cube;

	// marble plane
	void initPlane();
	void renderPlane();
	Mesh plane;

	// boilerplates
	void startFrame();
	void endFrame();
	void updateImGuiConfig();
	void initRender();

	// helper functions
	std::string toString(glm::vec3);
};