#define NOMINMAX
#include <iostream>
#include "glstate.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>x`
#include "util.hpp"
	
// Constructor
GLState::GLState() :
	shader(0),
	xformLoc(0),
	camGround(GROUND_VIEW),
    camOverhead(OVERHEAD_VIEW) {}

// Destructor
GLState::~GLState() {
	// Release OpenGL resources
	if (shader)	glDeleteProgram(shader);
}

// Called when OpenGL context is created (some time after construction)
void GLState::initializeGL() {
	// General settings
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);

	// Initialize OpenGL state
	initShaders();

	// Create the scene
	scene = std::unique_ptr<Scene>(new Scene());
}

// Called when window requests a screen redraw
void GLState::paintGL() {
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set shader to draw with
	glUseProgram(shader);

	// Construct a transformation matrix for the camera
	glm::mat4 xform(1.0f), proj, view;

	auto objects = scene->getSceneObjects();
	for (auto& meshObj : objects) {
		glm::mat4 modelMat = meshObj->getModelMat();
		proj = (whichCam == GROUND_VIEW) ? camGround.getProj() : camOverhead.getProj();
		view = (whichCam == GROUND_VIEW) ? camGround.getView() : camOverhead.getView();
		xform = proj * view * modelMat;  // opengl does matrix multiplication from right to left
		glUniformMatrix4fv(xformLoc, 1, GL_FALSE, glm::value_ptr(xform));
		// Draw the mesh
		meshObj->draw();
	}		

	glUseProgram(0);
}

// Called when window is resized
void GLState::resizeGL(int w, int h) {
	// Tell OpenGL the new dimensions of the window
	camGround.setWH(w, h);
	camOverhead.setWH(w, h);
	glViewport(0, 0, w, h);
}

// Create shaders and associated state
void GLState::initShaders() {
	// Compile and link shader files
	std::vector<GLuint> shaders;
	shaders.push_back(compileShader(GL_VERTEX_SHADER, "shaders/v.glsl"));
	shaders.push_back(compileShader(GL_FRAGMENT_SHADER, "shaders/f.glsl"));
	shader = linkProgram(shaders);
	// Cleanup extra state
	for (auto s : shaders)
		glDeleteShader(s);
	shaders.clear();

	// Get uniform locations
	xformLoc = glGetUniformLocation(shader, "xform");
}
