#pragma once
#include "Vector2.h"


namespace ColliderCollection
{
	struct CircleCollider
	{
		Vector2 center{};
		float radius = 1.0f;
	};

	struct BoxCollider
	{
		Vector2 min{};
		Vector2 max{};
	};

	bool IsCollision(const CircleCollider& c1, const CircleCollider& c2);

	bool IsCollision(const BoxCollider& b, const Vector2& p);
};

