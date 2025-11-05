#include "Scene.h"

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

#include <assimp/Importer.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


void Scene::render()
{
    initRender();
    initMeshes();

	while (!glfwWindowShouldClose(window)) {
        if (highlightObject) renderHighlightObject();
        else simpleRender();
	}
}

void Scene::initMeshes()
{
    initCube();
    initPlane();
    initGrass();
    initGlass();
}


void Scene::initCube()
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
    cube = Mesh(cubeVertices, size);
    cube.textureInit(resourceDir + "metal.png", "texture_diffuse");
    cube.shader = Shader(shaderDir + "vertex.vs", shaderDir + "metalCube.fs");
}

void Scene::initPlane()
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
    plane = Mesh(planeVertices, size);
    plane.textureInit(resourceDir + "marble.jpg", "texture_diffuse");
    plane.shader = Shader(shaderDir + "vertex.vs", shaderDir + "marblePlane.fs");
}

void Scene::initGrass()
{
    float cubeVertices[] = { // positions          // normals           // texture coords
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
    grass = Mesh(cubeVertices, size);
    grass.textureInit(resourceDir + "grass.png", "texture_diffuse", GL_CLAMP_TO_EDGE);
    grass.shader = Shader(vertexDir, shaderDir + "grass.fs");
}

void Scene::initGlass()
{
    float cubeVertices[] = { // positions          // normals           // texture coords
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
    glass = Mesh(cubeVertices, size);
    glass.textureInit(resourceDir + "blending_transparent_window.png", "texture_diffuse", GL_CLAMP_TO_EDGE);
    glass.shader = Shader(vertexDir, shaderDir + "glass.fs");
}

void Scene::renderCube()
{
    // 1st cube
	cube.shader.use();
	cube.shader.setMat4("trans", proj * view);

    // vs
    cube.model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.01f, -1.0f));
	cube.shader.setMat4("model", cube.model);

    // fs 
    
	cube.drawArr(36);


    // 2nd cube
    cube.model = glm::translate(cube.model, glm::vec3(2.0f, 0.01f, 0.0f));
	cube.shader.setMat4("model", cube.model);

	cube.drawArr(36);
}

void Scene::renderPlane()
{
    plane.shader.use();

    // vs
    glm::mat4 model(1.0f);
    plane.shader.setMat4("model", model);
	plane.shader.setMat4("trans", proj * view);

    // fs

    plane.drawArr(6);
}

void Scene::renderGrass()
{
    std::vector<glm::vec3> locations;
    locations.push_back(glm::vec3(-1.5f, 0.0f, -0.48f));
    locations.push_back(glm::vec3(1.5f, 0.0f, 0.51f));
    locations.push_back(glm::vec3(0.0f, 0.0f, 0.7f));
    locations.push_back(glm::vec3(-0.3f, 0.0f, -2.3f));
    locations.push_back(glm::vec3(0.5f, 0.0f, -0.6f));

    for (glm::vec3 location : locations) {
        grass.shader.use();
        grass.shader.setMat4("trans", proj * view);
        grass.shader.setMat4("model", glm::translate(glm::mat4(1.0f), location));
        grass.drawArr(6);
    }
}

void Scene::renderGlass()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    std::vector<glm::vec3> locations;
    locations.push_back(glm::vec3(-1.5f, 0.0f, -0.48f));
    locations.push_back(glm::vec3(1.5f, 0.0f, 0.51f));
    locations.push_back(glm::vec3(0.0f, 0.0f, 0.7f));
    locations.push_back(glm::vec3(-0.3f, 0.0f, -2.3f));
    locations.push_back(glm::vec3(0.5f, 0.0f, -0.6f));

    // NOTE : render the glasses from back to front to prevent failure of depth buffer testing from discarding the fragments
    std::map<float, glm::vec3> sorted;
    for (glm::vec3 location : locations) {
        float distance = glm::length(location - camera.position);
        sorted[distance] = location;
    }

    for ( auto it = sorted.rbegin() ; it != sorted.rend(); it++ ) {
        glm::vec3 location = it->second;
        location.z += 0.1f;
        glass.shader.use();
        glass.shader.setMat4("trans", proj * view);
        glass.shader.setMat4("model", glm::translate(glm::mat4(1.0f), location));
        glass.drawArr(6);
    }
}



void Scene::updateImGuiConfig() 
{
    ImGui::Begin("Configs.");
    ImGui::Text("Innocent Grey fanboy");

    std::string posString = "pos: " + toString(camera.position);
    std::string frontString = " facing " + toString(camera.front);
    ImGui::Text((posString + frontString).c_str());

    ImGui::Checkbox("highlight", &highlightObject);
    ImGui::Checkbox("highlight borders only", &highlightBorderOnly);
    ImGui::ColorEdit3("highlight color", &highlightColor[0]);

    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
};



#pragma region renders

void Scene::simpleRender()
{
    // opague
    startFrame();
    renderCube();
    renderPlane();
    renderGrass();

	// transparent
    renderGlass();
    endFrame();
}

// can pack into the Mesh class
void Scene::renderHighlightObject()
{
    startFrame();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);

    glStencilMask(0x00);
    renderPlane();

    GLenum highlightMode = highlightBorderOnly ? GL_REPLACE : GL_KEEP;
    glStencilOp(GL_KEEP, highlightMode, highlightMode);

	glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);
    renderCube();

    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00);
    glDisable(GL_DEPTH_TEST);	// highlight color "penetrates" all objects in front

    highlightShader.use();
    highlightShader.setMat4("trans", proj * view);

    highlightShader.setMat4("model", glm::scale(cube.model, glm::vec3(1.05f)));
    highlightShader.setVec3("color", highlightColor);
    glBindVertexArray(cube.VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glm::mat4 secondModel = glm::translate(cube.model, glm::vec3(-2.0f, 0.0f, 0.0f));
    highlightShader.setMat4("model", glm::scale(secondModel, glm::vec3(1.05f)));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glEnable(GL_DEPTH_TEST);

    renderGrass();
    renderGlass();

    endFrame();
}


#pragma endregion



#pragma region boilerplates
void Scene::initRender()
{
	glEnable(GL_DEPTH_TEST);
    GUI::ImGuiInit(window);
}

void Scene::startFrame()
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
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

std::string Scene::toString(glm::vec3 vec)
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

Scene::Scene(GLFWwindow* window, Camera& camera, int height, int width) : window(window), camera(camera), SCREEN_HEIGHT(height), SCREEN_WIDTH(width) {};
#pragma endregion
