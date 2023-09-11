#include "window.h"
#include "../gfx/gl/glad.h"

#include <iostream>

#include <string>


window::window()
{
	screen_width = 800;
	screen_height = 600;
	dt = 0;

	gl_window = nullptr;
}

void window::init()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	int monitor_x, monitor_y;
	glfwGetMonitorPos(glfwGetPrimaryMonitor(), &monitor_x, &monitor_y);
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	int monitor_width = mode->width;
	int monitor_height = mode->height;

	int center_x = monitor_x + (monitor_width - screen_width) / 2;
	int center_y = monitor_y + (monitor_height - screen_height) / 2;


	GLFWwindow* window = glfwCreateWindow(screen_width, screen_height, "rhodes", NULL, NULL);

	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}

	glfwSetWindowPos(window, center_x, center_y);
	glfwMakeContextCurrent(window);

	gl_window = window;

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	run();
}

void window::run()
{
	double current_time = glfwGetTime();
	double time_last_fps_update = 0;

	while (!glfwWindowShouldClose(gl_window))
	{
		double new_time = glfwGetTime();
		dt = new_time - current_time;
		current_time = new_time;

		time_last_fps_update += dt;
		if (time_last_fps_update >= 0.1)
		{
			std::string title = "rhodes fps: ";
			title += std::to_string(int(1.0f / dt));
			glfwSetWindowTitle(gl_window, title.c_str());
			time_last_fps_update = 0;
		}

		update(dt);
		render();
		glfwSwapBuffers(gl_window);

		glfwPollEvents();
	}
}

window::~window()
{
	glfwTerminate();
}

void window::update(float dt)
{

}

void window::render()
{

}