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

	// Mesh vertex format
	struct Vertex {
		glm::vec3 pos;		// Position
		glm::vec3 norm;		// Normal
	};
	// Local geometry data
	std::vector<Vertex> vertices;

protected:
	void release();		// Release OpenGL resources

	// Bounding box
	glm::vec3 minBB;
	glm::vec3 maxBB;

	glm::mat4 modelMat = glm::mat4(1.0f);  // the model matrix; apply this matrix to the model to transfer local coordinates to world coordinates

	// OpenGL resources
	GLuint vao;		// Vertex array object
	GLuint vbuf;	// Vertex buffer
	GLsizei vcount;	// Number of vertices

	std::string modelType;

private:
};

#endif
