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
        //glEnable(GL_FRAMEBUFFER_SRGB);

        updateLightPos();
        shadowMapRender();
        renderDebugQuad();

		endFrame();
	}
}

void ModelScene::initMeshes()
{
    initSkyBox();
    initModelSponza();
    initCube();
    initLightCube();
    initBackpack();
    //initReflectiveCube();
    //initAsteriod();
    //initOrbit();

    initShadowMap();
    initShadowCubeMap();
    initDebugQuad();
}


#pragma region renderPasses
void ModelScene::simpleRender()
{
    renderSkyBox();
    renderModelSponza();
    renderCube();
    renderLightCube();
    renderBackpack();
    //renderReflectiveCube();
    //renderAsteriod();
    //renderOrbit();
}

void ModelScene::shadowMapRender()
{
    glEnable(GL_DEPTH_TEST);

    directionalLightShadowPass();	// binds 2d texture to : depthMap

    pointLightShadowPass();			// binds cubemap shadow to : depthCubeMap

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap);
    simpleRender();
}

void ModelScene::directionalLightShadowPass()
{
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);

	depthShader.use();
	shadowMapPos = glm::vec3(shadowMapPosRadius * glm::cos(glm::radians(shadowMapPosAngle)), 10.254, shadowMapPosRadius * glm::sin(glm::radians(shadowMapPosAngle)));
	lightView = glm::lookAt(shadowMapPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	lightProj = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, nearPlaneLight, farPlaneLight);
	lightTrans = lightProj * lightView;
	depthShader.setMat4("lightTrans", lightTrans);

    depthShader.setMat4("model", sponza.model);
    sponza.draw(depthShader);

    
    depthShader.setMat4("model", cube.model);
    cube.drawArr(36, depthShader);
    
    backpack.model = glm::translate(glm::mat4(1.0f), backpackPos);
    backpack.model = glm::rotate(backpack.model, glm::radians(backpackRotate), glm::normalize(glm::vec3(1.0, 0.0, 0.0)));
	backpack.model = glm::scale(backpack.model, glm::vec3(backpackSize));

    depthShader.setMat4("model", backpack.model);
    backpack.draw(depthShader);
}
#pragma endregion


#pragma region init
void ModelScene::initLightProperties()
{
    // directional light
	dirLightDir = glm::vec3(-1.0f, -1.0f, -1.0f);
	dirLightAmbient = glm::vec3(0.1f);
	dirLightDiffuse = glm::vec3(1.0f);
	dirLightSpecular = glm::vec3(0.1f);
    movingDirLight = false;

    // point light
	pointLightPos = glm::vec3(2.118f, 1.794f, -1.0f);
	pointLightAmbient = glm::vec3(0.1f);
	pointLightDiffuse = glm::vec3(0.3f);
	pointLightSpecular = glm::vec3(0.55f);
	pointLightAttenuation = glm::vec3(1.0f, 0.22f, 0.2f);

    // directional light shadow mapping
	nearPlaneLight = 0.1f;
	farPlaneLight = 23.302f;
	shadowMapPosAngle = 200.0f;
	shadowMapPosRadius = 4.8f;
	shadowMapPos = glm::vec3(shadowMapPosRadius * glm::cos(glm::radians(shadowMapPosAngle)), 10.254, shadowMapPosRadius * glm::sin(glm::radians(shadowMapPosAngle)));
}

void ModelScene::initLightCube()
{
    lightCube = Mesh(cubeVerticesVec.data(), cubeVerticesVec.size());
    lightCube.shader = Shader(vsDir , shaderDir + "singleColor.fs");
    lightCube.model = glm::translate(glm::mat4(1.0f), pointLightPos);
    lightCube.model = glm::scale(lightCube.model, glm::vec3(0.3f));
    outputMat4(lightCube.model);
}

void ModelScene::initModelSponza()
{
    stbi_set_flip_vertically_on_load(false);
	std::string modelSponzaPath = "../Resources/Models/Sponza/sponza.obj";
    sponza = Model(modelSponzaPath);
    sponzaShader = Shader(vsDir, shaderDir + "texture.fs");
    sponza.model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.5f, 0.0f));
    sponza.model = glm::scale(sponza.model, glm::vec3(0.007f, 0.007f, 0.007f));
    sponzaShader.use();
    sponzaShader.setInt("shadowMap", 5);
    sponzaShader.setInt("shadowCubeMap", 6);

    sponzaShininess = 128.0f;
}

