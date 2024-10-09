#pragma once
#include <set>

#include "Components.h"
class Systems
{
public:
   virtual void Update();
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
    std::map<Entity,int> index;  
};

class SystemManager : public I_system_handler
{
public:
    std::map<std::string, I_system_handler*> m_systems_map;
    template <typename T>
    void AddSystem()
    {
        if (m_systems_map.contains(typeid(T).name()) == false)
        {
            m_systems_map[typeid(T).name()] = new system_handler<T>();
        }
        system_handler<T> *handler = static_cast<system_handler<T>*>(m_systems_map[typeid(T).name()]);
        handler->Systems.emplace_back();
        handler->index[handler->Systems.size() - 1] = handler->Systems.size() - 1;
    }
};

struct movementSystem : public Systems
{
    /*position_component *position;*/
    transform_component *transform;
    movement_component *movement;
    void Update() override
    {
        transform->PlayerPos += movement->Velocity * movement->speed;
    }

};

struct HealthSystem : public Systems
{
    health_component *health;
    attack_component *attack;
    void Update() override
    {
        
    }
};

struct matrix_system : public Systems
{
    matrix_component *matrix;
    //position_component *position;
    transform_component *transform;
    void Update() override
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
plane_component* Plane;

void CreatePlane (model& plane_model)
{

}
};