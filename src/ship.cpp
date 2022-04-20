#define NOMINMAX
#include "ship.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <fstream>
#include <iostream>
#include <sstream>


// Ship::Ship(std::string filename, float speed) : 
//     Mesh(filename) {}

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


