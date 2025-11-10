#pragma once

#include <vector>
#include <string>

#include "Shader.h"
#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model {
public:
	Model(char* path);
	Model(std::string path);
	Model();
	void draw(Shader& shader);

	// requires edit in Scene.cpp for quickly removing loaded textures (too lazy to overload it now)
	std::vector<Texture> textures_loaded;

	// normally this is private. Set to public for instance rendering learning purposes
	std::vector<Mesh> meshes;
private:

	// the directory of where the input path is
	std::string directory;

	// num of loadable nodes
	int loadingCount = 0;
	int totalCount = 0;

	void loadModel(std::string path);
	Mesh processMesh(aiMesh *mesh, const aiScene* scene);
	void processNode(aiNode* node, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

	// helper
	int countLoading(aiNode* node);
};