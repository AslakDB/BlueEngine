#pragma once
#include "Model.h"

class Collision
{
public:
    /// collisjon mellom alle spheres i prosjektet
    /// @param sphere_models 
    void SphereSphereCollision(std::vector<model*> sphere_models);

    /// collision sjekk mellom Sphere og AABB
    /// @param sphere_models 
    /// @param other_models 
    void SphereBoxCollision(const std::vector<model*>& sphere_models, const std::vector<model*>& other_models);


    /// En gammel collsison check som er basert AABB collision (ikke i bruk)
    /// @param AllModels 
    void CollisionCheck(std::vector<model*> AllModels);

  
};