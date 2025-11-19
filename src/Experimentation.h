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

class Experimentation : public SceneTemplate {
public:
	using SceneTemplate::SceneTemplate;
	void render();

private:
	std::string shaderDir = SceneTemplate::shaderDir + "Experimentation/";
	std::string vsDir = shaderDir + "vertex.vs";
	std::string textfsDir = shaderDir + "texture.fs";
	std::string modelDir = "../Resources/Models/";

	// meshes
	void initQuad();
	void renderQuad();
	unsigned quadVAO = 0, quadVBO;
	Shader quadShader;

	// lightCube
	void initLightCube();
	void renderLightCube();
	Mesh lightCube;
	glm::vec3 pointLightPos = glm::vec3(0.0f);
	glm::vec3 pointLightAmbient = glm::vec3(0.1);
	glm::vec3 pointLightDiffuse = glm::vec3(0.5);
	glm::vec3 pointLightSpecular = glm::vec3(0.1);

	// plane
	void initPlane();
	void renderPlane();
	Mesh plane;
	glm::vec3 planePos = glm::vec3(0.0);

	// cube
	void initCube();
	void renderCube();
	Mesh cube;

	// render functions	
	void simpleRender();
	void renderDepthMap();
	void renderScene(const Shader&);

	void initMeshes();
	void updateImGuiConfig();
	void endFrame() override;

	// shader
	// directional shadow map
	void initDepthMap();
	unsigned depthMap, depthMapFBO;
	const unsigned SHADOW_WIDTH = 4096;
	const unsigned SHADOW_HEIGHT = 4096;
	const float nearPlaneLight = 1.0f;
	const float farPlaneLight = 7.5f;
	glm::mat4 lightView = glm::lookAt(pointLightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 lightProj = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, nearPlaneLight, farPlaneLight);
	glm::mat4 lightTrans = lightProj * lightView;
	Shader depthShader;

	// unidirectional shadow map
	void initCubeDepthMap();
	void renderCubeDepthMap();
	void updateShadowTransforms();
	unsigned depthCubeMap, depthCubeMapFBO;
	float nearPlaneLightCube = 1.0f;
	float farPlaneLightCube = 25.0f;
	glm::mat4 shadowProj;
	std::vector<glm::mat4> shadowTransforms;
	Shader depthShaderCube;

	void renderSceneCube(const Shader&);
	Shader renderCubeShader;

	void renderCubePointShadow();

	// light
	void setLightShader(const Shader&);

	// helper

	// returns x : x \in [-offset, offset - 1 / step] uniformly
	float generateDeltaInterval(float offset, float step);

	// vertices
	std::vector<float> quadVertices = {
		// positions        // texture Coords
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	};
	std::vector<float> planeVertices = {
		// positions            // normals         // texcoords
		 25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
		-25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
		-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,

		 25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
		-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
		 25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 25.0f
	};

	// temp
	unsigned loadTexture(std::string path);
};

// shadowMap is binded to TEXTURE5
// shadowCubeMap is binded to TEXTURE6


