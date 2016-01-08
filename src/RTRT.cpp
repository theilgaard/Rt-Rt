#include "header/RTRT.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <cmath>
#include <math.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <exception>

#include "header/ShaderManager.h"
#include "header/Input.h"
#include "header/Config.h"

#define localWorkgroupSize 16

static double PI = 4 * std::atan(1);

GLuint FSQShaderProgram;
GLuint ComputeShaderProgram;

GLuint rayTraceTexture;

int windowx;
int windowy;
int fov;
double midWindowX;
double midWindowY;

//Values for the sphere array
GLuint sphereArrayBo;
int numSpheres = 30;
float maxSphereSize = 15;
float minSphereSize = 1;
float sphereMaxPos = 100;

struct sphere {
	float x, y, z, ra;
	float r, g, b, a;
};


static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}



float rand_FloatRange(float a, float b)
{
	return ((b-a)*((float)rand()/(float)RAND_MAX))+a;
}

RTRT::RTRT(void){}
RTRT::RTRT(Camera* cam) : camera(cam){}
RTRT::~RTRT(void){}

void RTRT::setupGL()
{	
	//Generate the texture we render to from the raytracer
	glGenTextures(1, &rayTraceTexture);
	glBindTexture(GL_TEXTURE_2D, rayTraceTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, windowx, windowy, 
					0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);

	
	ShaderManager * shaderMan = new ShaderManager();

	//Load FSQ shader program
	std::vector<std::pair<std::string, GLenum>> shaders;
	shaders.push_back(std::make_pair("resources/shaders/FSQ.vert", GL_VERTEX_SHADER));
	shaders.push_back(std::make_pair("resources/shaders/FSQ.frag", GL_FRAGMENT_SHADER));
	FSQShaderProgram = shaderMan->loadShaderProgram(&shaders);
	
	glUseProgram(FSQShaderProgram);

	//set the location for the renderTex texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, rayTraceTexture);
	glUniform1i(glGetUniformLocation(FSQShaderProgram, "renderTex"), 0);

	//Load compute shader program
	std::vector<std::pair<std::string, GLenum>> shaders2;
	shaders2.push_back(std::make_pair("resources/shaders/ComputeShader.glsl", GL_COMPUTE_SHADER));
	ComputeShaderProgram = shaderMan->loadShaderProgram(&shaders2);

	glUseProgram(ComputeShaderProgram);
	glBindImageTexture(0, rayTraceTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);

	//set height and width uniforms in the compute shader
	glUniform1f(glGetUniformLocation(ComputeShaderProgram, "windowx"), (GLfloat)windowx);
	glUniform1f(glGetUniformLocation(ComputeShaderProgram, "windowy"), (GLfloat)windowy);

/*
	//Setup sphere array SSBO
	glGenBuffers(1, &sphereArrayBo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, sphereArrayBo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, numSpheres * sizeof(struct sphere), NULL, GL_STATIC_DRAW);
	GLint bufMask = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT;

	//generate random spheres
	srand(10);
	struct sphere *spheres = (struct sphere *)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, numSpheres*sizeof(struct sphere), bufMask);
	for(int i = 0; i < numSpheres; i++) {
		spheres[i].x = rand_FloatRange(-sphereMaxPos, sphereMaxPos);
		spheres[i].y = rand_FloatRange(-sphereMaxPos, sphereMaxPos);
		spheres[i].z = rand_FloatRange(-sphereMaxPos, sphereMaxPos);
		spheres[i].ra = rand_FloatRange(minSphereSize, maxSphereSize);

		spheres[i].r = rand_FloatRange(0.0f, 1.0f);
		spheres[i].g = rand_FloatRange(0.0f, 1.0f);
		spheres[i].b = rand_FloatRange(0.0f, 1.0f);
		spheres[i].a = 1.0f;
	}
	*/
	//Set texture for the compute shader program... This doesn't work tho, i think
	//glUniform1i(glGetUniformLocation(ComputeShaderProgram, "outputTex"), 0);

	
	
}

int RTRT::run()
{
	try{
	Config::parseConfigFile("resources/config/config.cfg");
	windowx = std::stoi(Config::lookupString("windowx"));
	windowy = std::stoi(Config::lookupString("windowy"));
	fov		= std::stoi(Config::lookupString("fov"));
	}catch(std::exception& e){
		printf("Exception: %s\n", e.what());
	}
	midWindowX = windowx / 2;
	midWindowY = windowy / 2;



	GLFWwindow* window;
	Input &input = Input::getInstance();
	input.setCamera(this->camera);
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);
    window = glfwCreateWindow(windowx, windowy, "RTRT!", NULL, NULL);

    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

	//glfwSwapInterval(1); //vsync i think
    glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwSetKeyCallback(window, &Input::key_callback);
	glfwSetCursorPosCallback(window, &Input::mouse_callback);
	glfwSetCursorPos(window, midWindowX, midWindowY);

	GLenum glewErr;
	if (glewErr = glewInit()) {
		std::cout << glewGetErrorString(glewErr) << std::endl;
		exit(EXIT_FAILURE);
	}

	setupGL();

	while(!glfwWindowShouldClose(window)) {
		glMemoryBarrier(GL_ALL_BARRIER_BITS);
		//Run the compute shader:
		glUseProgram(ComputeShaderProgram);		
		GLint cLoc = glGetUniformLocation(ComputeShaderProgram, "C");
		glUniformMatrix4x3fv(cLoc, 1, false, glm::value_ptr(camera->getCameraMatrix()));
		glDispatchCompute(windowx/localWorkgroupSize, windowy/localWorkgroupSize, 1);
		glMemoryBarrier(GL_ALL_BARRIER_BITS);

		//Run the FSQ shader:
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(FSQShaderProgram);
		
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		glfwSwapBuffers(window);
        glfwPollEvents();

		camera->calculateCameraMovement();
	}

	glfwTerminate();
	return 0;
}
