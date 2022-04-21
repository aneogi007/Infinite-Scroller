#define NOMINMAX
#include "ship.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <fstream>
#include <iostream>
#include <sstream>


//For Player Movement
void Ship::moveLeft(float speed) {
	if (getCurrentPosition().x > -Xconstraint) {
		glm::mat4 newModelMat = glm::translate(glm::mat4(1.0f), glm::vec3(-speed, 0.0f, 0.0f)) * getModelMat();
		setModelMat(newModelMat);		
	}
}

void Ship::moveRight(float speed) {
	if (getCurrentPosition().x < Xconstraint) {
		glm::mat4 newModelMat = glm::translate(glm::mat4(1.0f), glm::vec3(speed, 0.0f, 0.0f)) * getModelMat();
		setModelMat(newModelMat);
	}
}

void Ship::continueDrag() {


	if (ssr) {
		smartDragRight(1);
	} else if (ssl) {
		smartDragLeft(1);
	}

}

void Ship::smartDragLeft(int press) {
	
	ssl = true;

	if (smartDrag == 0) {
		smartDrag = press * DragExp;
	}

	if (getCurrentPosition().x > -(Xconstraint)) {
		if(currentDragFrame < smartDrag) {
			glm::mat4 newModelMat = glm::translate(glm::mat4(1.0f), glm::vec3(-currentDragSpeed, 0.0f, 0.0f)) * getModelMat();
			currentDragSpeed = currentDragSpeed - (shipSpeed / smartDrag);
			setModelMat(newModelMat);
			currentDragFrame++;
			return;
		}
	}
	resetDrag();
}

void Ship::smartDragRight(int press) {
	ssr = true;
	if (smartDrag == 0) {
		smartDrag = press * DragExp;
	}
	if (getCurrentPosition().x < Xconstraint) {
		if(currentDragFrame < smartDrag) {
			glm::mat4 newModelMat = glm::translate(glm::mat4(1.0f), glm::vec3(currentDragSpeed, 0.0f, 0.0f)) * getModelMat();
			currentDragSpeed = currentDragSpeed - (shipSpeed / smartDrag);
			setModelMat(newModelMat);
			currentDragFrame++;
			return;
		}
	}
	resetDrag();
}



