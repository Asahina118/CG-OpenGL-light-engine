#pragma once
#include "SceneTemplate.h"

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Camera.h"
#include "Shader.h"
#include "Mesh.h"
#include "InputHandler.h"
#include "Model.h"

class ModelScene : public SceneTemplate {
public:
	using SceneTemplate::SceneTemplate;
	void render();

private:
	std::string shaderDir = SceneTemplate::shaderDir + "ModelScene/";
	std::string vsDir = shaderDir + "vertex.vs";
	std::string modelDir = "../Resources/Models/";

	// init functions
	void initModelSponza();
	void renderModelSponza();
	void sponzaConfig();
	Model sponza;
	Shader sponzaShader;
	float sponzaTuning = 1.0f;
	float sponzaShininess = 128.0f;

	// lightCube
	void initLightCube();
	void renderLightCube();
	Mesh lightCube;

	// backpack
	void initBackpack();
	void renderBackpack();
	void backpackConfig();
	Model backpack;
	Shader backpackShader;
	float backpackSize = 0.392f;
	float backpackRotate = -15.153f;
	glm::vec3 backpackPos = glm::vec3(2.025f, 0.092f, 0.951f);
	bool backpackShowNormal = 0;

	
	// texture Cube
	void initCube();
	void renderCube();
	Mesh cube;
	

	// skybox
	void initSkyBox();
	void renderSkyBox();
	Mesh skybox;


	// reflective
	void initReflectiveCube();
	void renderReflectiveCube();
	Mesh reflectCube;


	// asteriod
	void initAsteriod();
	void renderAsteriod();
	unsigned amount = 100000;
	float asteriodHeight = 50.0f;
	Model asteriod;
	Shader asteriodShader;

	void initOrbit();
	void renderOrbit();
	Model orbit;
	Shader orbitShader;
	std::vector<glm::mat4> modelMats;



	// shaders
	Shader normalVecShader = Shader(shaderDir + "normalVec.vs", shaderDir + "normalVec.gs", shaderDir + "normalVec.fs");
	void setLightSources(Shader& shader);
	void setDirLight(Shader& shader);
	void setPointLight(Shader& shader);

	// dirLight
	void dirLightConfig();
	glm::vec3 dirLightDir = glm::vec3(-1.0f, -1.0f, -1.0f);
	glm::vec3 dirLightAmbient = glm::vec3(0.1f);
	glm::vec3 dirLightDiffuse = glm::vec3(0.6f);
	glm::vec3 dirLightSpecular = glm::vec3(0.1f);

	// pointLight
	void pointLightConfig();
	glm::vec3 pointLightPos = glm::vec3(0.0f, 2.0f, 0.0f);
	glm::vec3 pointLightAmbient = glm::vec3(0.005f);
	glm::vec3 pointLightDiffuse = glm::vec3(1.0f);
	glm::vec3 pointLightSpecular = glm::vec3(0.55f);

	glm::vec3 pointLightAttenuation = glm::vec3(1.0f, 0.22f, 0.2f);

	// shadow map
	void shadowMapRender();
	void initShadowMap();
	void renderSceneShadowMap();
	const unsigned SHADOW_WIDTH = 1024;
	const unsigned SHADOW_HEIGHT = 1024;
	unsigned depthMapFBO;
	unsigned depthMap;
	glm::mat4 lightProj = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, nearPlane, farPlane);
	glm::mat4 lightView = glm::lookAt(pointLightPos,
		dirLightDir,
		glm::vec3(0.0f, 1.0f, 0.0f));
	//glm::mat4 lightView = camera.getViewMatrix();
	glm::mat4 lightTrans = lightProj * lightView;
	Shader depthShader;

	// debug quad
	void initDebugQuad();
	void renderDebugQuad();
	unsigned debugQuadVAO, debugQuadVBO;
	Shader quadShader;

	//render functions
	void simpleRender();

	// helper
	float generateDeltaInterval(float offset, float step);
	void outputMat4(glm::mat4);


	void initMeshes();
	void updateImGuiConfig();
	void endFrame() override;
};
