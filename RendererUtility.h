#pragma once
#include "Vector2.h"
#include "ColliderCollection.h"

namespace RendererUtility
{
	struct RenderData
	{
		Vector2 center{};
		int texHandle = -1;
		Vector2 size = { 1.0f ,1.0f };
		float angle = 0.0f;

		Vector2 texLeftTop{};
		Vector2 texFrameSize{ 1.0f, 1.0f };
		unsigned int color = 0xffffffff;
	};


	struct Scroll
	{
		Vector2* target = nullptr;
		Vector2 value{};
		Vector2 startPoint{};
	};

	struct Line
	{
		Vector2 start{};
		Vector2 end{};
	};

#pragma region GrobalVariavle 

	using namespace std;
	const float kWinWidth = 1280.0f;
	const float kWinHeight = 720.0f;
	const float kMaxStageWidth = kWinWidth;// * 3.0f;
	const float kMaxStageHeight = kWinHeight;// * 3.0f;
	const float kDeltaTime = 1.0f / 60.0f;
	const Vector2 kOrigin{ 640.0f, 360.0f };
	const ColliderCollection::BoxCollider kStage{
		.min = Vector2(-kMaxStageWidth / 2.0, -kMaxStageHeight / 2.0f),
		.max = Vector2(kMaxStageWidth / 2.0, kMaxStageHeight / 2.0f),
	};
#pragma endregion

	Vector2 MoveScroll(const Scroll& s);

	void DrawSprite(const RenderData& s);

	void DrawLine(const Line& l, unsigned int color = 0xffffffff);
};

