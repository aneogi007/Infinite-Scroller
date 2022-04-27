#ifndef SHIP_HPP
#define SHIP_HPP

#include "mesh.hpp"

class Ship: public Mesh {
    public:
        
        Ship(std::string filename, float s, float d, float h, float w) : 
         Mesh(filename) {

            objColor.r = 120;
            objColor.g = 188;
            objColor.b = 196;
            objColor /= 255.0f;
            
            height = h;
            width = w;
            
            // shipSpeed = s;
            // DragExp = d;
         }

        //for player movement left to right
        void moveLeft(float speed);
        void moveRight(float speed);

        void rotateCC(float speed);
        void rotateZ(float speed);

        // void largeDragRight();
        // void smallDragRight();

        // void largeDragLeft();
        // void smallDragLeft();

        // void smartDragLeft(int i);
        // void smartDragRight(int i);

        // void continueDrag();

        // float getCoolDownFrames() { return largeDrag; }
        // float getSmallDrag() { return smallDrag; }

        // float getShipSpeed() { return shipSpeed; }

        // void setDragState(bool isDrag) { dragState = isDrag; }
        // bool inDragState() {return dragState; }

        // void resetDrag() {
        //     currentDragSpeed = shipSpeed; 
        //     currentDragFrame = 0;
        //     dragState = false;
        //     ssl = false;
        //     ssr = false;
        //     smartDrag = 0;
        //     }


        void smartMove();

        inline void setSpawnMat(const glm::mat4 model) { spawnMat = model; }
	    inline glm::mat4 getSpawnMat() { return spawnMat; }

        inline void setMenuMat(const glm::mat4 model) { menuMat = model; }
	    inline glm::mat4 getMenuMat() { return menuMat; }

        inline void setSpeedText(std::string st) { speedText = st; } 
	    inline std::string getSpeedText() { return speedText; }

        inline void setControlText(std::string ct) { controlText = ct; } 
	    inline std::string getControlText() { return controlText; }

        inline void setLeftMove(bool move) { leftMove = move; }
        inline void setRightMove(bool move) { rightMove = move; }
        inline void setdriftMode(bool move) { driftMode = move; }


   


    protected:
        //control
        float maxSpeed = 0.05;
        float currSpeed = 0;
        float acceleration = 0.0001;
        float coolDrag = 0.00001;
        float tilt = 0.08;
        float currentTilt = 0.0;
        float maxTilt = 45.0;
        //flags
        bool leftMove = false;
        bool rightMove = false;
        bool driftMode = false;



        
        std::string xmodelName;
        float Xconstraint = 5.0f;
        
        //Unique Ship Features
        // float shipSpeed = 0.01f;
        // float health = 100.0f; 

        // float largeDrag = 200.0f;
        // float smallDrag = 100.0f;

        // float smartDrag = 0;
        // float DragExp = 1.0;
        
        // //current State 
        // bool dragState = false;
        // bool ssr = false;
        // bool ssl = false;


        // int currentDragFrame = 0;
        // float currentDragSpeed = shipSpeed;

        //spawn in mat
        glm::mat4 spawnMat = glm::mat4(1.0f);
        //matrix for menu
        glm::mat4 menuMat = glm::mat4(1.0f);

        std::string speedText = "---";
        std::string controlText = "---";



    private:
};

#endif
