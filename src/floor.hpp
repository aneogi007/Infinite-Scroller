#ifndef FLOOR_HPP
#define FLOOR_HPP

#include "mesh.hpp"

class Floor: public Mesh {
    public:
        
        Floor(std::string filename) : 
        Mesh(filename) {
            objColor.r = 50;
            objColor.g = 50;
            objColor.b = 50;
            objColor /= 255.0f;
        }

        //for player movement left to right
        void moveBack(float speed);
	    void setSpawn(glm::mat4 newSpawnMat) { spawnMat = newSpawnMat; }

    protected:
        
        glm::mat4 spawnMat = glm::mat4(1.0f);
        float Zconstraint = 20.0f;

    private:
};

#endif
