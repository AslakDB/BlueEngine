#pragma once
#include <set>

#include "Components.h"
class Systems
{
public:
    virtual ~Systems() = default;
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


    template <typename T>
    void UpdateSystems()
    {
        std::string system_name = typeid(T).name();
        if (m_systems_map.find(system_name) == m_systems_map.end())
        {
            return;
        }
        system_handler<T>* handler = static_cast<system_handler<T>*>(m_systems_map[system_name]);
        for (T& system : handler->Systems)
        {
            system.Update();
        }
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
        for (int i = 0; i < health->health.size(); i++)
        {
            health->health[i] -= attack->damage;
        }
        
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
    plane_model.vertices.emplace_back(glm::vec3(5.f, -0.5f, 5.f), glm::vec3(0.f), glm::vec3(0.5f, 0.f, 0.6f));
    plane_model.vertices.emplace_back(glm::vec3(5.f, -0.5f, -5.f), glm::vec3(0.f), glm::vec3(0.5f, 0.f, 0.6f));
    plane_model.vertices.emplace_back(glm::vec3(-5.f, -0.5f, -5.f), glm::vec3(0.f), glm::vec3(0.5f, 0.f, 0.6f));
    plane_model.vertices.emplace_back(glm::vec3(-5.f, -0.5f, 5.f), glm::vec3(0.f), glm::vec3(0.5f, 0.f, 0.6f));

    plane_model.indices.emplace_back(0,1,3);
    plane_model.indices.emplace_back(1,2,3);
        
    for (Triangle& index : plane_model.indices)
    {
        glm::vec3 normal = glm::cross(plane_model.vertices[index.B].XYZ - plane_model.vertices[index.A].XYZ, plane_model.vertices[index.C].XYZ - plane_model.vertices[index.A].XYZ);

        plane_model.vertices[index.A].Normals += glm::normalize(normal);
        plane_model.vertices[index.B].Normals += glm::normalize(normal);
        plane_model.vertices[index.C].Normals += glm::normalize(normal);
    }
    plane_model.Bind();
}
};

struct sphere_system : public Systems
{
    sphere_component *sphere;
   void SubDivide(int A, int B, int C, int NumOfDiv, model& SphereModel)
{
       if(NumOfDiv > 0)
        {
            glm::vec3 v1 =glm::normalize(SphereModel.vertices[A].XYZ + SphereModel.vertices[B].XYZ);
            glm::vec3 v2 =glm::normalize(SphereModel.vertices[A].XYZ+SphereModel.vertices[C].XYZ);
            glm::vec3 v3 =glm::normalize(SphereModel.vertices[C].XYZ+SphereModel.vertices[B].XYZ );
        

            int index1 =SphereModel.vertices.size(); 
            SphereModel.vertices.emplace_back(v1,glm::vec3(0.f),glm::vec3(1.f,0.f,0.f));
            int index2 = SphereModel.vertices.size();
            SphereModel.vertices.emplace_back(v2,glm::vec3(0.f),glm::vec3(1.f,0.f,0.f));
            int index3 = SphereModel.vertices.size();
            SphereModel.vertices.emplace_back(v3,glm::vec3(0.f),glm::vec3(1.f,0.f,0.f));
        
            SubDivide(A,index1,index2, NumOfDiv -1, SphereModel);
            SubDivide(C,index2,index3, NumOfDiv -1,SphereModel);
            SubDivide(B,index3,index1, NumOfDiv -1,SphereModel);
            SubDivide(index3,index2,index1, NumOfDiv -1,SphereModel);
        }
        else
        {
            SphereModel.indices.emplace_back(A,B,C);
        }
}
    void CreateSphere(model& SphereModel)
{
    SphereModel.vertices.emplace_back(glm::vec3(0.f,0.f,1.f), glm::vec3(0.f), glm::vec3(0.6f));
    SphereModel.vertices.emplace_back(glm::vec3(1.f,0.f,0.f), glm::vec3(0.f), glm::vec3(0.6f));
    SphereModel.vertices.emplace_back(glm::vec3(0.f,1.f,0.f), glm::vec3(0.f), glm::vec3(0.6f));
    SphereModel.vertices.emplace_back(glm::vec3(-1.f,0.f,0.f), glm::vec3(0.f), glm::vec3(0.6f));
    SphereModel.vertices.emplace_back(glm::vec3(0.f,-1.f,0.f), glm::vec3(0.f), glm::vec3(0.6f));
    SphereModel.vertices.emplace_back(glm::vec3(0.f,0.f,-1.f), glm::vec3(0.f), glm::vec3(0.6f));

    SubDivide(0,1,2,4, SphereModel);
    SubDivide(0,2,3,4, SphereModel);
    SubDivide(0,3,4,4, SphereModel);
    SubDivide(0,4,1,4, SphereModel);
    SubDivide(5,2,1,4, SphereModel);
    SubDivide(5,3,2,4, SphereModel);
    SubDivide(5,4,3,4, SphereModel);
    SubDivide(5,1,4,4, SphereModel);
    for (Triangle& index : SphereModel.indices)
    {
        glm::vec3 normal = glm::cross(SphereModel.vertices[index.B].XYZ - SphereModel.vertices[index.A].XYZ, SphereModel.vertices[index.C].XYZ - SphereModel.vertices[index.A].XYZ);

        SphereModel.vertices[index.A].Normals += glm::normalize(normal);
        SphereModel.vertices[index.B].Normals += glm::normalize(normal);
        SphereModel.vertices[index.C].Normals += glm::normalize(normal);}
       
       SphereModel.Bind();
       
    SphereModel.PlayerScale = glm::vec3(0.5f);
}};

struct cube_system : public Systems
{
    cube_component *cube;
    
};