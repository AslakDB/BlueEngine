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
    T& getComponent(unsigned int id)
    {
        component_handler<T>* compPtr = get_component_handler<T>();

        return compPtr->Components[compPtr->index[id]];
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
    glm::vec3 Scale = glm::vec3(1.f);
    glm::vec3 Rotation = glm::vec3(0.f);
};
struct movement_component : public Components
{
    glm::vec3 Velocity = glm::vec3(0.f);
    float speed = 0.f;
};

struct health_component : public Components
{
    int health;
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

struct mesh_component : public Components
{
    unsigned int VBO, VAO, EBO;
    std::vector<Vertex> vertices;
    std::vector<Triangle> indices;
    std::vector<glm::vec3> corners;

    mesh_component() :  VBO(0),VAO(0), EBO(0) { }

    ~mesh_component()
    {
        if (VAO) glDeleteVertexArrays(1, &VAO);
        if (VBO) glDeleteBuffers(1, &VBO);
        if (EBO) glDeleteBuffers(1, &EBO);
    }

    void bind()
    {
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        // Generate and bind the Vertex Buffer Object (VBO)
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

        // Generate and bind the Element Buffer Object (EBO)
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(Triangle), indices.data(), GL_STATIC_DRAW);

        // Set the vertex attribute pointers
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        // Unbind the VBO (optional)
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Unbind the VAO (optional)
        glBindVertexArray(0);
    }
    
    int modelLoc = 0;
};

struct model_component : public Components
{
    std::string MeshName;
    glm::vec3 colour = glm::vec3(1.f);
};