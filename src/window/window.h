#ifndef __window_h_
#define __window_h_

#include <GLFW/glfw3.h>

class window
{
public:
	window();
	~window();

	void init();
private:
	void run();

	void update(float dt);

	GLFWwindow* gl_window;
	int16_t screen_width;
	int16_t screen_height;

	float dt;
};

#endif // __window_h_