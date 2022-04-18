#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <array>
#include <glm/glm.hpp>
#include "gl_core_3_3.h"

class Light {
public:
	Light();
	~Light();
	// Disallow copy
	Light(const Light& other) = delete;
	Light& operator=(const Light& other) = delete;
	// Move constructor and assignment
	Light(Light&& other);
	Light& operator=(Light&& other);

	enum LightType {
		POINT = 0,
		DIRECTIONAL = 1,
	};

	static const int BIND_PT = 0;
	static const int MAX_LIGHTS = 1;

	// Render a graphical representation of the light source
	void drawIcon(glm::mat4 xform) const;

	// Accessors
	bool getEnabled() const { return data.enabled; }
	LightType getType() const { return (LightType)data.type; }
	glm::vec3 getPos() const { return data.pos; }
	glm::vec3 getColor() const { return data.color; }
	// Modifiers
	void setEnabled(bool enabled);
	void setType(LightType type);
	void setPos(glm::vec3 pos);
	void setColor(glm::vec3 color);

	// Rotation and offset
	bool isRotating() const { return rotating; }
	void beginRotate(glm::vec2 mousePos);
	void endRotate();
	void rotateLight(glm::vec2 mousePos);
	void offsetLight(float offset);

protected:
	// Light properties, arranged for UBO storage (std140 layout)
	struct LightData {
		LightData();
		int enabled;		// Whether light is on or off
		int type;			// Point light or directional light
		glm::vec2 padding0;
		glm::vec3 pos;		// Position (or direction) of the light
		float padding1;
		glm::vec3 color;	// Color of the light
		float padding2;
	} data;
	int index = -1;		// Index into UBO (set upon enable)

	// Rotation state
	bool rotating;			// Whether light is rotating
	glm::vec2 initRot;		// Initial rotation angles on click
	glm::vec2 initMousePos;	// Initial mouse position on click

	// OpenGL state -- shared by all Light objects
	static unsigned int refcount;	// Number of light objects instantiated
	static std::array<bool, MAX_LIGHTS> enabledLights;	// Which lights are enabled
	static GLuint ubo;				// Uniform buffer object for storing light data
	// Icon drawing state
	static GLuint shader;			// Icon shader
	static GLuint vao;				// Vertex array object
	static GLuint vbuf;				// Vertex buffer
	static GLuint vcountPoint;		// Number of vertices in point icon
	static GLuint vcountDir;		// Number of vertices in directional icon
	static GLuint xformLoc;			// Location of the xform matrix
	static GLuint colorLoc;			// Location of color uniform
	static float iconScale;			// Scale of the icons

	// OpenGL state management
	void initializeGL();
	void destroyGL();
	void initUBO();
	void updateUBO();
	int findAvailableIndex();
	// Icon setup
	void initShader();
	void initGeometry();
};

#endif
