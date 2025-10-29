#pragma once
#include "Model.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Model::Model(char* path) 
{
	loadModel(path);
}

void Model::draw(Shader& shader) 
{
	for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i].draw(shader);
	}
}

void Model::loadModel(std::string path) 
{
	//Assimp::Importer import;
	//const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	//if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
	//	std::cout << "[ERROR] Assimp::" << import.GetErrorString() << std::endl;
	//	return;
	//}
	//directory = path.substr(0, path.find_last_of('/'));

	//processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		// NOTE : recall node.mMeshes contains indices that we "dereference" in the scene mMeshes[]
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		// NOTE : using recursion can capture the structural of the model (exp: a box containing a bottle). Then it can do something like: translate box => also translates the bottle inside the box through this parent-children relation in the recursion.
		processNode(node->mChildren[i], scene);
	}
}

void Model::processModel(aiNode* node, const aiScene* scene)
{
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	// Vertex Init
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		glm::vec3 tmpVector;
		tmpVector.x = mesh->mVertices[i].x;
		tmpVector.y = mesh->mVertices[i].y;
		tmpVector.z = mesh->mVertices[i].z;
		vertex.position = tmpVector;

		tmpVector.x = mesh->mNormals[i].x;
		tmpVector.y = mesh->mNormals[i].y;
		tmpVector.z = mesh->mNormals[i].z;
		vertex.normal = tmpVector;

		// NOTE : assimp allows a model to have 8 different texCoords. Right now we only care about the first one.
		if (mesh->mTextureCoords[0]) {
			glm::vec2 tmp;
			tmp.x = mesh->mTextureCoords[0][i].x;
			tmp.y = mesh->mTextureCoords[0][i].y;
			vertex.texCoords = tmp;
		}
		else {
			vertex.texCoords = glm::vec2(0.0f);
		}

		vertices.push_back(vertex);
	}

	// Indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) 
			indices.push_back(face.mIndices[j]);
	}

	return Mesh(vertices, indices, textures);
}

