#include "Scene.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <map>

#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include "Shader.h"
#include "Camera.h"
#include "InputHandler.h"
#include "GUI.h"
#include "Model.h"

#include <assimp/Importer.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Dependencies/stb_image.h"

void Scene::render()
{
    initRender();
    initMeshes();

    // NOTE : all renders wrapped inside renderFramebuffers() for now. (might consider tidying it up in the future)
	while (!glfwWindowShouldClose(window)) {
        startFrame();
        renderFramebuffers();
        endFrame();
	}
}

void Scene::initMeshes()
{
    initCube();
    initPlane();
    initGrass();
    initGlass();
    initSkyBox();
    //initBackpack();
    initReflective();
}


void Scene::initCube()
{
    float cubeVertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };
    unsigned size = sizeof(cubeVertices) / sizeof(float);
    cube = Mesh(cubeVertices, size);
    cube.textureInit(resourceDir + "metal.png", "texture_diffuse");
    cube.shader = Shader(shaderDir + "vertex.vs", shaderDir + "metalCube.fs");
}

void Scene::initPlane()
{
    float planeVertices[] = {
         5.0f, -0.5f,  5.0f,  0.0f,  0.0f, -1.0f,  2.0f, 0.0f,
        -5.0f, -0.5f,  5.0f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f,  0.0f, -1.0f,  0.0f, 2.0f,

         5.0f, -0.5f,  5.0f,  0.0f,  0.0f, -1.0f,  2.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f,  0.0f, -1.0f,  0.0f, 2.0f,
         5.0f, -0.5f, -5.0f,  0.0f,  0.0f, -1.0f,  2.0f, 2.0f
    };
    unsigned size = sizeof(planeVertices) / sizeof(float);
    plane = Mesh(planeVertices, size);
    plane.textureInit(resourceDir + "marble.jpg", "texture_diffuse");
    plane.shader = Shader(shaderDir + "vertex.vs", shaderDir + "marblePlane.fs");
}

void Scene::initGrass()
{
    float cubeVertices[] = { // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };
    unsigned size = sizeof(cubeVertices) / sizeof(float);
    grass = Mesh(cubeVertices, size);
    grass.textureInit(resourceDir + "grass.png", "texture_diffuse", GL_CLAMP_TO_EDGE);
    grass.shader = Shader(vertexDir, shaderDir + "grass.fs");
}

void Scene::initGlass()
{
    float cubeVertices[] = { // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };
    unsigned size = sizeof(cubeVertices) / sizeof(float);
    glass = Mesh(cubeVertices, size);
    glass.textureInit(resourceDir + "blending_transparent_window.png", "texture_diffuse", GL_CLAMP_TO_EDGE);
    glass.shader = Shader(vertexDir, shaderDir + "glass.fs");
}

void Scene::initSkyBox()
{
	stbi_set_flip_vertically_on_load(false);
    std::vector<std::string> faces
    {
        "right.jpg",
		"left.jpg",
		"top.jpg",
		"bottom.jpg",
		"front.jpg",
		"back.jpg"
    };
    for (std::string& face : faces) {
        face = skyBoxDir + face;
    }
    skyBox = Mesh(skyboxVertices);
    skyBox.loadCubeMap(faces);
    skyBox.shader = Shader(shaderDir + "skyBox.vs", shaderDir + "skyBox.fs");
    skyBox.shader.use();
    skyBox.shader.setInt("skyBoxTexture", 0);
}

// init after skyBox as it requires skyBox texture
void Scene::initBackpack()
{
    std::string path = resourceDir + "backpack/backpack.obj";
    backpack = Model((char*)path.c_str());
    backpackShader = Shader(shaderDir + "backpack.vs", shaderDir + "backpack.fs");

    Texture skyBoxtext;
    skyBoxtext.id = skyBox.cubeMapTexture;
    skyBoxtext.type = "texture_diffuse";
    std::vector<Texture> skyBoxTexture = {};
    skyBoxTexture.push_back(skyBoxtext);

    backpack.textures_loaded.clear();
    backpack.textures_loaded = skyBoxTexture;
}

void Scene::initReflective()
{
    float* vertices = cubeVerticesVec.data();
    unsigned size = cubeVerticesVec.size();
    reflectiveCube = Mesh(vertices, size);
    reflectiveCube.shader = Shader(shaderDir + "reflectiveCube.vs", shaderDir + "reflectiveCube.fs");
    reflectiveCube.shader.use();
    reflectiveCube.shader.setInt("skyBox", 0);
    //test
}



void Scene::renderCube()
{
    // 1st cube
	cube.shader.use();
	cube.shader.setMat4("trans", proj * view);

    // vs
    cube.model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.01f, -1.0f));
	cube.shader.setMat4("model", cube.model);

    // fs 
    
	cube.drawArr(36);


    // 2nd cube
    cube.model = glm::translate(cube.model, glm::vec3(2.0f, 0.01f, 0.0f));
	cube.shader.setMat4("model", cube.model);

	cube.drawArr(36);
}

