#include "ModelScene.h"

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

void ModelScene::render()
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

// init
void ModelScene::initMeshes()
{
    initSkyBox();
    initModelSponza();
    initCube();
    initBackpack();
    initReflectiveCube();
    initAsteriod();
    initOrbit();
}
// render
void ModelScene::simpleRender()
{
    renderSkyBox();
    renderModelSponza();
    renderCube();
    renderBackpack();
    renderReflectiveCube();
    renderAsteriod();
    renderOrbit();
}


void ModelScene::initModelSponza()
{
    stbi_set_flip_vertically_on_load(false);
	std::string modelSponzaPath = "../Resources/Models/Sponza/sponza.obj";
    sponza = Model(modelSponzaPath);
    sponzaShader = Shader(vsDir, shaderDir + "sponza.fs");
}

void ModelScene::initCube()
{
    cube = Mesh(cubeVerticesVec.data(), cubeVerticesVec.size());
    cube.shader = Shader(vsDir , shaderDir + "cube.fs");
    cube.textureInit(resourceDir + "container2.png", "texture_diffuse");
    cube.textureInit(resourceDir + "container2_specular.png", "texture_specular");
}

void ModelScene::initBackpack()
{
    stbi_set_flip_vertically_on_load(true);
    std::string path = resourceDir + "backpack/backpack.obj";
    backpack = Model(path);
    backpackShader = Shader(vsDir, shaderDir + "backpack.fs");
}

void ModelScene::initSkyBox()
{
	stbi_set_flip_vertically_on_load(false);
    std::vector<std::string> faces
    {
        "right.jpg",
		"left.jpg",
		"top.jpg",
		"bottom.jpg",
		"front.jpg",
		"back.jpg"
    };
    for (std::string& face : faces) {
        face = skyBoxDir + face;
    }
    skybox = Mesh(skyboxVertices);
    skybox.loadCubeMap(faces);
    skybox.shader = Shader(shaderDir + "skyBox.vs", shaderDir + "skyBox.fs");
    skybox.shader.use();
    skybox.shader.setInt("skyBoxTexture", 0);
}

void ModelScene::initReflectiveCube()
{
    reflectCube = Mesh(cubeVerticesVec.data(), cubeVerticesVec.size());
    reflectCube.shader = Shader(shaderDir + "reflectiveCube.vs", shaderDir + "reflectiveCube.fs");
    reflectCube.shader.use();
    reflectCube.shader.setInt("skyBox", 0);
}

void ModelScene::initAsteriod()
{
    asteriod = Model(modelDir + "Planet/planet.obj");
    asteriodShader = Shader(vsDir, shaderDir + "texture.fs");
}

void ModelScene::initOrbit()
{
    orbit = Model(modelDir + "Rock/rock.obj");
    orbitShader = Shader(shaderDir + "orbit.vs", shaderDir + "texture.fs");

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

        model = glm::translate(model, glm::vec3(x, y + asteriodHeight, z));

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

    for (unsigned i = 0; i < orbit.meshes.size(); i++) {
        unsigned VAO = orbit.meshes[i].VAO;
        glBindVertexArray(VAO);

        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

        glEnableVertexAttribArray(3);
        glEnableVertexAttribArray(4);
        glEnableVertexAttribArray(5);
        glEnableVertexAttribArray(6);

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);

        glBindVertexArray(0);
    }
}



void ModelScene::renderModelSponza()
{
    sponzaShader.use();
    sponzaShader.setMat4("view", view);
    sponzaShader.setMat4("proj", proj);

    // vs	
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.5f, 0.0f));
    model = glm::scale(model, glm::vec3(0.015f, 0.015f, 0.015f));
    sponzaShader.setMat4("model", model);

    // fs

    sponza.draw(sponzaShader);
}

void ModelScene::renderCube()
{
    cube.shader.use();
    cube.shader.setMat4("view", view);
    cube.shader.setMat4("proj", proj);

    // vs
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 0.0f));
    cube.shader.setMat4("model", model);

    cube.drawArr(36);
}

void ModelScene::renderBackpack()
{
    backpackShader.use();
    backpackShader.setMat4("view", view);
    backpackShader.setMat4("proj", proj);

    //vs 
    glm::mat4 model = glm::translate(glm::mat4(1.0f), backpackPos);
    model = glm::rotate(model, glm::radians(backpackRotate), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(backpackSize));
    backpackShader.setMat4("model", model);

    //fs

    backpack.draw(backpackShader);

    normalVecShader.use();
    normalVecShader.setMat4("model", model);
    normalVecShader.setMat4("view", view);
    normalVecShader.setMat4("proj", proj);
    if (backpackShowNormal)
		backpack.draw(normalVecShader);
}

void ModelScene::renderSkyBox()
{
    glDepthMask(GL_FALSE);
    skybox.shader.use();
    skybox.shader.setMat4("view", view);
    skybox.shader.setMat4("proj", proj);
    glBindVertexArray(skybox.VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.cubeMapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthMask(GL_TRUE);
}

void ModelScene::renderReflectiveCube()
{
	reflectCube.shader.use();
    reflectCube.shader.setMat4("trans", proj * view);

    //vs
    reflectCube.shader.setMat4("model", glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.05f, 2.2f)));

    //fs	
    reflectCube.shader.setVec3("cameraPos", camera.position);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.cubeMapTexture);
    reflectCube.drawArr(36);
}

void ModelScene::renderAsteriod()
{
    asteriodShader.use();
    asteriodShader.setMat4("proj", proj);
    asteriodShader.setMat4("view", view);


    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, asteriodHeight, 0.0f));
    asteriodShader.setMat4("model", model);

    asteriod.draw(asteriodShader);
}

void ModelScene::renderOrbit()
{
    orbitShader.use();
    orbitShader.setMat4("view", view);
    orbitShader.setMat4("proj", proj);

    orbitShader.setInt("material.texture_diffuse1", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, orbit.textures_loaded[0].id);
    for (unsigned i = 0; i < orbit.meshes.size(); i++) {
        glBindVertexArray(orbit.meshes[i].VAO);
        glDrawElementsInstanced(GL_TRIANGLES, orbit.meshes[i].indices.size(), GL_UNSIGNED_INT, 0, amount);
    }
}





void ModelScene::updateImGuiConfig() 
{
    ImGui::Begin("Configs.");
    ImGui::Text("Innocent Grey fanboy");

    ImGuiIO& io = ImGui::GetIO();
    ImGui::Text("(%.1f FPS)", io.Framerate);

    std::string posString = "pos: " + toString(camera.position);
    std::string frontString = " facing " + toString(camera.front);
    ImGui::Text((posString + frontString).c_str());

    backpackConfig();

    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

// wrapped in updateImGuiConfig()
void ModelScene::backpackConfig()
{
    ImGui::Text("backpack");
    //ImGui::SliderFloat("backpack rotate", &backpackRotate, -90, 90);
    //ImGui::SliderFloat("backpackSize", &backpackSize, 0, 1);
    //ImGui::SliderFloat("backpack x", &backpackPos.x, -5, 5);
    //ImGui::SliderFloat("backpack y", &backpackPos.y, -5, 5);
    //ImGui::SliderFloat("backpack z", &backpackPos.z, -5, 5);
    ImGui::Checkbox("show normal", &backpackShowNormal);
}

void ModelScene::endFrame()
{
    updateImGuiConfig();
    SceneTemplate::endFrame();
}

float ModelScene::generateDeltaInterval(float offset, float step) 
{
	return (rand() % (int)(2 * offset * step)) / step - offset;
}

// look into uniform buffer when the performance starts to drop
// also face culling which i didnt implement here
