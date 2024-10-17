#pragma once
#include <vector>
#include <algorithm>
#include <iostream>

class Entity
{
public:
	int ID = 0;
	Entity(int id) : ID(id) {}
	bool operator<(const Entity& other) const { return ID < other.ID; }
	bool operator==(const Entity& other) const { return ID == other.ID; }
};

class entity_manager
{
public:
	entity_manager() {
		for (int i = 0; i < 500; ++i)
		{
			EntityVector.emplace_back(i);
		}
	}

	Entity create_entity()
	{
		if (EntityVector.empty())
		{
			std::cout << "No more entities available" << std::endl;
			return Entity(-1);
		}
		Entity id = EntityVector.back();
		EntityVector.pop_back();
		ActiveEntities.push_back(id);
		return id;
	}

	void destroy_entity(Entity entity)
	{
		EntityVector.push_back(entity);
		ActiveEntities.erase(std::remove(ActiveEntities.begin(), ActiveEntities.end(), entity), ActiveEntities.end());
	}
	

private:
	std::vector<Entity> EntityVector;
	std::vector<Entity> ActiveEntities;
};
