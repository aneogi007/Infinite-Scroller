#define NOMINMAX
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "util.hpp"
#include "gtext.hpp"

#include < GL/freeglut.h>
#define GLT_IMPLEMENTATION
#define GLT_MANUAL_VIEWPORT
#include "text/gltext.h"




Gtext::Gtext(float x, float y, float z, float scale) {
    x_val = x;
    y_val = y;
    z_val = z;
    scal_val = scale;

    gltInit();
 

}

//void Gtext::changeColor(float r, float g, float b, float a) {
//   gltColor(r, g, b, a);
//
//}


void Gtext::drawText(std::string newText, GLfloat Textview[16], GLfloat Textproj[16], 
    float r, float g, float b, float a, double scale) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    GLTtext *text = gltCreateText();
    gltSetText(text, newText.c_str());
    gltBeginDraw();
    if (r == 0 && g == 0 && b == 0 && a == 0) {
        gltColor(0.9f, 0.9f, 0.9f, 1.0f);
    }
    else {
        gltColor(r, g, b, a);
    }
	//gltDrawText2DAligned(text, 0.0, 0.0, 4.0, GLT_CENTER, GLT_TOP);
	gltDrawText3D(text, x_val, y_val, z_val, scale, Textview, Textproj);
    gltEndDraw();
    glDisable(GL_BLEND);
}

void Gtext::changeText(std::string newText) {
   
}



