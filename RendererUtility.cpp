#include "RendererUtility.h"
#include <Novice.h>

using namespace RendererUtility;

Vector2 RendererUtility::MoveScroll(const Scroll &s)
{
	Vector2 target = *s.target;
	Vector2 diff{};
	diff.x = target.x - s.startPoint.x;
	diff.y = target.y - s.startPoint.y;

	if (diff.x <= kStage.min.x + kWinWidth / 2.0f) { diff.x = -kMaxStageWidth / 2.0f + kWinWidth / 2.0f; }
	if (diff.x >= kStage.max.x - kWinWidth / 2.0f) { diff.x = kMaxStageWidth / 2.0f - kWinWidth / 2.0f; }
	if (diff.y <= kStage.min.y + kWinHeight / 2.0f) { diff.y = -kMaxStageHeight / 2.0f + kWinHeight / 2.0f; }
	if (diff.y >= kStage.max.y - kWinHeight / 2.0f) { diff.y = kMaxStageHeight / 2.0f - kWinHeight / 2.0f; }
	return Vector2::Lerp(s.value, diff, 0.05f);
}

void RendererUtility::DrawSprite(const RenderData &s)
{
	Vector2 vertecies[4]{};
	vertecies[0] = { s.center.x - s.size.x, s.center.y + s.size.y };
	vertecies[1] = { s.center.x + s.size.x, s.center.y + s.size.y };
	vertecies[2] = { s.center.x - s.size.x, s.center.y - s.size.y };
	vertecies[3] = { s.center.x + s.size.x, s.center.y - s.size.y };

	for (Vector2 &v : vertecies)
	{
		v.x += kOrigin.x;
		v.y -= kOrigin.y;
		v.y *= -1.0f;
	}

	Novice::DrawQuad(
		static_cast<int>(vertecies[0].y),

		static_cast<int>(vertecies[1].x),
		static_cast<int>(vertecies[1].y),

		static_cast<int>(vertecies[2].x),
		static_cast<int>(vertecies[2].y),

		static_cast<int>(vertecies[3].x),
		static_cast<int>(vertecies[3].y),

		static_cast<int>(s.texLeftTop.x),
		static_cast<int>(s.texLeftTop.y),
		static_cast<int>(s.texFrameSize.x),
		static_cast<int>(s.texFrameSize.y),
		s.texHandle,
		s.color
	);

//#ifdef _DEBUG
//	Vector2 renderPos = s.center;
//	renderPos.x += kOrigin.x;
//	renderPos.y -= kOrigin.y;
//	renderPos.y *= -1.0f;
//	Novice::DrawEllipse(
//		static_cast<int>(renderPos.x),
//		static_cast<int>(renderPos.y),
//		static_cast<int>(s.size.x),
//		static_cast<int>(s.size.y),
//		s.angle,
//		(0xffffffff - s.color) | 0xff,
//		kFillModeWireFrame
//	);
//#endif 
}

void RendererUtility::DrawLine(const Line &l, unsigned int color)
{
	Vector2 start = l.start;
	start.x += kOrigin.x;
	start.y -= kOrigin.y;
	start.y *= -1.0f;

	Vector2 end = l.end;
	end.x += kOrigin.x;
	end.y -= kOrigin.y;
	end.y *= -1.0f;

	Novice::DrawLine(
		static_cast<int>(start.x),
		static_cast<int>(start.y),
		static_cast<int>(end.x),
		static_cast<int>(end.y),
		color
	);
}