void Scene::renderPlane()
{
    plane.shader.use();

    // vs
    glm::mat4 model(1.0f);
    plane.shader.setMat4("model", model);
	plane.shader.setMat4("trans", proj * view);

    // fs

    plane.drawArr(6);
}

void Scene::renderGrass()
{
    std::vector<glm::vec3> locations;
    locations.push_back(glm::vec3(-1.5f, 0.0f, -0.48f));
    locations.push_back(glm::vec3(1.5f, 0.0f, 0.51f));
    locations.push_back(glm::vec3(0.0f, 0.0f, 0.7f));
    locations.push_back(glm::vec3(-0.3f, 0.0f, -2.3f));
    locations.push_back(glm::vec3(0.5f, 0.0f, -0.6f));

    for (glm::vec3 location : locations) {
        grass.shader.use();
        grass.shader.setMat4("trans", proj * view);
        grass.shader.setMat4("model", glm::translate(glm::mat4(1.0f), location));
        grass.drawArr(6);
    }
}

void Scene::renderGlass()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    std::vector<glm::vec3> locations;
    locations.push_back(glm::vec3(-1.5f, 0.0f, -0.48f));
    locations.push_back(glm::vec3(1.5f, 0.0f, 0.51f));
    locations.push_back(glm::vec3(0.0f, 0.0f, 0.7f));
    locations.push_back(glm::vec3(-0.3f, 0.0f, -2.3f));
    locations.push_back(glm::vec3(0.5f, 0.0f, -0.6f));

    // NOTE : render the glasses from back to front to prevent failure of depth buffer testing from discarding the fragments
    std::map<float, glm::vec3> sorted;
    for (glm::vec3 location : locations) {
        float distance = glm::length(location - camera.position);
        sorted[distance] = location;
    }

    for ( auto it = sorted.rbegin() ; it != sorted.rend(); it++ ) {
        glm::vec3 location = it->second;
        location.z += 0.1f;
        glass.shader.use();
        glass.shader.setMat4("trans", proj * view);
        glass.shader.setMat4("model", glm::translate(glm::mat4(1.0f), location));
        glass.drawArr(6);
    }
}

void Scene::renderSkyBox()
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glDepthMask(GL_FALSE);
    skyBox.shader.use();
    skyBox.shader.setMat4("view", view);
    skyBox.shader.setMat4("proj", proj);
    glBindVertexArray(skyBox.VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyBox.cubeMapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthMask(GL_TRUE);
}

void Scene::renderBackpack()
{
    backpackShader.use();

    //vs
    backpackModel = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.05f, 1.5f));
    backpackModel = glm::scale(backpackModel, glm::vec3(0.01f, 0.01f, 0.01f));
    backpackShader.setMat4("model", backpackModel);
    backpackShader.setMat4("trans", proj * view);

    //fs
    backpackShader.setInt("cubeTexture", 0);
    backpackShader.setVec3("cameraPos", camera.position);

    backpack.draw(backpackShader);
}

void Scene::renderReflective()
{
    reflectiveCube.shader.use();
    reflectiveCube.shader.setMat4("trans", proj * view);

    //vs
    reflectiveCube.shader.setMat4("model", glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.05f, -2.5f)));

    //fs	
    reflectiveCube.shader.setVec3("cameraPos", camera.position);

    reflectiveCube.drawArr(36);
}



