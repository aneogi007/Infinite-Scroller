#version 330

const int LIGHTTYPE_POINT = 0;
const int LIGHTTYPE_DIRECTIONAL = 1;

const int OBJTYPE_FLOOR = 0;
const int OBJTYPE_CUBE = 1;

layout(location = 0) in vec3 pos;			// Model-space position
layout(location = 1) in vec3 norm;		    // Model-space face normal
layout(location = 2) in vec2 uv;	        // Texture coordinates
layout(location = 3) in vec3 tangent;	    // Right vector in tangent space
layout(location = 4) in vec3 bitangent;	    // Forward vector in tangent space

smooth out vec3 fragPos;	    // Interpolated position in world-space
smooth out vec3 fragNorm;	    // Interpolated normal in world-space
smooth out vec3 fragColor;	    // Interpolated color (for Gouraud shading)
smooth out vec2 fragUV;         // Interpolated texture coordinates
// Normal mapping
smooth out vec3 tanLightPos;    // Light position in tangent space
smooth out vec3 tanViewer;      // Viewing vector in tangent space
smooth out vec3 tanFragPos;     // Fragment position in tangent space
// Shadow mapping
smooth out vec4 lightFragPos;   // Fragment position in light space

// Light information
struct LightData {
	bool enabled;
	int type;
	vec3 pos;
	vec3 color;
};

// Array of lights
const int MAX_LIGHTS = 1;
layout (std140) uniform LightBlock {
	LightData lights [MAX_LIGHTS];
};

uniform mat4 modelMat;		 // Model-to-world transform matrix
uniform mat4 lightSpaceMat;  // World-to-light matrix location
uniform int objType;         // 0 for floor and 1 for cube
uniform mat4 viewProjMat;	 // World-to-clip transform matrix
uniform int shadingMode;     // Phong vs. colored normals
uniform vec3 camPos;         // Camera position

uniform vec3 floorColor;
uniform float floorAmbStr;
uniform float floorDiffStr;
uniform float floorSpecStr;
uniform float floorSpecExp;
uniform vec3 cubeColor;
uniform float cubeAmbStr;
uniform float cubeDiffStr;
uniform float cubeSpecStr;
uniform float cubeSpecExp;

void main() {
	// Choose which normals to use

	// Get world-space position and normal
	fragPos = vec3(modelMat * vec4(pos, 1.0));
	fragNorm = vec3(modelMat * vec4(norm, 0.0));

	// Get light-space position, pass to fragment shader
	lightFragPos = lightSpaceMat * vec4(fragPos, 1.0);

	// TODO: =========================================================
	// Construct TBN (a mat3 transform matrix) coordinate system
	// Then convert "lights[0].pos", "camPos", and "fragPos" to tangent space: light position ("tanLightPos"), camera position ("tanViewer") and fragment position ("tanFragPos") to tangent space

	vec3 T = normalize(vec3(modelMat * vec4(tangent,   0.0)));
   	vec3 B = normalize(vec3(modelMat * vec4(bitangent, 0.0)));
   	vec3 N = normalize(vec3(modelMat * vec4(norm,    0.0)));
   	
	mat3 TBN = transpose(mat3(T, B, N));

	tanLightPos = TBN * lights[0].pos ;
	tanViewer = TBN * camPos;
	tanFragPos = TBN * fragPos;
	
	// Pass the interpolated texture coordinates to the fragment shader
	fragUV = uv;

	// Output clip-space position
	gl_Position = viewProjMat * vec4(fragPos, 1.0);
}
