#include "renderer.h"

#include <glm/gtc/type_ptr.hpp>

renderer& renderer::get_instance()
{
	static renderer instance;
	return instance;
}

void renderer::init(GLFWwindow* w_ptr)
{
	this->w_ptr = w_ptr;
	vao = 0;
	vbo = 0;
	ebo = 0;
	float vertices[] = {
		// positions        // colors
		-0.5f, -0.5f, 0.0f,  0.4f, 0.2f, 0.1f, // bottom left
		 0.5f, -0.5f, 0.0f,  0.4f, 0.2f, 0.1f, // bottom right
		 0.5f,  0.5f, 0.0f,  0.4f, 0.2f, 0.1f, // top right
		-0.5f,  0.5f, 0.0f,  0.4f, 0.2f, 0.1f  // top left
	};

	// Define the indices for the quad
	unsigned int indices[] = {
		0, 1, 2, // first triangle
		2, 3, 0  // second triangle
	};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);


	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	up = glm::vec3(0.0f, 1.0f, 0.0f);
	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	position = glm::vec3(0.0f, 0.0f, 3.0f);

	// initialize the target vector to be at the center of the quad
	target = glm::vec3(0.0f, 0.0f, 0.0f);


	// initialize the view matrix to look at the origin from a distance
	view = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), // camera position
		glm::vec3(0.0f, 0.0f, 0.0f), // camera target
		glm::vec3(0.0f, 1.0f, 0.0f)); // camera up vector

	// initialize the projection matrix to a perspective projection
	projection = glm::perspective(glm::radians(45.0f), // field of view angle
		(float)800 / (float)600, // aspect ratio
		0.1f, // near clipping plane
		100.0f); // far clipping plane

	//move camera up to simulate being a person in the world
position.y += 1.0f;

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
}

std::string renderer::read_file(const std::string& path)
{
	std::ifstream file(path);
	if (!file.is_open())
	{
		std::cout << "Failed to open file: " << path << std::endl;
		return "";
	}
	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();
	return buffer.str();
}

GLuint renderer::compile_shader(GLenum shader_type, const std::string& source)
{
	GLuint shader = glCreateShader(shader_type);
	const char* src = source.c_str();
	glShaderSource(shader, 1, &src, nullptr);
	glCompileShader(shader);
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE)
	{
		GLint length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		char* log = new char[length];
		glGetShaderInfoLog(shader, length, nullptr, log);
		std::cerr << "Failed to compile shader: " << log << std::endl;
		delete[] log;
		glDeleteShader(shader);
		return 0;
	}
	return shader;
}

GLuint renderer::link_program(GLuint vertex_shader, GLuint fragment_shader)
{
	GLuint program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);
	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status != GL_TRUE)
	{
		GLint length;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
		char* log = new char[length];
		glGetProgramInfoLog(program, length, nullptr, log);
		std::cerr << "Failed to link program: " << log << std::endl;
		delete[] log;
		glDeleteProgram(program);
		return 0;
	}
	return program;
}


void renderer::load_shaders()
{
	std::string vertex_shader_source = read_file("src/gfx/shaders/vertex.glsl");
	std::string fragment_shader_source = read_file("src/gfx/shaders/fragment.glsl");

	GLuint vertex_shader = compile_shader(GL_VERTEX_SHADER, vertex_shader_source);
	GLuint fragment_shader = compile_shader(GL_FRAGMENT_SHADER, fragment_shader_source);

	if (vertex_shader == 0 || fragment_shader == 0)
	{
		std::cerr << "Failed to load shaders" << std::endl;
		return;
	}

	program_id = link_program(vertex_shader, fragment_shader);

	if (program_id == 0)
	{
		std::cerr << "Failed to create program" << std::endl;
		return;
	}

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	std::cout << "Shaders loaded successfully" << std::endl;
}

void renderer::update(float dt)
{
	float speed = 2.0f; // camera speed per second
	float distance = speed * dt; // camera distance per frame
	float angle = dt * 1.5f; // camera angle per second

	// get the direction vector of the camera
	glm::vec3 direction = glm::normalize(target - position);

	// get the right vector of the camera
	glm::vec3 right = glm::normalize(glm::cross(direction, up));

	// check if W key is pressed
	if (glfwGetKey(w_ptr, GLFW_KEY_W) == GLFW_PRESS)
	{
		// move the camera forward along the direction vector
		position += direction * distance;
		target += direction * distance;
	}

	// check if S key is pressed
	if (glfwGetKey(w_ptr, GLFW_KEY_S) == GLFW_PRESS)
	{
		// move the camera backward along the direction vector
		position -= direction * distance;
		target -= direction * distance;
	}

	// check if Q key is pressed
	if (glfwGetKey(w_ptr, GLFW_KEY_A) == GLFW_PRESS)
	{
		// move the camera left along the right vector
		position -= right * distance;
		target -= right * distance;
	}

	// check if E key is pressed
	if (glfwGetKey(w_ptr, GLFW_KEY_D) == GLFW_PRESS)
	{
		// move the camera right along the right vector
		position += right * distance;
		target += right * distance;
	}

	// check if A key is pressed
	if (glfwGetKey(w_ptr, GLFW_KEY_Q) == GLFW_PRESS)
	{
		glm::quat rotation = glm::angleAxis(angle, up);
		target = rotation * (target - position) + position;
	}

	// check if D key is pressed
	if (glfwGetKey(w_ptr, GLFW_KEY_E) == GLFW_PRESS)
	{
		// rotate the camera right around the up vector
		glm::quat rotation = glm::angleAxis(-angle, up);
		target = rotation * (target - position) + position;
	}
	// update the view matrix with the new position and target
	view = glm::lookAt(position, target, up);
}

void renderer::draw()
{
	glClearColor(0.07f, 0.11f, 0.12f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(program_id);

	int modelLoc = glGetUniformLocation(program_id, "model");
	int viewLoc = glGetUniformLocation(program_id, "view");
	int projectionLoc = glGetUniformLocation(program_id, "projection");

	// set the matrix uniforms using the glm matrices
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glUseProgram(0);
}