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
	float sponzaShininess;

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
	float backpackSize;
	float backpackRotate;
	glm::vec3 backpackPos;
	bool backpackShowNormal;

	
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
	unsigned amount;
	float asteriodHeight;
	Model asteriod;
	Shader asteriodShader;

	void initOrbit();
	void renderOrbit();
	Model orbit;
	Shader orbitShader;
	std::vector<glm::mat4> modelMats;



	// shaders
	Shader normalVecShader;
	void setLightSources(Shader&);
	void setDirLight(Shader&);
	void setPointLight(Shader&);

	// init function
	void initLightProperties();

	// dirLight
	void dirLightConfig();
	glm::vec3 dirLightDir;
	glm::vec3 dirLightAmbient;
	glm::vec3 dirLightDiffuse;
	glm::vec3 dirLightSpecular;

	bool movingDirLight = false;

	// pointLight
	void pointLightConfig();
	glm::vec3 pointLightPos;
	glm::vec3 pointLightAmbient;
	glm::vec3 pointLightDiffuse;
	glm::vec3 pointLightSpecular;
	glm::vec3 pointLightColor;
	glm::vec3 pointLightAttenuation;

	bool movingPointLight = false;

	// shadow map
	void shadowMapRender();
	void initShadowMap();
	void directionalLightShadowPass();
	void shadowMapConfig();
	const unsigned SHADOW_WIDTH = 1024 * 4;
	const unsigned SHADOW_HEIGHT = 1024 * 4;
	unsigned depthMapFBO;
	unsigned depthMap;
	float nearPlaneLight;
	float farPlaneLight;
	float shadowMapPosAngle;
	float shadowMapPosRadius;
	glm::vec3 shadowMapPos;
	//glm::vec3 shadowMapPos = glm::vec3(6.516, 10.254, 1.558);
	glm::mat4 lightProj;
	glm::mat4 lightView;
	glm::mat4 lightTrans;
	//glm::mat4 lightView = camera.getViewMatrix();
	Shader depthShader;

	// point shadow map
	void initShadowCubeMap();
	void pointLightShadowPass();
	void renderPointLightScene();
	void updatePointLightMats();
	unsigned depthCubeMap, depthCubeMapFBO;
	float nearPlanePointLight;
	float farPlanePointLight;
	glm::mat4 pointLightProj;
	std::vector<glm::mat4> pointLightMats;
	Shader depthCubeMapShader;




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
		
	void updateLightPos();


	void initRender();
	void initMeshes();
	void updateImGuiConfig();
	void endFrame() override;

	// vertices
	std::vector<float> debugQuadVertices = {
		// positions        // texture Coords
		0.7f,  -0.7f, 0.0f, 0.0f, 1.0f,
		0.7f, -1.0f, 0.0f, 0.0f, 0.0f,
		 1.0f,  -0.7f, 0.0f, 1.0f, 1.0f,
		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	};
};
