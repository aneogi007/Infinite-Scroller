#define NOMINMAX
#include "staticObstacle.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <fstream>
#include <iostream>
#include <sstream>


void StaticObstacle::moveBack(float speed) {
	glm::mat4 newModelMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, speed)) * getModelMat();
	
	if (getCurrentPosition().y >= Zconstraint) {
		newModelMat = getSpawnMat();
        setNewSpawn();
		destroyed = false;
	}
	//glm::mat4 newModelMat = translate * curModelMat;
	setModelMat(newModelMat);
}

void StaticObstacle::setNewSpawn() {
    //will offset original spawn by a random value withing the constraints
	float rDum = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/10.0f));
    rDum = rDum - 5.0f;
	//std::cout << rDum << "\n";
	height = 3.0f;
	//width = 3.0f

    spawnMat[3][0] = rDum;
    //get random value inbetween -5 and 5
}

