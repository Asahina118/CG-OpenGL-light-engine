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

    while (!glfwWindowShouldClose(window))
    {
        startFrame();

        renderDepthMap();

        endFrame();
    }

}

void Experimentation::initMeshes()
{
    initLightCube();
    initPlane();
    initCube();
    initQuad();
    initDepthMap();
}


#pragma region init

void Experimentation::initLightCube()
{
    lightCube = Mesh(cubeVerticesVec.data(), cubeVerticesVec.size());
    lightCube.shader = Shader(vsDir, shaderDir + "singleColor.fs");
}

void Experimentation::initPlane() 
{
    plane = Mesh(planeVertices.data(), planeVertices.size());
    plane.shader = Shader(vsDir, textfsDir);
    plane.textureInit(resourceDir + "wood.png", "texture_diffuse");
    plane.shader.use();
    plane.shader.setInt("shadowMap", 5);
}

void Experimentation::initCube()
{
    cube = Mesh(cubeVerticesVec.data(), cubeVerticesVec.size());
    cube.shader = Shader(vsDir, textfsDir);
    cube.textureInit(resourceDir + "wood.png", "texture_diffuse");
    cube.shader.use();
    cube.shader.setInt("shadowMap", 5);
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
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    depthShader = Shader(shaderDir + "depth.vs", shaderDir + "depth.fs");
}

#pragma endregion


#pragma region render

void Experimentation::renderLightCube()
{
    lightCube.shader.use();
    lightCube.shader.setMat4("view", view);
    lightCube.shader.setMat4("proj", proj);

    lightCube.model = glm::translate(glm::mat4(1.0f), pointLightPos);
    lightCube.model = glm::scale(lightCube.model, glm::vec3(0.2));
    lightCube.shader.setMat4("model", lightCube.model);

    lightCube.drawArr(36);
}

void Experimentation::renderQuad()
{
    if (quadVAO == 0)
    {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);


    //quadShader.use();
    //glBindVertexArray(quadVAO);
    //glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    //glBindVertexArray(0);
}

void Experimentation::renderPlane()
{
    plane.shader.use();

    // vs
    plane.shader.setMat4("view", view); 
    plane.shader.setMat4("proj", proj);
    plane.shader.setMat4("lightTrans", lightTrans);

    plane.shader.setMat4("model", plane.model);

    // fs
    setLightShader(plane.shader);

    plane.drawArr(6);
}

void Experimentation::renderScene(const Shader& shader)
{
	glm::mat4 model = glm::mat4(1.0f);

    plane.model = glm::translate(glm::mat4(1.0), planePos);
	shader.setMat4("model", model);
	glBindVertexArray(plane.VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	// cubes
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
	model = glm::scale(model, glm::vec3(0.5f));
	shader.setMat4("model", model);
    glBindVertexArray(cube.VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
	model = glm::scale(model, glm::vec3(0.5f));
	shader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 2.0));
	model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
	model = glm::scale(model, glm::vec3(0.25f));
	shader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

void Experimentation::renderCube()
{
    cube.shader.use();
    // vs
    cube.shader.setMat4("lightTrans", lightProj * lightView);
    cube.shader.setMat4("view", view);
    cube.shader.setMat4("proj", proj);

    // fs
    setLightShader(cube.shader);


	// cubes
	cube.model = glm::mat4(1.0f);
	cube.model = glm::translate(cube.model, glm::vec3(0.0f, 1.5f, 0.0));
	cube.model = glm::scale(cube.model, glm::vec3(0.5f));
	cube.shader.setMat4("model", cube.model);
    cube.drawArr(36);

	cube.model = glm::mat4(1.0f);
	cube.model = glm::translate(cube.model, glm::vec3(2.0f, 0.0f, 1.0));
	cube.model = glm::scale(cube.model, glm::vec3(0.5f));
	cube.shader.setMat4("model", cube.model);
	cube.drawArr(36);

	cube.model = glm::mat4(1.0f);
	cube.model = glm::translate(cube.model, glm::vec3(-1.0f, 0.0f, 2.0));
	cube.model = glm::rotate(cube.model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
	cube.model = glm::scale(cube.model, glm::vec3(0.25f));
    cube.shader.setMat4("model", cube.model);
    cube.drawArr(36);
}

void Experimentation::renderDepthMap()
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

	depthShader.use();
	lightView = glm::lookAt(pointLightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
    lightTrans = lightProj * lightView;
	depthShader.setMat4("lightTrans", lightTrans);

	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	renderScene(depthShader);


	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    renderLightCube();
    renderPlane();
    renderCube();

	//quadShader.use();
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, depthMap);
	//renderQuad();
}


#pragma endregion


void Experimentation::setLightShader(Shader& shader)
{
    shader.use();

    // camera
    shader.setVec3("viewPos", camera.position);

    // ptLight
    shader.setVec3("pointLight.position", pointLightPos);
    shader.setVec3("pointLight.diffuse", pointLightDiffuse);
    shader.setVec3("pointLight.ambient", pointLightAmbient);
    shader.setVec3("pointLight.specular", pointLightSpecular);
}


void Experimentation::updateImGuiConfig() 
{
    ImGui::Begin("Configs.");
    ImGui::Text("Innocent Grey fanboy");

    std::string posString = "pos: " + toString(camera.position);
    std::string frontString = " facing " + toString(camera.front);
    ImGui::Text((posString + frontString).c_str());

    ImGuiIO& io = ImGui::GetIO();
    ImGui::Text("(%.1f FPS)", io.Framerate);

    ImGui::SliderFloat("light x", &pointLightPos.x, -5, 5);
    ImGui::SliderFloat("light y", &pointLightPos.y, -5, 5);
    ImGui::SliderFloat("light z", &pointLightPos.z, -5, 5);

    ImGui::Image((void*)(intptr_t)depthMap, ImVec2(100, 100));

    ImGui::SliderFloat("plane height", &planePos.y, -5, 5);

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
