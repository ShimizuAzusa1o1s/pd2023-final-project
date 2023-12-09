#pragma once
#include "component.h"
#include "..\vector2D.h"

//Component used to record the entity's position.
class TransformComponent : public Component
{
    public:
        Vector2D position;
        
        TransformComponent(){
            this->position.x = 0;
            this->position.y = 0;
        }
        
        TransformComponent( float x, float y ){
            this->position.x = x;
            this->position.y = y;
        }

        void update() override{
            
        }

};