#include "camera.h"

camera::camera(const glm::vec3& position, const glm::vec3& target)
{
	this->position = position;
	this->target = target;
	proj_matrix = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 100.0f);
}
