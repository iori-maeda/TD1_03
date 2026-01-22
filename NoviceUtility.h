#pragma once
#include "GameObject.h"
#include "ColliderCollection.h"

namespace NoviceUtility
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
		Vector2 target{};
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
	const Vector2 kDrawMargin = Vector2(10.0f, 10.0f);
#pragma endregion

	/// <summary>
	/// 
	/// </summary>
	/// <param name="obj">描画オブジェクトの情報</param>
	/// <param name="graphHandle">画像ハンドル</param>
	/// <param name="color">色</param>
	/// <param name="scale">倍率</param>
	/// <param name="uvPosition">画像切り取り開始左上頂点</param>
	/// <param name="drawArea">画像切り取り範囲</param>
	void DrawSprite(
		const GameObject &obj,
		int graphHandle,
		const Vector2 &uvPosition = Vector2(),
		const Vector2 &drawArea = Vector2(1.0f, 1.0f),
		const Vector2 &scale = Vector2(1.0f, 1.0f)
	);

	void DrawDebugObjectInformation(const Vector2 &position, const GameObject &info);

	Vector2 MoveScroll(const Scroll &s);

	void DrawSprite(const RenderData &s);

	void DrawLine(const Line &l, unsigned int color = 0xffffffff);
};