#include "camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <glm/gtx/string_cast.hpp>

using namespace std;

namespace {
	float pan_speed = 0.1f;
	float roll_speed = 0.1f;
	float rotation_speed = 0.05f;
	float zoom_speed = 0.1f;
};

void Camera:: setFPS() {
	fps_mode = !fps_mode;
}

// FIXME: Calculate the view matrix
glm::mat4 Camera::get_view_matrix() const
{
	// glm::mat4 ViewMatrix = glm::translate(glm::mat4(), glm::vec3(0.5f, 0.0f, 0.0f));
	glm::mat4 CameraMatrix = glm::lookAt(eye_, glm::vec3(0,0,0), up_);
	// glm::mat4 CameraMatrix = glm::lookAt(eye_, look_, up_);
	// std::cout << glm::to_string(CameraMatrix) << std::endl;
	CameraMatrix = Camera::lookAt();
	// std::cout << glm::to_string(CameraMatrix) << std::endl << std::endl;
	return CameraMatrix;
}

glm::mat4 Camera::lookAt() const
{
	glm::vec3 center = eye_ + camera_distance_*look_;
	glm::vec3 forward = glm::normalize(center - eye_);				// Z axis
	glm::vec3 right = glm::normalize(glm::cross(forward, up_));		// X axis
	glm::vec3 up = glm::normalize(glm::cross(right, forward));		// Y axis
	// reverse the Z because openGL
	glm::vec4 f4 = glm::vec4(forward, glm::dot(forward, eye_));
	f4.x *= -1;
	f4.y *= -1;
	f4.z *= -1;
	glm::vec4 l4 = glm::vec4(right, -1*glm::dot(right, eye_));
	glm::vec4 u4 = glm::vec4(up, -1*glm::dot(up, eye_));
	glm::mat4 view = glm::mat4();
	view[0] = l4;
	view[1] = u4;
	view[2] = f4;
	view[3] = glm::vec4(0,0,0,1);
	// int s = 0;
	// for(int i=0; i<4; i++) {
	// 	for(int k=0; k<4; k++) {
	// 		view[i][k] = s++;
	// 	}
	// }
	view = glm::transpose(view);
	return view;
}

void Camera::zoomMouse(float zoomDir) 
{
	// if center = eye + camer_distance * look
	// center and look will remain constant while we update eye and camera distance
	// eye will move back in the -look direction
	// cout << "Eye: 			" << glm::to_string(eye_) << endl;
	// cout << "Look:		 	" << glm::to_string(look_) << endl;
	// cout << "Camera_dist: 	" << camera_distance_<< endl;
	// cout << "Center: 		" << glm::to_string(eye_ + camera_distance_*look_) << endl;
	camera_distance_ +=  zoomDir * zoom_speed;
	eye_ += zoomDir * zoom_speed * look_;
}

void Camera::zoomKey(float zoomDir)
{
	cout << "zoomign with w s" << endl;
	if(!fps_mode) {
		camera_distance_ +=  zoomDir * zoom_speed;
		eye_ += zoomDir * zoom_speed * look_;
	}
	else {
		eye_ += zoomDir * zoom_speed * look_;
	}
}