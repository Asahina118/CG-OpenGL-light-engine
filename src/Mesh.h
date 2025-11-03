#pragma once

#include <string.h>
#include <vector>

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <assimp/Importer.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

#define MAX_BONE_INFLUENCE 4

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;

	glm::vec3 tangent;
	glm::vec3 biTangent;

	int m_BoneIDs[MAX_BONE_INFLUENCE];
	float m_Weights[MAX_BONE_INFLUENCE];
};

struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh {
public:	
	// mesh data
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	void draw(Shader& shader);
	Mesh();

	// rawArr for quick Init
	Shader shader;
	glm::mat4 model = glm::mat4(1.0f);
	Mesh(const float* vertices, unsigned size);
	void textureInit(std::string path, std::string type);
	void drawArr(int);
	unsigned int VAO, VBO, EBO;

private:
	void setupMesh();
};
