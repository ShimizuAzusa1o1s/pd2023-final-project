#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>

//Generated from ChatGPT 3.5
//https://chat.openai.com/share/21bbde48-e30d-420d-9ddf-d04fc584567a

class Component;
class Entity;
class Manager;

using ComponentID = std::size_t;
using Group = std::size_t;

//Distributing ID to the component(a pre-process for the I/O files)
//inline: This function's implementation can replace their command at which they're called.
inline ComponentID getNewComponentTypeID(){
    static ComponentID lastID = 0u;
    return lastID++;
}

//noexcept: Do not return any exception.
template <typename T> inline ComponentID getComponentTypeID() noexcept{
    static_assert( std::is_base_of< Component, T >::value, "" );
    static ComponentID typeID = getNewComponentTypeID();
    return typeID;
}

//constexpr: 
//Variable's value with this prefix should be determined in the compile stage,
//and it cannot be modified by any syntax.  
constexpr std::size_t maxComponents = 32;
constexpr std::size_t maxGroups = 32;

//bitset: Optimized bool array.
//To set a bitset storing status of the components.
using ComponentBitSet = std::bitset< maxComponents >;
using GroupBitSet = std::bitset< maxGroups >;

//To store the components themselves.
using ComponentArray = std::array< Component*, maxComponents >;

class Component
{
    public:
        Entity* entity;
        //Virtual function.
        virtual void init(){}
        virtual void update(){}
        virtual void draw(){}
        virtual ~Component(){}
};

class Entity
{
    private:
        Manager& manager;
        bool active = true;
        //unique_ptr: 
        //Pointer that owns and manages another object through a pointer 
        //and disposes of that object when the unique_ptr goes out of scope.
        std::vector< std::unique_ptr< Component > > components;
        ComponentArray componentArray;
        ComponentBitSet componentBitSet;
        GroupBitSet groupBitset;
    public:
        Entity( Manager& mManager ) : manager( mManager ){}
        void update(){
            //auto: Let the compiler tell the given variable's type by itself.
            //Update all the components in the entity.  
            for( auto& c: components ) c->update();
        }
        void draw(){
            for( auto& c: components ) c->draw();
        }
        bool isActive(){ return this->active; }
        void destroy(){ this->active = false; }

        bool hasGroup( Group mGroup ){
            return groupBitset[ mGroup ];
        }

        void addGroup( Group mGroup );
        
        void delGroup( Group mGroup ){ 
            groupBitset[ mGroup ] = false;
        }

        //Check if the entity has a certain component.
        template< typename T > bool hasComponent(){ return componentBitSet[ getComponentTypeID<T>() ]; }
        
        //typename... TArgs: 
        //To tell the compiler that we have several arguments behind.
        //To put simply, the number of arguments are now flexible.
        template< typename T, typename... TArgs > T& addComponent( TArgs&&... mArgs ){
            //Constructing the component's information.
            //forward: 
            //A helper function to allow perfect forwarding of arguments 
            //taken as rvalue references to deduced types.
            T* c( new T( std::forward< TArgs >( mArgs )... ) );
            c->entity = this;
            std::unique_ptr<Component> uPtr{ c };
            //Put the component at the end of the vector,
            //and it is constructed in place using TArgs as the arguments for its constructor.
            //Details: https://cplusplus.com/reference/vector/vector/emplace_back/
            //move:
            //A helper function to force move semantics on values, even if they have a name.
            //Simply put, what this part does is like what the copy constructor does.
            this->components.emplace_back(std::move(uPtr));

            //Recording the component's information.
            this->componentArray[ getComponentTypeID<T>() ] = c;
            this->componentBitSet[ getComponentTypeID<T>() ] = true;

            //Initialize the object c pointing to.
            c->init();
            return *c;
        }

        //Obtaining the component's information.
        template <typename T> T& getComponent() const{
            auto ptr( this->componentArray[ getComponentTypeID<T>() ] );
            return *static_cast<T*>(ptr);
        }
};

class Manager
{
    private:
        std::vector< std::unique_ptr< Entity > > entities;
        std::array< std::vector< Entity* >, maxGroups > groupedEntities;
    public:
        //Update the entity's information.
        void update(){
            for( auto& e : this->entities ) e->update();
        }

        //Draw the entity on the window.
        void draw(){
            for( auto& e : this->entities ) e->draw();
        }

        //Removes inactive entities from the entities collection based on the isActive() method of the Entity objects.
        //Details: https://chat.openai.com/share/c111527a-eb08-491e-bc28-e3bf77d064b9
        void refresh(){
            for( auto i(0u); i < maxGroups; i++ ){
                auto& v( groupedEntities[i] );
                v.erase(
                    std::remove_if(
                        std::begin(v),
                        std::end(v),
                        [i]( Entity* mEntity ){ return !mEntity->isActive() || !mEntity->hasGroup(i); }
                    ),
                    std::end(v)
                );
            }

            this->entities.erase( 
                std::remove_if( 
                    std::begin( entities ), 
                    std::end( entities ), 
                    []( const std::unique_ptr< Entity > &mEntity ){ return !mEntity->isActive(); } 
                ),
                std::end( entities )
            );
        }

        void addToGroup( Entity* mEntity, Group mGroup ){
            groupedEntities[ mGroup ].emplace_back( mEntity );
        }

        std::vector<Entity*>& getGroup( Group mGroup ){
            return groupedEntities[ mGroup ];
        }

        //Add entity to the manager.
        Entity& addEntity(){
            Entity* e = new Entity(*this);
            std::unique_ptr< Entity > uPtr{ e };
            this->entities.emplace_back( std::move( uPtr ) );
            return *e;
        }
};