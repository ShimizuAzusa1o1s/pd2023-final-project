#pragma once
#include "components.h"

class StatsComponent : public Component
{
    public:
        int healthPoint;
        int invincibleTime = 0;
        bool skillCooldown = true;
    
        StatsComponent( int hp ){
            this->healthPoint = hp;
        }

        void init() override{

        }

        void update() override{

        }

        void draw() override{

        }

};