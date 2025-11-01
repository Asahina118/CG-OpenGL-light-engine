#include "renderModel.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <assimp/Importer.hpp>

#include "Shader.h"
#include "Camera.h"

#include "Dependencies/stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <filesystem>

void renderModel::render()
{
	glEnable(GL_DEPTH_TEST);
	stbi_set_flip_vertically_on_load(true);

    // init
    backpackInit();
    lightSourceInit();
    //testingCubeInit();

    ImGuiInit();

    while (!glfwWindowShouldClose(window)) {
        ImGuiNewFrame();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        processInputs();

        renderBackpack(backpackShader, backpack);
        renderLightSource(lightSource.shader);
        //renderTestingCube();

        updateImGuiConfig();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void renderModel::backpackInit() 
{
	const char* filePath = "src/Resources/backpack/backpack.obj";
	backpack = Model((char*)filePath);
    backpackShader = Shader("src/Shaders/renderModel/renderModel.vs", "src/Shaders/renderModel/renderModel.fs");
}

void renderModel::renderBackpack(Shader& backpackShader, Model& backpack)
{
	backpackShader.use();

    // .vs
    backpackModel = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 3.0f));
    backpackModel = glm::rotate(backpackModel, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    backpackModel = glm::scale(backpackModel, glm::vec3(0.3f));
	backpackShader.setMat4("model", backpackModel);
	backpackShader.setMat4("transform", proj * view);

	// .fs
    backpackShader.setInt("NR_POINT_LIGHTS", 1);
    backpackShader.setInt("NR_SPOTLIGHTS", 1);
    backpackShader.setBool("enableFlashlight", camera.enableFlashlight);


    backpackShader.setVec3("viewPos", camera.position);
    backpackShader.setInt("NR_POINT_LIGHTS", 1);
    backpackShader.setInt("NR_SPOTLIGHTS", 1);
    backpackShader.setBool("enableFlashlight", camera.enableFlashlight);

    // DirLight
    backpackShader.setVec3("dirLight.direction", dirLightDirection);
    backpackShader.setVec3("dirLight.ambient", glm::vec3(dirLightAmbient.x));
    backpackShader.setVec3("dirLight.diffuse", glm::vec3(dirLightDiffuse.x));
    backpackShader.setVec3("dirLight.specular", glm::vec3(dirLightSpecular.x));

    // PointLight
    backpackShader.setVec3("pointLights[0].position", lightPos);
    backpackShader.setVec3("pointLights[0].attenuationParams", glm::vec3(1.0f, 0.22f, 0.2f));
    backpackShader.setVec3("pointLights[0].ambient", glm::vec3(0.1f));
    backpackShader.setVec3("pointLights[0].diffuse", lightSource.diffuse);
    backpackShader.setVec3("pointLights[0].specular", glm::vec3(lightSource.specular.x));

    // Flashlight
    backpackShader.setVec3("spotLights[0].position", camera.position);
    backpackShader.setVec3("spotLights[0].direction", camera.front);
    backpackShader.setVec3("spotLights[0].attenuationParams", glm::vec3(1.0f, 0.09f, 0.032f));
    backpackShader.setVec3("spotLights[0].ambient", glm::vec3(0.05f));
    backpackShader.setVec3("spotLights[0].diffuse", flashlightColor);
    backpackShader.setVec3("spotLights[0].specular", glm::vec3(1.0f));
    backpackShader.setFloat("spotLights[0].cosCutOff", glm::cos(glm::radians(flashlightCutOff)));

    float diff = (90.0f - flashlightCutOff) / 10;
    backpackShader.setFloat("spotLights[0].cosOuterCutOff", glm::cos(glm::radians(flashlightCutOff + diff * flashlightOuterCutOff)));

    // Material
    backpackShader.setFloat("material.shininess", materialShininess);

	backpack.draw(backpackShader);
}

void renderModel::lightSourceInit() 
{
    lightSource.shader = Shader((shaderDir + "lightSource.vs").c_str(), (shaderDir + "lightSource.fs").c_str());

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
    unsigned int VBO;

    glGenVertexArrays(1, &lightSource.VAO);
    glGenBuffers(1, &VBO);

    // simple Cube ==============
    glBindVertexArray(lightSource.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(simpleCube), simpleCube, GL_STATIC_DRAW);

    int stride = 8 * sizeof(float);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    lightSource.diffuse = glm::vec3(1.0f);
    lightSource.specular = glm::vec3(0.4f);
}

void renderModel::renderLightSource(Shader& lightSourceShader)
{
    lightSource.shader.use();

    // .vs
    float xPos = lightSourceRadius * cos(lightSourceRotationSpeed * glfwGetTime());
    float zPos = lightSourceRadius * sin(lightSourceRotationSpeed * glfwGetTime());
    lightSource.model = glm::translate(glm::mat4(1.0f), glm::vec3(xPos, lightSourceHeight, 3.0 + zPos));
    lightSource.model = glm::scale(lightSource.model, glm::vec3(0.2f));

    lightSource.shader.setMat4("model", lightSource.model);
    lightSource.shader.setMat4("transform", proj * view);

    // .fs
    lightSource.shader.setVec3("lightColor", lightSource.diffuse);

    // render
    glBindVertexArray(lightSource.VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // update lightPos
    lightPos = lightSource.model * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
}

void renderModel::updateImGuiConfig() 
{
    ImGui::Begin("Configs.");
    ImGui::Text("Innocent Grey fanboy");

    ImGui::SliderFloat("y pos", &modelPos.y, -5.0f, 5.0f);

    ImGui::Text("light");
    ImGui::Checkbox("turn off", &turnOffLightSource);
    ImGui::ColorEdit3("light source color", &lightSource.diffuse.x);
    ImGui::SliderFloat("rotation speed", &lightSourceRotationSpeed, 0.0f, 10.0f);
    ImGui::SliderFloat("rotation radius", &lightSourceRadius, 0.0f, 2.0f);
    ImGui::SliderFloat("height", &lightSourceHeight, -3.0f, 3.0f);
    //ImGui::SliderFloat("ambient0", &lightSource.x, 0.0f, 1.0f);
    ImGui::SliderFloat("specular0", &lightSource.specular.x, 0.0f, 1.0f);

    ImGui::Text("directional light");
    ImGui::SliderFloat("x", &dirLightDirection.x, -1.0f, 1.0f);
    ImGui::SliderFloat("y", &dirLightDirection.y, -1.0f, 1.0f);
    ImGui::SliderFloat("z", &dirLightDirection.z, -1.0f, 1.0f);
    ImGui::SliderFloat("diffuse", &dirLightDiffuse.x, 0.0f, 1.0f);
    ImGui::SliderFloat("ambient", &dirLightAmbient.x, 0.0f, 1.0f);
    ImGui::SliderFloat("specular", &dirLightSpecular.x, 0.0f, 1.0f);

    ImGui::Text("flashlight");
    ImGui::ColorEdit3("color", &flashlightColor.x);
    ImGui::SliderFloat("angle", &flashlightCutOff, 1.0f, 89.0f);
    ImGui::SliderInt("smoothing", &flashlightOuterCutOff, 0.0f, 10.0f);

    ImGui::Text("material");
    ImGui::SliderFloat("shininess", &materialShininess, 2.0f, 512.0f);

    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
};





// Input && ImGui
void renderModel::processInputs() 
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

void renderModel::processCamera() {
    float dTime = 0.001f;

    // WASD Movement
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

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

void renderModel::processChangeMouseInput()
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

void renderModel::ImGuiNewFrame() 
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void renderModel::ImGuiInit() 
{
	IMGUI_CHECKVERSION(); 
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
}


// ===== debug functions =====
void renderModel::testingCubeInit() 
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
    unsigned int VBO;

    glGenVertexArrays(1, &testingCube.VAO);
    glGenBuffers(1, &VBO);

    // simple Cube ==============
    glBindVertexArray(testingCube.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(simpleCube), simpleCube, GL_STATIC_DRAW);

    int stride = 8 * sizeof(float);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    loadTextureContainer(&testingCube.textureDiffuse, &testingCube.textureSpecular);
    testingCube.shader.use();
    testingCube.shader.setInt("material.texture_diffuse1", 0);
    testingCube.shader.setInt("material.texture_specular1", 1);

    testingCube.model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 3.0f));
}

