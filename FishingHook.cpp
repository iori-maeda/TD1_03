#include "FishingHook.h"
#include "NoviceUtility.h"
#include "Player.h"

using namespace NoviceUtility;

void FishingHook::OnCollision(const GameObject &obj)
{
	if (!obj.CompareTag(ObjectTag::Player)) { return; }

	Vector2 toAnglerNorm = Vector2::Normalize(obj.GetPosition() - mCenterPosition);
	const Player &player = static_cast<const Player &>(obj);

	Vector2 offset = -toAnglerNorm * (mCollider.radius + player.GetCollider().radius);
	mCenterPosition = obj.GetPosition() + offset;
	mVelocity = {};
}

void FishingHook::Update()
{
	mCenterPosition += mVelocity;
	mVelocity *= 0.999f;

	mCollider.center = mCenterPosition;
}

void FishingHook::MoveUpdate()
{
	Vector2 toTarget = mTarget->GetPosition() - mCenterPosition;
	Vector2 toTargetNorm = Vector2::Normalize(toTarget);

	mVelocity += toTargetNorm * mSpeed * kDeltaTime;
}

void FishingHook::CheckStageReflect()
{
	if (mCenterPosition.y + mCollider.radius > kStage.max.y)
	{
		mCenterPosition.y = kStage.max.y - mCollider.radius;
		mVelocity.y *= -1.0f;
	}
	if (mCenterPosition.y - mCollider.radius < kStage.min.y)
	{
		mCenterPosition.y = kStage.min.y + mCollider.radius;
		mVelocity.y *= -1.0f;
	}

	if (mCenterPosition.x + mCollider.radius > kStage.max.x)
	{
		mCenterPosition.x = kStage.max.x - mCollider.radius;
		mVelocity.x *= -1.0f;
	}
	if (mCenterPosition.x - mCollider.radius < kStage.min.x)
	{
		mCenterPosition.x = kStage.min.x + mCollider.radius;
		mVelocity.x *= -1.0f;
	}
}

void FishingHook::AddVelocity(const Vector2 &velocity)
{
	mVelocity += velocity;
}