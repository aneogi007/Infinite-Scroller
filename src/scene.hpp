#ifndef SCENE_HPP
#define SCENE_HPP

#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <glm/glm.hpp>
#include "staticObstacle.hpp"
#include "gl_core_3_3.h"
#include "mesh.hpp"
#include "ship.hpp"
#include "light.hpp"
#include "floor.hpp"


class Scene {
public:
	// ctor and dtor:
	Scene() { parseScene(); }
	~Scene() { objects.clear(); }
	// access:
	inline std::vector<std::shared_ptr<Mesh>>& getSceneObjects() { return objects; }
	inline std::vector<std::shared_ptr<Ship>>& getShips() { return ships; }
	inline std::vector<std::shared_ptr<StaticObstacle>>& getObstacles() { return obstacles; }
	inline std::vector<std::shared_ptr<Floor>>& getFloors() { return floors; }

	std::vector<Light> lights;		// Lights

	// output:
	static void printMat3(const glm::mat3 mat);
	static void printMat4(const glm::mat4 mat);
	static void printVec3(const glm::vec3 vec) {
		std::cout << vec.x << ", " << vec.y << ", " << vec.z << std::endl;
	}
	static void printVec4(const glm::vec4 vec) {
		std::cout << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << std::endl;
	}

protected:
	// scene construction:
	void parseScene();  // TODO: read ./models/scene.txt to get the rotation & translation matrices of the .obj models


	unsigned int nObj;  // number of objects in the scene
	std::vector<std::shared_ptr<Mesh>> objects;  // mesh objects in the scene
	std::vector<std::shared_ptr<Ship>> ships;
	std::vector<std::shared_ptr<StaticObstacle>> obstacles;
	std::vector<std::shared_ptr<Floor>> floors;
	
	
};

#endif
