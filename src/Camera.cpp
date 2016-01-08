#include "header/Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

Camera::Camera(void):speed(0.3){
	float c[12] = {1, 0, 0,
				  0, 1, 0, 
				  0, 0, 1,
				  0, 0, 0};
	C = glm::make_mat4x3(c);
	movingUp = false;
	movingDown = false;
	movingRight = false;
	movingLeft = false;
	movingForward = false;
	movingBack = false;
}

Camera::~Camera(void){
}

void Camera::calculateCameraMovement(){
	movedir = glm::vec3(0.0);
	movedir += movingRight ? speed*C[0] : movingLeft ? -speed*C[0] : glm::vec3(0.0) ;
	movedir += movingUp ? speed*C[1] : movingDown ? -speed*C[1] : glm::vec3(0.0) ;
	movedir += movingBack ? speed*C[2] : movingForward ? -speed*C[2] : glm::vec3(0.0) ;
	C[3] += movedir;
}

void Camera::moveDown(bool val){
	movingDown = val;
}

void Camera::moveUp(bool val){
	movingUp = val;
}

void Camera::moveRight(bool val){
	movingRight = val;
}

void Camera::moveLeft(bool val){
	movingLeft = val;
}

void Camera::moveForward(bool val){
	movingForward = val;
}

void Camera::moveBack(bool val){
	movingBack = val;
}

void Camera::pitch(float angle){
	rotMat = glm::mat4(	1,	0,	0, 0,
						0,	cos(angle),	sin(angle), 0,
						0,	-sin(angle),cos(angle), 0,
						0, 0, 0, 1);
	C = C*rotMat;
}

void Camera::yaw(float angle){
	rotMat = glm::mat4(	cos(angle),	0,	-sin(angle), 0,
						0,			1,	0, 0,
						sin(angle),	0,	cos(angle), 0,
						0, 0, 0, 1);
	C = C*rotMat;
}

void Camera::roll(float angle){
	rotMat = glm::mat4(	cos(angle),	sin(angle),	0, 0,
						-sin(angle), cos(angle), 0, 0,
						0,	0,	0, 0,
						0,	0,	0,	1);
	C = C*rotMat;
}

glm::mat4x3 Camera::getCameraMatrix(){
	return C;
}
