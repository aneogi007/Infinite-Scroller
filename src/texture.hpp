#pragma once

#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <string>
#include "gl_core_3_3.h"

class Texture {
public:
	Texture() {}
	~Texture() {}
	// Disallow copy, move, & assignment
	Texture(const Texture& other) = delete;
	Texture& operator=(const Texture& other) = delete;
	Texture(Texture&& other) = delete;
	Texture& operator=(Texture&& other) = delete;

	void load();  // Load images (in ./textures/) as textures
	void prepareDepthMap();
	void activeTextures();
	void activeDepthMap();

	// Access
	inline void getShadowWidthHeight(int& width, int&height) const {
		width = SHADOW_WIDTH;
		height = SHADOW_HEIGHT;
	}
	inline GLuint getdepthMapFBO() { return depthMapFBO; }

protected:
	GLuint texPlane;     // Plane/floor texture
	GLuint texCube;      // Cube texture
	GLuint texCubeNorm;  // Cube normal texture

	const unsigned int SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;  // depth map resolution
	GLuint depthMapFBO = 0;  // depth map as frame buffer
	GLuint depthMap = 0;     // depth map

	unsigned int prepareTexture(const char* filename);
};

#endif
