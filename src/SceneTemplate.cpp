#include "SceneTemplate.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <map>

#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include "Shader.h"
#include "Camera.h"
#include "InputHandler.h"
#include "GUI.h"
#include "Model.h"

#include <assimp/Importer.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Dependencies/stb_image.h"

void SceneTemplate::render()
{
    initRender();

	while (!glfwWindowShouldClose(window)) {
        startFrame();
        testRender();
		updateImGuiConfig();	// NOTE : take it out for inheritance convenience
        endFrame();
	}
}






#pragma region boilerplates

void SceneTemplate::updateImGuiConfig() 
{
    ImGui::Begin("Configs.");
    ImGui::Text("Innocent Grey fanboy");

    std::string posString = "pos: " + toString(camera.position);
    std::string frontString = " facing " + toString(camera.front);
    ImGui::Text((posString + frontString).c_str());

    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


void SceneTemplate::initRender()
{
    GUI::ImGuiInit(window);
}

void SceneTemplate::startFrame()
{
    GUI::ImGuiNewFrame();
	input.processBasicInput(&view);
	proj = glm::perspective(glm::radians(camera.FOV), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, nearPlane, farPlane);
}

void SceneTemplate::endFrame()
{
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void SceneTemplate::testRender()
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

std::string SceneTemplate::toString(glm::vec3 vec)
{
    std::string result = "(";
    int length = 3;
    for (int i = 0; i < length; i++) {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << vec[i];
        result += ss.str();
        if (i != length - 1) result += ", ";
        else result += ")";
    }
    return result;
}


SceneTemplate::SceneTemplate(GLFWwindow* window, Camera& camera, int height, int width) : window(window), camera(camera), SCREEN_HEIGHT(height), SCREEN_WIDTH(width) {};
#pragma endregion
/* quite a few bugs here :
1. using post processing now breaks the buffers. Need to investigate that further later
2. adding backpack was unsuccessful
3. the highlight effects cannot penetrate thorugh the reflective/refractive cube
*/