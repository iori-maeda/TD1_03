#pragma once
#include "GameObject.h"
#include "NoviceUtility.h"

class FishingLine
{
public:
	FishingLine() = default;
	FishingLine(GameObject *hook, GameObject *angler, float maxLength = 100.0f, unsigned int color = 0xffffffff)
		:mHook(hook), mAngler(angler), mMaxLength(maxLength), mColor(color)
	{};

	bool CheckLineBroken();
	void LineConnected();

public:
	unsigned int GetColor() const { return mColor; }
	NoviceUtility::Line GetLine() const { return NoviceUtility::Line(mAngler->GetPosition(), mHook->GetPosition()); }
	Vector2 GetToAnglerNorm() const { return Vector2::Normalize(mAngler->GetPosition() - mHook->GetPosition()); }

private:
	GameObject *mHook = nullptr;
	GameObject *mAngler = nullptr;
	const float mMaxLength = 0.0f;

	unsigned int mColor = 0xffffffff;
	bool mIsBroken = false;
};