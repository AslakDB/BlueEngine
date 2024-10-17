#pragma once
#include "Model.h"
#include "Components.h"

class Collision
{
public:
    /// collisjon mellom alle spheres i prosjektet
    /// @param sphere_models 
    bool SphereSphereCollision(glm::vec3 playerPos, std::vector<glm::vec3> otherPos);

    /// collision sjekk mellom Sphere og AABB
    /// @param sphere_models 
    /// @param other_models 
    void SphereBoxCollision(const std::vector<model*>& sphere_models, const std::vector<model*>& other_models);


    /// En gammel collsison check som er basert AABB collision (ikke i bruk)
    /// @param AllModels 
    void CollisionCheck(std::vector<model*> AllModels);

  
};