void Scene::updateImGuiConfig() 
{
    ImGui::Begin("Configs.");
    ImGui::Text("Innocent Grey fanboy");

    std::string posString = "pos: " + toString(camera.position);
    std::string frontString = " facing " + toString(camera.front);
    ImGui::Text((posString + frontString).c_str());

    ImGui::Checkbox("highlight", &highlightObject);
    ImGui::Checkbox("highlight borders only", &highlightBorderOnly);
    ImGui::ColorEdit3("highlight color", &highlightColor[0]);

    ImGui::Text("post processing");
    const char* items[] = { "no effects", "inversion", "grey scale", "sharpen", "blur"};
    ImGui::Combo("post processing effects", &postProcessingChoice, items, IM_ARRAYSIZE(items));

    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
};


#pragma region renders

void Scene::simpleRender()
{
    // opague
    renderCube();
    renderPlane();
    renderGrass();
    //renderBackpack();
    renderReflective();

	// transparent
    renderGlass();
}

// can pack into the Mesh class
void Scene::renderHighlightObject()
{
    glClear(GL_STENCIL_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);

    glStencilMask(0x00);
    renderPlane();

    GLenum highlightMode = highlightBorderOnly ? GL_REPLACE : GL_KEEP;
    glStencilOp(GL_KEEP, highlightMode, highlightMode);

	glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);
    renderCube();

    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00);
    glDisable(GL_DEPTH_TEST);	// highlight color "penetrates" all objects in front

    highlightShader.use();
    highlightShader.setMat4("trans", proj * view);

    highlightShader.setMat4("model", glm::scale(cube.model, glm::vec3(1.05f)));
    highlightShader.setVec3("color", highlightColor);
    glBindVertexArray(cube.VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glm::mat4 secondModel = glm::translate(cube.model, glm::vec3(-2.0f, 0.0f, 0.0f));
    highlightShader.setMat4("model", glm::scale(secondModel, glm::vec3(1.05f)));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glEnable(GL_DEPTH_TEST);

    renderGrass();
    renderReflective();
    renderGlass();
}

void Scene::renderFramebuffers()
{
    if (postProcessingChoice) {
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    }

    renderSkyBox();

	if (highlightObject) {
		renderHighlightObject();
	}
	else {
        simpleRender();
	}

    if (postProcessingChoice) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
		renderFrameShader.use();
        renderFrameShader.setInt("choice", postProcessingChoice);
		glBindVertexArray(quadVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, quadTextureID);
		glDrawArrays(GL_TRIANGLES, 0, 6);
    }
}

#pragma endregion


void Scene::initFrameBuffers()
{
    renderFrameShader = Shader(shaderDir + "framebuffer.vs", shaderDir + "framebuffer.fs");
    renderFrameShader.use();
    renderFrameShader.setInt("screenTexture", 0);

    float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };
    unsigned int quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));


    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    // create a color attachment texture
    glGenTextures(1, &quadTextureID);
    glBindTexture(GL_TEXTURE_2D, quadTextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, quadTextureID, 0);
    // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCREEN_WIDTH, SCREEN_HEIGHT); // use a single renderbuffer object for both a depth AND stencil buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
    // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

#pragma region boilerplates
void Scene::initRender()
{
	glEnable(GL_DEPTH_TEST);
    GUI::ImGuiInit(window);
    initFrameBuffers();
}

void Scene::startFrame()
{
    GUI::ImGuiNewFrame();
	input.processBasicInput(&view);
	proj = glm::perspective(glm::radians(camera.FOV), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, nearPlane, farPlane);
}

void Scene::endFrame()
{
    updateImGuiConfig();
	glfwSwapBuffers(window);
	glfwPollEvents();
}

std::string Scene::toString(glm::vec3 vec)
{
    std::string result = "(";
    int length = 3;
    for (int i = 0; i < length; i++) {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << vec[i];
        result += ss.str();
        if (i != length - 1) result += ", ";
        else result += ")";
    }
    return result;
}


Scene::Scene(GLFWwindow* window, Camera& camera, int height, int width) : window(window), camera(camera), SCREEN_HEIGHT(height), SCREEN_WIDTH(width) {};
#pragma endregion


#pragma region vertices
#pragma endregion


/* quite a few bugs here :
1. using post processing now breaks the buffers. Need to investigate that further later
2. adding backpack was unsuccessful
3. the highlight effects cannot penetrate thorugh the reflective/refractive cube
*/
