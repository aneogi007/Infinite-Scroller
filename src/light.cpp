#define NOMINMAX
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "light.hpp"
#include "util.hpp"
#include <iostream>

// Static Light members (OpenGL state)
unsigned int Light::refcount = 0;
std::array<bool, Light::MAX_LIGHTS> Light::enabledLights;
GLuint Light::ubo = 0;
GLuint Light::shader = 0;
GLuint Light::vao = 0;
GLuint Light::vbuf = 0;
GLuint Light::vcountPoint = 0;
GLuint Light::vcountDir = 0;
GLuint Light::xformLoc = 0;
GLuint Light::colorLoc = 0;
float Light::iconScale = 0.06f;

// LightData constructor
Light::LightData::LightData() :
	enabled(false),
	type((int)POINT),
	pos(0.0, 2.0, 0.0),
	color(1.0, 1.0, 1.0) {}

// Constructor
Light::Light() :
	rotating(false) {

	// Create OpenGL state if we're the first light
	if (refcount == 0)
		initializeGL();
	refcount++;
}

// Destructor
Light::~Light() {
	// Disable the light (updates UBO)
	setEnabled(false);

	refcount--;
	// Destroy OpenGL state if we're the last light
	if (refcount == 0)
		destroyGL();
}

// Move constructor
Light::Light(Light&& other) :
	data(other.data),
	index(other.index),
	rotating(false) {

	other.data.enabled = false;
	other.index = -1;
	// Increment reference count (temp will decrement upon destructor)
	refcount++;
}

// Move assignment
Light& Light::operator=(Light&& other) {
	data = other.data;
	index = other.index;
	rotating = false;

	other.data.enabled = false;
	other.index = -1;
	// Refcount stays the same

	return *this;
}

// Allow light to affect the scene
void Light::setEnabled(bool enabled) {
	// Assign an index upon enabling
	if (enabled && index < 0) {
		index = findAvailableIndex();
		if (index < 0)
			throw std::runtime_error("Cannot enable more than "
				+ std::to_string(MAX_LIGHTS) + " lights");
		enabledLights[index] = true;
	}

	data.enabled = enabled;
	// Update UBO
	if (index >= 0)
		updateUBO();

	// Relinquish index if disabled
	if (!enabled && index >= 0) {
		enabledLights[index] = false;
		index = -1;
	}
}

// Point-light or direcitonal-light
void Light::setType(LightType type) {
	data.type = type;

	// Update UBO
	if (data.enabled && index >= 0)
		updateUBO();
}

void Light::setPos(glm::vec3 pos) {
	data.pos = pos;

	// Update UBO
	if (data.enabled && index >= 0)
		updateUBO();
}

void Light::setColor(glm::vec3 color) {
	data.color = color;

	// Update UBO
	if (data.enabled && index >= 0)
		updateUBO();
}

// Set initial rotation state
void Light::beginRotate(glm::vec2 mousePos) {
	// Get initial rotation angles
	initRot.y = glm::asin(glm::dot(glm::normalize(data.pos), glm::vec3(0.0f, 1.0f, 0.0f)));
	glm::vec2 axis(data.pos.x, data.pos.z);
	if (glm::length(axis) < 1e-5)
		initRot.x = 0.0f;
	else
		initRot.x = glm::atan(-axis.y, axis.x);

	rotating = true;
	initMousePos = mousePos;
}

// Stop rotating
void Light::endRotate() {
	rotating = false;
}

// Rotate the light about the origin
void Light::rotateLight(glm::vec2 mousePos) {
	if (rotating) {
		// Get new angles
		float rotScale = 4.0f;
		glm::vec2 mouseDelta = mousePos - initMousePos;
		mouseDelta.y *= -1.0f;
		glm::vec2 newAngle = initRot + mouseDelta * rotScale;

		// Get new position
		glm::mat4 rotMat(1.0f);
		rotMat = glm::rotate(rotMat, newAngle.x, glm::vec3(0.0f, 1.0f, 0.0f));
		rotMat = glm::rotate(rotMat, newAngle.y, glm::vec3(0.0f, 0.0f, 1.0f));
		glm::vec3 newPos(glm::length(data.pos), 0.0f, 0.0f);
		newPos = glm::vec3(rotMat * glm::vec4(newPos, 1.0f));

		setPos(newPos);
	}
}

// Move the light closer/further to/from the origin
void Light::offsetLight(float offset) {
	float minDist = 0.2f;
	glm::vec3 dir = glm::normalize(data.pos);
	glm::vec3 newPos = data.pos + dir * offset;
	if (glm::length(newPos) < minDist)
		newPos = dir * minDist;
	setPos(newPos);
}

void Light::drawIcon(glm::mat4 viewProj) const {
	glUseProgram(shader);

	// Get light position in NDC space
	glm::vec4 clipPos = viewProj * glm::vec4(data.pos, 1.0f);
	glm::vec4 ndcPos = clipPos / clipPos.w;
	// Transform icon from NDC space to clip space
	float aspect = glm::length(glm::vec3(glm::transpose(viewProj)[1])) /
		glm::length(glm::vec3(glm::transpose(viewProj)[0]));
	glm::mat4 xform(1.0f);
	xform[0][0] = iconScale / aspect;
	xform[1][1] = iconScale;
	xform[3][0] = ndcPos.x;
	xform[3][1] = ndcPos.y;
	xform[3][2] = ndcPos.z;
	xform = glm::mat4(clipPos.w) * xform;	// Multiply by W (NDC -> clip)
	glUniformMatrix4fv(xformLoc, 1, GL_FALSE, glm::value_ptr(xform));
	// Set color of icon
	glUniform3fv(colorLoc, 1, glm::value_ptr(data.color));

	glBindVertexArray(vao);

	if (data.type == POINT)
		glDrawArrays(GL_LINES, 0, vcountPoint);
	else if (data.type == DIRECTIONAL)
		glDrawArrays(GL_LINES, vcountPoint, vcountDir);

	glBindVertexArray(0);
	glUseProgram(0);
}

