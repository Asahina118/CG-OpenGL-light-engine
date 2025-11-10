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
	Model sponza;
	Shader sponzaShader;


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

	//render functions
	void simpleRender();

	// helper
	float generateDeltaInterval(float offset, float step);


	void initMeshes();
	void updateImGuiConfig();
	void endFrame() override;
};
