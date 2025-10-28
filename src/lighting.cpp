#include "lighting.h"
// debugging GUI
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "Camera.h"

#include "Dependencies/stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//#define GLM_ENABLE_EXPERIMENTAL
//#include <glm/gtx/io.hpp>

void Lighting::VAOInit(unsigned int* VAO, unsigned int* lightVAO, unsigned int* textureVAO)
{
    float simpleCube[] = {
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
	unsigned int VBO, textureVBO;

	glGenVertexArrays(1, VAO);
    glGenVertexArrays(1, lightVAO);
	glGenVertexArrays(1, textureVAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &textureVBO);

    // simple Cube ==============
	glBindVertexArray(*VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(simpleCube), simpleCube, GL_STATIC_DRAW);

	int stride = 8 * sizeof(float);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

    // light source =============
    glBindVertexArray(*lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);

    // texture background ==========
	float verticesCube[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	glBindVertexArray(*textureVAO);
	glBindBuffer(GL_ARRAY_BUFFER, textureVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCube), verticesCube, GL_STATIC_DRAW);
	stride = 5 * sizeof(float);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
}

void Lighting::processInputs()
{
    processChangeMouseInput();

    // Matrix update
    if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
		processCamera();
		view = camera.getViewMatrix();
    }

    // update perspectives
    proj = glm::perspective(glm::radians(camera.FOV), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, nearPlane, farPlane);
}

