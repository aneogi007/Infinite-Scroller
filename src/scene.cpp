#define NOMINMAX
#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "scene.hpp"
using namespace std;
namespace fs = std::filesystem;

std::string trim(const std::string& line);

void Scene::parseScene() {  // TODO: rewrite
	fs::path modelsDir = "models";					// Models directory
	fs::path sceneFile = modelsDir / "scene.txt";	// Scene file
	ifstream istr;
	istr.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		// Read the number of objects in the scene
		istr.open(sceneFile);
		istr >> nObj;
		
		for (int i = 0; i < nObj; i++) {
			// Skip any lines that don't contain ".obj" (e.g. whitespace)
			string line;
			std::size_t found = std::string::npos;
			while (found == std::string::npos) {
				getline(istr, line);
				found = line.find(".obj");
			}

			// Load the mesh
			string objFilename = trim(line);
			string modelName = objFilename.substr(0, objFilename.size()-4);

			// TODO: read the rotation and translation of the mesh

			//read matrix in form of 
			//  __				     __
			// | 	rXx, rYx, rZx, 0   |
			// | 	rXy, rYy, rZy, 0   |
			// | 	rXz, rYz, rZz, 0   |
			// |__  tx,  ty,  tz,  1 __|
			//

			float rXx, rXy, rXz, rYx, rYy, rYz, rZx, rZy, rZz, tx, ty, tz;
			istr >> rXx >> rXy >> rXz >>  rYx >> rYy >> rYz >> rZx >> rZy >> rZz >> tx >> ty >> tz;

			glm::mat4 model = glm::mat4(
				rXx, rYx, rZx, 0,   
			 	rXy, rYy, rZy, 0,  
			 	rXz, rYz, rZz, 0,  
			  	tx,  ty,  tz,  1 
			);

			if (modelName == "motorcycle") {
				float speed = .02f;
				float drag = 1.5f;
				float height = 10.0f;
				float width = 0.5f;
				auto mosh = std::shared_ptr<Ship>(new Ship((modelsDir / objFilename).string(), speed, drag, height, width));  // construct the mesh
				ships.push_back(mosh);  // store the mesh
				mosh->setModelMat(model);
				mosh->setModelType(modelName);
			} else if (modelName == "car") {
				float speed = .01f;
				float drag = 1.0f;
				float height = 3.0f;
				float width = 2.0f;
				auto mosh = std::shared_ptr<Ship>(new Ship((modelsDir / objFilename).string(), speed, drag, height, width));  // construct the mesh
				ships.push_back(mosh);  // store the mesh
				mosh->setModelMat(model);
				mosh->setModelType(modelName);
				
			} else if (modelName == "obstacle") {
				
				int numObstacles = 2;
				for (int j = 0; j < numObstacles; j++) {
					auto mesh = std::shared_ptr<StaticObstacle>(new StaticObstacle((modelsDir / objFilename).string()));  // construct the mesh
					obstacles.push_back(mesh);  // store the mesh
					mesh->setModelMat(model);
					mesh->setModelType(modelName);
					mesh->setSpawnMat(model);
					mesh->setNewSpawn();
				}
				
			} else if (modelName == "floor") {
				int numObstacles = 1;
				for (int j = 0; j < numObstacles; j++) {
					auto mesh = std::shared_ptr<Floor>(new Floor((modelsDir / objFilename).string()));  // construct the mesh
					floors.push_back(mesh);  // store the mesh
					mesh->setModelMat(model);
					mesh->setModelType(modelName);
					mesh->setSpawn(model);
				}
			} 
		}
	}
	catch (const std::exception& e) {
		// Construct an error message and throw again
		std::stringstream ss;
		ss << "Failed to read " << sceneFile << ": " << e.what();
		throw std::runtime_error(ss.str());
	}
}

void Scene::printMat3(const glm::mat3 mat) {
	for (int i = 0; i < 3; i++) {  // copy the rotation matrix
		for (int j = 0; j < 3; j++) {
			std::cout << mat[i][j] << " ";
		}
		std::cout << "\n";
	}
	std::cout << std::endl;
}

void Scene::printMat4(const glm::mat4 mat) {
	for (int i = 0; i < 4; i++) {  // copy the rotation matrix
		for (int j = 0; j < 4; j++) {
			std::cout << mat[i][j] << " ";
		}
		std::cout << "\n";
	}
	std::cout << std::endl;
}

// Trim leading and trailing whitespace
std::string trim(const std::string& line) {
	const std::string whitespace = " \t\r\n";
	auto first = line.find_first_not_of(whitespace);
	if (first == std::string::npos)
		return "";
	auto last = line.find_last_not_of(whitespace);
	auto range = last - first + 1;
	return line.substr(first, range);
}
