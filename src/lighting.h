#pragma once

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Camera.h"
#include "Shader.h"


class Lighting {
public:
	Lighting(GLFWwindow* window, Camera& camera) : window(window), camera(camera)  {};
	void render();

private:
	const int SCREEN_WIDTH  = 1200;
	const int SCREEN_HEIGHT = 850;

	const int PI = 3.14159265358979;


	// Configurable Variables in ImGui (if i cared to implement)
	// NOTE : Avoid making other variables not in the below list to be configurable.	
	// Engine Configurable
	float size        = 1.0f;
	float nearPlane   = 0.1f;
	float farPlane    = 500.0f;

	// Lighting Congfigs
	float ambientStrength  = 0.1f;
	float specularStrength = 0.5f;

	// Light Source Configurable
	float lightColorR = 1.0f;
	float lightColorG = 1.0f;
	float lightColorB = 1.0f;

	float lightPosX = 0.0f;
	float lightPosY = 0.0f;
	float lightPosZ = 0.0f;

	float lightSourceRadius        = 1.5f;
	float lightSourceRotationSpeed = 1.5f;
	float lightSourceHeight        = 0.5f;

	// 0 - 4 corresponding to the lookup table in the init function
	int attenuationParamOption = 2;

	bool rainbowColor = false;

	// Simple Cube Configurable
	float simpleCubeColorR = 1.0f;
	float simpleCubeColorG = 0.5f;
	float simpleCubeColorB = 0.31f;

	int shininess = 32;
	bool simpleCubeRotation = true;

	// Flashlight properties
	glm::vec3 flashlightColor = glm::vec3(1.0f);
	float flashlightCutOff = 0.2f;
	bool toggleLockFlashlight = false;
	// end of Configurables =============


	// matrices
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 proj = glm::perspective(glm::radians(60.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, nearPlane, farPlane);

	// Objs
	Camera& camera;
	GLFWwindow* window;

	void VAOInit(unsigned int*, unsigned int*, unsigned int*);

	// Input processing
	void processInputs();
	void processCamera();
	bool toggleLockSprinting = false;

	void processChangeMouseInput();
	bool toggleLockChangeMouseInput = false;
	float tempTheta = 0.0f;
	float tempPhi = 0.0f;

	// object init func
	void simpleCubeInit(Shader*);
	glm::mat4 modelSimpleCube;

	// simpleCube material properties
	glm::vec3 simpleCubeDiffuse;
	glm::vec3 simpleCubeAmbient;
	glm::vec3 simpleCubeSpecular;
	float simpleCubeShininess;

	unsigned int simpleCubeTexture;
	unsigned int simpleCubeTextureSpecular;
	unsigned int simpleCubeEmissionTexture;

	// lighting source
	void lightingSourceInit();
	glm::mat4 modelLightSource;
	glm::vec3 lightSourceAmbient;
	glm::vec3 lightSourceDiffuse;
	glm::vec3 lightSourceSpecular;

	// textureBackground
	glm::mat4 textureBackgroundInit(Shader*, unsigned int&);

	// render func
	void renderSimpleCube(Shader*, unsigned int*);
	void renderLightSource(Shader*, unsigned int*);
	void renderTextureBackground(Shader*, unsigned int& textureVAO, unsigned int& texture, glm::mat4 model);

	// ImGui func
	// Only update Configurable variables 
	void updateImguiConfig();
	void ImGuiNewFrame();
	void ImGuiInit(GLFWwindow*);
	
};
