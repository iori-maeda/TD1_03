#include "FishingLine.h"


bool FishingLine::CheckLineBroken()
{
	if (mIsBroken) { return true; }
	Vector2 diff = mAngler->GetPosition() - mHook->GetPosition();
	mIsBroken = diff.Length() >= mMaxLength;
	return mIsBroken;
}

void FishingLine::LineConnected()
{
	mIsBroken = false;
}