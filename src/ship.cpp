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

void Ship::smartMove() {
	if (getCurrentPosition().x >= Xconstraint) {
		currSpeed *= (-1.0f / 1.5f);
	} else if (getCurrentPosition().x <= -Xconstraint) {
		currSpeed *= (-1.0f / 1.5f);
	}
	
	if(leftMove) {
		if (currSpeed > -maxSpeed) {
			currSpeed -= acceleration;
			if (currentTilt < maxTilt) {
				rotateCC(tilt);	
				currentTilt += tilt;
			}
			
		}
		
		//std::cout << currSpeed << "\n";
		glm::mat4 newModelMat = glm::translate(glm::mat4(1.0f), glm::vec3(currSpeed, 0.0f, 0.0f)) * getModelMat();
		setModelMat(newModelMat);
		//rotateZ(.08);

	} else if (rightMove) {
		if (currSpeed < maxSpeed) {
			currSpeed += acceleration;
			if (currentTilt > -maxTilt) {
				rotateCC(-tilt);
				currentTilt -= tilt;	
			}
			
		}
		
		//std::cout << currSpeed << "\n";
		glm::mat4 newModelMat = glm::translate(glm::mat4(1.0f), glm::vec3(currSpeed, 0.0f, 0.0f)) * getModelMat();
		setModelMat(newModelMat);
		
		//rotateZ(-.08);
		
	} else if (driftMode) {
		//std::cout << "DRIFTING" << "\n";
		if (currSpeed > 0) {
			currSpeed -= coolDrag;
			glm::mat4 newModelMat = glm::translate(glm::mat4(1.0f), glm::vec3(currSpeed, 0.0f, 0.0f)) * getModelMat();
			setModelMat(newModelMat);
			if (currentTilt < 0) {
				rotateCC(tilt / 2);	
				currentTilt += tilt / 2;
			}
			
		} else if (currSpeed < 0) {
			currSpeed += coolDrag;
			glm::mat4 newModelMat = glm::translate(glm::mat4(1.0f), glm::vec3(currSpeed, 0.0f, 0.0f)) * getModelMat();
			setModelMat(newModelMat);
			
			if (currentTilt > 0) {
				rotateCC(-tilt / 2);	
				currentTilt -= tilt / 2;
			}
		} else {
			driftMode = false;
			std::cout << "speed is 0" << "\n";
		}

	}
}

void Ship::rotateCC(float speed) {
	//modelMat = glm::rotate(modelMat, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	modelMat = glm::rotate(modelMat, glm::radians(speed), glm::vec3(0.0f, 1.0f, 0.0f));
	//modelMat = glm::rotate(modelMat, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

void Ship::rotateZ(float speed) {
	modelMat = glm::rotate(modelMat, glm::radians(speed), glm::vec3(0.0f, 0.0f, 1.0f));
}

// void Ship::continueDrag() {


// 	if (ssr) {
// 		smartDragRight(1);
// 	} else if (ssl) {
// 		smartDragLeft(1);
// 	}

// }

// void Ship::smartDragLeft(int press) {
	
// 	ssl = true;

// 	if (smartDrag == 0) {
// 		smartDrag = press * DragExp;
// 	}

// 	if (getCurrentPosition().x > -(Xconstraint)) {
// 		if(currentDragFrame < smartDrag) {
// 			glm::mat4 newModelMat = glm::translate(glm::mat4(1.0f), glm::vec3(-currentDragSpeed, 0.0f, 0.0f)) * getModelMat();
// 			currentDragSpeed = currentDragSpeed - (shipSpeed / smartDrag);
// 			setModelMat(newModelMat);
// 			currentDragFrame++;
// 			return;
// 		}
// 	}
// 	resetDrag();
// }

// void Ship::smartDragRight(int press) {
// 	ssr = true;
// 	if (smartDrag == 0) {
// 		smartDrag = press * DragExp;
// 	}
// 	if (getCurrentPosition().x < Xconstraint) {
// 		if(currentDragFrame < smartDrag) {
// 			glm::mat4 newModelMat = glm::translate(glm::mat4(1.0f), glm::vec3(currentDragSpeed, 0.0f, 0.0f)) * getModelMat();
// 			currentDragSpeed = currentDragSpeed - (shipSpeed / smartDrag);
// 			setModelMat(newModelMat);
// 			currentDragFrame++;
// 			return;
// 		}
// 	}
// 	resetDrag();
// }



