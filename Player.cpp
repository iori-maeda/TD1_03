#include "Player.h"
#include <algorithm>
#include <memory>
#include <numbers>
#include <cmath>
#include "NoviceUtility.h"

using namespace NoviceUtility;

void Player::OnCollision(const GameObject &obj)
{
	obj;
}

void Player::Update()
{
	if (!mIsActive) { return; }
	if (Vector2::Normalize(mMoveDir).Length() <= 0.0f) { return; }

	mVelocity = Vector2::Normalize(mMoveDir) * mSpeed * kDeltaTime;
	mCenterPosition += mVelocity;
	mMoveDir = {};

	GameObject::ClampGameArea();

	mCollider.center = mCenterPosition;
}

void Player::MoveLeft()
{
	mMoveDir.x -= 1.0f;
}

void Player::MoveRight()
{
	mMoveDir.x += 1.0f;
}

void Player::MoveUp()
{
	mMoveDir.y += 1.0f;
}

void Player::MoveDown()
{
	mMoveDir.y -= 1.0f;
}