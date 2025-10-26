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

#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//#define GLM_ENABLE_EXPERIMENTAL
//#include <glm/gtx/io.hpp>

void Lighting::VAOInit(unsigned int* VAO, unsigned int* lightVAO, unsigned int* textureVAO)
{
    float simpleCube[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };
	glEnable(GL_DEPTH_TEST);
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

	int stride = 6 * sizeof(float);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

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
    shader->use();
    shader->setVec3("objectColor", 1.0f, 0.5f, 0.31f);
    shader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);
    shader->setFloat("size", 1.0f);

    modelSimpleCube = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 3.0f));
}

void Lighting::renderSimpleCube(Shader* shader, unsigned int* VAO) 
{
    shader->use();

	// vertex
    modelSimpleCube = glm::rotate(modelSimpleCube, glm::radians(.5f), glm::vec3(0.0f, 1.0f, 1.0f));

    shader->setFloat("size", size);
    shader->setMat4("model", modelSimpleCube);
    shader->setMat4("view", view);
    shader->setMat4("proj", proj);

	// fragment
	shader->setVec3("objectColor", simpleCubeColorR, simpleCubeColorG, simpleCubeColorB);
	shader->setVec3("lightColor", lightColorR, lightColorG, lightColorB);
    shader->setVec3("lightPos", lightPosX, lightPosY, lightPosZ);
    shader->setInt("shininess", shininess);

    shader->setVec3("cameraPos", camera.position.x, camera.position.y, camera.position.z);
    shader->setFloat("ambientStrength", ambientStrength);
    shader->setFloat("specularStrength", specularStrength);

	glBindVertexArray(*VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Lighting::lightingSourceInit()
{
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
    lightingShader->setVec3("lightColor", lightColorR, lightColorG, lightColorB);

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
	stbi_set_flip_vertically_on_load(true);
	const char* imagePath = "container.jpg";
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

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(textureVAO);

    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Lighting::render()
{
    unsigned int VAO, lightVAO, textureVAO;
    VAOInit(&VAO, &lightVAO, &textureVAO);

    // Simple Cube
    Shader* normalShader = new Shader("lighting.vs", "lighting.fs");
    simpleCubeInit(normalShader); // NOTE : initialize color and model matrix

    // Light Source
    Shader* lightingShader = new Shader("lighting.vs", "lightSource.fs");
    lightingSourceInit();

    //Background
    unsigned int texture;
    Shader* textureBackgroundShader = new Shader("textureBackground.vs", "textureBackground.fs");
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
        renderTextureBackground(textureBackgroundShader, textureVAO, textureVAO, modelTextureBackground);

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


}

void Lighting::processChangeMouseInput()
{
    if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS) {
        if (!toggleLockChangeMouseInput) {
            toggleLockChangeMouseInput = true;
			if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL) {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			}
			else {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
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
	ImGui::Text("lightColor");
    // lightSource color
	ImGui::SliderFloat("[lightSource] red", &lightColorR, 0.0f, 1.0f);
	ImGui::SliderFloat("[lightSource] green", &lightColorG, 0.0f, 1.0f);
	ImGui::SliderFloat("[lightSource] blue", &lightColorB, 0.0f, 1.0f);
    // lightSource Position 
	ImGui::SliderFloat("[lightSource] rotation speed", &lightSourceRotationSpeed, 0.0f, 10.0f);
	ImGui::SliderFloat("[lightSource] rotation radius", &lightSourceRadius, 0.0f, 2.0f);
	ImGui::SliderFloat("[lightSource] height", &lightSourceHeight, -3.0f, 3.0f);


    // simpleCube Configs =================================
	ImGui::Text("Simple Cube Color");
	ImGui::SliderFloat("red", &simpleCubeColorR, 0.0f, 1.0f);
	ImGui::SliderFloat("green", &simpleCubeColorG, 0.0f, 1.0f);
	ImGui::SliderFloat("blue", &simpleCubeColorB, 0.0f, 1.0f);
    ImGui::SliderInt("shininess", &shininess, 2, 10000);

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
