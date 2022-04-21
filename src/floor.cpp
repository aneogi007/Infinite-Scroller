#define NOMINMAX
#include "floor.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <fstream>
#include <iostream>
#include <sstream>


void Floor::moveBack(float speed) {
	glm::mat4 newModelMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, speed)) * getModelMat();
	
	if (getCurrentPosition().y >= Zconstraint) {
		newModelMat = spawnMat;
	}

    setModelMat(newModelMat);
}