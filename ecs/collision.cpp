#include "collision.h"
#include "collider_component.h"

// AABB (axis-aligned bounding box) test.
// Two objects collide if their colliders overlap.
bool Collision::aabb(const SDL_Rect &rect_a, const SDL_Rect &rect_b) {
    if (
        rect_a.x + rect_a.w >= rect_b.x &&
        rect_b.x + rect_b.w >= rect_a.x &&
        rect_a.y + rect_a.h >= rect_b.y &&
        rect_b.y + rect_b.h >= rect_a.y
    ) {
        return true;
    }
    return false;
}

bool Collision::aabb(const ColliderComponent &collider_a, const ColliderComponent &collider_b) {
    if (aabb(collider_a.collider, collider_b.collider)) {
        return true;
    } else {
        return false;
    }
}
