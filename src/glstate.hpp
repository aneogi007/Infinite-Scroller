#ifndef GLSTATE_HPP
#define GLSTATE_HPP

#include <string>
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "gl_core_3_3.h"
#include "mesh.hpp"

// Manages OpenGL state, e.g. camera transform, objects, shaders
class GLState {
public:
	GLState();
	~GLState();
	// Disallow copy, move, & assignment
	GLState(const GLState& other) = delete;
	GLState& operator=(const GLState& other) = delete;
	GLState(GLState&& other) = delete;
	GLState& operator=(GLState&& other) = delete;

	// Callbacks
	void initializeGL();
	void paintGL();
	void resizeGL(int w, int h);

	// Camera control
	bool isCamRotating() const { return camRotating; }
	void beginCameraRotate(glm::vec2 mousePos);
	void endCameraRotate();
	void rotateCamera(glm::vec2 mousePos);
	void offsetCamera(float offset);
	void animate(float x, float y);

	// Set object to display
	void showTetrahedron();
	void showObjFile(const std::string& filename);

	// Per-vertex attributes
	struct Vertex {
		glm::vec3 pos;		// Position
		glm::vec3 norm;		// Normal
	};

protected:
	// Initialization
	void initShaders();
	void initGeometry();

	// Camera state
	int width, height;		// Width and height of the window
	float fovy;				// Vertical field of view in degrees
	glm::vec3 camCoords;	// Camera spherical coordinates
	bool camRotating;		// Whether camera is currently rotating
	glm::vec2 initCamRot;	// Initial camera rotation on click
	glm::vec2 initMousePos;	// Initial mouse position on click

	// Object state
	enum ObjMode {
		OBJMODE_TETRAHEDRON,		// Show the built-in tetrahedron
		OBJMODE_MESH,				// Show a given obj file
	};
	ObjMode objMode;				// Which object state are we in
	std::string meshFilename;		// Name of the obj file being shown
	std::unique_ptr<Mesh> mesh;		// Pointer to mesh object

	// OpenGL state
	GLuint shader;		// GPU shader program
	GLuint xformLoc;	// Transformation matrix location
	GLuint vao;			// Vertex array object
	GLuint vbuf;		// Vertex buffer
	GLuint ibuf;		// Index buffer
	GLsizei vcount;		// Number of indices to draw
};

#endif
