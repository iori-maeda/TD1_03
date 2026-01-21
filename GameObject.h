#pragma once
#include "RendererUtility.h"
#include "ColliderCollection.h"

class GameObject : ColliderCollection::CircleCollider
{
	Vector2 velocity{};
	RendererUtility::RenderData renderData{};
	float speed = 10.0f;
};

