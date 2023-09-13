#include "window.h"
#include "../gfx/gl/glad.h"

#include "../gfx/renderer/renderer.h"
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

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
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


	gl_window = glfwCreateWindow(screen_width, screen_height, "terrain fps", NULL, NULL);

	if (gl_window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}

	glfwSetWindowPos(gl_window, center_x, center_y);
	glfwMakeContextCurrent(gl_window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	renderer::get_instance().init(gl_window);
	renderer::get_instance().load_shaders();


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
			std::string title = "terrain fps: ";
			title += std::to_string(int(1.0f / dt));
			glfwSetWindowTitle(gl_window, title.c_str());
			time_last_fps_update = 0;
		}
		glfwPollEvents();

		update(dt);

		renderer::get_instance().draw();
		glfwSwapBuffers(gl_window);
	}
}

window::~window()
{
	glfwTerminate();
}

void window::update(float dt)
{
	renderer::get_instance().update(dt);
}
