#pragma once
#include <map>
#include <string>
#include <vcruntime_typeinfo.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "Vertex.h"

class Entity
{
public:
    int ID  = 0;
    Entity(int id) : ID(id) {}
};

class Components
{
public:
    
};

struct i_component_handler 
{
   };

 template <typename T> 
struct component_handler : i_component_handler
{
     std::vector<T> Components;
     std::map<Entity,int> index;  
};

struct component_manager
{
    std::map<std::string,i_component_handler*> component_map;

    template <typename T>
    void add_component(unsigned int id)
    { 
        if (component_map.contains(typeid(T).name()) == false)
        {
            component_map[typeid(T).name()] = new component_handler<T>();
        }
        component_handler<T>* handler = static_cast<component_handler<T>*>(component_map[typeid(T).name()]);
        handler->Components.emplace_back();
        handler->index[id] = handler->Components.size() - 1;
    }
    
};


struct position_component : public Components
{
    glm::vec3 PlayerPos = glm::vec3(0.f);
};

struct transform_component : public Components
{
    glm::vec3 Scale = glm::vec3(0.f);
    glm::vec3 Rotation = glm::vec3(0.f);
};

struct movement_component : public Components
{
    glm::vec3 Velocity = glm::vec3(0.f);
    float speed = 0.f;
};

struct health_component : public Components
{
    std::vector<int> health;
};

struct matrix_component : public Components
{
    glm::mat4 ModelMatrix = glm::mat4(1.f);
};

struct attack_component : public Components
{
    int damage = 0;
};
