#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <string>
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "gl_core_3_3.h"

enum CameraType {
	GROUND_VIEW,   // front view
	OVERHEAD_VIEW  // overhead view
};

class Camera {
public:
	// Ctor and Dtor:
	Camera(const CameraType cType = GROUND_VIEW);
	~Camera() {}

	// *********************************** TODO: keyboard control
	void turnLeft();
	void turnRight();
	void moveForward();
	void moveBackward();
	// ***********************************

	// access methods:
	inline void setWH(const int w, const int h) {  // update width and height
		width = w;
		height = h;
		updateViewProj();
	}
	inline int getW() { return width; }
	inline int getH() { return height; }
	inline float getFovy() { return fovy; }
	inline glm::mat4 getView() { return view; }
	inline glm::mat4 getProj() { return proj; }
	glm::mat4 myTranslate(float x, float y, float z);

protected:
	// Camera state
	CameraType camType;     // of the two types
	int width, height;		// Width and height of the window
	float fovy;				// Vertical field of view in degrees
	glm::mat4 view, proj;   // veiw and projection matrices

	GLfloat rotStep = 2.0f;   // rotation step
	GLfloat moveStep = 0.2f;  // moving step

	// *********************************** TODO
	void updateViewProj();  // update view and projection matrices of this class: view, proj
	void updateView();
	void updateProj();
	
	
	glm::mat4 rotateX(float r);
	glm::mat4 rotateY(float r);
	
};

#endif
