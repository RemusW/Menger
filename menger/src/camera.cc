#include "camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace {
	float pan_speed = 0.1f;
	float roll_speed = 0.1f;
	float rotation_speed = 0.05f;
	float zoom_speed = 0.1f;
};

// FIXME: Calculate the view matrix
glm::mat4 Camera::get_view_matrix() const
{
	glm::mat4 ViewMatrix = glm::translate(glm::mat4(), glm::vec3(0.5f, 0.0f, 0.0f));
	glm::mat4 CameraMatrix = glm::lookAt(eye_, glm::vec3(0,0,0), up_);
	return CameraMatrix;
	return glm::mat4(1.0);
	return ViewMatrix;
}

glm::mat4 Camera::lookAt() const
{
	return glm::mat4(1.0);
}

glm::mat4 Camera::zoomMouse() const 
{
	return glm::mat4(1.0);
}

glm::mat4 Camera::zoomKey() const
{
	return glm::mat4(1.0);
}