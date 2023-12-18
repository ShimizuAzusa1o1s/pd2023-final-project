#pragma once
#include "components.h"

class StatsComponent : public Component
{
    public:
        int healthPoint;
        int invincibleTime = 0;
        bool skillCooldown = true;
        bool toBeDestroyed = false;
        
    
        StatsComponent( int hp ){
            this->healthPoint = hp;
        }

        void init() override{

        }

        void update() override{
            if( healthPoint == 0 ){
                toBeDestroyed = true;
            }
        }

        void draw(){

        }

};