#pragma once
#include "Model.h"
#include "Components.h"
#include "Systems.h"

class Sphere{
    
public:
   
    float radius = 0.25f;
    
    void SubDivide(int A,int B, int C, int NumOfDiv, model& SphereModel);
    void CreateSphere(model& SphereModel);
    void Move(model& SphereModel, float deltatime, glm::vec3 RandSpeed);
    
};

struct sphere_component : public Components
{
    float radius = 0.5f;
    model sphere_model;
};
struct sphere_system : public Systems
{
    sphere_component sphere;
    void Update(unsigned int ShaderProgram, component_manager& componentManager) override;
   void SubDivide(int A, int B, int C, int NumOfDiv, model& SphereModel);
    void Draw(component_manager componentManager)override; 
  };
