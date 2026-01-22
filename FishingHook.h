#pragma once
#include "Vector2.h"
#include "GameObject.h"
#include "ColliderCollection.h"

struct FishingHookConfig : GameObjectConfig
{
	GameObject *target = nullptr;
	float speed = 0.0f;

	ColliderCollection::CircleCollider collider{};
};


class FishingHook : public GameObject
{
public:

	FishingHook() = default;
	FishingHook(const FishingHookConfig &config)
		: GameObject(config),
		mTarget(config.target),
		mSpeed(config.speed),
		mCollider(config.collider),
		mMoveDir{},
		mVelocity{}
	{};

	void OnCollision(const GameObject &obj) override;
	void Update() override;
	void MoveUpdate();
	void CheckStageReflect();
	void AddVelocity(const Vector2 &velocity);

public:
	float GetSpeed()const { return mSpeed; }
	Vector2 GetVelocity() const { return mVelocity; }
	ColliderCollection::CircleCollider GetCollider() const { return mCollider; }


private:

	GameObject *mTarget = nullptr;
	const float mSpeed = 15.0f;

	Vector2 mMoveDir{};
	Vector2 mVelocity{};

	ColliderCollection::CircleCollider mCollider{};
};

