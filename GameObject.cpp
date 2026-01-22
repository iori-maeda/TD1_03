#include "GameObject.h"
#include "NoviceUtility.h"

#include <algorithm>

using namespace NoviceUtility;

void GameObject::ClampGameArea()
{
	Vector2 clampPosition{
			std::clamp(mCenterPosition.x, kStage.min.x + mSizeHalf.x + kDrawMargin.x, kStage.max.x - mSizeHalf.x - kDrawMargin.x),
			std::clamp(mCenterPosition.y, kStage.min.y + mSizeHalf.y + kDrawMargin.y, kStage.max.y - mSizeHalf.y - kDrawMargin.y)
	};
	mCenterPosition = clampPosition;
}