void renderModel::renderTestingCube() {
    testingCube.shader.use();

    // .vs
    testingCube.model = glm::rotate(testingCube.model, glm::radians(0.5f), glm::vec3(1.0f, 1.0f, 1.0f));
    testingCube.shader.setMat4("model", testingCube.model);
    testingCube.shader.setMat4("view", view);
    testingCube.shader.setMat4("proj", proj);

    // .fs
    testingCube.shader.setInt("NR_POINT_LIGHTS", 1);
    testingCube.shader.setInt("NR_SPOTLIGHTS", 1);
    testingCube.shader.setBool("enableFlashlight", camera.enableFlashlight);


    testingCube.shader.setVec3("viewPos", camera.position);
    testingCube.shader.setInt("NR_POINT_LIGHTS", 1);
    testingCube.shader.setInt("NR_SPOTLIGHTS", 1);
    testingCube.shader.setBool("enableFlashlight", camera.enableFlashlight);

    // DirLight
    testingCube.shader.setVec3("dirLight.direction", dirLightDirection);
    testingCube.shader.setVec3("dirLight.ambient", glm::vec3(dirLightAmbient.x));
    testingCube.shader.setVec3("dirLight.diffuse", glm::vec3(dirLightDiffuse.x));
    testingCube.shader.setVec3("dirLight.specular", glm::vec3(0.5f));

    // PointLight
    testingCube.shader.setVec3("pointLights[0].position", lightPos);
    testingCube.shader.setVec3("pointLights[0].attenuationParams", glm::vec3(1.0f, 0.09f, 0.032f));
    testingCube.shader.setVec3("pointLights[0].ambient", glm::vec3(0.1f));
    testingCube.shader.setVec3("pointLights[0].diffuse", lightSource.diffuse);
    testingCube.shader.setVec3("pointLights[0].specular", lightSource.specular);

    // Flashlight
    testingCube.shader.setVec3("spotLights[0].position", camera.position);
    testingCube.shader.setVec3("spotLights[0].direction", camera.front);
    testingCube.shader.setVec3("spotLights[0].attenuationParams", glm::vec3(1.0f, 0.09f, 0.032f));
    testingCube.shader.setVec3("spotLights[0].ambient", glm::vec3(0.05f));
    testingCube.shader.setVec3("spotLights[0].diffuse", flashlightColor);
    testingCube.shader.setVec3("spotLights[0].specular", glm::vec3(1.0f));
    testingCube.shader.setFloat("spotLights[0].cosCutOff", glm::cos(glm::radians(flashlightCutOff)));

    float diff = (90.0f - flashlightCutOff) / 10;
    testingCube.shader.setFloat("spotLights[0].cosOuterCutOff", glm::cos(glm::radians(flashlightCutOff + diff * flashlightOuterCutOff)));

    // Material
    testingCube.shader.setFloat("material.shininess", 5);

    glBindVertexArray(testingCube.VAO);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, testingCube.textureDiffuse);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, testingCube.textureSpecular);

	glDrawArrays(GL_TRIANGLES, 0, 36);
}

// constructor
renderModel::renderModel(GLFWwindow* window, Camera& camera, int SCREEN_HEIGHT, int SCREEN_WIDTH) : window(window), camera(camera), SCREEN_HEIGHT(SCREEN_HEIGHT), SCREEN_WIDTH(SCREEN_WIDTH)
{
}


void renderModel:: loadTextureContainer(unsigned int* textureDiffuse, unsigned int* textureSpecular)
{
    glGenTextures(1, textureDiffuse);
    glBindTexture(GL_TEXTURE_2D, *textureDiffuse);

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
    glGenTextures(1, textureSpecular);
    glBindTexture(GL_TEXTURE_2D, *textureSpecular);

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

}
