#pragma once
#include <set>

#include "Components.h"
class Systems
{
public:
   virtual void Update();
    std::pmr::set<Entity> mEntities;
};

class SystemManager
{
    
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

/*void CreatePlane (model& plane_model)
{

}*/
};