void Lighting::simpleCubeInit(Shader* shader)
{
	// material properties
    simpleCubeAmbient = glm::vec3(1.0f, 0.5f, 0.31f);
    simpleCubeDiffuse = glm::vec3(1.0f, 0.5f, 0.31f);
    simpleCubeSpecular = glm::vec3(0.5f, 0.5f, 0.5f);
    simpleCubeShininess = 32.0f;

    shader->use();
    shader->setVec3("objectColor", 1.0f, 0.5f, 0.31f);
    shader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);
    shader->setFloat("size", 1.0f);

    modelSimpleCube = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 3.0f));


    // adding texture (decided to separate unsigned integers initializations)
    glGenTextures(1, &simpleCubeTexture);
    glBindTexture(GL_TEXTURE_2D, simpleCubeTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, _;
    const char* imagePath = "src/Resources/container2.png";
    unsigned char* data = stbi_load(imagePath, &width, &height, &_, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "[ERROR] Failed to load texture from stbi loader\n" << imagePath << std::endl;
    }
    stbi_image_free(data);


    // adding texture for high specular part of the image
    glGenTextures(1, &simpleCubeTextureSpecular);
    glBindTexture(GL_TEXTURE_2D, simpleCubeTextureSpecular);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    imagePath = "src/Resources/container2_specular.png";
    data = stbi_load(imagePath, &width, &height, &_, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "[ERROR] Failed to load texture from stbi loader\n" << imagePath << std::endl;
    }
    stbi_image_free(data);


    // adding matrix emission for fun
    glGenTextures(1, &simpleCubeEmissionTexture);
    glBindTexture(GL_TEXTURE_2D, simpleCubeEmissionTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    imagePath = "src/Resources/emissionMap.png";
    data = stbi_load(imagePath, &width, &height, &_, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "[ERROR] Failed to load texture from stbi loader\n" << imagePath << std::endl;
    }
    stbi_image_free(data);

    shader->setInt("material.diffuse", 0);
    shader->setInt("material.specular", 1);
    shader->setInt("material.emission", 2);
}

void Lighting::renderSimpleCube(Shader* shader, unsigned int* VAO) 
{

    // lookup table for attenuation, constant term is always 1.0f for attenuation <= 1
    int attenuationParamsStride = 2;
	float attenuationParams[] = {
		.7f,	1.8f,
		.35f,	.44f,
		.22f,	.2f,
		.09f,	.032f,
		.014f,	.0007f,
	};
    // the smaller the both values, the "brigter" it gets (for not accounting for effects on distance)

    shader->use();

	// vertex
    float rotationSpeed = (simpleCubeRotation) ? .5f : 0.0f;
    modelSimpleCube = glm::rotate(modelSimpleCube, glm::radians(rotationSpeed), glm::vec3(0.0f, 1.0f, 1.0f));

    shader->setFloat("size", size);
    shader->setMat4("model", modelSimpleCube);
    shader->setMat4("view", view);
    shader->setMat4("proj", proj);

	// fragment
	shader->setVec3("objectColor", simpleCubeColorR, simpleCubeColorG, simpleCubeColorB);
    shader->setVec3("cameraPos", camera.position.x, camera.position.y, camera.position.z);

    // material properties
    shader->setInt("material.diffuse", 0);
    shader->setVec3("material.specular", simpleCubeSpecular);
    shader->setFloat("material.shininess", simpleCubeShininess);

    // lightSource properties
    shader->setVec3("light.position", lightPosX, lightPosY, lightPosZ);
    shader->setVec3("light.ambient", lightColorR, lightColorG, lightColorB);
    shader->setVec3("light.diffuse", lightSourceDiffuse);
    shader->setVec3("light.specular", lightSourceSpecular);
    shader->setVec3("light.attenuationParams", 1.0f, attenuationParams[attenuationParamOption * attenuationParamsStride], attenuationParams[attenuationParamOption * attenuationParamsStride + 1] );

    // flashlight properties
    shader->setBool("flashlight.enableFlashlight", camera.enableFlashlight);
    shader->setVec3("flashlight.color", flashlightColor);
    shader->setVec3("flashlight.position", camera.position);
    shader->setVec3("flashlight.direction", camera.front);

    shader->setVec3("flashlight.ambient", glm::vec3(0.05f));
    shader->setVec3("flashlight.diffuse", glm::vec3(flashlightColor));
    shader->setVec3("flashlight.specular", glm::vec3(1.0f));
    shader->setFloat("flashlight.cosCutOff", glm::cos(glm::radians(flashlightCutOff)));

    // formula : cutoffOuter \in [0.0f, 10.0f] is mapped to final parameter \in [flashlightCutOff, 90.0f] 
    float diff = (90.0f - flashlightCutOff) / 10;
    shader->setFloat("flashlight.outerCosCutOff", glm::cos(glm::radians(flashlightCutOff + diff * flashlightCutOffOuter)));
    
    // textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, simpleCubeTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, simpleCubeTextureSpecular);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, simpleCubeEmissionTexture);

	glBindVertexArray(*VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Lighting::lightingSourceInit()
{
    lightSourceAmbient = glm::vec3(0.2f, 0.2f, 0.2f);
    lightSourceDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
    lightSourceSpecular = glm::vec3(1.0f, 1.0f, 1.0f);
    modelLightSource = glm::mat4(1.0f);

}

void Lighting::renderLightSource(Shader* lightingShader, unsigned int* lightVAO)
{
	lightingShader->use();

    // vertex
    float xPos = lightSourceRadius * cos(lightSourceRotationSpeed*glfwGetTime());
    float zPos = lightSourceRadius * sin(lightSourceRotationSpeed*glfwGetTime());
    modelLightSource = glm::translate(glm::mat4(1.0f), glm::vec3(xPos, lightSourceHeight, 3.0 + zPos));
    modelLightSource = glm::scale(modelLightSource, glm::vec3(0.2f));
    // NOTE : scaling before translation affects the magnitude of the translation.

	lightingShader->setFloat("size", 1.0f);
    lightingShader->setMat4("model", modelLightSource);
    lightingShader->setMat4("view", view);
    lightingShader->setMat4("proj", proj);

    // fragment
    if (rainbowColor) {
        lightColorR = sin(glfwGetTime() * 2.0f) / 2 + 0.5;
        lightColorG = sin(glfwGetTime() * 0.7f) / 2 + 0.5;
        lightColorB = sin(glfwGetTime() * 1.3f) / 2 + 0.5;
        lightingShader->setVec3("lightColor", lightColorR, lightColorG, lightColorB);
    }
    else {
		lightingShader->setVec3("lightColor", lightColorR, lightColorG, lightColorB);
    }

	glBindVertexArray(*lightVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);

    // update lightPos for simpleCube shader
    glm::vec4 newLightPos = modelLightSource * glm::vec4(lightPosX, lightPosY, lightPosZ, 1.0f);
    lightPosX = newLightPos.x;
    lightPosY = newLightPos.y;
    lightPosZ = newLightPos.z;

}

// returns new model matrix and modify unsigned int texture;
glm::mat4 Lighting::textureBackgroundInit(Shader* textureShader, unsigned int& texture)
{
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, _;
	const char* imagePath = "src/Resources/container.jpg";
	unsigned char* data = stbi_load(imagePath, &width, &height, &_, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "[ERROR] Failed to load texture from stbi loader\n" << imagePath << std::endl;
	}
	stbi_image_free(data);

    textureShader->use();
    textureShader->setInt("texture1", 0);

    return glm::mat4(1.0f);

}

void Lighting::renderTextureBackground(Shader* textureShader, unsigned int &textureVAO, unsigned int& texture, glm::mat4 model)
{
    textureShader->use();
    textureShader->setFloat("size", 10.0f);
    textureShader->setMat4("mat", proj * view * model);

    //glActiveTexture(GL_TEXTURE0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(textureVAO);

    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Lighting::render()
{
	stbi_set_flip_vertically_on_load(true);
	glEnable(GL_DEPTH_TEST);

    unsigned int VAO, lightVAO, textureVAO;
    VAOInit(&VAO, &lightVAO, &textureVAO);

    // Simple Cube
    Shader* normalShader = new Shader("src/Shaders/Lighting/lighting.vs", "src/Shaders/Lighting/lighting.fs");
    simpleCubeInit(normalShader); // NOTE : initialize color and model matrix

    // Light Source
    Shader* lightingShader = new Shader("src/Shaders/Lighting/lighting.vs", "src/Shaders/Lighting/lightSource.fs");
    lightingSourceInit();

    //Background
    unsigned int texture;
    Shader* textureBackgroundShader = new Shader("src/Shaders/Lighting/textureBackground.vs", "src/Shaders/Lighting/textureBackground.fs");
    glm::mat4 modelTextureBackground = textureBackgroundInit(textureBackgroundShader, texture);


    // Imgui
    ImGuiInit(window);

    while (!glfwWindowShouldClose(window)) {
        ImGuiNewFrame();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        processInputs();


        renderLightSource(lightingShader, &lightVAO);
        renderSimpleCube(normalShader, &VAO);
        renderTextureBackground(textureBackgroundShader, textureVAO, texture, modelTextureBackground);

        updateImguiConfig();


        glfwSwapBuffers(window);
        glfwPollEvents();
    }


}

void Lighting::processCamera() {
    float dTime = 0.001f;

    // WASD Movement
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(this->window, true);

    // Sprinting (Toggle)
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        if (!toggleLockSprinting) {
            toggleLockSprinting = true;
			if (camera.sprinting == 1.0f) {
				camera.sprinting = 5.0f;
			}
			else {
				camera.sprinting = 1.0f;
			}

        }
    }
    else {
        toggleLockSprinting = false;
    }
    
    // Front-back movement
    float yPos = camera.position.y;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.processKeyboard(FORWARD, dTime);
		camera.position.y = yPos;
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.processKeyboard(BACKWARD, dTime);
		camera.position.y = yPos;
    }

	// Left-right movement
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.processKeyboard(LEFT, dTime);
    }
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.processKeyboard(RIGHT, dTime);
    }

    // Up-down movement
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        camera.processKeyboard(UP, dTime);
    }	
    else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        camera.processKeyboard(DOWN, dTime);
    }

    // Toggle flashlight
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
        if (!toggleLockFlashlight) {
			camera.enableFlashlight = (camera.enableFlashlight) ? false : true;
            toggleLockFlashlight = true;
        }
    }
    else {
        toggleLockFlashlight = false;
    }


}

