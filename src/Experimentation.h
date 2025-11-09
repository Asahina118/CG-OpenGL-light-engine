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

	// meshes
	void initQuad();
	void renderQuad();
	Mesh quad;

	// render functions	
	void simpleRender();

	void initMeshes();
	void updateImGuiConfig();
	void endFrame() override;

	// vertices
	std::vector<float> quadVertices = {
		// positions     // colors
		-0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
		 0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
		-0.05f, -0.05f,  0.0f, 0.0f, 1.0f,

		-0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
		 0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
		 0.05f,  0.05f,  0.0f, 1.0f, 1.0f
	};
};