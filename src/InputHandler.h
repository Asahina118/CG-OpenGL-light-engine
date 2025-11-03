#pragma once

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Camera.h"

class InputHandler {
public:
	InputHandler(GLFWwindow*, Camera&);
	void processBasicInput(glm::mat4* view);

private:
	void processCamera();
	void processChangeMouseInput();
	GLFWwindow* window;
	Camera& camera;

	bool toggleLockSprinting;
	bool toggleLockFlashlight;
	bool toggleLockChangeMouseInput;
	float tempTheta;
	float tempPhi;
};