// Create UBO and icon state
void Light::initializeGL() {
	// Light data uniform creation
	initUBO();

	// Icon state creation
	initShader();
	initGeometry();
}

// Destroy OpenGL state
void Light::destroyGL() {
	if (ubo) { glDeleteBuffers(1, &ubo); ubo = 0; }
	if (shader) { glDeleteProgram(shader); shader = 0; }
	if (vao) { glDeleteVertexArrays(1, &vao); vao = 0; }
	if (vbuf) { glDeleteBuffers(1, &vbuf); vbuf = 0; }
}

// Create Light data uniform buffer
void Light::initUBO() {
	enabledLights.fill(false);
	std::vector<LightData> emptyLights(MAX_LIGHTS);

	// Create the uniform buffer and fill with empty lights
	glGenBuffers(1, &ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	glBufferData(GL_UNIFORM_BUFFER, emptyLights.size() * sizeof(LightData),
		emptyLights.data(), GL_STATIC_DRAW);

	// Set the binding point index of the buffer
	glBindBufferBase(GL_UNIFORM_BUFFER, BIND_PT, ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

// Update this light's entry in the uniform buffer
void Light::updateUBO() {
	if (index < 0) return;

	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	glBufferSubData(GL_UNIFORM_BUFFER, index * sizeof(LightData),
		sizeof(LightData), &data);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

// Loop through array of lights to find the first available index
int Light::findAvailableIndex() {
	int index = -1;
	for (int i = 0; i < (int)enabledLights.size(); i++) {
		if (!enabledLights[i]) {
			index = i;
			break;
		}
	}
	return index;
}

// Compile and link shader
void Light::initShader() {
	std::vector<GLuint> shaders;
	shaders.push_back(compileShader(GL_VERTEX_SHADER, "shaders/icon_v.glsl"));
	shaders.push_back(compileShader(GL_FRAGMENT_SHADER, "shaders/icon_f.glsl"));
	shader = linkProgram(shaders);
	// Cleanup extra state
	for (auto s : shaders)
		glDeleteShader(s);
	shaders.clear();

	// Get uniform locations
	colorLoc = glGetUniformLocation(shader, "color");
	xformLoc = glGetUniformLocation(shader, "xform");
}

// Create icon geometry
void Light::initGeometry() {
	// Point icon geometry
	std::vector<glm::vec2> pointIconVerts = {
		{  0.0f,    0.5f   }, { -0.354f,  0.354f },
		{ -0.354f,  0.354f }, { -0.5f,    0.0f   },
		{ -0.5f,    0.0f   }, { -0.354f, -0.354f },
		{ -0.354f, -0.354f }, {  0.0f,   -0.5f   },
		{  0.0f,   -0.5f   }, {  0.354f, -0.354f },
		{  0.354f, -0.354f }, {  0.5f,    0.0f   },
		{  0.5f,    0.0f   }, {  0.354f,  0.354f },
		{  0.354f,  0.354f }, {  0.0f,    0.5f   },

		{  0.0f,    0.7f   }, {  0.0f,    1.0f   },
		{ -0.495f,  0.495f }, { -0.707f,  0.707f },
		{ -0.7f,    0.0f   }, { -1.0f,    0.0f   },
		{ -0.495f, -0.495f }, { -0.707f, -0.707f },
		{  0.0f,   -0.7f   }, {  0.0f,   -1.0f   },
		{  0.495f, -0.495f }, {  0.707f, -0.707f },
		{  0.7f,    0.0f   }, {  1.0f,    0.0f   },
		{  0.495f,  0.495f }, {  0.707f,  0.707f }
	};
	vcountPoint = (GLuint)pointIconVerts.size();
	// Directional icon geometry
	std::vector<glm::vec2> directionIconVerts = {
//		{  0.8f,   -0.9f   }, {  0.8f,    0.9f   },
		{ -0.8f,    0.8f   }, {  0.8f,    0.5f   },
		{  0.8f,    0.5f   }, {  0.719f,  0.616f },
		{  0.8f,    0.5f   }, {  0.684f,  0.419f },
		{ -0.8f,    0.3f   }, {  0.8f,    0.0f   },
		{  0.8f,    0.0f   }, {  0.719f,  0.116f },
		{  0.8f,    0.0f   }, {  0.684f, -0.081f },
		{ -0.8f,   -0.2f   }, {  0.8f,   -0.5f   },
		{  0.8f,   -0.5f   }, {  0.719f, -0.384f },
		{  0.8f,   -0.5f   }, {  0.684f, -0.581f }
	};
	vcountDir = (GLuint)directionIconVerts.size();

	// Combine verts into a single buffer
	std::vector<glm::vec2> verts;
	verts.reserve(vcountPoint + vcountDir);
	verts.insert(verts.end(), pointIconVerts.begin(), pointIconVerts.end());
	verts.insert(verts.end(), directionIconVerts.begin(), directionIconVerts.end());

	// Create vertex array object
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Send geometry to vertex buffer
	glGenBuffers(1, &vbuf);
	glBindBuffer(GL_ARRAY_BUFFER, vbuf);
	glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(glm::vec2),
		verts.data(), GL_STATIC_DRAW);

	// Specify vertex format
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (GLvoid*)0);

	// Reset state
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
