#ifndef GTEXT_HPP
#define GTEXT_HPP

#include <string>
#include <array>
#include <vector>
#include <map>
#include <glm/glm.hpp>
#include "gl_core_3_3.h"


class Gtext {
public:
	Gtext(float x, float y, float z, float scale);
	
	// Render a graphical representation of the light source
	void drawText(std::string text, GLfloat Textview[16], GLfloat Textproj[16], 
		float r, float g, float b, float a, double scale);
    void changeText(std::string newtext);
	//void changeColor(float r, float g, float b, float a);

	// Accessors

	// Modifiers


protected:
	// Light properties, arranged for UBO storage (std140 layout)
    float x_val = -4;
    float y_val = 0;
    float z_val = 0;
    float scal_val;

};

#endif
