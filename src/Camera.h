#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

// extension note : Ctrl + Alt + ] (also works in vim mode)
// default values
const float THETA = 90.0f;
const float PHI = 0.0f;
const float SPEED = 20.0f;
const float MOUSE_SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;


class Camera {
public:
	// frames
	glm::vec3 position;
	glm::vec3 front ;
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 right = glm::vec3(-1.0f, 0.0f, 0.0f);
	glm::vec3 worldup;

	// Euler angles
	float theta = THETA;
	float phi = PHI;

	// Camera Options
	float movementSpeed;
	float mouseSensitivity;
	float zoom;
	float sprinting = 1.0;	// times movement speed directly
	float FOV = 60.0f;
	bool firstMouse = true;
	bool enableFlashlight = false;
	bool stopUpdating = false;

	// Vector initialization
	Camera(
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
		float theta = THETA,
		float phi = PHI
	);

	glm::mat4 getViewMatrix();

	void processKeyboard(Camera_Movement direction, float deltaTime);

	void processMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch = true);

	void processMouseScroll(float yOffset);

private:
	void updateCameraVectors();
};


#endif
