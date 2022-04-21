#define NOMINMAX
#include <iostream>
#include <fstream>
#include <sstream>
#include "glstate.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "util.hpp"
	
// Constructor
GLState::GLState() :
	shader(0),
	modelMatLoc(0),
	viewProjMatLoc(0),
	camGround(GROUND_VIEW),
    camOverhead(OVERHEAD_VIEW),
	camPosLoc(0),
	objColorLoc(0),
	ambStrLoc(0),
	diffStrLoc(0),
	specStrLoc(0),
	specExpLoc(0) {}

// Destructor
GLState::~GLState() {
	// Release OpenGL resources
	if (shader)	glDeleteProgram(shader);
}

// Called when OpenGL context is created (some time after construction)
void GLState::initializeGL() {
	// General settings
	glm::vec3 bgColor;
	bgColor.r = 246;
	bgColor.g = 241;
	bgColor.b = 231;
	bgColor /= 255.0f;
	glClearColor(bgColor.r, bgColor.g, bgColor.b, 1.0);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);

	// Initialize OpenGL state
	initShaders();

	// Create lights
	lights.resize(Light::MAX_LIGHTS);

	// Create the scene
	scene = std::unique_ptr<Scene>(new Scene());
	init = true;
}

// Called when window requests a screen redraw
void GLState::paintGL() {
	//std::cout << "This is keyPress" << keyPressTime <<"\n";
	// Clear the color and depth buffers
	
	if (keyPress == true) {
		keyPressTime++;
	}

	if (keyPress == false) {
		keyPressTime = 0;
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set shader to draw with
	glUseProgram(shader);

	// Construct a transformation matrix for the camera
	glm::mat4 viewProjMat(1.0f);

	glm::mat4 xform(1.0f), proj, view;

	auto objects = scene->getSceneObjects();

	auto ships = scene->getShips();

	auto obstacles = scene->getObstacles();

	auto floors = scene->getFloors();
	
	{
		auto& meshObj = ships[getCurrentSpaceShip()];
		
		if (meshObj->inDragState()) {
			meshObj->continueDrag();
		} 

		if (isMovingLeft() && !meshObj->inDragState() && !isMovingRight()) {
			meshObj->moveLeft(meshObj->getShipSpeed());
		} else if (isMovingRight() && !meshObj->inDragState()) {
			meshObj->moveRight(meshObj->getShipSpeed());
		}


		glm::mat4 modelMat = meshObj->getModelMat();
		proj = (whichCam == GROUND_VIEW) ? camGround.getProj() : camOverhead.getProj();
		view = (whichCam == GROUND_VIEW) ? camGround.getView() : camOverhead.getView();
		xform = proj * view * modelMat;  // opengl does matrix multiplication from right to left
		
		viewProjMat = proj * view;
		glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, glm::value_ptr(modelMat));
		glUniformMatrix4fv(viewProjMatLoc, 1, GL_FALSE, glm::value_ptr(viewProjMat));

		// glm::vec3 camPos = glm::vec3(glm::inverse(view)[3]);
		// glUniform3fv(camPosLoc, 1, glm::value_ptr(camPos));
		// Draw the mesh
		meshObj->draw();
	}

	for (auto& meshObj : obstacles) {
		
		meshObj->moveBack(mapVelocity);
		if ((!meshObj->isdestroyed()) && detectCollision((ships[getCurrentSpaceShip()]),(meshObj))) {
			std::cout << "COLLISION DETECTED " << ", ";
		}
		if (meshObj->isdestroyed()) {
			continue;
		}
		
		glm::mat4 modelMat = meshObj->getModelMat();
		proj = (whichCam == GROUND_VIEW) ? camGround.getProj() : camOverhead.getProj();
		view = (whichCam == GROUND_VIEW) ? camGround.getView() : camOverhead.getView();
		xform = proj * view * modelMat;  // opengl does matrix multiplication from right to left
		
		viewProjMat = proj * view;
		glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, glm::value_ptr(modelMat));
		glUniformMatrix4fv(viewProjMatLoc, 1, GL_FALSE, glm::value_ptr(viewProjMat));

		glm::vec3 camPos = glm::vec3(glm::inverse(view)[3]);
		glUniform3fv(camPosLoc, 1, glm::value_ptr(camPos));


		// Draw the mesh
		meshObj->draw();
	}

	for (auto& meshObj : floors) {

		meshObj->moveBack(mapVelocity);

		glm::mat4 modelMat = meshObj->getModelMat();
		proj = (whichCam == GROUND_VIEW) ? camGround.getProj() : camOverhead.getProj();
		view = (whichCam == GROUND_VIEW) ? camGround.getView() : camOverhead.getView();
		xform = proj * view * modelMat;  // opengl does matrix multiplication from right to left
		
		viewProjMat = proj * view;
		glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, glm::value_ptr(modelMat));
		glUniformMatrix4fv(viewProjMatLoc, 1, GL_FALSE, glm::value_ptr(viewProjMat));

		glm::vec3 camPos = glm::vec3(glm::inverse(view)[3]);
		glUniform3fv(camPosLoc, 1, glm::value_ptr(camPos));

		// Draw the mesh
		meshObj->draw();

	}
	for (auto& l : lights)
		if (l.getEnabled()) l.drawIcon(viewProjMat);

	glUseProgram(0);
}

