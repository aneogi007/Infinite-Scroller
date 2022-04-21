#version 330

const int LIGHTTYPE_POINT = 0;			// Point light
const int LIGHTTYPE_DIRECTIONAL = 1;	// Directional light

smooth in vec3 fragPos;		// Interpolated position in world-space
smooth in vec3 fragNorm;	// Interpolated normal in world-space

out vec3 outCol;	// Final pixel color

// Light information
struct LightData {
	bool enabled;	// Whether the light is on
	int type;		// Type of light (0 = point, 1 = directional)
	vec3 pos;		// World-space position/direction of light source
	vec3 color;		// Color of light
};

// Array of lights
const int MAX_LIGHTS = 8;
layout (std140) uniform LightBlock {
	LightData lights [MAX_LIGHTS];
};

uniform vec3 camPos;			// World-space camera position
uniform vec3 objColor;			// Object color
uniform float ambStr;			// Ambient strength
uniform float diffStr;			// Diffuse strength
uniform float specStr;			// Specular strength
uniform float specExp;			// Specular exponent

void main() {
	// Visualize normals as colors
	//outCol = normalize(fragNorm) * 0.5f + vec3(0.5f);

	// TODO ====================================================================
	// Implement Phong illumination
	vec3 final;
	
	for (int i = 0; i < MAX_LIGHTS; i++) {
		if (lights[i].enabled == true) {
			vec3 lightDir;
			if (lights[i].type == 1) {
				lightDir = normalize(lights[i].pos);
			} else {
				lightDir = normalize(lights[i].pos - fragPos);  
			}

			vec3 norm = normalize(fragNorm);
			vec3 viewDir = normalize(camPos - fragPos);
			vec3 reflectDir = reflect(-lightDir, norm); 

			//ambient
			vec3 ambient = ambStr * lights[i].color ;
			
			//diffuse
			float diff = max(dot(norm, lightDir), 0.0);	//cos(angle between norm and lightDir)
			vec3 diffuse =  diff * diffStr * lights[i].color;

			//specular
			float spec = pow(max(dot(viewDir, reflectDir), 0.0), specExp); //cos(angle between viewDir and reflectDir)
			vec3 specular = (specStr)* spec * lights[i].color;

			final += ((ambient + diffuse + specular) * objColor);
		}	
	}
	outCol = final;
}
