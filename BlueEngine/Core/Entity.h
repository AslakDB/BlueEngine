#pragma once
#include <iostream>
#include "Vertex.h"
#include <vector>
class Entity
{
public:
    int ID  = 0;
    Entity(int id) : ID(id) {}
    
   
};

class entity_manager
{
public:
    entity_manager() {
        for (int i = 0; i < 500; i++)
        {
           EntityVector.emplace_back(i);
        }
    }

 Entity CreateEntity()
{
    	if(EntityVector.empty())
		{
			std::cout << "No more entities available" << std::endl;
			return Entity(-1);
		}
    	Entity id = EntityVector.back();
    	EntityVector.pop_back();
    	ActiveEntities.push_back(id);
    	return id;
}

void DestroyEntity(Entity entity)
{
 	// Add the destroyed entity to the queue
	EntityVector.push_back(entity);
	// Remove the entity from the active entities
	ActiveEntities.erase(std::remove(ActiveEntities.begin(), ActiveEntities.end(), entity), ActiveEntities.end());
}
private:
	// Queue of unused entity IDs
	std::vector<Entity> EntityVector;
	std::vector<Entity> ActiveEntities;

	// Array of signatures where the index corresponds to the entity ID
	//std::array<Signature, MAX_ENTITIES> mSignatures{};

	// Total living entities - used to keep limits on how many exist
	uint32_t mEntityCount{};
};