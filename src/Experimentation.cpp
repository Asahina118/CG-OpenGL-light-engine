#include "Experimentation.h"

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

void Experimentation::render()
{

	initRender();	
    initMeshes();

	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window)) {
		startFrame();
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderDepthMap();

		endFrame();
	}
}

void Experimentation::initMeshes()
{
    initPlane();
    initCube();
    initQuad();
    initDepthMap();
}


#pragma region init

void Experimentation::initPlane() 
{
    plane = Mesh(planeVertices.data(), planeVertices.size());
}

void Experimentation::initCube()
{
    cube = Mesh(cubeVerticesVec.data(), cubeVerticesVec.size());
}

void Experimentation::initQuad()
{
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, quadVertices.size() * sizeof(float), quadVertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

	quadShader = Shader(shaderDir + "quad.vs", shaderDir + "quad.fs");
    quadShader.use();
    quadShader.setInt("depthMap", 0);
}



void Experimentation::initDepthMap()
{
    glGenFramebuffers(1, &depthMapFBO);
    // create depth texture
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    depthShader = Shader(shaderDir + "depth.vs", shaderDir + "depth.fs");
}

#pragma endregion


#pragma region render

void Experimentation::renderQuad()
{
    quadShader.use();
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

void Experimentation::renderPlane()
{
    plane.shader.use();
    plane.shader.setMat4("view", view);
    plane.shader.setMat4("proj", proj);

    plane.shader.setMat4("model", plane.model);

    plane.drawArr(6);
}

void Experimentation::renderScene(const Shader& shader)
{
	glm::mat4 model = glm::mat4(1.0f);
	shader.setMat4("model", model);
	glBindVertexArray(plane.VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	// cubes
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
	model = glm::scale(model, glm::vec3(0.5f));
	shader.setMat4("model", model);
	renderCube();

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
	model = glm::scale(model, glm::vec3(0.5f));
	shader.setMat4("model", model);
	renderCube();

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 2.0));
	model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
	model = glm::scale(model, glm::vec3(0.25));
	shader.setMat4("model", model);
	renderCube();
}

void Experimentation::renderCube()
{
    glBindVertexArray(cube.VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

void Experimentation::renderDepthMap()
{
    std::vector<float> depthData(SHADOW_WIDTH * SHADOW_HEIGHT);

    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    lightView = glm::lookAt(pointLightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
    lightProj = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, nearPlane, farPlane);

    depthShader.use();
    depthShader.setMat4("lightTrans", lightProj * lightView);
    renderScene(depthShader);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    quadShader.use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, depthMap);

    std::vector<float> pixels(SHADOW_WIDTH * SHADOW_HEIGHT);

    glGetTexImage(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, GL_FLOAT, pixels.data());

    for (int i = 0; i < SHADOW_WIDTH; i++) {
        std::cout << pixels[i];
        if ((i + 1) % SHADOW_WIDTH == 0) std::cout << '\n';
        else std::cout << ", ";
    }

    renderQuad();
}


#pragma endregion


void Experimentation::updateImGuiConfig() 
{
    ImGui::Begin("Configs.");
    ImGui::Text("Innocent Grey fanboy");

    std::string posString = "pos: " + toString(camera.position);
    std::string frontString = " facing " + toString(camera.front);
    ImGui::Text((posString + frontString).c_str());

    ImGuiIO& io = ImGui::GetIO();
    ImGui::Text("(%.1f FPS)", io.Framerate);

    ImGui::Image((void*)(intptr_t)depthMap, ImVec2(100, 100));

    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


void Experimentation::endFrame()
{
    updateImGuiConfig();
    SceneTemplate::endFrame();
}

float Experimentation::generateDeltaInterval(float offset, float step)
{
	return (rand() % (int)(2 * offset * step)) / step - offset;
}

// look into uniform buffer when the performance starts to drop
// also face culling which i didnt implement here
