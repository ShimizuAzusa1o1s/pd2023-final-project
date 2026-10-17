#pragma once
#include "components.h"

class StatsComponent : public Component {
public:
    // Member variables.
    int health_point;
    int invincible_time = 0;
    bool skill_cooldown = true;

    // Initialize the component.
    StatsComponent(int health_point) {
        this->health_point = health_point;
    }

    void init() override {}
    void update() override {}
    void draw() override {}
};
