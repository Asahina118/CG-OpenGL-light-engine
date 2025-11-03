#pragma once

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Camera.h"
#include "Shader.h"
#include "Model.h"

// used for debugging
struct Cube {
	unsigned int VAO;
	Shader shader = Shader("src/Shaders/renderModel/testing.vs", "src/Shaders/renderModel/testing.fs");
	glm::mat4 model;

	unsigned int textureDiffuse;
	unsigned int textureSpecular;
};

struct renderUnit {
	unsigned int VAO, EBO;
	Shader shader;
	glm::mat4 model;

	glm::vec3 diffuse;
	glm::vec3 specular;
};

class renderModel {
public:
	renderModel(GLFWwindow* window, Camera& camera, int SCREEN_HEIGHT, int SCREEN_WIDTH);
	renderModel() = delete;
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

	std::string shaderDir = "src/Shaders/renderModel/";

	// Configurables
	glm::vec3 modelPos = glm::vec3(1.0f, 1.0f, 1.0f);


	// render units ================

	// backpack:
	glm::mat4 backpackModel = glm::mat4(1.0f);
	Model backpack;
	Shader backpackShader;
	void backpackInit();
	void renderBackpack(Shader&, Model&);

	float materialShininess = 2.0f;

	// lightsource:
	renderUnit lightSource;
	glm::vec4 lightPos;
	void lightSourceInit();
	void renderLightSource(Shader&);

	float lightSourceRotationSpeed = 1.5f;
	float lightSourceRadius = 1.5f;
	float lightSourceHeight = 0.5f;

	bool turnOffLightSource = false;

	// render units END ============


	// light properties ============

	// directional light
	glm::vec3 dirLightDirection = glm::vec3(0.0f, -1.0f, 1.0f);
	glm::vec3 dirLightAmbient;
	glm::vec3 dirLightDiffuse = glm::vec3(0.5f);
	glm::vec3 dirLightSpecular = glm::vec3(0.1f);

	// flashlight
	glm::vec3 flashlightColor = glm::vec3(1.0f);
	float flashlightCutOff = 5.0f;		// in degrees
	int flashlightOuterCutOff = 2;
	// light properties END ========

	// keyboard mouse controls (also need to modularize later)
	void processInputs();
	void processCamera();
	void processChangeMouseInput();

	// ImGui (need to modularize ImGui and common keyboard control later)
	void ImGuiNewFrame();
	void ImGuiInit();
	void updateImGuiConfig();

	// toggle locks
	bool toggleLockSprinting = false;
	bool toggleLockFlashlight = false;
	bool toggleLockChangeMouseInput = false;

	// avoid camera jump after interacting with ImGui
	float tempTheta;
	float tempPhi;

	// loading textures quickly
	void loadTextureContainer(unsigned int* textureDiffuse, unsigned int* textureSpecular);

	// ======= debug functions ======
	// functions for testing
	void testingCubeInit();
	void renderTestingCube();
	Cube testingCube;
};