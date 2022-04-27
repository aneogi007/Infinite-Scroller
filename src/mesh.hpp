#ifndef MESH_HPP
#define MESH_HPP

#include <string>
#include <vector>
#include <utility>
#include <glm/glm.hpp>
#include "gl_core_3_3.h"

class Mesh {
public:
	Mesh(std::string filename, bool keepLocalGeometry = false);
	~Mesh() { release(); }
	// Disallow copy, move, & assignment
	Mesh(const Mesh& other) = delete;
	Mesh& operator=(const Mesh& other) = delete;
	Mesh(Mesh&& other) = delete;
	Mesh& operator=(Mesh&& other) = delete;

	// Return the bounding box of this object
	std::pair<glm::vec3, glm::vec3> boundingBox() const
	{ return std::make_pair(minBB, maxBB); }

	void load(std::string filename, bool keepLocalGeometry = false);
	void draw();

	// access:
	inline void setModelMat(const glm::mat4 model) { modelMat = model; }
	inline glm::mat4 getModelMat() { return modelMat; }
	
	inline void setModelType(std::string modelName) { modelType = modelName; } 
	inline std::string getModelType() { return modelType; }

	inline void setStartModelMat(const glm::mat4 model) { startModelMat = model; }
	inline glm::mat4 getStartModelMat() { return startModelMat; }


	// //for player movement left to right
	// void moveLeft(float speed);
	// void moveRight(float speed);

	//for obstacles and floors
	//void moveBack(float speed);

	// Mesh vertex format
	struct Vertex {
		glm::vec3 pos;			// Position
		glm::vec3 face_norm;	// Face normal
		glm::vec3 smooth_norm;	// Smoothed normal
		Vertex();
	};
	// Local geometry data
	std::vector<Vertex> vertices;

	//return current position on grid (x, z)
	glm::vec2 getCurrentPosition() { return glm::vec2(modelMat[3][0], modelMat[3][2]); }
	
	void setheight(float newHeight) { height = newHeight; }
	float getheight() { return height; }

	void setwidth(float newWidth) { width = newWidth; }
	float getwidth() { return width; }

	glm::vec3 getObjColor() { return objColor; };

	//determines if obstacle was collided with or not
	//reduces multiple calls for one collision



protected:
	void release();		// Release OpenGL resources

	// Bounding box
	glm::vec3 minBB;
	glm::vec3 maxBB;
	
	glm::mat4 modelMat = glm::mat4(1.0f);  // the model matrix; apply this matrix to the model to transfer local coordinates to world coordinates

	//Start Position moving background
	glm::mat4 startModelMat = glm::mat4(1.0f);

	// OpenGL resources
	GLuint vao;		// Vertex array object
	GLuint vbuf;	// Vertex buffer
	GLsizei vcount;	// Number of vertices


	//holds name of the mesh
	std::string modelType;

	//Holds length and width of object (bounding box)
	float height = 2.0f;
	float width = 2.0f;
	
	glm::vec3 objColor;


private:
};

#endif