// Read config file
void GLState::parseLights() {
	
		// Read the file contents into a string stream	
	float ambStr, diffStr, specStr, specExp;
	ambStr = 0.7;
	diffStr = 0.9;
	specStr = 0.8;
	specExp = 64.0;
	glm::vec3 objColor;
	objColor.r = 120;
	objColor.g = 188;
	objColor.b = 196;
	objColor /= 255.0f;
	// Set material properties
	setAmbientStrength(ambStr);
	setDiffuseStrength(diffStr);
	setSpecularStrength(specStr);
	setSpecularExponent(specExp);
	setObjectColor(objColor);

	// Read number of lights
	unsigned int numLights = 1;

	int enabled, type;
	enabled = 1;
	type = 0;
	glm::vec3 lightColor, lightPos;
	
	lightColor.r = 246;
	lightColor.g = 241;
	lightColor.b = 231;
	lightColor /= 255.0f;

	lightPos.x = 0.0;
	lightPos.y = -1.0;
	lightPos.z = -3.0;

	lights[0].setEnabled((bool)enabled);
	lights[0].setType((Light::LightType)type);
	lights[0].setColor(lightColor);
	lights[0].setPos(lightPos);
	for (int k = 1; k < 8; k++) {
		lights[k].setEnabled(false);
	}


}







// Called when window is resized
void GLState::resizeGL(int w, int h) {
	// Tell OpenGL the new dimensions of the window
	camGround.setWH(w, h);
	camOverhead.setWH(w, h);
	glViewport(0, 0, w, h);
}

// Get object color
glm::vec3 GLState::getObjectColor() const {
	glm::vec3 objColor;
	glGetUniformfv(shader, objColorLoc, glm::value_ptr(objColor));
	return objColor;
}

// Get ambient strength
float GLState::getAmbientStrength() const {
	float ambStr;
	glGetUniformfv(shader, ambStrLoc, &ambStr);
	return ambStr;
}

// Get diffuse strength
float GLState::getDiffuseStrength() const {
	float diffStr;
	glGetUniformfv(shader, diffStrLoc, &diffStr);
	return diffStr;
}

// Get specular strength
float GLState::getSpecularStrength() const {
	float specStr;
	glGetUniformfv(shader, specStrLoc, &specStr);
	return specStr;
}

// Get specular exponent
float GLState::getSpecularExponent() const {
	float specExp;
	glGetUniformfv(shader, specExpLoc, &specExp);
	return specExp;
}

// Set object color
void GLState::setObjectColor(glm::vec3 color) {
	// Update value in shader
	glUseProgram(shader);
	glUniform3fv(objColorLoc, 1, glm::value_ptr(color));
	glUseProgram(0);
}

// Set ambient strength
void GLState::setAmbientStrength(float ambStr) {
	// Update value in shader
	glUseProgram(shader);
	glUniform1f(ambStrLoc, ambStr);
	glUseProgram(0);
}

// Set diffuse strength
void GLState::setDiffuseStrength(float diffStr) {
	// Update value in shader
	glUseProgram(shader);
	glUniform1f(diffStrLoc, diffStr);
	glUseProgram(0);
}

// Set specular strength
void GLState::setSpecularStrength(float specStr) {
	// Update value in shader
	glUseProgram(shader);
	glUniform1f(specStrLoc, specStr);
	glUseProgram(0);
}

// Set specular exponent
void GLState::setSpecularExponent(float specExp) {
	// Update value in shader
	glUseProgram(shader);
	glUniform1f(specExpLoc, specExp);
	glUseProgram(0);
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
	modelMatLoc = glGetUniformLocation(shader, "modelMat");
	viewProjMatLoc = glGetUniformLocation(shader, "viewProjMat");

	camPosLoc = glGetUniformLocation(shader, "camPos");
	objColorLoc = glGetUniformLocation(shader, "objColor");
	ambStrLoc = glGetUniformLocation(shader, "ambStr");
	diffStrLoc = glGetUniformLocation(shader, "diffStr");
	specStrLoc = glGetUniformLocation(shader, "specStr");
	specExpLoc = glGetUniformLocation(shader, "specExp");

	// Bind lights uniform block to binding index
	glUseProgram(shader);
	GLuint lightBlockIndex = glGetUniformBlockIndex(shader, "LightBlock");
	glUniformBlockBinding(shader, lightBlockIndex, Light::BIND_PT);
	glUseProgram(0);
}



bool GLState::detectCollision(std::shared_ptr<Ship> ship, std::shared_ptr<StaticObstacle> obstacle) {
	glm::vec2 shipCoord = ship->getCurrentPosition();
	glm::vec2 obstacleCoord = obstacle->getCurrentPosition();

	shipCoord.x = shipCoord.x - (ship->getwidth() / 2);
	shipCoord.y = shipCoord.y + (ship->getheight() / 2);
	
	obstacleCoord.x = obstacleCoord.x - (ship->getwidth() / 2);
	obstacleCoord.y = obstacleCoord.y + (ship->getheight() / 2);

	if ((shipCoord.x + ship->getwidth()) >= obstacleCoord.x
	&& shipCoord.x <= (obstacleCoord.x + obstacle->getwidth())
	&& (shipCoord.y + ship->getheight()) >= obstacleCoord.y
	&& shipCoord.y <= (obstacleCoord.y + obstacle->getheight())) {
		obstacle->setdestroyed(true);
		return true;
	}

	return false;
}
