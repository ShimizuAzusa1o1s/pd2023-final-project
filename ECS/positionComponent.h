#pragma once
#include "component.h"

//Component used to record the entity's position.
class PositionComponent : public Component
{
    private:
        int xpos;
        int ypos;

    public:
        PositionComponent(){
            this->xpos = 0;
            this->ypos = 0;
        }
        
        PositionComponent( int x, int y ){
            this->xpos = x;
            this->ypos = y;
        }

        int x(){ return this->xpos; }
        int y(){ return this->ypos; }

        void init() override{
            this->xpos = 0;
            this->ypos = 0;
        }

        void update() override{
            this->xpos++;
            this->ypos++;
        }

        void setPos( int x, int y ){
            this->xpos = x;
            this->ypos = y;
        }

};