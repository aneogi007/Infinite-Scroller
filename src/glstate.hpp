#ifndef GLSTATE_HPP
#define GLSTATE_HPP

#include <string>
#include <memory>
#include <glm/glm.hpp>
#include "gl_core_3_3.h"
#include "camera.hpp"
#include "scene.hpp"

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

	std::unique_ptr<Scene> scene;	// Pointer to the scene object

	// Camera access
	inline CameraType getCamType() { return whichCam; }
	inline Camera& getCamera(const CameraType camType = GROUND_VIEW) {
		return (camType == GROUND_VIEW) ? camGround : camOverhead;
	}
	inline void switchCam() {  // switch between the two cameras
		whichCam = (whichCam == GROUND_VIEW) ? OVERHEAD_VIEW : GROUND_VIEW;
	}

	//PLAYER OBJECT
	//SpaceShip
	inline int getCurrentSpaceShip() const { return currentSpaceShip; }
	inline int getGround() const { return Ground; }

	//movement speed
	inline float getMoveStep() { return moveStep; }

	inline float getPlayerSpeed() { return playerSpeed; }
	inline void setPlayerSpeed(float newSpeed) { playerSpeed = newSpeed; }
	
	

protected:
	// Initialization
	void initShaders();

	

	// OpenGL state
	GLuint shader;		// GPU shader program
	GLuint xformLoc;	// Transformation matrix location

	// cameras:
	Camera camGround, camOverhead;
	CameraType whichCam = GROUND_VIEW;  // which camera is active currently

	// Index of SpaceShip
	unsigned int currentSpaceShip = 0;
	unsigned int Ground = 0;

	float moveStep = 0.5f;  // Move step

	//MOVING BACKGROUND PARAMETERS
	// Floor Position and Velocity Vector
	float mapVelocity = 0.03f;

	//Player Speed
	float playerSpeed = 0.9f;

	bool detectCollision(std::shared_ptr<Ship> ship, std::shared_ptr<StaticObstacle> obstacle);


};

#endif
