#include "Fish.h"
#include "NoviceUtility.h"
#include "Random.h"
#include "FishingHook.h"
#include <Windows.h>

using namespace NoviceUtility;

void Fish::OnCollision(const GameObject &obj)
{
	if (mIsFishing) { return; }
	if (mState == State::Escape) { return; }
	if (!obj.CompareTag(ObjectTag::FishingHook)) { return; }

	const FishingHook &hook = static_cast<const FishingHook &>(obj);
	float hookPower = hook.GetVelocity().Length();
	mLifePower -= hookPower;
	if (mLifePower <= 0.0f)
	{
		//mState = State::Escape;
		mLifePower = 0.0f;
		mIsFishing = true;
		mScale *= 2.0f;
		Sleep(100);
		return;
	}
}

void Fish::Update()
{
	//Excursion();

	StateUpdate();

	mCollider.center = mCenterPosition;

	mScale = Vector2::Lerp(mScale, Vector2(1.0f, 1.0f), 0.1f);
}

void Fish::Spawn(const FishConfig &config)
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
	Move();

	GameObject::ClampGameArea();
}

void Fish::GoArea()
{
	Vector2 toStart = mStartPosition - mCenterPosition;
	mMoveDir = Vector2::Normalize(toStart);
	mVelocity = mMoveDir * mSpeed * 2.0f * kDeltaTime;

	Move();
	if (toStart.Length() < 5.0f)
	{
		mState = Fish::State::Excursion;
		return;
	}
}

void Fish::Escape()
{
	mState = Fish::State::Escape;
	mIsFishing = false;
	Vector2 toAway = Vector2::Normalize(mCenterPosition - mTraget->GetPosition());
	mMoveDir = toAway;
	mVelocity = mMoveDir * mSpeed * 3.0f * kDeltaTime;
	Move();
	if (mCenterPosition.Length() > kStage.max.x + 100.0f)
	{
		mIsActive = false;
		return;
	}
}

void Fish::StateUpdate()
{
	switch (mState)
	{
	case Fish::State::Spawn:
		break;
	case Fish::State::GoArea:
		GoArea();
		break;
	case Fish::State::Excursion:
		Excursion();
		break;
	case Fish::State::Escape:
		Escape();
		break;
	default:
		break;
	}
}

void Fish::TailWave()
{
	mTailWaveValue += mTailWaveSpeed * kDeltaTime;
	//mTailWaveValue = std::clamp(mTailWaveValue, 0.0f, 360.0f);
}

void Fish::Move()
{
	mCenterPosition += mVelocity;
	TailWave();
}

float Fish::TailWaveAngle()
{
	return sinf(mTailWaveValue) / 5.0f;
}
