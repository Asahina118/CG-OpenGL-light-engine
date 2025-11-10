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
    initAsteriod();
}

void Experimentation::simpleRender()
{
    renderAsteriod();
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

void Experimentation::initAsteriod()
{
    asteriod = Model(modelDir + "Planet/planet.obj");
    rock = Model(modelDir + "Rock/rock.obj");
    asteriodShader = Shader(vsDir, shaderDir + "asteriod.fs");
    rockShader = Shader(shaderDir + "vertexInstance.vs", shaderDir + "rock.fs");


    rockShader.use();
    srand(glfwGetTime());
    float radius = 150.0f;
    float offset = 25.0f;
    for (unsigned i = 0; i < amount; i++) {
        glm::mat4 model(1.0f);

        float angle = (float)i / (float)amount * 360.0f;
        // NOTE : rand() % 2 * offset * 100 \in [0, 2 * offset * 100 - 1]
        // -> [0,  ( 2 * offset * 100 - 1 ) / 100]
        // -> delta \in [-offset, offset - 1/100] 
        // approximately [-offset, offset) with size = 2 * offset, and 100 as the step size
        float delta = generateDeltaInterval(offset, 100.0f);
        float x = cos(angle) * radius + delta;

        delta = generateDeltaInterval(offset, 100.0f);
        float y = delta * 0.4f;

        delta = generateDeltaInterval(offset, 100.0f);
        float z = sin(angle) * radius + delta;

        model = glm::translate(model, glm::vec3(x, y, z));

        float scale = static_cast<float>((rand() % 20) / 100.0 + 0.05);
        model = glm::scale(model, glm::vec3(scale));

        float rotate = static_cast<float>((rand() % 360));
        model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.4f, 0.6f, 0.8f));

        // distributed in a torus uniformly
        modelMats.push_back(model);
    }

    unsigned buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), modelMats.data(), GL_STATIC_DRAW);

    for (unsigned i = 0; i < rock.meshes.size(); i++) {
        unsigned VAO = rock.meshes[i].VAO;
        glBindVertexArray(VAO);

        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

        glEnableVertexAttribArray(3);
        glEnableVertexAttribArray(4);
        glEnableVertexAttribArray(5);
        glEnableVertexAttribArray(6);

        // NOTE : the location should be updated 1 time for every new instace
        // Later for different effects we can set it to 2, then OpenGL would update the content of the vertex attribute to the next element
        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);

        glBindVertexArray(0);
    }
}




void Experimentation::renderQuad()
{
    quad.shader.use();
    glBindVertexArray(quad.VAO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100);
}

void Experimentation::renderAsteriod()
{
    asteriodShader.use();
    asteriodShader.setMat4("view", view);
    asteriodShader.setMat4("proj", proj);

    //vs
    glm::mat4 model(1.0f);
    asteriodShader.setMat4("model", model);

    //fs

    asteriod.draw(asteriodShader);

    // rock orbit
    renderOrbit();
}

void Experimentation::renderOrbit()
{

    rockShader.use();
    rockShader.setMat4("view", view);
    rockShader.setMat4("proj", proj);

    //fs
    //rockShader.setInt("material.texture_height1", 0);
    rockShader.setInt("material.texture_diffuse1", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, rock.textures_loaded[0].id);
    for (unsigned i = 0; i < rock.meshes.size(); i++) {
        glBindVertexArray(rock.meshes[i].VAO);
        glDrawElementsInstanced(GL_TRIANGLES, rock.meshes[i].indices.size(), GL_UNSIGNED_INT, 0, amount);
    }

}











void Experimentation::updateImGuiConfig() 
{
    ImGui::Begin("Configs.");
    ImGui::Text("Innocent Grey fanboy");

    std::string posString = "pos: " + toString(camera.position);
    std::string frontString = " facing " + toString(camera.front);
    ImGui::Text((posString + frontString).c_str());

    ImGuiIO& io = ImGui::GetIO();
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

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
