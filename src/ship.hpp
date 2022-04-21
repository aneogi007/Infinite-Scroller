#ifndef SHIP_HPP
#define SHIP_HPP

#include "mesh.hpp"

class Ship: public Mesh {
    public:
        
        Ship(std::string filename, float s, float d, float h, float w) : 
         Mesh(filename) {
            
            height = h;
            width = w;
            
            shipSpeed = s;
            DragExp = d;
         }

        //for player movement left to right
        void moveLeft(float speed);
        void moveRight(float speed);

        void largeDragRight();
        void smallDragRight();

        void largeDragLeft();
        void smallDragLeft();

        void smartDragLeft(int i);
        void smartDragRight(int i);

        void continueDrag();

        float getCoolDownFrames() { return largeDrag; }
        float getSmallDrag() { return smallDrag; }

        float getShipSpeed() { return shipSpeed; }

        void setDragState(bool isDrag) { dragState = isDrag; }
        bool inDragState() {return dragState; }

        void resetDrag() {
            currentDragSpeed = shipSpeed; 
            currentDragFrame = 0;
            dragState = false;
            ssl = false;
            ssr = false;
            smartDrag = 0;
            }


    protected:
        
        std::string xmodelName;
        float Xconstraint = 5.0f;
        
        //Unique Ship Features
        float shipSpeed = 0.01f;
        float health = 100.0f; 

        float largeDrag = 200.0f;
        float smallDrag = 100.0f;

        float smartDrag = 0;
        float DragExp = 1.0;


        
        //current State 
        bool dragState = false;
        bool ssr = false;
        bool ssl = false;


        int currentDragFrame = 0;
        float currentDragSpeed = shipSpeed;


    private:
};

#endif
