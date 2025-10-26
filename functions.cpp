// debugging GUI
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "functions.h"
#include "Shader.h"

//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 850;

void processInput(GLFWwindow* window) {
	//if the key is not pressed, glfwGetKey would return GFLW_RELEASE
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

GLFWwindow* Init(const char* name) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, name, NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return NULL;
	}
	glfwMakeContextCurrent(window);

	// Initialize function pointer for OpenGL from GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return NULL;
	}

	return window;
}

void ImGuiInit(GLFWwindow* window) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

}

void ImGuiNewFrame() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

}

void ImGuiConfig3D(bool& drawObject, float& size) {
	ImGui::Begin("Configs.");
	ImGui::Text("‚â‚Á‚Ù`");
	ImGui::Checkbox("Draw Object", &drawObject);
	ImGui::SliderFloat("Size", &size, 0.0f, 10.0f);
	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

void ImGuiConfigCamera(bool* draw, float* size, float* FOV) {
	ImGui::Begin("Configs.");
	ImGui::Text("Ciallo~");
	ImGui::Checkbox("Draw Object", draw);
	ImGui::SliderFloat("Size", size, 0.0f, 10.0f);
	ImGui::SliderFloat("FOV", FOV, 0.5f, 140.0f);
	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

void processAlpha3D(GLFWwindow* window, float& alpha) {
	if (glfwGetKey(window, GLFW_KEY_LEFT_BRACKET) == GLFW_PRESS) {
		alpha += 0.01;
		if (alpha > 1.0) alpha = 1;
	}
	else if (glfwGetKey(window, GLFW_KEY_RIGHT_BRACKET) == GLFW_PRESS) {
		alpha -= 0.01;
		if (alpha < 0.0) alpha = 0;
	}
}

void processModel3D(GLFWwindow* window, glm::mat4& model) {
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		model = glm::rotate(model, glm::radians(1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	}
	else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		model = glm::rotate(model, glm::radians(-1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	}

}

void processMovementLR3D(GLFWwindow* window, glm::mat4& view) {
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, 0.01f));
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -0.01f));
	}

}

void processMovementFB3D(GLFWwindow* window, glm::mat4& view) {
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		view = glm::translate(view, glm::vec3(0.01f, 0.0f, 0.0f));
	} 
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		view = glm::translate(view, glm::vec3(-0.01f, 0.0f, 0.0f));
	}
}

void updateFOV3D(GLFWwindow* window, glm::mat4& proj, float& FOV) {
	if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) {
		FOV += 0.1;
		proj = glm::perspective(glm::radians(FOV), (float) SCREEN_WIDTH / (float) SCREEN_HEIGHT, 0.1f, 100.0f);
	} 
	else if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) {
		FOV -= 0.1;
		proj = glm::perspective(glm::radians(FOV), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
	}
}

#pragma region [renderCamera]
float lastX = 400, lastY = 300;
float theta = -90.0f, phi = 0.0f;
bool firstMouse = true;
bool mouseInput;

void processModel3DRotation(GLFWwindow* window, glm::mat4& model, glm::vec3 axis) {
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		model = glm::rotate(model, glm::radians(1.0f), axis);
	}
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		model = glm::rotate(model, glm::radians(-1.0f), axis);
	}
}

void processMovementCameraFB(GLFWwindow* window, float cameraSpeed, glm::vec3& cameraPos, glm::vec3& cameraFront) {
	float yPos = cameraPos.y;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cameraPos += cameraSpeed * cameraFront;
		cameraPos.y = yPos;
	} 
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cameraPos -= cameraSpeed * cameraFront;
		cameraPos.y = yPos;
	}
}

void processMovementCameraLR(GLFWwindow* window, float cameraSpeed, glm::vec3& cameraPos, glm::vec3& cameraFront, glm::vec3& cameraUp) {
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cameraPos += cameraSpeed * glm::normalize(glm::cross(cameraUp, cameraFront));
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cameraPos -= cameraSpeed * glm::normalize(glm::cross(cameraUp, cameraFront));
	}
}

void processMovementCameraUD(GLFWwindow* window, float cameraSpeed, glm::vec3& cameraPos, glm::vec3 cameraUp) {
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		cameraPos += cameraSpeed * glm::normalize(cameraUp);
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		cameraPos -= cameraSpeed * glm::normalize(cameraUp);
	}
}

void updateCameraFront(GLFWwindow* window, glm::vec3& cameraFront) {
	glm::vec3 direction;

	// converting spherical -> cartesian
	direction.x = cos(glm::radians(theta)) * cos(glm::radians(phi));
	direction.z = sin(glm::radians(theta)) * cos(glm::radians(phi));
	direction.y = sin(glm::radians(phi));

	cameraFront = glm::normalize(direction);
};

bool lock = false;
void mouseInputEnable(GLFWwindow* window, bool& mouseInput) {
	if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS && !lock) {
		lock = true;
		if (mouseInput) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			mouseInput = false;
			std::cout << "[DEBUG] Current Input Mode is NORMAL" << std::endl;

		}
		else {
			glfwSetCursorPos(window, (float)(SCREEN_WIDTH / 2), (float) (SCREEN_HEIGHT / 2));
			firstMouse = true;
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			mouseInput = true;
			std::cout << "Setting to Disabled" << std::endl;

		}
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) != GLFW_PRESS && lock) {
		lock = false;
	}

}

