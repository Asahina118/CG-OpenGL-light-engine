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
	Model();
	void draw(Shader& shader);
private:
	std::vector<Texture> textures_loaded;
	std::vector<Mesh> meshes;

	// the directory of where the input path is
	std::string directory;

	void loadModel(std::string path);
	Mesh processMesh(aiMesh *mesh, const aiScene* scene);
	void processNode(aiNode* node, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};