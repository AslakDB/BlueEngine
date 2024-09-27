#pragma once
#include "Model.h"

class Sphere{
    
public:
   
    float radius = 0.25f;
    
    void SubDivide(int A,int B, int C, int NumOfDiv, model& SphereModel);
    void CreateSphere(model& SphereModel);
    void Move(model& SphereModel, float deltatime, glm::vec3 RandSpeed);


    
   
   
};