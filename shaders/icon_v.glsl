#version 330

layout(location = 0) in vec2 pos;		// Icon vertex positions

uniform mat4 xform;				// Model-to-clip space transform

void main() {
	// Transform vertex position
	gl_Position = xform * vec4(pos, 0.0, 1.0);
}
