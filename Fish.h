#pragma once
#include "Vector2.h"
#include "GameObject.h"
#include "ColliderCollection.h"

struct FishConfig : public GameObjectConfig
{
	float speed = 1.0f;
	ColliderCollection::CircleCollider collider{};
	GameObject* target = nullptr;
	float tailWaveSpeed = 10.0f;
};

class Fish : public GameObject
{
public:
	Fish()
		:mSpeed(1.0f), mMoveDir{}, mVelocity{}, mTraget(nullptr), mIsFishing(false), mCollider{}
	{
	};

	Fish(const FishConfig& config)
		:GameObject(config),
		mTraget(config.target),
		mSpeed(config.speed),
		mCollider(config.collider),
		mIsFishing(false),
		mTailWaveSpeed(config.tailWaveSpeed),
		mTailWaveValue(0.0f)
	{
	};

	void OnCollision(const GameObject& obj) override;

	void Update() override;
	void Spawn(const FishConfig& config);
	void Escape();

private:
	void Excursion();
	void GoArea();
	void StateUpdate();

public:

	bool IsFishing() const { return mIsFishing; }
	Vector2 GetMoveDir() const { return mMoveDir; }
	Vector2 GetVelocity() const { return mVelocity; }
	ColliderCollection::CircleCollider GetCollider() const { return mCollider; }

	void SetPosition(const Vector2& position) { mCenterPosition = position; }

private:
	enum class State
	{
		Spawn,
		GoArea,
		Excursion,
		Escape
	} mState = State::Spawn;
	Vector2 mStartPosition{};


	GameObject* mTraget = nullptr;

	float mSpeed = 1.0f;
	Vector2 mMoveDir{};
	Vector2 mVelocity{};
	float mTailWaveValue = 0.0f;
	float mTailWaveSpeed = 10.0f;

	bool mIsFishing = false;
	ColliderCollection::CircleCollider mCollider{};
};

