#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

class Camera {
public:
	glm::mat4 get_view_matrix() const;
	glm::mat4 lookAt() const;
	// rotation
	void camRotation();
	// zooming
	void zoomMouse(float zoomDir);
	void zoomKeyWS(float zoomDir);
	// 'a' and 'd' keys
	void camKeyAD(float dir);
	void camKeyUpDown(float dir);
	void camKeyLeftRight(float dir);
	// FIXME: add functions to manipulate camera objects.
	void setFPS();
	//void getFPS(){return fps};
private:
	float zoom_speed = 0.1f;
	float pan_speed = 0.2f;
	float roll_speed = 5.0f;
	float rotation_speed = 0.1f;
	float camera_distance_ = 3.0;
	glm::vec3 look_ = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 up_ = glm::vec3(0.0f, 1.0, 0.0f);
	glm::vec3 eye_ = glm::vec3(0.0f, 0.0f, camera_distance_);
	// glm::vec3 look_ = glm::vec3(-1.0f, -1.0f, -1.0f);
    // glm::vec3 up_ = glm::vec3(1.0f, 2.0, 1.0f);
    // glm::vec3 eye_ = glm::vec3(camera_distance_, camera_distance_, camera_distance_);
	// Note: you may need additional member variables
	bool fps_mode = false;
	glm::vec3 getZ() const;
	glm::vec3 getX() const;
	glm::vec3 getY() const;
};

#endif
