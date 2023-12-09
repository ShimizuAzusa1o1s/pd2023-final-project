#pragma once
#include <SDL2/SDL.h>
#include "ECS.h"
#include "transformComponent.h"
#include "spriteComponent.h"

class TileComponent : public Component
{
    public:
        TransformComponent* transform;
        SpriteComponent* sprite;
        SDL_Rect tileRect;
        int tileID;
        char* path;

        TileComponent() = default;
        TileComponent( int x, int y, int w, int h, int id ){
            this->tileRect.x = x;
            this->tileRect.y = y;
            this->tileRect.w = w;
            this->tileRect.h = h;
            this->tileID = id;

            switch( tileID ){
                case 0:
                    this->path = "assets/Water.bmp";
                    break;
                case 1:
                    this->path = "assets/Grass.bmp";
                    break;
                case 2:
                    this->path = "assets/Dirt.bmp";
                    break;
                default:
                    break;
            }
        }

        void init() override{
            entity->addComponent< TransformComponent >( (float)tileRect.x, (float)tileRect.y, tileRect.w, tileRect.h, 1 );
            transform = &entity->getComponent< TransformComponent >();

            entity->addComponent< SpriteComponent >( path );
            sprite = &entity->getComponent< SpriteComponent >();
        }
};