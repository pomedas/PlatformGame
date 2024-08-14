#pragma once

#include "Input.h"
#include "Render.h"
#include "tinyxml2.h"

using namespace tinyxml2;

enum class EntityType
{
	PLAYER,
	UNKNOWN
};

class Entity
{
public:

	Entity(EntityType type) : type(type), active(true) {}

	virtual bool Awake()
	{
		return true;
	}

	virtual bool Start()
	{
		return true;
	}

	virtual bool Update(float dt)
	{
		return true;
	}

	virtual bool CleanUp()
	{
		return true;
	}

	void Enable()
	{
		if (!active)
		{
			active = true;
			Start();
		}
	}

	void Disable()
	{
		if (active)
		{
			active = false;
			CleanUp();
		}
	}

public:

	std::string name;
	EntityType type;
	bool active = true;
	XMLNode* parameters; // This is a pointer to the XML node that contains the parameters of the entity

	// Possible properties, it depends on how generic we
	// want our Entity class, maybe it's not renderable...
	Vector2D position;       
	bool renderable = true;
};