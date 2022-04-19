#define NOMINMAX
#include <iostream>
#include "camera.hpp"
#include "scene.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace std;


float camRot; // camera rotation about the y-axis
float downTilt; //initial camera rotatoin around x 


glm::vec3 camPos;  // camera position


// constructor
Camera::Camera(const CameraType cType) :
	camType(cType),
	width(1), height(1),
	fovy(45.0f),
	view(glm::mat4(1.0f)), proj(glm::mat4(1.0f)) {

	// *********************************** TODO: this line below is hard-coded to help you get a better initial view of the scene, REMOVE it afterwards
	view = glm::mat4(0.1f);
	
	camRot = 0.0f;
	
	if (camType == GROUND_VIEW) {
		proj = glm::perspective(45.0f, float(getW()/getH()), 0.1f, 100.0f);
		view[3][3] = 1.0f;
		camPos = glm::vec3(0.0f, -0.25f, -1.0f);
		view =  rotateX(-20.0) * view;
	} else if (camType == OVERHEAD_VIEW) {
		proj = glm::ortho(-0.5f, 0.5f, -0.5f, 0.5f, -2.0f, 2.0f);
		view[3][3] = 3.0f;
		camPos = glm::vec3(0.0f,0.0f, 0.0f);
		view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.3f)) * view;
		view =  rotateX(-90.0) * view;
	}
	updateViewProj();
}

void Camera::turnLeft() {
	// *********************************** TODO
	camRot = rotStep;
	camPos = glm::vec3(0.0f);
	updateViewProj();
}

void Camera::turnRight() {
	// *********************************** TODO
	camRot = rotStep * -1;
	camPos = glm::vec3(0.0f);
	updateViewProj();
}

void Camera::moveForward() {
	camRot = 0.0f;
	camPos = glm::vec3(0.0f, 0.0f, (moveStep / 10.0f));

	updateViewProj();
}

void Camera::moveBackward() {
	camRot = 0.0f;
	camPos = glm::vec3(0.0f, 0.0f, (moveStep / 10.0f) * -1.0f);
	updateViewProj();
}

void Camera::updateViewProj() {
	
	// *********************************** TODO
	updateView();
}

void Camera::updateView() {

	if (camType == GROUND_VIEW) {
		proj = glm::perspective(45.0f, float(getW()/getH()), 0.1f, 100.0f);
		view =  rotateX(20.0) * view;
		view = rotateY(camRot) * view;
		view = myTranslate(camPos.x, camPos.y, camPos.z) * view;
		view =  rotateX(-20.0) * view;
	} else if (camType == OVERHEAD_VIEW) {
		proj = glm::ortho(-0.5f, 0.5f, -0.5f, 0.5f, -2.0f, 2.0f);
		view =  rotateX(90.0) * view;
		view = rotateY(camRot) * view;
		view = myTranslate(camPos.x, camPos.y, camPos.z) * view;
		view =  rotateX(-90.0) * view;
	}
	

}

glm::mat4 Camera::myTranslate(float x, float y, float z) {
	glm::mat4 model = glm::mat4(
		1,		0,		0,		0,   
		0,		1,		0,		0,  
		0,		0,		1,		0,  
		x,		y,		z,		1 
	); 
	return model;	
}

glm::mat4 Camera::rotateX(float r) {
	glm::mat4 model = glm::mat4(
		1,		0,									0,									0,   
		0,		glm::cos(glm::radians(r)),		glm::sin(glm::radians(r)) * -1,			0,  
		0,		glm::sin(glm::radians(r)),		glm::cos(glm::radians(r)),				0,  
		0,		0,									0,									1 
	); 
	return model;	
}

glm::mat4 Camera::rotateY(float r) {
	glm::mat4 rot;
	rot = glm::mat4(
		glm::cos(glm::radians(r)),		0,									glm::sin(glm::radians(r)),			0,   
		0,								1,									0,									0,  
		glm::sin(glm::radians(r)) * -1,	0,									glm::cos(glm::radians(r)),			0,  
		0,								0,									0,									1 
	); 

	return rot;	
}


