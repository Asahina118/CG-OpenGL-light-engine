#include <iostream>
#include "Shader.h";

#include <string>
#include <fstream>
#include <sstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void Shader::checkShaderCompilation(const unsigned int& shader, std::string path) {
	int success;
	char infolog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, infolog);
		std::cout << "[ERROR] shader compilation failed:\n" << infolog << std::endl;
	}
	else {
		std::cout << "[SUCCESS] shader compiled from " << path << std::endl;
	}
}

void Shader::checkProgramCompilation(const unsigned int& program) {
	int success;
	char infoLog[512];
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		std::cout << "[ERROR] shader linking failed:\n" << infoLog << std::endl;
	}
	else {
		std::cout << "[SUCCESS] programe shader linking succeeded" << std::endl;
	}
	std::cout << '\n';
}


Shader::Shader() {}

Shader::Shader(const char* vertexPath, const char* fragmentPath) 
{
	shaderInit(vertexPath, fragmentPath);
}

Shader::Shader(std::string vertexPath, std::string fragmentPath)
{
	shaderInit(vertexPath.c_str(), fragmentPath.c_str());
}

Shader::Shader(std::string vertexPath, std::string geometryPath, std::string fragmentPath)
{
	shaderInit(vertexPath.c_str(), geometryPath.c_str(), fragmentPath.c_str());
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

void Shader::setVec2(const std::string& name, glm::vec2 value) const
{
	glUniform2f(glGetUniformLocation(ID, name.c_str()), value.x, value.y);
}

void Shader::setVec3(const std::string& name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

void Shader::setVec3(const std::string& name, glm::vec3 value) const
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), value.x, value.y, value.z);
}

void Shader::shaderInit(const char* vertexPath, const char* geometryPath, const char* fragmentPath)
{
	std::string vertexCode;
	std::string geometryCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream gShaderFile;
	std::ifstream fShaderFile;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		vShaderFile.open(vertexPath);
		std::stringstream vShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		vShaderFile.close();
		vertexCode = vShaderStream.str();
	}
	catch (std::ifstream::failure e) {
		std::cout << "[ERROR] Cannot read vertex shader from " << vertexPath << std::endl;
	}

	try {
		gShaderFile.open(geometryPath);
		std::stringstream gShaderStream;
		gShaderStream << gShaderFile.rdbuf();
		gShaderFile.close();
		geometryCode = gShaderStream.str();
	}
	catch (std::ifstream::failure e) {
		std::cout << "[ERROR] Cannot read geometry vertex shader from " << geometryPath << std::endl;
	}

	try {
		std::stringstream fShaderStream;
		fShaderFile.open(fragmentPath);
		fShaderStream << fShaderFile.rdbuf();
		fShaderFile.close();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e) {
		std::cout << "[ERROR] Cannot read fragment shader from " << fragmentPath << std::endl;
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* gShaderCode = geometryCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	// Compile shaders
	unsigned int vertexShader, geometryShader, fragmentShader;
	int success;
	char infoLog[512];

	// here i'll just stick with tutorial first instead of using my own defined functions in case anything goes wrong later
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vShaderCode, NULL);
	glCompileShader(vertexShader);
	checkShaderCompilation(vertexShader, vertexPath);

	geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(geometryShader, 1, &gShaderCode, NULL);
	glCompileShader(geometryShader);
	checkShaderCompilation(geometryShader, geometryPath);

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
	glCompileShader(fragmentShader);
	checkShaderCompilation(fragmentShader, fragmentPath);

	this->ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, geometryShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);
	checkProgramCompilation(ID);

	glDeleteShader(vertexShader);
	glDeleteShader(geometryShader);
	glDeleteShader(fragmentShader);
}

void Shader::shaderInit(const char* vertexPath, const char* fragmentPath)
{
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		//open files
		vShaderFile.open(vertexPath);

		//read buffer contents into streams
		std::stringstream vShaderStream;
		vShaderStream << vShaderFile.rdbuf();

		//close file handlers
		vShaderFile.close();

		vertexCode = vShaderStream.str();

	}
	catch (std::ifstream::failure e) {
		std::cout << "[ERROR] Cannot read vertex shader from " << vertexPath << std::endl;
	}

	try {
		std::stringstream fShaderStream;
		fShaderFile.open(fragmentPath);
		fShaderStream << fShaderFile.rdbuf();
		fShaderFile.close();
		fragmentCode = fShaderStream.str();

	}
	catch (std::ifstream::failure e) {
		std::cout << "[ERROR] Cannot read fragment shader from " << fragmentPath << std::endl;
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
	checkShaderCompilation(vertexShader, vertexPath);

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
	glCompileShader(fragmentShader);
	checkShaderCompilation(fragmentShader, fragmentPath);

	this->ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);
	checkProgramCompilation(ID);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
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