void Lighting::processChangeMouseInput()
{
    if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS) {
        if (!toggleLockChangeMouseInput) {
            toggleLockChangeMouseInput = true;
			if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL) {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                camera.stopUpdating = false;

                // firstMouse couldnt solve the cursor jump
                camera.theta = tempTheta;
                camera.phi = tempPhi;
			}
			else {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                camera.stopUpdating = true;

                // firstMouse couldnt solve the cursor jump
                tempTheta = camera.theta;
                tempPhi = camera.phi;
			}

        }
    }
    else {
        toggleLockChangeMouseInput = false;
    }
}

void Lighting::updateImguiConfig() {
	ImGui::Begin("Configs.");
	ImGui::Text("Ciallo~");

    // Engine Cofigs =======================================
	ImGui::SliderFloat("[Engine] ambientStrength", &ambientStrength, 0.01f, 1.0f);
	ImGui::SliderFloat("[Engine] specularStrength", &specularStrength, 0.01f, 1.0f);


    // lightSource Configs =================================
	ImGui::Text("light source configs");
    ImGui::Checkbox("rainbow", &rainbowColor);
    // lightSource color
	ImGui::SliderFloat("[light] red", &lightColorR, 0.0f, 1.0f);
	ImGui::SliderFloat("[light] green", &lightColorG, 0.0f, 1.0f);
	ImGui::SliderFloat("[light] blue", &lightColorB, 0.0f, 1.0f);
    // lightSource Position 
	ImGui::SliderFloat("[light] rotation speed", &lightSourceRotationSpeed, 0.0f, 10.0f);
	ImGui::SliderFloat("[light] rotation radius", &lightSourceRadius, 0.0f, 2.0f);
	ImGui::SliderFloat("[light] height", &lightSourceHeight, -3.0f, 3.0f);

	ImGui::SliderFloat("[light] ambient", &lightSourceAmbient.x, 0.0f, 1.0f);
	ImGui::SliderFloat("[light] specular", &lightSourceSpecular.x, 0.0f, 1.0f);
    lightSourceAmbient = glm::vec3(lightSourceAmbient.x);
    lightSourceSpecular = glm::vec3(lightSourceSpecular.x);

    ImGui::SliderInt("[light] attenuation distance", &attenuationParamOption, 0, 4);


    // simpleCube Configs =================================
    ImGui::Text("simple cube configs");
    ImGui::Checkbox("rotation", &simpleCubeRotation);

    ImGui::SliderFloat("shininess", &simpleCubeShininess, 2, 200);

    ImGui::SliderFloat("ambient", &simpleCubeAmbient.x, 0.0f, 1.0f);
    simpleCubeAmbient = glm::vec3(simpleCubeAmbient.x, simpleCubeAmbient.x, simpleCubeAmbient.x);
    
    ImGui::SliderFloat("diffuse", &simpleCubeDiffuse.x, 0.0f, 1.0f);
    simpleCubeDiffuse = glm::vec3(simpleCubeDiffuse.x, simpleCubeAmbient.x, simpleCubeDiffuse.x);

    // flashlight Configs
    ImGui::SliderFloat("flashlight angle", &flashlightCutOff, 1.0f, 89.0f);
    ImGui::SliderFloat("flashlight smoothing", &flashlightCutOffOuter, 0.0f, 10.0f);

	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

void Lighting::ImGuiNewFrame() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void Lighting::ImGuiInit(GLFWwindow* window) {
	IMGUI_CHECKVERSION(); 
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
}
