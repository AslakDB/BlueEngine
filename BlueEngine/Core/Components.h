#pragma once
#include <map>
#include <string>
#include <vcruntime_typeinfo.h>
#include <vector>
#include "Model.h"
#include "Entity.h"

class Components
{
public:
    
};

struct i_component_handler 
{
    virtual ~i_component_handler() = default;
};

 template <typename T> 
struct component_handler : i_component_handler
{
     std::vector<T> Components;
     std::map<int,int> index;  
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

    template <typename T>
    void remove_component(unsigned int id)
    {
        std::string component_name = typeid(T).name();
        if (component_map.contains(component_name) == false)
        {
            return;
        }
        component_handler<T>* handler = static_cast<component_handler<T>*>(component_map[component_name]);
        int idx = handler->index[id];
        
        handler->Components.erase(handler->Components.begin() + idx);
        handler->index.erase(id);

        for (auto& [key, value] : handler->index)
        {
            if (value > idx)
            {
                --value;
            }
        }
        
    }
    template <typename T>
    component_handler<T>* get_component_handler()
    {
        return static_cast<component_handler<T>*>(component_map[typeid(T).name()]);
    }
};


/*struct position_component : public Components
{
    
};*/

struct transform_component : public Components
{
    glm::vec3 PlayerPos = glm::vec3(0.f);
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


struct cube_component : public Components
{
    model cube_model;
};

struct plane_component : public Components
{
    model plane_model;
};

struct test_component : public Components
{
    std::string TestString;
};

struct model_component : public Components
{
    unsigned int VBO, VAO, EBO;
    std::vector<Vertex> vertices;
    std::vector<Triangle> indices;
    std::vector<glm::vec3> corners;
    
    int modelLoc = 0;
};
