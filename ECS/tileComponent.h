#pragma once
#include <SDL2/SDL.h>
#include "ECS.h"
#include "transformComponent.h"
#include "spriteComponent.h"

class TileComponent : public Component
{
    public:
        //Member Variables.
        TransformComponent* transform;
        SpriteComponent* sprite;
        SDL_Rect tileRect;
        int tileID;
        const char* path;
        //Constructors.
        TileComponent() = default;
        TileComponent( int x, int y, int w, int h, int id ){
            this->tileRect.x = x;
            this->tileRect.y = y;
            this->tileRect.w = w;
            this->tileRect.h = h;
            this->tileID = id;
            //Determine the texture by ID number.
            switch( tileID ){
                case 0:
                    this->path = "assets/Background_Pink.bmp";
                    break;
                default:
                    break;
            }
        }
        //Initialize the component.
        void init() override{
            entity->addComponent< TransformComponent >( (float)tileRect.x, (float)tileRect.y, tileRect.w, tileRect.h, 1 );
            transform = &entity->getComponent< TransformComponent >();
            entity->addComponent< SpriteComponent >( path );
            sprite = &entity->getComponent< SpriteComponent >();
        }
};