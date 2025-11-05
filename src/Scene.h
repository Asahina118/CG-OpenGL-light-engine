#pragma once

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Camera.h"
#include "Shader.h"
#include "Mesh.h"
#include "InputHandler.h"

class Scene {
public:
	Scene(GLFWwindow*, Camera&, int, int);
	Scene() = delete;
	void render();

private:
	GLFWwindow* window;
	Camera& camera;
	InputHandler input = InputHandler(window, camera);

	const int SCREEN_HEIGHT;
	const int SCREEN_WIDTH;

	const float nearPlane = 0.1f;
	const float farPlane = 500.0f;

	glm::mat4 view;
	glm::mat4 proj;

	std::string shaderDir = "src/Shaders/Advanced/";
	std::string resourceDir = "src/Resources/";
	std::string vertexDir = shaderDir + "vertex.vs";

	// shaders
	Shader highlightShader = Shader(vertexDir, shaderDir + "singleColor.fs");

	// render
	void simpleRender();

	void renderHighlightObject();
	bool highlightObject = false;
	bool highlightBorderOnly = true;
	glm::vec3 highlightColor = glm::vec3(0.42f, 0.26f, 0.72f);

	void renderFramebuffers();
	Shader renderFrameShader;
	float quadVertices;
	Mesh quad;
	unsigned quadVAO, quadTextureID;


	void initFrameBuffers();
	unsigned int framebuffer;
    bool enableFramebuffer = false;

	// Meshes
	void initMeshes();

	// cube
	void initCube();
	void renderCube();
	Mesh cube;

	// plane
	void initPlane();
	void renderPlane();
	Mesh plane;

	// grass
	void initGrass();
	void renderGrass();
	Mesh grass;

	// glasses
	void initGlass();
	void renderGlass();
	Mesh glass;

	// boilerplates
	void startFrame();
	void endFrame();
	void updateImGuiConfig();
	void initRender();

	// helper functions
	std::string toString(glm::vec3);

	void test();
};



/*
void Scene::test()
{
    Shader screenShader(shaderDir + "framebuffer.vs", shaderDir + "framebuffer.fs");
    screenShader.use();
    screenShader.setInt("screenTexture", 0);

    float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };
    unsigned int quadVAO, quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));


    unsigned int framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    // create a color attachment texture
    unsigned int textureColorbuffer;
    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
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


    while (!glfwWindowShouldClose(window)) {
        //      input.processBasicInput(&view);
              //proj = glm::perspective(glm::radians(camera.FOV), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, nearPlane, farPlane);

        startFrame();

        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)

        // make sure we clear the framebuffer's content
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderCube();
        renderPlane();
        renderGrass();
        renderGlass();

        // now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
        // clear all relevant buffers
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
        glClear(GL_COLOR_BUFFER_BIT);

        screenShader.use();
        glBindVertexArray(quadVAO);
        glBindTexture(GL_TEXTURE_2D, textureColorbuffer);	// use the color attachment texture as the texture of the quad plane
        glDrawArrays(GL_TRIANGLES, 0, 6);


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        //glfwSwapBuffers(window);
        //glfwPollEvents();
        endFrame();
    }
} 
*/
