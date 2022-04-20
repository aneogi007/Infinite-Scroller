#ifndef STATICOBSTACLE_HPP
#define STATICOBSTACLE_HPP

#include "mesh.hpp"

class StaticObstacle: public Mesh {
    public:
        
        StaticObstacle(std::string filename) : 
         Mesh(filename) {}

        //for player movement left to right
        void moveBack(float speed);
        void setNewSpawn();

        inline void setSpawnMat(const glm::mat4 model) { spawnMat = model; }
	    inline glm::mat4 getSpawnMat() { return spawnMat; }
        
        float isdestroyed() { return destroyed; }
	    void setdestroyed(bool flag) { destroyed = flag; }
        

    protected:
        
        //how far player can move left and right on grid
        // float xXconstraint = 5.0f;

        // bool xdestroyed = false;

        // float xSpd;
        //how far player can move left and right on grid

        std::string obsName;

        glm::mat4 spawnMat = glm::mat4(1.0f);

        float Zconstraint = 10.0f;
	    bool destroyed = false;


    private:
};

#endif
