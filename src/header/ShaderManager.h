#pragma once
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>


struct Shader {
	std::vector<std::pair<std::string, GLenum>> * shaders;
	GLuint shaderProgramID;
};

class ShaderManager
{
public:

	ShaderManager(void);
	~ShaderManager(void);

	Shader* getShaderProgram(int programIndex) {
		return ShaderPrograms[programIndex];
	}

	GLuint loadShaderProgram(std::vector<std::pair<std::string, GLenum>> * shaders);
private:
	std::vector<Shader*> ShaderPrograms;

	std::string loadShaderCodeFromFile(std::string filePath);
	GLuint compileShader(std::string shaderSource, GLenum shaderType);
	GLuint linkShaderProgram(std::vector<GLuint> * shaderIDs);
};
