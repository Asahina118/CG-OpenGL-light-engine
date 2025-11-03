#include <iostream>
#include "Shader.h";

#include <string>
#include <fstream>
#include <sstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void Shader::checkShaderCompilation(const unsigned int& shader) {
	int success;
	char infolog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, infolog);
		std::cout << "shader::compilation::error\n" << infolog << std::endl;
	}
	else {
		std::cout << "[SUCCESS] shader compilation" << std::endl;
	}
}

void Shader::checkProgramCompilation(const unsigned int& program) {
	int success;
	char infoLog[512];
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		std::cout << "PROGRAM::SHADER_LINKING::ERROR\n" << infoLog << std::endl;
	}
	else {
		std::cout << "[SUCCESS] Program shader linking" << std::endl;
	}
}


Shader::Shader() {}

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		//open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);

		//read buffer contents into streams
		std::stringstream vShaderStream, fShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		//close file handlers
		vShaderFile.close();
		fShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e) {
		std::cout << "[ERROR] Cannot read shader files." << std::endl;
	}

	// we want const char* for the parameter type in the glShaderSource parameter
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	// Compile shaders
	unsigned int vertexShader, fragmentShader;
	int success;
	char infoLog[512];

	// here i'll just stick with tutorial first instead of using my own defined functions in case anything goes wrong later
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vShaderCode, NULL);
	glCompileShader(vertexShader);
	checkShaderCompilation(vertexShader);

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
	glCompileShader(fragmentShader);
	checkShaderCompilation(fragmentShader);

	this->ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);
	checkProgramCompilation(ID);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	std::cout << "[SUCCESS] Shader object initialization succeeded" << std::endl;
}

Shader::Shader(std::string vertexPath, std::string fragmentPath)
{
	
}

void Shader::use() {
	glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const {
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setMat4(const std::string& name, glm::mat4 transform) const {
	glUniformMatrix4fv(glGetUniformLocation(this->ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(transform));
}

void Shader::setVec3(const std::string& name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

void Shader::setVec3(const std::string& name, glm::vec3 value) const
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), value.x, value.y, value.z);
}

void Shader::checkCompilationErrors(unsigned int shader, std::string type) {
	int success;
	char infoLog[1024];
	if (type == "SHADER") {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "[ERROR] Shader compilation error of type = " << type << std::endl;
			std::cout << infoLog << std::endl;

		}
	}
	else {
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "[ERROR] Program linking error of type = " << type << std::endl;
			std::cout << infoLog << std::endl;
		}
	}

}