void ModelScene::initCube()
{
    cube = Mesh(cubeVerticesVec.data(), cubeVerticesVec.size());
    cube.shader = Shader(vsDir , shaderDir + "texture.fs");
    cube.textureInit(resourceDir + "container2.png", "texture_diffuse");
    cube.textureInit(resourceDir + "container2_specular.png", "texture_specular");
    cube.model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, -0.32f, 0.0f));
    cube.model = glm::scale(cube.model, glm::vec3(0.4));
    outputMat4(cube.model);
    cube.shader.use();
    cube.shader.setInt("shadowMap", 5);
    cube.shader.setInt("shadowCubeMap", 6);
}

void ModelScene::initBackpack()
{
    stbi_set_flip_vertically_on_load(true);
    std::string path = resourceDir + "backpack/backpack.obj";
    backpack = Model(path);
    backpackShader = Shader(vsDir, shaderDir + "texture.fs");
    backpack.model = glm::translate(glm::mat4(1.0f), backpackPos);
    backpack.model = glm::rotate(backpack.model, glm::radians(backpackRotate), glm::vec3(1.0f, 0.0f, 0.0f));
    backpack.model = glm::scale(backpack.model, glm::vec3(backpackSize));
    backpackShader.use();
    backpackShader.setInt("shadowMap", 5);
    backpackShader.setInt("shadowCubeMap", 6);

    backpackSize = 0.2f;
    backpackRotate = -31.7f;
    backpackPos = glm::vec3(1.0f, -0.256f, 0.496f);
    backpackShowNormal = 0;
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

    amount = 100000;
    asteriodHeight = 50.0f;

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

void ModelScene::initDebugQuad()
{
    glGenVertexArrays(1, &debugQuadVAO);
    glGenBuffers(1, &debugQuadVBO);

    glBindVertexArray(debugQuadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, debugQuadVBO);
    glBufferData(GL_ARRAY_BUFFER, debugQuadVertices.size() * sizeof(float), debugQuadVertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    quadShader = Shader(shaderDir + "debugQuad.vs", shaderDir + "debugQuad.fs");
    quadShader.use();
    quadShader.setInt("depthMap", 0);
}
#pragma endregion


#pragma region renderMeshes
void ModelScene::renderLightCube()
{
    lightCube.shader.use();
    lightCube.shader.setMat4("view", view);
    lightCube.shader.setMat4("proj", proj);

    // vs
    lightCube.model = glm::translate(glm::mat4(1.0f), pointLightPos);
    lightCube.model = glm::scale(lightCube.model, glm::vec3(0.15f));
    lightCube.shader.setMat4("model", lightCube.model);
    lightCube.shader.setVec3("color", pointLightDiffuse);
    lightCube.drawArr(36);
}

void ModelScene::renderModelSponza()
{
    sponzaShader.use();
    sponzaShader.setMat4("view", view);
    sponzaShader.setMat4("proj", proj);
    sponzaShader.setMat4("lightTrans", lightTrans);

    // vs	
    sponzaShader.setMat4("model", sponza.model);

    // fs
    setLightSources(sponzaShader);
    sponzaShader.setFloat("material.shininess", sponzaShininess);
    sponzaShader.setFloat("farPlane", farPlanePointLight);

    sponza.draw(sponzaShader);

    if (backpackShowNormal) {
		normalVecShader.use();
		normalVecShader.setMat4("model", sponza.model);
		normalVecShader.setMat4("view", view);
		normalVecShader.setMat4("proj", proj);
        sponza.draw(normalVecShader);
    }
}

void ModelScene::renderCube()
{
    cube.shader.use();
    cube.shader.setMat4("view", view);
    cube.shader.setMat4("proj", proj);
    cube.shader.setMat4("lightTrans", lightTrans);

    // vs

    cube.shader.setMat4("model", cube.model);

    // fs
    setLightSources(cube.shader);
    cube.shader.setFloat("material.shininess", 32.0f);
    sponzaShader.setFloat("farPlane", farPlanePointLight);

    cube.drawArr(36);
}

void ModelScene::renderBackpack()
{
    backpackShader.use();
    backpackShader.setMat4("view", view);
    backpackShader.setMat4("proj", proj);
    backpackShader.setMat4("lightTrans", lightTrans);

    //vs 
    backpackShader.setMat4("model", backpack.model);

    //fs
    setLightSources(backpackShader);
    backpackShader.setFloat("material.shininess", 32.0f);
    backpackShader.setFloat("farPlane", farPlanePointLight);

    backpack.draw(backpackShader);

    if (backpackShowNormal) {
		normalVecShader.use();
		normalVecShader.setMat4("model", backpack.model);
		normalVecShader.setMat4("view", view);
		normalVecShader.setMat4("proj", proj);
		backpack.draw(normalVecShader);
    }
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

void ModelScene::renderDebugQuad()
{
    quadShader.use();
    glBindVertexArray(debugQuadVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

#pragma endregion


#pragma region shaders
void ModelScene::setLightSources(Shader& shader)
{
    shader.setVec3("viewPos", camera.position);
    // dirLigith
    setDirLight(shader);
    setPointLight(shader);
}

void ModelScene::setDirLight(Shader& shader)
{
    dirLightDir = glm::normalize(-shadowMapPos);
    shader.setVec3("dirLight.direction", dirLightDir);
    shader.setVec3("dirLight.ambient", dirLightAmbient);
    shader.setVec3("dirLight.diffuse", dirLightDiffuse);
    shader.setVec3("dirLight.specular", dirLightSpecular);
}

void ModelScene::setPointLight(Shader& shader)
{
    shader.setVec3("pointLight.position", pointLightPos);
    shader.setVec3("pointLight.ambient", pointLightAmbient);
    shader.setVec3("pointLight.diffuse", pointLightDiffuse);
    shader.setVec3("pointLight.specular", pointLightSpecular);
    shader.setVec3("pointLight.attenuationParams", pointLightAttenuation);
}

void ModelScene::initShadowMap()
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

    depthShader = Shader(shaderDir + "shadowMap.vs", shaderDir + "shadowMap.fs");
}

void ModelScene::initShadowCubeMap()
{
    glGenFramebuffers(1, &depthCubeMapFBO);
    glGenTextures(1, &depthCubeMap);

    glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap);
    for (int i = 0; i < 6; i++) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindFramebuffer(GL_FRAMEBUFFER, depthCubeMapFBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubeMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    depthCubeMapShader = Shader(shaderDir + "pointShadowDepth.vs", shaderDir + "pointShadowDepth.gs", shaderDir + "pointShadowDepth.fs");

    nearPlanePointLight = 0.0f;
    farPlanePointLight = 30.0f;

    pointLightProj = glm::perspective(glm::radians(90.0f), (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, nearPlanePointLight, farPlanePointLight);
}

void ModelScene::updatePointLightMats()
{
    pointLightProj = glm::perspective(glm::radians(90.0f), (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, nearPlanePointLight, farPlanePointLight);

    pointLightMats.clear();

    pointLightMats.push_back(pointLightProj *
        glm::lookAt(pointLightPos, pointLightPos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
    pointLightMats.push_back(pointLightProj *
        glm::lookAt(pointLightPos, pointLightPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
    pointLightMats.push_back(pointLightProj *
        glm::lookAt(pointLightPos, pointLightPos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
    pointLightMats.push_back(pointLightProj *
        glm::lookAt(pointLightPos, pointLightPos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
    pointLightMats.push_back(pointLightProj *
        glm::lookAt(pointLightPos, pointLightPos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
    pointLightMats.push_back(pointLightProj *
        glm::lookAt(pointLightPos, pointLightPos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));
}

void ModelScene::pointLightShadowPass()
{
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    
    glBindFramebuffer(GL_FRAMEBUFFER, depthCubeMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    updatePointLightMats();
    renderPointLightScene();
}

void ModelScene::renderPointLightScene()
{
    depthCubeMapShader.use();
    for (int i = 0; i < 6; i++) {
        depthCubeMapShader.setMat4("pointLightMats[" + std::to_string(i) + "]", pointLightMats[i]);
    }
    depthCubeMapShader.setFloat("farPlane", farPlanePointLight);
    depthCubeMapShader.setVec3("lightPos", pointLightPos);

    depthCubeMapShader.setMat4("model", sponza.model);
    sponza.draw(depthCubeMapShader);

    
    depthCubeMapShader.setMat4("model", cube.model);
    cube.drawArr(36, depthCubeMapShader);
    
    backpack.model = glm::translate(glm::mat4(1.0f), backpackPos);
    backpack.model = glm::rotate(backpack.model, glm::radians(backpackRotate), glm::normalize(glm::vec3(1.0, 0.0, 0.0)));
	backpack.model = glm::scale(backpack.model, glm::vec3(backpackSize));

    depthCubeMapShader.setMat4("model", backpack.model);
    backpack.draw(depthCubeMapShader);
}


#pragma endregion


#pragma region config
void ModelScene::updateImGuiConfig() 
{
    ImGui::Begin("Configs.");
    ImGui::Text("Debug interface");

    if (ImGui::BeginTabBar("tabs")) {
        if (ImGui::BeginTabItem("info")) // Begin a tab item with its label
        {
			ImGuiIO& io = ImGui::GetIO();
			ImGui::Text("(%.1f FPS)", io.Framerate);

			std::string posString = "pos: " + toString(camera.position);
			std::string frontString = " facing " + toString(camera.front);
			ImGui::Text((posString + frontString).c_str());

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("light")) {
			ImGui::Checkbox("moving point light: ", &movingPointLight);
			ImGui::Checkbox("moving directional light: ", &movingDirLight);

            shadowMapConfig();
			dirLightConfig();
			pointLightConfig();

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("models")) {
			sponzaConfig();
			backpackConfig();

            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }


    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ModelScene::shadowMapConfig()
{
    ImGui::SliderFloat("shadowMapAngle", &shadowMapPosAngle, 0.0, 360.0);
    ImGui::SliderFloat("shadowMapRadius", &shadowMapPosRadius, 0.0, 10.0);
    ImGui::SliderFloat("nearPlaneLight", &nearPlaneLight, 1.0, 100.0);
    ImGui::SliderFloat("farPlaneLight", &farPlaneLight, 7.5, 500.0);
}

void ModelScene::backpackConfig()
{
    ImGui::Text("backpack");
    ImGui::SliderFloat("backpack rotate", &backpackRotate, -90, 90);
    ImGui::SliderFloat("backpackSize", &backpackSize, 0, 1);
    ImGui::SliderFloat("backpack x", &backpackPos.x, -5, 5);
    ImGui::SliderFloat("backpack y", &backpackPos.y, -5, 5);
    ImGui::SliderFloat("backpack z", &backpackPos.z, -5, 5);
    ImGui::Checkbox("show normal", &backpackShowNormal);
}

void ModelScene::dirLightConfig()
{
    ImGui::SliderFloat("dirLight x", &dirLightDir.x, -1, 1);
    ImGui::SliderFloat("dirLight y", &dirLightDir.y, -1, 1);
    ImGui::SliderFloat("dirLight z", &dirLightDir.z, -1, 1);

    ImGui::SliderFloat("dirLight ambient", &dirLightAmbient.x, 0, 1);
    dirLightAmbient = glm::vec3(dirLightAmbient.x);

    ImGui::SliderFloat("dirLight diffuse", &dirLightDiffuse.x, 0, 1);
    dirLightDiffuse = glm::vec3(dirLightDiffuse.x);

    ImGui::SliderFloat("dirLight specular", &dirLightSpecular.x, 0, 1);
    dirLightSpecular = glm::vec3(dirLightSpecular.x);

    //ImGui::ColorEdit3("dirLight color", &dirLightDiffuse[0]);
}

void ModelScene::pointLightConfig()
{
    ImGui::Text("pointLight");

    ImGui::SliderFloat("pointLight x", &pointLightPos.x, -5, 30);
    ImGui::SliderFloat("pointLight y", &pointLightPos.y, -5, 30);
    ImGui::SliderFloat("pointLight z", &pointLightPos.z, -5, 30);

    ImGui::SliderFloat("pointLight ambient", &pointLightAmbient.x, 0, 1);
    pointLightAmbient = glm::vec3(pointLightAmbient.x);


    ImGui::ColorEdit3("pointLight color", &pointLightDiffuse[0]);

    ImGui::SliderFloat("pointLight specular", &pointLightSpecular.x, 0, 1);
    pointLightSpecular = glm::vec3(pointLightSpecular.x);

    ImGui::Text("point light shadow map:");

    ImGui::SliderFloat("near plane point light", &nearPlanePointLight,0.0f, 50.0f);
    ImGui::SliderFloat("far plane point light", &farPlanePointLight, 10.0f, 100.0f);

}

void ModelScene::sponzaConfig()
{
    ImGui::Text("Sponza config");
    ImGui::SliderFloat("sponza shininess", &sponzaShininess, 2, 128);
}
#pragma endregion




#pragma region helper/boilerplates
void ModelScene::endFrame()
{
    updateImGuiConfig();
    SceneTemplate::endFrame();
}

float ModelScene::generateDeltaInterval(float offset, float step) 
{
	return (rand() % (int)(2 * offset * step)) / step - offset;
}

void ModelScene::outputMat4(glm::mat4 mat)
{
    std::cout << "Matrix is : \n";
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            std::cout << mat[j][i] << ", ";
        }
        std::cout << std::endl;
    }
    std::cout << "end of output\n";
}

void ModelScene::updateLightPos()
{
    float time = glfwGetTime();
    float offset = 5.0f;

    // pt light 
    if (movingPointLight)
		pointLightPos.z = glm::sin(time + offset) * 2.5f;

    //pointLightPos = camera.position + camera.front;

	// dir light
    if (movingDirLight) {
        float expectedDiff = 217.0f - 150.0f;
        shadowMapPosAngle = (glm::sin(time / 3.0f) * 0.5f + 0.5f + 150.0f) * expectedDiff;
    }

}

void ModelScene::initRender()
{
    GUI::ImGuiInit(window);
    initLightProperties();
	normalVecShader = Shader(shaderDir + "normalVec.vs", shaderDir + "normalVec.gs", shaderDir + "normalVec.fs");
}
// look into uniform buffer when the performance starts to drop
// also face culling which i didnt implement here
#pragma endregion