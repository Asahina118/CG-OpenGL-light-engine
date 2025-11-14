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

	Mesh();
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	void draw(Shader& shader);

	// rawArr for quick Init
	Shader shader;
	glm::mat4 model = glm::mat4(1.0f);
	Mesh(const float* vertices, unsigned size);
	void textureInit(std::string path, std::string type, GLenum drawingStrat = GL_REPEAT);
	void drawArr(int);
	void drawArr(int, Shader&);

	// wont overload drawArr for potential new features for now
	Mesh(std::vector<float> vertices);
	void drawHighlight(int numFaces, Shader& highlightShader, bool highlightBorderOnly);
	unsigned int VAO, VBO, EBO;

	// cube map
	// for simple use (asking for another refactor in the near future LMAO)
	unsigned cubeMapTexture;
	void loadCubeMap(std::vector<std::string> faces);

private:
	void setupMesh();
};
