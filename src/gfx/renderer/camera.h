#ifndef __camera_h_
#define __camera_h_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class camera {
public:
    camera(const glm::vec3& position, const glm::vec3& target);

    glm::mat4 get_view_matrix() const;

    void set_position(const glm::vec3& position);

    void set_target(const glm::vec3& target);

private:
    glm::vec3 position;
    glm::vec3 target;

    glm::vec3 up;
    glm::mat4 proj_matrix;
};

#endif // __camera_h_