#pragma once
#include "Vector2.h"
#include "GameObject.h"
#include "ColliderCollection.h"

struct PlayerConfig : public GameObjectConfig
{
	float speed = 1.0f;
	ColliderCollection::CircleCollider collider{};
};

class Player : public GameObject
{
public:
	Player()
		:mSpeed(1.0f), mMoveDir{}
	{};

	Player(const PlayerConfig &config)
		:GameObject(config), mSpeed(config.speed), mCollider(config.collider)
	{};

	void OnCollision(const GameObject &obj) override;

	void Update() override;

	void MoveLeft();
	void MoveRight();
	void MoveUp();
	void MoveDown();

public:
	ColliderCollection::CircleCollider GetCollider() const { return mCollider; }

private:
	const float mSpeed = 1.0f;
	Vector2 mMoveDir{};
	Vector2 mVelocity{};

	ColliderCollection::CircleCollider mCollider{};
};