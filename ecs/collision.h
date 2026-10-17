#pragma once
#include <SDL2/SDL.h>

class Collision {
public:
    // Static member functions.
    static bool aabb(const SDL_Rect &rect_a, const SDL_Rect &rect_b);
    static bool aabb(const ColliderComponent &collider_a, const ColliderComponent &collider_b);
};
