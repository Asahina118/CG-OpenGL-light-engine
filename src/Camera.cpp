#include "Camera.h"
#include <iostream>
Camera::Camera(
	glm::vec3 position,
	glm::vec3 worldup,
	float theta,
	float phi
) :
	front(glm::vec3(0.0f, 0.0f, 1.0f)),
	movementSpeed(SPEED),
	mouseSensitivity(MOUSE_SENSITIVITY),
	zoom(ZOOM)
{
	this->position = position;
	this->worldup = worldup;

	// default camera front.
	this->theta = theta;
	this->phi = phi;

	updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(position, position + front, up);
}

void Camera::processKeyboard(Camera_Movement direction, float deltaTime)
{
	float velocity = sprinting * movementSpeed * deltaTime;

	if (direction == FORWARD)
		position += front * velocity;

	if (direction == BACKWARD)
		position -= front * velocity;

	if (direction == RIGHT)
		position += right * velocity;

	if (direction == LEFT)
		position -= right * velocity;

	if (direction == UP)
		position += worldup * velocity;

	if (direction == DOWN)
		position -= worldup * velocity;

}

void Camera::processMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch)
{
	if (stopUpdating) return;

	// firstMouse is field member
	if (firstMouse) {
		xOffset = 0;
		yOffset = 0;
		firstMouse = false;
	}

	xOffset *= mouseSensitivity;
	yOffset *= mouseSensitivity;

	theta += xOffset;
	phi += yOffset;

	if (constrainPitch) {
		if (phi > 89.0f) phi = 89.0f;
		if (phi < -89.0f) phi = -89.0f;
	}

	updateCameraVectors();
}

void Camera::processMouseScroll(float yOffset)
{
	if (stopUpdating) return;

	zoom -= (float)yOffset;
	if (zoom < 1.0f) zoom = 1.0f;
	if (zoom > 45.0f) zoom = 45.0f;
}


void Camera::updateCameraVectors()
{
	glm::vec3 frontNew;
	frontNew.x = cos(glm::radians(theta)) * cos(glm::radians(phi));
	frontNew.y = sin(glm::radians(phi));
	frontNew.z = sin(glm::radians(theta)) * cos(glm::radians(phi));
	this->front = glm::normalize(frontNew);

	this->right = glm::normalize(glm::cross(front, worldup));
	this->up = glm::normalize(glm::cross(right, front));
}
