#ifndef SHIP_HPP
#define SHIP_HPP

#include "mesh.hpp"

class Ship: public Mesh {
    public:
        
        Ship(std::string filename, float spd) : 
         Mesh(filename) {}

        //for player movement left to right
        void moveLeft(float speed);
        void moveRight(float speed);

    protected:
        
        //how far player can move left and right on grid
        // float xXconstraint = 5.0f;

        // bool xdestroyed = false;

        // float xSpd;
        //how far player can move left and right on grid
	    float Xconstraint = 5.0f;
        
        std::string xmodelName;

    private:
};

#endif
