#ifndef INPUT_H
#define INPUT_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <stdlib.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "Config.h"

class Input 
{
public:
	static Input& getInstance(){
		static Input instance;
		return instance;
	}
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		getInstance().key_callbackImpl(window, key, scancode, action, mods);
	}

	static void mouse_callback(GLFWwindow* window, double xpos, double ypos)
	{
		getInstance().mouse_callbackImpl(window, xpos, ypos);
	}
	
	void mouse_callbackImpl(GLFWwindow* window, double xpos, double ypos)
	{ 
		int horiMovement = xpos - windowx/2; // change 400 to half the window size.
		int vertMovement = ypos - windowy/2;
		
		camera->pitch(-vertMovement / vertMouseSensitivity);
		camera->yaw(-horiMovement / horizMouseSensitivity);

		glfwSetCursorPos(window, windowx/2, windowy/2);
	}

	void key_callbackImpl(GLFWwindow* window, int key, int scancode, int action, int mods){
		if(action == GLFW_PRESS){
			switch(key){
				case GLFW_KEY_ESCAPE:
					glfwSetWindowShouldClose(window, GL_TRUE);
					break;
				case GLFW_KEY_D:
					camera->moveRight(true);
					break;
				case GLFW_KEY_A:
					camera->moveLeft(true);
					break;
				case GLFW_KEY_SPACE:
				case GLFW_KEY_E:
					camera->moveUp(true);
					break;
				case GLFW_KEY_LEFT_CONTROL:
				case GLFW_KEY_Q:
					camera->moveDown(true);
					break;
				case GLFW_KEY_W:
					camera->moveForward(true);
					break;
				case GLFW_KEY_S:
					camera->moveBack(true);
					break;
				case GLFW_KEY_LEFT: 
					camera->yaw(-0.3);
					break;
				case GLFW_KEY_RIGHT: 
					camera->yaw(0.3);
					break;
				case GLFW_KEY_R:
					//FSQShaderProgram = LoadShaders("C:\\gfx\\RTRT\\FSQ.vert","C:\\gfx\\RTRT\\FSQ.frag");
					break;
				default:
					// Nothing
					break;
			}
		}else if (action == GLFW_RELEASE){ // Key is released
			switch(key){ 
				case GLFW_KEY_D:
					camera->moveRight(false);
					break;
				case GLFW_KEY_A:
					camera->moveLeft(false);
					break;
				case GLFW_KEY_SPACE:
				case GLFW_KEY_E:
					camera->moveUp(false);
					break;
				case GLFW_KEY_LEFT_CONTROL:
				case GLFW_KEY_Q:
					camera->moveDown(false);
					break;
				case GLFW_KEY_W:
					camera->moveForward(false);
					break;
				case GLFW_KEY_S:
					camera->moveBack(false);
					break;
				default:
					//nothing
					break;
			}
		}
	}
	void setCamera(Camera* cam){
		camera = cam;
	}
private:
	Camera* camera;
	Input(void) { // private constructor necessary to allow only 1 instance
		vertMouseSensitivity  = std::stof(Config::lookupString("vertMouseSens"));
		horizMouseSensitivity = std::stof(Config::lookupString("horiMouseSens"));
		windowx = std::stof(Config::lookupString("windowx"));
		windowy = std::stof(Config::lookupString("windowy"));
	} 
	Input(Input const&); // prevent copies
    void operator=(Input const&); // prevent assignments
	GLfloat vertMouseSensitivity;
	GLfloat horizMouseSensitivity;
	GLfloat windowx;
	GLfloat windowy;
};

#endif
