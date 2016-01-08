#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>

class Camera{
public:
	Camera(void);
	~Camera(void);
	glm::mat4x3 getCameraMatrix();
	void moveUp(bool val);
	void moveDown(bool val);
	void moveRight(bool val);
	void moveLeft(bool val);
	void moveForward(bool val);
	void moveBack(bool val);
	void pitch(float angle);
	void roll(float angle);
	void yaw(float angle);
	void calculateCameraMovement();
private:
	float speed;
	glm::vec3 movedir;
	glm::mat4x3 C;
	glm::mat4 pitchMat, rollMat, yawMat, rotMat;
	int pitchAngle, rollAngle, yawAngle;
	bool movingUp, movingDown, movingLeft, movingRight, movingForward, movingBack;
};