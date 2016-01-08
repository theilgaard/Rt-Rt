#include <iostream>
#include <fstream>
#include <string.h>

#include "header/ShaderManager.h"

ShaderManager::ShaderManager(void){}
ShaderManager::~ShaderManager(void ){}

std::string ShaderManager::loadShaderCodeFromFile(std::string filePath) {
	std::string shaderCode;
	std::ifstream shaderCodeStream(filePath, std::ios::in);
	if(shaderCodeStream.is_open()) {
		std::string line = "";
		while(getline(shaderCodeStream, line))
			shaderCode += line + "\n";
		shaderCodeStream.close();
	}
	else {
		//todo do some proper error handling here.
		throw std::runtime_error("Could not open file " + filePath + "\n ");
	}
	return shaderCode;
}

GLuint ShaderManager::compileShader(std::string shaderSource, GLenum shaderType) {
	GLuint shaderID = glCreateShader(shaderType);
	//printf("Compiling shader : %s\n", vertex_file_path);
	char const * shaderSourcePtr = shaderSource.c_str();
	glShaderSource(shaderID, 1, &shaderSourcePtr , NULL);
	glCompileShader(shaderID);

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Check the Shader
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(shaderID, InfoLogLength, NULL, &ShaderErrorMessage[0]);
		printf("%s\n", &ShaderErrorMessage[0]);
	}
	return shaderID;
}

GLuint ShaderManager::linkShaderProgram(std::vector<GLuint> * shaderIDs) {
	// Link the program
	GLuint ProgramID = glCreateProgram();
	for(int i = 0 ; i < shaderIDs->size() ; i++) {
		glAttachShader(ProgramID, shaderIDs->at(i));
	}
	glLinkProgram(ProgramID);

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	return ProgramID;
}

GLuint ShaderManager::loadShaderProgram(std::vector<std::pair<std::string, GLenum>> * shaders) {

	// Create the shaders
	std::vector<GLuint> shaderIDs; 
	std::string shaderCode;
	for(int i = 0 ; i < shaders->size() ; i++) {
		shaderCode = loadShaderCodeFromFile((shaders->at(i)).first);
		shaderIDs.push_back(compileShader(shaderCode, (shaders->at(i)).second));
	}

	GLuint shaderProgram = linkShaderProgram(&shaderIDs);
	for(int i = 0 ; i < shaderIDs.size() ; i ++) 
		glDeleteShader(shaderIDs[i]);

	return shaderProgram;
}
