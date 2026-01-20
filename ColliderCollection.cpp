#include "ColliderCollection.h"

bool ColliderCollection::IsCollision(const CircleCollider& c1, const CircleCollider& c2)
{
	Vector2 diff = c1.center - c2.center;
	float sumR = c1.radius + c2.radius;

	return diff.LengthSquared() <= sumR * sumR;
}

bool ColliderCollection::IsCollision(const BoxCollider& b, const Vector2& p)
{
	bool isHitX = p.x >= b.min.x && p.x <= b.max.x;
	bool isHitY = p.y >= b.min.y && p.y <= b.max.y;
	return isHitX && isHitY;
}
