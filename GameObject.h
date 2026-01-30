#pragma once
#include "Vector2.h"

enum class ObjectTag
{
	None,
	Player,
	Fish,
	FishingHook,
	Particle
};

struct GameObjectConfig
{
	ObjectTag tag = ObjectTag::None;
	bool isActive = true;
	Vector2 centerPosition{};
	Vector2 sizeHalf{ 5.0f, 5.0f };
	Vector2 scale{ 1.0f, 1.0f };
	unsigned int color = 0xffffffff;
};

class GameObject
{
public:
	GameObject()
		:
		mTag(ObjectTag::None),
		mIsActive(true),
		mCenterPosition{},
		mSizeHalf{ 32.0f, 32.0f },
		mScale{ 1.0f ,1.0f },
		mColor(0xffffffff)
	{};

	GameObject(ObjectTag tag, const Vector2 &position, const Vector2 &sizeHalf, unsigned int color = 0xffffffff, bool isActive = true)
		:
		mTag(tag),
		mIsActive(isActive),
		mCenterPosition(position),
		mSizeHalf(sizeHalf),
		mScale{ 1.0f ,1.0f },
		mColor(color)
	{};

	GameObject(const GameObjectConfig &config)
		:mTag(config.tag),
		mIsActive(config.isActive),
		mCenterPosition(config.centerPosition),
		mSizeHalf(config.sizeHalf),
		mScale(config.scale),
		mColor(config.color)
	{};

	virtual void Update() {};
	virtual void OnCollision([[maybe_unused]] const GameObject &obj) {};

	virtual void ClampGameArea();

public:

	ObjectTag GetTag() const { return mTag; }
	bool CompareTag(ObjectTag tag)const { return mTag == tag; }
	bool IsActive() const { return mIsActive; }
	Vector2 GetPosition() const { return mCenterPosition; }
	Vector2 GetSizeHalf() const { return mSizeHalf; }
	Vector2 GetScale() const { return mScale; }
	unsigned int GetColor() const { return mColor; }

	void SetPosition(const Vector2 &position) { mCenterPosition = position; }

protected:
	ObjectTag mTag = ObjectTag::None;
	bool mIsActive = true;
	Vector2 mCenterPosition{};
	Vector2 mSizeHalf{};
	Vector2 mScale{ 1.0f ,1.0f };

	unsigned int mColor = 0xffffffff;
};

