#pragma once
#include "Model.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Dependencies/stb_image.h"

unsigned int TextureFromFile(const char * path, const std::string &directory, bool gamma = false) 
{
	std::string fileName = std::string(path);
	fileName = directory + '/' + fileName;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(fileName.c_str(), &width, &height, &nrComponents, 0);
	if (data) {
		GLenum format;
		if (nrComponents == 1) format = GL_RED;
		else if (nrComponents == 3) format = GL_RGB;
		else if (nrComponents == 4) format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else {
		std::cout << "[ERROR] failed to load textures at path: " << path << '\n';
	}
	stbi_image_free(data);

	return textureID;
}

Model::Model(char* path) 
{
	loadModel(path);
}

Model::Model(std::string path)
{
	loadModel(path);
}

Model::Model()
{
}

void Model::draw(Shader& shader) 
{
	for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i].draw(shader);
	}
}

void Model::loadModel(std::string path) 
{
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "[ERROR] Assimp::" << import.GetErrorString() << std::endl;
		return;
	}

	std::cout << "[SUCCESS] loaded model with assimp from " << path << std::endl;

	directory = path.substr(0, path.find_last_of('/'));

	totalCount = countLoading(scene->mRootNode);
	
	std::cout << "[INFO] start processing meshes from the model:\n";
	processNode(scene->mRootNode, scene);
	std::cout << "\n[SUCCESS] finished processing meshes from the model\n";
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
		std::cout << "\r[INFO] Progress : " << ++loadingCount << " / " << totalCount;
	}
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

			//tmpVector.x = mesh->mTangents[i].x;
			//tmpVector.y = mesh->mTangents[i].y;
			//tmpVector.z = mesh->mTangents[i].z;
			//vertex.tangent = tmpVector;

			//tmpVector.x = mesh->mBitangents[i].x;
			//tmpVector.y = mesh->mBitangents[i].y;
			//tmpVector.z = mesh->mBitangents[i].z;
			//vertex.biTangent = tmpVector;
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

	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

		std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_height");
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
	}

	return Mesh(vertices, indices, textures);
}


std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) 
{
	std::vector<Texture> textures;
	bool skip = false;
	Texture texture;
	aiString str;

	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
		mat->GetTexture(type, i, &str);
		skip = false;

		// optimization in loading textures as it is expensive (loading took ~30 seconds before optimization, crazy.)
		for (unsigned int j = 0; j < textures_loaded.size(); j++) {
			if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0) {
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}

		if (!skip) {
			texture.id = TextureFromFile(str.C_Str(), directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			textures_loaded.push_back(texture);
		}
	}

	return textures;
}

// helper
int Model::countLoading(aiNode* node)
{
	int count = node->mNumChildren;
	for (int i = 0; i < node->mNumChildren; i++) {
		count += countLoading(node->mChildren[i]);
	}
	return count;
}
