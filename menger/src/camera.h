#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

class Camera {
public:
	glm::mat4 get_view_matrix() const;
	glm::mat4 lookAt() const;
	// zooming
	void zoomMouse(float zoomDir);
	void zoomKey(float zoomDir);
	// 'a' and 'd' keys
	void panOrbit();
	void strafeFPS();
	// FIXME: add functions to manipulate camera objects.
	void setFPS();
private:
	float zoom_speed = 0.01f;
	float camera_distance_ = 3.0;
	glm::vec3 look_ = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 up_ = glm::vec3(0.0f, 1.0, 0.0f);
	glm::vec3 eye_ = glm::vec3(0.0f, 0.0f, camera_distance_);
	// glm::vec3 look_ = glm::vec3(-1.0f, -1.0f, -1.0f);
    // glm::vec3 up_ = glm::vec3(1.0f, 2.0, 1.0f);
    // glm::vec3 eye_ = glm::vec3(camera_distance_, camera_distance_, camera_distance_);
	// Note: you may need additional member variables
	bool fps_mode = false;
};

#endif
