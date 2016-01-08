#pragma once
#include "Camera.h"
#include "Input.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>


class RTRT
{
public:
	RTRT(void);
	RTRT(Camera* cam);
	~RTRT(void);
	int run();
	void setupGL();
private:
	Camera * camera;
};

