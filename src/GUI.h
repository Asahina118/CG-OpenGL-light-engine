#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace GUI {
	void ImGuiInit(GLFWwindow*);
	void ImGuiNewFrame();
};