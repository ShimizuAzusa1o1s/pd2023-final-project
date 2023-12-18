#pragma once
#include "components.h"
#include "..\vector2D.h"

//Component used to record the entity's position.
class TransformComponent : public Component
{
    public:
        Vector2D position;
        Vector2D velocity;

        int height = 16;
        int width = 16;
        int scale = 2;

        float speed = 3;

        TransformComponent(){
            this->position.zero();
        }

        TransformComponent( int sc ){
            this->position.zero();
            this->scale = sc;
        }
        
        TransformComponent( float x, float y ){
            this->position.x = x;
            this->position.y = y;
        }

        TransformComponent( float x, float y, int w, int h, int sc ){
            this->position.x = x;
            this->position.y = y;
            this->width = w;
            this->height = h;
            this->scale = sc;
        }

        void init() override{
            this->velocity.zero();
        }

        void update() override{
            position.x += velocity.x * speed;
            position.y += velocity.y * speed;
        }

};