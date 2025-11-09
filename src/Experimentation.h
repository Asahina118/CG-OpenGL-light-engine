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

	// init funct	//render functions
	void simpleRender();

	void initMeshes();
	void updateImGuiConfig();
	void endFrame() override;
};