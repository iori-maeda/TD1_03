#include "Fish.h"
#include "NoviceUtility.h"
#include "FishingHook.h"

using namespace NoviceUtility;

void Fish::OnCollision(const GameObject &obj)
{
	if(!obj.CompareTag(ObjectTag::FishingHook)){return;}

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

void Fish::Spawn(const FishConfig &config)
{
	// uniqur_ptrがメンバに追加されたら変更が必要
	*this = Fish(config);
}

void Fish::Excursion()
{
	Vector2 toMe = mCenterPosition - mTraget->GetPosition();
	mMoveDir = Vector2::Normalize(Vector2(-toMe.y, toMe.x));
	mVelocity = mMoveDir * mSpeed * kDeltaTime;

	mCenterPosition += mVelocity;
}