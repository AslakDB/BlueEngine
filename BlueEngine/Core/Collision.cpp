#include "Collision.h"

bool Collision::SphereSphereCollision(glm::vec3 playerPos, std::vector<glm::vec3> otherPos)
{ float radius = 0.5f;


    for (auto element : otherPos)
    {
            glm::vec3 distanceVector = playerPos - element;
            float distanceSquared = glm::dot(distanceVector, distanceVector);
            float radiusSum = 2.0f * radius;
            float radiusSumSquared = radiusSum * radiusSum;

            if (distanceSquared < radiusSumSquared)
            {

                std::cout << "BallCollision" << '\n';
                return true;
            }
        }
    }


void Collision::SphereBoxCollision(const std::vector<model*>& sphere_models, const std::vector<model*>& other_models)
{
    float radius = 0.5f;
        
    for (const auto& sphereModel : sphere_models)
    {
        for (const auto& boxModel : other_models)
        {
              
            glm::vec3 closestPoint = glm::clamp(
                sphereModel->PlayerPos,
                boxModel->BoundingBox.MinPos,
                boxModel->BoundingBox.MaxPos
            );
                
            glm::vec3 distanceVector = sphereModel->PlayerPos - closestPoint;
            float distanceSquared = glm::dot(distanceVector, distanceVector);
            float radiusSquared = radius * radius;

                
            if (distanceSquared < radiusSquared)
            {
                glm::vec3 collisionNormal = glm::normalize(distanceVector);
                sphereModel->Velocity = glm::reflect(sphereModel->Velocity, collisionNormal);
                std::cout << "BallBoxCollision" << '\n';
            }
        }
    }
}

void Collision::CollisionCheck(std::vector<model*> AllModels)
{
    glm::vec3 ClampedNormal = glm::vec3(0.f);
    for (int i = 0; i < AllModels.size() -1; i++)
    {
        for (int j = i +1; j < AllModels.size(); j++)
            if (aabb::AABBIntersect(AllModels[i]->BoundingBox,AllModels[j]->BoundingBox))
            {
                ClampedNormal = glm::clamp(glm::vec3(0.f),AllModels[j]->PlayerPos, AllModels[i]->PlayerPos);
                ClampedNormal = glm::normalize(ClampedNormal);
                std::cout<<"Collision"<<'\n';
                AllModels[i]->PlayerPos = AllModels[i]->PlayerPos;
                AllModels[i]->Velocity = glm::reflect(AllModels[i]->Velocity, ClampedNormal);
            }
           
    }
}
