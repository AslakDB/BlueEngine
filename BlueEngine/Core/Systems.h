#pragma once
#include <set>

#include "Components.h"
class Systems
{
public:
    virtual ~Systems() = default;
    virtual void Draw( component_manager componentManager ) {};
   virtual void Update(unsigned int ShaderProgram, component_manager& componentManager) = 0;
   std::pmr::set<Entity> mEntities;
};


struct I_system_handler
{
    virtual ~I_system_handler() = default;
};

template <typename T> 
struct system_handler : public I_system_handler
{
    std::vector<T> Systems;
    std::map<int,int> index;  
};

class SystemManager : public I_system_handler
{
public:
    ~SystemManager()
    {
        for (auto& [key, handler] : m_systems_map)
        {
            delete handler;
        }
    }
    std::map<std::string, I_system_handler*> m_systems_map;
    template <typename T>
    void AddSystem(unsigned int id)
    {
        if (m_systems_map.contains(typeid(T).name()) == false)
        {
            m_systems_map[typeid(T).name()] = new system_handler<T>();
        }
        system_handler<T> *handler = static_cast<system_handler<T>*>(m_systems_map[typeid(T).name()]);
        handler->Systems.emplace_back();
        handler->index[id] = handler->Systems.size() - 1;
    }

    template <typename T>
    void draw_meshes( component_manager& componentManager)
    {
        std::string system_name = typeid(T).name();
        if (m_systems_map.find(system_name) == m_systems_map.end())
        {
            return;
        }
        system_handler<T>* handler = static_cast<system_handler<T>*>(m_systems_map[system_name]);
        for (T& system : handler->Systems)
        {
            system.Draw(componentManager); //unsure what to have here
        }
    }

    
    template <typename T>
    void UpdateSystems(unsigned int ShaderSystem, component_manager& componentManager)
    {
        std::string system_name = typeid(T).name();
        if (m_systems_map.find(system_name) == m_systems_map.end())
        {
            return;
        }
        system_handler<T>* handler = static_cast<system_handler<T>*>(m_systems_map[system_name]);
        for (T& system : handler->Systems)
        {
            system.Update(ShaderSystem,  componentManager); //unsure what to have here
        }
    }
};

struct movementSystem : public Systems
{
    /*position_component *position;*/
    transform_component *transform;
    movement_component *movement;
    void Update(unsigned int ShaderProgram, component_manager& componentManager) override
    {
        transform->PlayerPos += movement->Velocity * movement->speed;
    }

};

struct HealthSystem : public Systems
{
    health_component *health;
    attack_component *attack;
    void Update(unsigned int ShaderProgram, component_manager& componentManager) override
    {  
        component_handler<health_component> *health = componentManager.get_component_handler<health_component>();
        component_handler<attack_component> *attack = componentManager.get_component_handler<attack_component>();
        /*for (health_component &element : health->Components)
        {
            for (attack_component &element2 : attack->Components)
            {
                if (element2.Attack == true)
                {
                    element.Health -= 10;
                    element2.Attack = false;
                }
            }
        }*/
        
    }
};

struct matrix_system : public Systems
{
    matrix_component *matrix;
    //position_component *position;
    transform_component *transform;
    void Update(unsigned int ShaderProgram, component_manager& componentManager) override
    {
        matrix->ModelMatrix = glm::mat4(1.f);
        matrix->ModelMatrix = glm::translate(matrix->ModelMatrix, transform->PlayerPos);
        matrix->ModelMatrix = glm::rotate(matrix->ModelMatrix, transform->Rotation.x, glm::vec3(1.f, 0.f, 0.f));
        matrix->ModelMatrix = glm::rotate(matrix->ModelMatrix, transform->Rotation.y, glm::vec3(0.f, 1.f, 0.f));
        matrix->ModelMatrix = glm::rotate(matrix->ModelMatrix, transform->Rotation.z, glm::vec3(0.f, 0.f, 1.f));
        matrix->ModelMatrix = glm::scale(matrix->ModelMatrix, transform->Scale);
    }
};

struct plane_system : public Systems
{


    void Update(unsigned int ShaderProgram, component_manager& componentManager) override
    {
        component_handler<plane_component> *plane = componentManager.get_component_handler<plane_component>();
        for (plane_component &element : plane->Components)
        {
            element.plane_model.CalculateMatrix();
            element.plane_model.CalculateMatrix(); 
        }
       
    }
   
void Draw ( component_manager componentManager) override
{
    component_handler<plane_component> *plane = componentManager.get_component_handler<plane_component>();

    for (plane_component &element : plane->Components)
    {
   
    element.plane_model.vertices.emplace_back(glm::vec3(5.f, -0.5f, 5.f), glm::vec3(0.f), glm::vec3(0.5f, 0.f, 0.6f));
    element.plane_model.vertices.emplace_back(glm::vec3(5.f, -0.5f, -5.f), glm::vec3(0.f), glm::vec3(0.5f, 0.f, 0.6f));
    element.plane_model.vertices.emplace_back(glm::vec3(-5.f, -0.5f, -5.f), glm::vec3(0.f), glm::vec3(0.5f, 0.f, 0.6f));
    element.plane_model.vertices.emplace_back(glm::vec3(-5.f, -0.5f, 5.f), glm::vec3(0.f), glm::vec3(0.5f, 0.f, 0.6f));

    element.plane_model.indices.emplace_back(0,1,3);
    element.plane_model.indices.emplace_back(1,2,3);
        
    for (Triangle& index : element.plane_model.indices)
    {
        glm::vec3 normal = glm::cross(element.plane_model.vertices[index.B].XYZ - element.plane_model.vertices[index.A].XYZ, element.plane_model.vertices[index.C].XYZ - element.plane_model.vertices[index.A].XYZ);

        element.plane_model.vertices[index.A].Normals += glm::normalize(normal);
        element.plane_model.vertices[index.B].Normals += glm::normalize(normal);
        element.plane_model.vertices[index.C].Normals += glm::normalize(normal);
    }
    element.plane_model.Bind();
    }
}
};


struct cube_system : public Systems
{
    cube_component *cube;
    
};

struct test_system final : public Systems
{
    //std::shared_ptr<test_component> test = std::make_shared<test_component>(this);

    
    // test_component *test;
    void Update(unsigned int ShaderProgram, component_manager& componentManager) override
    {
        component_handler<test_component> *test = componentManager.get_component_handler<test_component>();
        for (test_component &element : test->Components)
        {
            element.TestString = "Hello World";
            std::cout<<element.TestString<<std::endl;
        }
    }
    
};

