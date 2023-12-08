#include "gameObject.h"
#include "textureManager.h"

GameObject::GameObject( const char* textureSheet, int x, int y ){
    //Loading the object's exterior.
    this->objectTexture = TextureManager::loadTexture( textureSheet );
    //Generating the object at the designated position.
    this->xpos = x;
    this->ypos = y;
}

//Update the object's information.
void GameObject::update(){
    this->xpos++;
    this->ypos++;

    this->srcRect.w = 32;
    this->srcRect.h = 32;
    this->srcRect.x = 0;
    this->srcRect.y = 0;

    //Updating the rectangle's position to ensure the rendering position is correct.
    this->destRect.x = xpos;
    this->destRect.y = ypos;
    this->destRect.w = this->srcRect.w * 2;
    this->destRect.h = this->srcRect.h * 2;
}

//Render the game object.
void GameObject::render(){
    SDL_RenderCopy( Game::renderer, this->objectTexture, &srcRect, &destRect );
}