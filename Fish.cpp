#include "Fish.h"
#include "NoviceUtility.h"
#include "Random.h"

using namespace NoviceUtility;

void Fish::OnCollision(const GameObject& obj)
{
	if (!obj.CompareTag(ObjectTag::FishingHook)) { return; }

	//const FishingHook& hook = static_cast<const FishingHook&>(obj);
	//float hookPower = hook.GetVelocity().Length();
	mIsFishing = true;
}

void Fish::Update()
{
	Excursion();

	GameObject::ClampGameArea();

	mCollider.center = mCenterPosition;
}

void Fish::Spawn(const FishConfig& config)
{
	// uniqur_ptrがメンバに追加されたら変更が必要
	// 定数もダメ
	*this = Fish(config);

	mStartPosition.x = Random::GetRandom(kStage.min.x + kDrawMargin.x, kStage.max.x - +kDrawMargin.x);
	mStartPosition.y = Random::GetRandom(kStage.min.y + kDrawMargin.y, kStage.max.y - +kDrawMargin.y);

	mState = Fish::State::GoArea;
}

void Fish::Excursion()
{
	Vector2 toMe = mCenterPosition - mTraget->GetPosition();
	mMoveDir = Vector2::Normalize(Vector2(-toMe.y, toMe.x));
	mVelocity = mMoveDir * mSpeed * kDeltaTime;

	mCenterPosition += mVelocity;
}

void Fish::GoArea()
{
	Vector2 toStart = mStartPosition - mCenterPosition;

	mCenterPosition += Vector2::Normalize(toStart) * mSpeed * 2.0f * kDeltaTime;

	if (toStart.Length() < 5.0f)
	{
		mState = Fish::State::Excursion;
		return;
	}
}

void Fish::Escape()
{
	mIsActive = false;
}

void Fish::StateUpdate()
{
	switch (mState)
	{
	case Fish::State::Spawn:
		break;
	case Fish::State::Excursion:
		break;
	case Fish::State::Escape:
		break;
	default:
		break;
	}
}
