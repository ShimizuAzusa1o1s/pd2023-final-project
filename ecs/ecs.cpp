#include "ecs.h"

// Add a game object to the group.
void Entity::add_group(Group group) {
    group_bit_set[group] = true;
    manager.add_to_group(this, group);
}