void updateFOVCamera(GLFWwindow* window, glm::mat4& proj, float& FOV) {
	proj = glm::perspective(glm::radians(FOV), (float) SCREEN_WIDTH / (float) SCREEN_HEIGHT, 0.1f, 500.0f);
}

void processInputsCamera(GLFWwindow* window, float& alpha, glm::mat4& model, glm::mat4& proj, float& FOV, bool& mouseInput) {
	const glm::vec3 ModelxAxis = glm::vec3(1.0f, 0.0f, 0.0f);

	processInput(window);
	processAlpha3D(window, alpha);
	processModel3D(window, model);
	processModel3DRotation(window, model, ModelxAxis);
	updateFOVCamera(window, proj, FOV);
	mouseInputEnable(window, mouseInput);
}

void processCameraMovements(GLFWwindow* window, glm::mat4& view, glm::vec3& cameraPos, glm::vec3& cameraFront, glm::vec3& cameraUp) {
	float cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		cameraSpeed = 0.5f;
	}
	else {
		cameraSpeed = 0.05f;
	}

	processMovementCameraFB(window, cameraSpeed, cameraPos, cameraFront);
	processMovementCameraLR(window, cameraSpeed, cameraPos, cameraFront, cameraUp);
	processMovementCameraUD(window, cameraSpeed, cameraPos, cameraUp);
	updateCameraFront(window, cameraFront);
	view = glm::lookAt(cameraPos, cameraFront + cameraPos, cameraUp);
}

unsigned int VAOInitCamera() {
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
	glEnable(GL_DEPTH_TEST);
	unsigned int VAO, VBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCube), verticesCube, GL_STATIC_DRAW);

	int stride = 5 * sizeof(float);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	return VAO;

}

void textureInitCamera(unsigned int& texture1, unsigned int& texture2) {
	glGenTextures(1, &texture1);
	glGenTextures(1, &texture2);

	glBindTexture(GL_TEXTURE_2D, texture1);

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

	glBindTexture(GL_TEXTURE_2D, texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	imagePath = "awesomeface.png";
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

void playGroundCamera() {
	// manually create our camera frame
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);

	// this is a reversed direction
	glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
	glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
	glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

	// Now usually, we can make our own lookAt matix using the above vectors. But OpenGL can do all the above procedures as well as also initialized the LookAt matrix for us. If you are interested in manually making the matrix check out https://learnopengl.com/Getting-started/Camera for the formula

	glm::mat4 view;
	// @params: cameraPosition, targetPosition, upVector in the world space
	view = glm::lookAt(
		glm::vec3(0.0f, 0.0f, 3.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f)
	);


}

void windowFocusCallback(GLFWwindow* window, int focused) {
	if (focused) firstMouse = true;
}
// This is for glfw function functional parameter input
void mouse_callback(GLFWwindow* window, double xPos, double yPos) {
	if (firstMouse) {
		xPos = lastX;
		yPos = lastY;
		firstMouse = false;
	}

	float offsetX = xPos - lastX;
	float offsetY = lastY - yPos; // reversed because the y-coordinate for glfw window and openGL are opposite
	lastX = xPos;
	lastY = yPos;

	const float sensitivity = 0.1f;
	offsetX *= sensitivity;
	offsetY *= sensitivity;

	theta += offsetX;
	phi += offsetY;

	if (phi > 89.0f) phi = 89.0f;
	if (phi < -89.0f) phi = -89.0f;

	ImGui_ImplGlfw_CursorPosCallback(window, xPos, yPos);
};

void renderCamera(GLFWwindow* window) {
	unsigned int texture1, texture2;
	textureInitCamera(texture1, texture2);
	Shader* shaderProgram = new Shader("camera.vs", "camera.fs");
	unsigned int VAO = VAOInitCamera();

	glm::mat4 model = glm::mat4(1.0f);

	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::mat4 view = glm::lookAt(cameraPos, cameraFront + cameraPos, cameraUp);

	glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 500.0f);

	shaderProgram->use();
	shaderProgram->setInt("texture1", 0);
	shaderProgram->setInt("texture2", 1);

	float radius = 10.0f;
	float camX, camZ;
	float alpha = 1.0f;
	float FOV = 45.0f;

	//ImGui variables
	bool draw = true;
	mouseInput = true; // true = using mouse to control the camera
	float size = 1.0f;

	ImGuiInit(window);

	// Mouse related setups
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetWindowFocusCallback(window, windowFocusCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		ImGuiNewFrame();
		processInputsCamera(window, alpha, model, proj, FOV, mouseInput);

		if (mouseInput)
			processCameraMovements(window, view, cameraPos, cameraFront, cameraUp);

		if (draw) {
			shaderProgram->use();
			shaderProgram->setFloat("size", size);
			shaderProgram->setFloat("alpha", alpha);
			shaderProgram->setMat4("model", model);
			shaderProgram->setMat4("view", view);
			shaderProgram->setMat4("proj", proj);

			glBindVertexArray(VAO);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture1);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, texture2);

			glDrawArrays(GL_TRIANGLES, 0, 36);

			// second box	
			shaderProgram->setMat4("model", glm::mat4(1.0f));
			shaderProgram->setMat4("view", view);
			shaderProgram->setMat4("proj", proj);

			shaderProgram->setFloat("size", 100.0f);
			shaderProgram->setFloat("alpha", 0.0f);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		ImGuiConfigCamera(&draw, &size, &FOV);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}


#pragma endregion
