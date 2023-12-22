#include "ECS.h"

//Add a game object to the group.
void Entity::addGroup( Group mGroup ){
    groupBitset[ mGroup ] = true;
    manager.addToGroup( this, mGroup );
}