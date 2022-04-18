#version 330

layout(location = 0) in vec3 pos;  // Model-space position

uniform mat4 lightSpaceMat;  // Convert to light space (to generate depth map)
uniform mat4 modelMat;       // Model-to-world transform matrix

void main()
{
	gl_Position = lightSpaceMat * modelMat * vec4(pos, 1.0);
}
