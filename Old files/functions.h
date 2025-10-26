#ifndef OWNFUNCTIONS_H
#define OWNFUNCTIONS_H

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

extern const float PI;
extern const char* globalFragmentShaderSource;
extern float verticesWithColor[];

GLFWwindow* Init(const char* name);
void checkShaderCompilationf(const unsigned int& shader);
void checkProgramCompilationf(const unsigned int& program);
void processInput(GLFWwindow* window);
unsigned int vertexShaderInit();
unsigned int fragmentShaderInit();
unsigned int fragmentShaderInit(const char* fragmentShaderSource);
unsigned int shaderProgramInit(unsigned int* shaders, const int& length);
unsigned int VAOInit();

void renderColoredVertices(GLFWwindow* window);

void renderTexture(GLFWwindow* window);

void renderTransformation(GLFWwindow* window);

void going3D(GLFWwindow* window);

void renderCamera(GLFWwindow* window);

void mouse_callback(GLFWwindow* window, double xPos, double yPos);

#endif
