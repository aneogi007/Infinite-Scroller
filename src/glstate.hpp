#ifndef GLSTATE_HPP
#define GLSTATE_HPP

#include <string>
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "gl_core_3_3.h"
#include "mesh.hpp"
#include "light.hpp"
#include "texture.hpp"

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

	// Enums
	enum ShadingMode {
		SHADINGMODE_NORMALS = 0,	// View normals as colors
		SHADINGMODE_PHONG = 1,		// Use Phong shading and illumination
	};
	enum NormalMapMode {
		NORMAL_MAPPING_ON = 0,      // Toggle normal mapping
		NORMAL_MAPPING_OFF = 1,     // Turn off
	};
	enum ShadowMapMode {
		SHADOW_MAPPING_ON = 0,      // Toggle shadow mapping
		SHADOW_MAPPING_OFF = 1,     // Turn off
	};

	bool isInit() const { return init; }
	void readConfig(std::string filename);	// Read from a config file

	// Drawing modes
	ShadingMode getShadingMode() const { return shadingMode; }
	NormalMapMode getNormalMapMode() const { return normalMapMode; }
	ShadowMapMode getShadowMapMode() const { return shadowMapMode; }
	void setShadingMode(ShadingMode sm);
	void setNormalMapMode(NormalMapMode nmm);
	void setShadowMapMode(ShadowMapMode smm);

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

	void setMaterialAttrs(
		glm::vec3 floorColor, glm::vec3 cubeColor,
		float floorAmbStr, float floorDiffStr, float floorSpecStr, float floorSpecExp,
		float cubeAmbStr, float cubeDiffStr, float cubeSpecStr, float cubeSpecExp
	);

	// Set the currently active cube (controlled by keyboard)
	inline void setActiveObj(const int objIndex) { activeObj = objIndex; }
	inline int getActiveObj() const { return activeObj; }

	// Mesh & Light access
	unsigned int getNumLights() const { return (unsigned int)lights.size(); }
	Light& getLight(int index) { return lights.at(index); }
	const Light& getLight(int index) const { return lights[index]; }
	// Get the list of objects
	inline std::vector<std::shared_ptr<Mesh>>& getObjects() { return objects; }

	// Camera control
	bool isCamRotating() const { return camRotating; }
	void beginCameraRotate(glm::vec2 mousePos);
	void endCameraRotate();
	void rotateCamera(glm::vec2 mousePos);
	void offsetCamera(float offset);
	inline float getMoveStep() { return moveStep; }

	// Set object to display
	void showObjFile(const std::string& filename, const unsigned int meshType, const glm::mat4& modelMat);

protected:
	bool init;  // Whether we've been initialized yet

	// Initialization
	void initShaders();

	// Calculate model matrix from rotation and translation
	static glm::mat4 calModelMat(const glm::mat3 rotMat, const glm::vec3 translation);

	// Drawing modes
	ShadingMode shadingMode;
	NormalMapMode normalMapMode;
	ShadowMapMode shadowMapMode;

	// Camera state
	int width, height;		// Width and height of the window
	float fovy;				// Vertical field of view in degrees
	glm::vec3 camCoords;	// Camera spherical coordinates
	bool camRotating;		// Whether camera is currently rotating
	glm::vec2 initCamRot;	// Initial camera rotation on click
	glm::vec2 initMousePos;	// Initial mouse position on click

	// Mesh and lights
	std::vector<std::shared_ptr<Mesh>> objects;		// Pointer to mesh object
	std::vector<Light> lights;		// Lights

	unsigned int numObjects;  // Number of objects in the scene
	unsigned int activeObj = 1;   // The current active cube (1-3)
	float moveStep = 0.1f;  // Move step

	// Textures
	Texture textures;

	// Shader state
	GLuint shader;			       // GPU shader program
	GLuint depthShader;	           // Depth shader program
	GLuint modelMatLoc;	           // Model-to-world matrix location, used in shader
	GLuint modelMatDepthLoc;	   // Model-to-world matrix location, used in depth shader
	GLuint lightSpaceMatLoc;       // World-to-light matrix location, used in shader
	GLuint lightSpaceMatDepthLoc;  // World-to-light matrix location, used in depth shader
	GLuint objTypeLoc;             // Object type location (decide which material attributes to use and which texture to map)
	GLuint viewProjMatLoc;	       // World-to-clip matrix location
	GLuint shadingModeLoc;	       // Shading mode location
	GLuint normalMapModeLoc;       // Normal mapping mode location
	GLuint shadowMapModeLoc;       // Shadow mapping mode location
	GLuint camPosLoc;		       // Camera position location
	GLuint floorColorLoc, cubeColorLoc;		    // Object color
	GLuint floorAmbStrLoc, cubeAmbStrLoc;		// Ambient strength location
	GLuint floorDiffStrLoc, cubeDiffStrLoc;		// Diffuse strength location
	GLuint floorSpecStrLoc, cubeSpecStrLoc;		// Specular strength location
	GLuint floorSpecExpLoc, cubeSpecExpLoc;		// Specular exponent location
};

#endif
