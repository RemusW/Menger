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

void Camera::setFPS() {
	cout << "Setting camera fps_mode: " << fps_mode << endl;
	fps_mode = !fps_mode;
}

glm::vec3 Camera::getZ() const{
	glm::vec3 center = eye_ + camera_distance_*look_;
	return glm::normalize(center - eye_);
}

glm::vec3 Camera::getX() const{
	return glm::normalize(glm::cross(getZ(), up_));
}

glm::vec3 Camera::getY() const{
	return glm::normalize(glm::cross(getX(), getZ()));
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
	// Make them into vec4 with scaled w
	glm::vec4 f4 = glm::vec4(forward, glm::dot(forward, eye_));
	glm::vec4 l4 = glm::vec4(right, -1*glm::dot(right, eye_));
	glm::vec4 u4 = glm::vec4(up, -1*glm::dot(up, eye_));
	// reverse the Z because openGL
	f4.x *= -1;
	f4.y *= -1;
	f4.z *= -1;
	glm::mat4 view = glm::mat4();
	view[0] = l4;
	view[1] = u4;
	view[2] = f4;
	view[3] = glm::vec4(0,0,0,1);
	view = glm::transpose(view);
	return view;
}

void Camera::camRotation(glm::vec4 mouse_dir)
{
	float rad = 3.14f/180*rotation_speed;
	glm::vec3 world_mouse = glm::vec3(0.0f,1.0f,0.0f); 
	world_mouse.x = mouse_dir.x;
	world_mouse.y = mouse_dir.y;
	world_mouse.z = mouse_dir.z;
	world_mouse = glm::vec3(0,1,0);
	glm::vec3 rotAxis = glm::normalize(glm::cross(look_, world_mouse));
	glm::mat4 rot = glm::rotate(glm::mat4(1), rad, rotAxis);
	if(!fps_mode) {
		// ORBIT MODE: update eye
		// position center at the origin of the world
		// rotate camera
		// reapply translation on center back to original positon
		// or, rotate the -look_ and recompute the eye
		glm::vec3 center = eye_ + camera_distance_*look_;
		glm::vec4 look4 = glm::vec4(look_, 0);
		look4 = look4 * rot;
		look_.x = look4.x;
		look_.y = look4.y;
		look_.z = look4.z;
		eye_ = center - camera_distance_*glm::vec3(look_);
		up_ = glm::vec3(glm::vec4(up_, 0) * rot);
		// cout << "Center: " << glm::to_string(center) << endl;
		// cout << "EYE: " << glm::to_string(eye_) << endl;
		// cout << "Look_: " << glm::to_string(look_) << endl;
	}
	else {
		// FPS MODE: update center
		glm::vec4 look4 = glm::vec4(look_, 0);
		look4 = look4 * rot;
		look_.x = look4.x;
		look_.y = look4.y;
		look_.z = look4.z;
		up_ = glm::vec3(glm::vec4(up_, 0) * rot);
	}
	cout << "Eye: 			" << glm::to_string(eye_) << endl;
	cout << "Look:		 	" << glm::to_string(look_) << endl;
	cout << "Camera_dist: 	" << camera_distance_<< endl;
	cout << "Center: 		" << glm::to_string(eye_ + camera_distance_*look_) << endl << endl;
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
	if (camera_distance_ >= 0 )
		eye_ += zoomDir * zoom_speed * look_;
	else
		camera_distance_ = 0;
}

void Camera::zoomKeyWS(float zoomDir)
{
	if(!fps_mode) {
		// ORBIT MODE: decrease camera distance and update eye
		zoomMouse(zoomDir);
	}
	else {
		// FPS MODE: move eye and center at same rate
		eye_ += zoomDir * zoom_speed * look_;
	}
}

void Camera::camKeyAD(float dir)
{
	if(!fps_mode) {
		// ORBIT MODE: shift the center

	}
	else {
		// FPS MODE: shift the eye side to side
	}
	glm::vec3 tangent = getX();
	eye_ += dir * pan_speed * tangent;
}

void Camera::camKeyUpDown(float dir)
{
	if(!fps_mode) {
		// ORBIT MODE: shift the center
	}
	else {
		// FPS MODE: shift the eye side to side
	}
	eye_ += dir * pan_speed * getY();
}

void Camera::camKeyLeftRight(float dir)
{
	float rad = dir*3.14f/180*roll_speed;
	glm::mat4 rot = glm::rotate(glm::mat4(1), rad, getZ());
	glm::vec4 up4 = glm::vec4(up_, 0);
	up4 = up4 * rot;
	up_.x = up4.x;
	up_.y = up4.y;
	up_.z = up4.z;
}