#ifndef __renderer_h_
#define __renderer_h_

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "shader.h"

class renderer
{
public:
	static renderer& get_instance();
	void init(GLFWwindow* w_ptr);
	void load_shaders();

	void update(float dt);
	void draw();
private:
	GLFWwindow* w_ptr;
	std::string read_file(const std::string& path);

	GLuint compile_shader(GLenum shader_type, const std::string& source);
	GLuint link_program(GLuint vertex_shader, GLuint fragment_shader);

	GLuint vao;
	GLuint vbo;
	GLuint ebo;

	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
	glm::vec3 up;

	glm::vec3 position;
	glm::vec3 target;

	GLuint program_id;

	shader* shader_program;
};
#endif // __renderer_h_