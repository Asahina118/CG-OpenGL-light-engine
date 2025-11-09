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

	while (!glfwWindowShouldClose(window)) {
		startFrame();
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        simpleRender();
		endFrame();
	}
}

void Experimentation::initMeshes()
{
    initQuad();
}

void Experimentation::simpleRender()
{
    renderQuad();
}




void Experimentation::initQuad()
{
    quad = Mesh();
    glGenVertexArrays(1, &quad.VAO);
    unsigned VBO;
    glGenBuffers(1, &VBO);

    glBindVertexArray(quad.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, quadVertices.size() * sizeof(float), quadVertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2*sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);



    quad.shader = Shader(shaderDir + "quad.vs", shaderDir + "quad.fs");


    std::vector<glm::vec2> translations;
    int idx = 0;
    float offset = 0.1f;
    // instance rendering setup
    for (int x = -10; x < 10; x += 2) {
        for (int y = -10; y < 10; y += 2) {
            glm::vec2 translation;
            translation.x = (float) x / 10.0f + offset;
            translation.y = (float) y / 10.0f + offset;
            translations.push_back(translation);
        }
    }
    quad.shader.use();
    for (unsigned i = 0; i < 100; i++) {
        quad.shader.setVec2(("offsets[" + std::to_string(i) + "]"), translations[i]);
    }

    unsigned instanceVBO;
    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 100, translations.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(2, 1);

    glBindVertexArray(0);
}

void Experimentation::renderQuad()
{
    quad.shader.use();
    glBindVertexArray(quad.VAO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100);
}













void Experimentation::updateImGuiConfig() 
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


void Experimentation::endFrame()
{
    updateImGuiConfig();
    SceneTemplate::endFrame();
}

// look into uniform buffer when the performance starts to drop
// also face culling which i didnt implement here
