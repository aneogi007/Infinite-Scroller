#ifndef GLSTATE_HPP
#define GLSTATE_HPP

#include <string>
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "gl_core_3_3.h"
#include "camera.hpp"
#include "scene.hpp"
#include "light.hpp"

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

	// ParseLights
	void parseLights();

	std::unique_ptr<Scene> scene;	// Pointer to the scene object

	// Camera access
	inline CameraType getCamType() { return whichCam; }
	inline Camera& getCamera(const CameraType camType = GROUND_VIEW) {
		return (camType == GROUND_VIEW) ? camGround : camOverhead;
	}
	inline void switchCam() {  // switch between the two cameras
		whichCam = (whichCam == GROUND_VIEW) ? OVERHEAD_VIEW : GROUND_VIEW;
	}

	//light array
	std::vector<Light> lights;		// Lights

	//object properties
	// Object properties
	float getAmbientStrength() const;
	float getDiffuseStrength() const;
	float getSpecularStrength() const;
	float getSpecularExponent() const;
	glm::vec3 getObjectColor() const;
	void setAmbientStrength(float ambStr);
	void setDiffuseStrength(float diffStr);
	void setSpecularStrength(float specStr);
	void setSpecularExponent(float specExp);
	void setObjectColor(glm::vec3 color);
	
	//PLAYER OBJECT
	//SpaceShip
	inline int getCurrentSpaceShip() const { return currentSpaceShip; }
	inline void setCurrentSpaceShip(int newShip) { currentSpaceShip = newShip; }

	inline int getGround() const { return Ground; }

	//movement speed
	inline float getMoveStep() { return moveStep; }

	inline float getPlayerSpeed() { return playerSpeed; }
	inline void setPlayerSpeed(float newSpeed) { playerSpeed = newSpeed; }

	
	bool isMovingRight() { return movingRight; }
	void setMovingRight(bool isMoving) { movingRight = isMoving; }
	

	bool isMovingLeft() { return movingLeft; }
	void setMovingLeft(bool isMoving) { movingLeft = isMoving; }


	void setKeyPress(bool isPress) { keyPress = isPress; }
	int getKeyPressTime() { return keyPressTime; }


	
	

protected:
	bool init;
	// Initialization
	void initShaders();

	// //helper funcs
	// std::string trim(const std::string& line);
	// std::string getNextLine(std::istream& istr);
	// std::string preprocessFile(std::string filename);

	// Shader state
	GLuint shader;		// GPU shader program
	GLuint modelMatLoc;	// Model-to-world matrix location
	GLuint viewProjMatLoc; // World-to-clip matrix location

	GLuint camPosLoc;		// Camera position location
	GLuint objColorLoc;		// Object color
	GLuint ambStrLoc;		// Ambient strength location
	GLuint diffStrLoc;		// Diffuse strength location
	GLuint specStrLoc;		// Specular strength location
	GLuint specExpLoc;		// Specular exponent location



	// cameras:
	Camera camGround, camOverhead;
	CameraType whichCam = GROUND_VIEW;  // which camera is active currently

	

	// Index of SpaceShip
	unsigned int currentSpaceShip = 0;
	unsigned int Ground = 0;

	float moveStep = 0.5f;  // Move step

	//MOVING BACKGROUND PARAMETERS
	// Floor Position and Velocity Vector
	float mapVelocity = 0.06f;

	//Player Speed
	float playerSpeed = 0.02f;

	bool movingRight = false;
	bool movingLeft = false;
	
	bool detectCollision(std::shared_ptr<Ship> ship, std::shared_ptr<StaticObstacle> obstacle);

	float keyPress = false;
	int keyPressTime = 0;

};

#endif
