#version 330

const int SHADINGMODE_NORMALS = 0;		// Show normals as colors
const int SHADINGMODE_PHONG = 1;		// Phong shading + illumination

const int NORMAL_MAPPING_ON = 0;        // Turn on normal mapping
const int NORMAL_MAPPING_OFF = 1;       // Turn off

const int SHADOW_MAPPING_ON = 0;        // Turn on shadow mapping
const int SHADOW_MAPPING_OFF = 1;       // Turn off

const int LIGHTTYPE_POINT = 0;			// Point light
const int LIGHTTYPE_DIRECTIONAL = 1;	// Directional light

const int OBJTYPE_FLOOR = 0;
const int OBJTYPE_CUBE = 1;

// Textures
uniform sampler2D texPlane;     // Floor texture
uniform sampler2D texCube;      // Cube texture
uniform sampler2D texCubeNorm;  // Normal map
uniform sampler2D shadowMap;    // Shadow map

smooth in vec3 fragPos;		    // Interpolated position in world-space
smooth in vec3 fragNorm;	    // Interpolated normal in world-space
smooth in vec3 fragColor;	    // Interpolated color (for Gouraud shading)
smooth in vec2 fragUV;          // Interpolated texture coordinates
// Normal mapping
smooth in vec3 tanLightPos;     // Light position in tangent space
smooth in vec3 tanViewer;       // Viewing vector in tangent space
smooth in vec3 tanFragPos;      // Fragment position in tangent space
// Shadow mapping
smooth in vec4 lightFragPos;    // Fragment position in light space

out vec3 outCol;	         // Final pixel color

// Light information
struct LightData {
	bool enabled;	// Whether the light is on
	int type;		// Type of light (0 = point, 1 = directional)
	vec3 pos;		// World-space position/direction of light source
	vec3 color;		// Color of light
};

// Array of lights
const int MAX_LIGHTS = 1;
layout (std140) uniform LightBlock {
	LightData lights [MAX_LIGHTS];
};

uniform int shadingMode;		// Which shading mode
uniform int normalMapMode;      // Whether turn on normal mapping
uniform int shadowMapMode;      // Whether turn on shadow mapping
uniform int objType;            // 0 for floor and 1 for cube
uniform vec3 camPos;			// World-space camera position

uniform vec3 floorColor;			// Object color
uniform float floorAmbStr;			// Ambient strength
uniform float floorDiffStr;			// Diffuse strength
uniform float floorSpecStr;			// Specular strength
uniform float floorSpecExp;			// Specular exponent
uniform vec3 cubeColor;			    // Object color
uniform float cubeAmbStr;			// Ambient strength
uniform float cubeDiffStr;			// Diffuse strength
uniform float cubeSpecStr;			// Specular strength
uniform float cubeSpecExp;			// Specular exponent

float calculateShadow(vec4 light_frag_pos) {  // TODO: add other parameters if needed
	// Perspective divide
	vec3 projCoords = light_frag_pos.xyz / light_frag_pos.w;
	// Remap to [0.0, 1.0]
	projCoords = projCoords * 0.5 + 0.5;

	// TODO: =========================================================
	// Decide whether a fragment is in shadow
	// Read the depth from depth map, then compare it with the depth of the current fragment

	float closestDepth = texture(shadowMap, projCoords.xy).x;

	float currentDepth = projCoords.z;


	
	float shadow;
	
	if (currentDepth > closestDepth) {
		shadow = 1.0;
	} else {
		shadow = 0.0;
	}
	

	// return a binary representing whether there is shadow (0.0 or 1.0)
	return shadow;  // TODO: remove this line after you have implemented this
}

void main() {
	// Decide material attributes
	vec3 objColor = floorColor;
	float ambStr = floorAmbStr, diffStr = floorDiffStr, specStr = floorSpecStr, specExp = floorSpecExp;
	if (objType == OBJTYPE_FLOOR) {
		objColor = texture(texPlane, fragUV).xyz;
		ambStr = floorAmbStr;
		diffStr = floorDiffStr;
		specStr = floorSpecStr;
		specExp = floorSpecExp;
	}
	else if (objType == OBJTYPE_CUBE) {
		objColor = texture(texCube, fragUV).xyz;
		ambStr = cubeAmbStr;
		diffStr = cubeDiffStr;
		specStr = cubeSpecStr;
		specExp = cubeSpecExp;
	}

	if (shadingMode == SHADINGMODE_NORMALS) {
		if (normalMapMode == NORMAL_MAPPING_ON && objType == OBJTYPE_CUBE)
			outCol = texture(texCubeNorm, fragUV).xyz;  // Get normal from normal map
		else
			outCol = normalize(fragNorm) * 0.5 + vec3(0.5);
	}
	else if (shadingMode == SHADINGMODE_PHONG) {
		outCol = vec3(0.0);
		for (int i = 0; i < MAX_LIGHTS; i++) {
			if (lights[i].enabled) {
				vec3 normal;

				if (normalMapMode == NORMAL_MAPPING_ON && objType == OBJTYPE_CUBE) {
					// TODO: =========================================================
					// Get the fragment normal, store it in "normal" SEE LINE 111
					normal = texture(texCubeNorm, fragUV).rgb;
					normal = normal * 2.0 - 1.0;
					normal = normalize(normal);
				}
				else
					normal = normalize(fragNorm);

				vec3 lightDir;
				if (lights[i].type == LIGHTTYPE_POINT)
					lightDir = normalize(lights[i].pos - fragPos);
				else if (lights[i].type == LIGHTTYPE_DIRECTIONAL) {
					if (normalMapMode == NORMAL_MAPPING_ON && objType == OBJTYPE_CUBE) {
						// TODO: =========================================================
						// Get vector to the light in tangent space, store it in "lightDir", SEE LINE 120
						lightDir = normalize(tanLightPos - tanFragPos);
					}
					else
						lightDir = normalize(lights[i].pos);
				}

				float ambient = ambStr;

				float diffuse = max(dot(normal, lightDir), 0.0) * diffStr;

				vec3 viewDir;
				if (normalMapMode == NORMAL_MAPPING_ON && objType == OBJTYPE_CUBE) {
					// TODO: =========================================================
					// Get vector to the viewer in tangent space, store it in "viewDir", SEE LINE 136
					viewDir = normalize(tanViewer - tanFragPos);
				}
				else
					viewDir = normalize(camPos - fragPos);

				vec3 reflectDir = -lightDir - 2 * dot(-lightDir, normal) * normal;
				float specular = max(dot(viewDir, reflectDir), 0.0);
				specular = pow(specular, specExp) * specStr;

				// TODO: =========================================================
				// Calculate shadow using the function calculateShadow, and modify the line (calculating the final color) below
				float shadow;
				if (shadowMapMode == SHADOW_MAPPING_ON)
					shadow = calculateShadow(lightFragPos);  // TODO: add more parameters if necessary
				else if (shadowMapMode == SHADOW_MAPPING_OFF)
					shadow = 0.0;
				outCol += (ambient + (1.0 - shadow)) * (diffuse + specular) * lights[i].color;  // TODO: use "shadow" variable here
			}
		}
		outCol *= objColor;
	}
}
