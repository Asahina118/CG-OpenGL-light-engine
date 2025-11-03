#include "InputHandler.h"

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Camera.h"

InputHandler::InputHandler(GLFWwindow* window, Camera& camera) : window(window), camera(camera) {}

void InputHandler::processBasicInput(glm::mat4* view)
{
    processChangeMouseInput();

    if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
        processCamera();
        *view = camera.getViewMatrix();
    }
}

void InputHandler::processCamera() {
    float dTime = 0.001f;

    // WASD Movement
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Sprinting (Toggle)
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        if (!toggleLockSprinting) {
            toggleLockSprinting = true;
            if (camera.sprinting == 1.0f) {
                camera.sprinting = 5.0f;
            }
            else {
                camera.sprinting = 1.0f;
            }

        }
    }
    else {
        toggleLockSprinting = false;
    }

    // Front-back movement
    float yPos = camera.position.y;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.processKeyboard(FORWARD, dTime);
        camera.position.y = yPos;
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.processKeyboard(BACKWARD, dTime);
        camera.position.y = yPos;
    }

    // Left-right movement
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.processKeyboard(LEFT, dTime);
    }
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.processKeyboard(RIGHT, dTime);
    }

    // Up-down movement
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        camera.processKeyboard(UP, dTime);
    }
    else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        camera.processKeyboard(DOWN, dTime);
    }

    // Toggle flashlight
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
        if (!toggleLockFlashlight) {
            camera.enableFlashlight = (camera.enableFlashlight) ? false : true;
            toggleLockFlashlight = true;
        }
    }
    else {
        toggleLockFlashlight = false;
    }


}

void InputHandler::processChangeMouseInput()
{
    if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS) {
        if (!toggleLockChangeMouseInput) {
            toggleLockChangeMouseInput = true;
            if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL) {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                camera.stopUpdating = false;

                // firstMouse couldnt solve the cursor jump
                camera.theta = tempTheta;
                camera.phi = tempPhi;
            }
            else {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                camera.stopUpdating = true;

                // firstMouse couldnt solve the cursor jump
                tempTheta = camera.theta;
                tempPhi = camera.phi;
            }

        }
    }
    else {
        toggleLockChangeMouseInput = false;
    }
}