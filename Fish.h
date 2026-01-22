#pragma once
#include "Vector2.h"
#include "GameObject.h"
#include "ColliderCollection.h"

struct FishConfig : public GameObjectConfig
{
	float speed = 1.0f;
	ColliderCollection::CircleCollider collider{};
	GameObject *target = nullptr;
};

class Fish : public GameObject
{
public:
	Fish()
		:mSpeed(1.0f), mMoveDir{}, mVelocity{}, mTraget(nullptr), mIsFishing(false), mCollider{}
	{};

	Fish(const FishConfig &config)
		:GameObject(config), mTraget(config.target), mSpeed(config.speed), mCollider(config.collider), mIsFishing(false)
	{};

	void OnCollision(const GameObject &obj) override;

	void Update() override;


	void Spawn(const FishConfig &config);
	void Excursion();

public:

	bool IsFishing() const {return mIsFishing;}
	Vector2 GetMoveDir() const { return mMoveDir; }
	Vector2 GetVelocity() const { return mVelocity; }
	ColliderCollection::CircleCollider GetCollider() const { return mCollider; }

	void SetPosition(const Vector2 &position) { mCenterPosition = position; }

private:
	GameObject *mTraget = nullptr;

	float mSpeed = 1.0f;
	Vector2 mMoveDir{};
	Vector2 mVelocity{};

	bool mIsFishing = false;
	ColliderCollection::CircleCollider mCollider{};
};

