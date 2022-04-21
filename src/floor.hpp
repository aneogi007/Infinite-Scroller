#ifndef FLOOR_HPP
#define FLOOR_HPP

#include "mesh.hpp"

class Floor: public Mesh {
    public:
        
        Floor(std::string filename) : 
        Mesh(filename) {}

        //for player movement left to right
        void moveBack(float speed);
	    void setSpawn(glm::mat4 newSpawnMat) { spawnMat = newSpawnMat; }

    protected:
        
        glm::mat4 spawnMat = glm::mat4(1.0f);
        float Zconstraint = 20.0f;

    private:
};

#endif
