#version 330

layout(location = 0) in vec3 pos;			// Model-space position
layout(location = 1) in vec3 face_norm;		// Model-space face normal
layout(location = 2) in vec3 smooth_norm;	// Model-space smoothed normal

smooth out vec3 fragPos;
smooth out vec3 fragNorm;	// Model-space interpolated normal

uniform mat4 xform;			// Model-to-clip space transform

uniform mat4 modelMat;		// Model-to-world transform matrix
uniform mat4 viewProjMat;	// World-to-clip transform matrix

void main() {
	// Transform vertex position

	vec3 norm;
	norm = face_norm;
	
	
	// Get world-space position and normal
	fragPos = vec3(modelMat * vec4(pos, 1.0));
	fragNorm = vec3(modelMat * vec4(norm, 0.0));

	// Output clip-space position
	gl_Position = viewProjMat * vec4(fragPos, 1.0);
}
