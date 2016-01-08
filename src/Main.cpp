#include "header/RTRT.h"

int main(void)
{
	Camera* camera = new Camera();
	RTRT* tracer = new RTRT(camera);
	tracer->run();
	delete tracer;
}
