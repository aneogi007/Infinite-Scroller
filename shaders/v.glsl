#version 330

layout(location = 0) in vec3 pos;		// Model-space position
layout(location = 1) in vec3 norm;		// Model-space normal

smooth out vec3 fragNorm;	// Model-space interpolated normal

uniform mat4 xform;			// Model-to-clip space transform

uniform mat4 trans;			// trans mat for animation


void main() {
	// Transform vertex position
	gl_Position = xform * vec4(pos, 1.0);
	gl_Position = trans * gl_Position;

	// Interpolate normals
	fragNorm = norm;
}