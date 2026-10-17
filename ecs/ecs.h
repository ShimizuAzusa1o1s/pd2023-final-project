#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>


class Component;
class Entity;
class Manager;

using ComponentID = std::size_t;
using Group = std::size_t;

// Distribute an ID to each component type (a pre-process for the I/O files).
// inline: This function's implementation can replace the command at which it is called.
inline ComponentID get_new_component_type_id() {
    static ComponentID last_id = 0u;
    return last_id++;
}

// noexcept: Do not throw any exception.
template <typename T> inline ComponentID get_component_type_id() noexcept {
    static_assert(std::is_base_of<Component, T>::value, "");
    static ComponentID type_id = get_new_component_type_id();
    return type_id;
}

// constexpr:
// The value of a variable with this prefix is determined at compile time,
// and it cannot be modified by any syntax.
constexpr std::size_t MAX_COMPONENTS = 32;
constexpr std::size_t MAX_GROUPS = 32;

// bitset: Optimized bool array.
// Store the status of the components and the groups.
using ComponentBitSet = std::bitset<MAX_COMPONENTS>;
using GroupBitSet = std::bitset<MAX_GROUPS>;

// Store the components themselves.
using ComponentArray = std::array<Component *, MAX_COMPONENTS>;

class Component {
public:
    Entity *entity;
    // Virtual functions for child classes.
    virtual void init() {}
    virtual void update() {}
    virtual void draw() {}
    virtual ~Component() {}
};

class Entity {
private:
    Manager &manager;
    bool active = true;
    // unique_ptr:
    // Pointer that owns and manages another object through a pointer,
    // and disposes of that object when the unique_ptr goes out of scope.
    std::vector<std::unique_ptr<Component>> components;
    ComponentArray component_array;
    ComponentBitSet component_bit_set;
    GroupBitSet group_bit_set;

public:
    // Constructor.
    Entity(Manager &owner) : manager(owner) {}

    // Update every component in the entity.
    void update() {
        // auto: Let the compiler tell the given variable's type by itself.
        for (auto &c : components) c->update();
    }

    // Draw every component in the entity.
    void draw() {
        for (auto &c : components) c->draw();
    }

    // Getters.
    bool is_active() { return this->active; }

    // Destroy the entity.
    void destroy() { this->active = false; }

    // Check if the entity belongs to a certain group.
    bool has_group(Group group) {
        return group_bit_set[group];
    }

    // Add the entity to the group.
    void add_group(Group group);

    // Remove the entity from the group.
    void del_group(Group group) {
        group_bit_set[group] = false;
    }

    // Check if the entity has a certain component.
    template <typename T> bool has_component() {
        return component_bit_set[get_component_type_id<T>()];
    }

    // typename... TArgs:
    // Tell the compiler that several arguments follow.
    // To put it simply, the number of arguments is flexible.
    template <typename T, typename... TArgs> T &add_component(TArgs &&...args) {
        // Construct the component's information.
        // forward:
        // A helper function to allow perfect forwarding of arguments
        // taken as rvalue references to deduced types.
        T *c(new T(std::forward<TArgs>(args)...));
        c->entity = this;
        std::unique_ptr<Component> u_ptr{c};
        // Put the component at the end of the vector,
        // and it is constructed in place using TArgs as the arguments for its constructor.
        // Details: https://cplusplus.com/reference/vector/vector/emplace_back/
        // move:
        // A helper function to force move semantics on values, even if they have a name.
        // Simply put, this part works like a copy constructor.
        this->components.emplace_back(std::move(u_ptr));

        // Record the component's information.
        this->component_array[get_component_type_id<T>()] = c;
        this->component_bit_set[get_component_type_id<T>()] = true;

        // Initialize the object that c points to.
        c->init();
        return *c;
    }

    // Obtain the component's information.
    template <typename T> T &get_component() const {
        auto ptr(this->component_array[get_component_type_id<T>()]);
        return *static_cast<T *>(ptr);
    }
};

// Manager contains entities.
class Manager {
private:
    std::vector<std::unique_ptr<Entity>> entities;
    std::array<std::vector<Entity *>, MAX_GROUPS> grouped_entities;

public:
    // Update the entity's information.
    void update() {
        for (auto &e : this->entities) e->update();
    }

    // Draw the entity on the window.
    void draw() {
        for (auto &e : this->entities) e->draw();
    }

    // Remove inactive entities from the entity collection,
    // based on the is_active() method of the Entity objects.
    void refresh() {
        for (auto i(0u); i < MAX_GROUPS; i++) {
            auto &v(grouped_entities[i]);
            v.erase(
                std::remove_if(
                    std::begin(v),
                    std::end(v),
                    [i](Entity *entity) { 
                        return !entity->is_active() || !entity->has_group(i); 
                    }
                ),
                std::end(v)
            );
        }
        this->entities.erase(
            std::remove_if(
                std::begin(entities),
                std::end(entities),
                [](const std::unique_ptr<Entity> &entity) { 
                    return !entity->is_active(); 
                }
            ),
            std::end(entities)
        );
    }

    // Add an entity to the group.
    void add_to_group(Entity *entity, Group group) {
        grouped_entities[group].emplace_back(entity);
    }

    // Get the whole vector of entities in the group.
    std::vector<Entity *> &get_group(Group group) {
        return grouped_entities[group];
    }

    // Add an entity to the manager.
    Entity &add_entity() {
        Entity *e = new Entity(*this);
        std::unique_ptr<Entity> u_ptr{e};
        this->entities.emplace_back(std::move(u_ptr));
        return *e;
    }
};
