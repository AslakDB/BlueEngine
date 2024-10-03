#include "Plane.h"

void Plane::CreateFloor(model& plane_model)
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
