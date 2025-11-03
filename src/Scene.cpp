#include "Scene.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <assimp/Importer.hpp>

#include "Shader.h"
#include "Camera.h"
#include "InputHandler.h"
#include "GUI.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


void Scene::render()
{
    initRender();
    initMeshes();

	while (!glfwWindowShouldClose(window)) {
        startFrame();
        renderMeshes();
        endFrame();
	}
}

void Scene::initMeshes()
{
    initMetalCube();
    initMarblePlane();
}


void Scene::initMetalCube()
{
    float cubeVertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };
    unsigned size = sizeof(cubeVertices) / sizeof(float);
    metalCube = Mesh(cubeVertices, size);
    metalCube.textureInit(resourceDir + "metal.png", "texture_diffuse");
    metalCube.shader = Shader(shaderDir + "vertex.vs", shaderDir + "metalCube.fs");
}

void Scene::initMarblePlane()
{
    float planeVertices[] = {
         5.0f, -0.5f,  5.0f,  0.0f,  0.0f, -1.0f,  2.0f, 0.0f,
        -5.0f, -0.5f,  5.0f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f,  0.0f, -1.0f,  0.0f, 2.0f,

         5.0f, -0.5f,  5.0f,  0.0f,  0.0f, -1.0f,  2.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f,  0.0f, -1.0f,  0.0f, 2.0f,
         5.0f, -0.5f, -5.0f,  0.0f,  0.0f, -1.0f,  2.0f, 2.0f
    };
    unsigned size = sizeof(planeVertices) / sizeof(float);
    marblePlane = Mesh(planeVertices, size);
    marblePlane.textureInit(resourceDir + "marble.jpg", "texture_diffuse");
    marblePlane.shader = Shader(shaderDir + "vertex.vs", shaderDir + "marblePlane.fs");
}

void Scene::renderMeshes()
{
    renderMetalCube();
    renderMarblePlane();
}

void Scene::renderMetalCube()
{
    // 1st cube
	metalCube.shader.use();
	metalCube.shader.setMat4("trans", proj * view);

    // vs
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.0f, -1.0f));
	metalCube.shader.setMat4("model", model);

    // fs 
    
	metalCube.drawArr(36);


    // 2nd cube
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
	metalCube.shader.setMat4("model", model);

    metalCube.drawArr(36);
}


void Scene::renderMarblePlane()
{
    marblePlane.shader.use();

    // vs
    //glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, -1.0f));
    glm::mat4 model(1.0f);
    marblePlane.shader.setMat4("model", model);
	metalCube.shader.setMat4("trans", proj * view);

    // fs

    marblePlane.drawArr(6);
}





void Scene::updateImGuiConfig() 
{
    ImGui::Begin("Configs.");
    ImGui::Text("Innocent Grey fanboy");



    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
};








// boilerplates
void Scene::initRender()
{
	glEnable(GL_DEPTH_TEST);
    GUI::ImGuiInit(window);
}

void Scene::startFrame()
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GUI::ImGuiNewFrame();
	input.processBasicInput(&view);
	proj = glm::perspective(glm::radians(camera.FOV), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, nearPlane, farPlane);
}

void Scene::endFrame()
{
    updateImGuiConfig();
	glfwSwapBuffers(window);
	glfwPollEvents();
}

Scene::Scene(GLFWwindow* window, Camera& camera, int height, int width) : window(window), camera(camera), SCREEN_HEIGHT(height), SCREEN_WIDTH(width) {};

