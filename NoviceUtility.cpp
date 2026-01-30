#include "NoviceUtility.h"
#include <Novice.h>
#include <cmath>

namespace NoviceUtility
{
	void DrawSprite(
		const GameObject &obj,
		int graphHandle,
		const Vector2 &uvPosition,
		const Vector2 &drawArea
	)
	{
		Vector2 drawSize = { obj.GetSizeHalf().x * obj.GetScale().x, obj.GetSizeHalf().y * obj.GetScale().y };
		Novice::DrawQuad(
			static_cast<int>(obj.GetPosition().x - drawSize.x),
			static_cast<int>(obj.GetPosition().y - drawSize.y),
			static_cast<int>(obj.GetPosition().x + drawSize.x),
			static_cast<int>(obj.GetPosition().y - drawSize.y),
			static_cast<int>(obj.GetPosition().x - drawSize.x),
			static_cast<int>(obj.GetPosition().y + drawSize.y),
			static_cast<int>(obj.GetPosition().x + drawSize.x),
			static_cast<int>(obj.GetPosition().y + drawSize.y),
			static_cast<int>(uvPosition.x),
			static_cast<int>(uvPosition.y),
			static_cast<int>(drawArea.x),
			static_cast<int>(drawArea.y),
			graphHandle,
			obj.GetColor()
		);
	}
	void DrawDebugObjectInformation(const Vector2 &position, const GameObject &info)
	{
		Novice::ScreenPrintf(
			static_cast<int>(position.x),
			static_cast<int>(position.y),
			"Tag:%d Pos:(%.1f,%.1f) Size:(%.1f,%.1f) Active:%d",
			static_cast<int>(info.GetTag()),
			info.GetPosition().x, info.GetPosition().y,
			info.GetSizeHalf().x, info.GetSizeHalf().y,
			static_cast<int>(info.IsActive())
		);
	}


	Vector2 MoveScroll(const Scroll &s)
	{
		Vector2 diff{};
		diff.x = s.target.x - s.startPoint.x;
		diff.y = s.target.y - s.startPoint.y;

		if (diff.x <= kStage.min.x + kWinWidth / 2.0f) { diff.x = -kMaxStageWidth / 2.0f + kWinWidth / 2.0f; }
		if (diff.x >= kStage.max.x - kWinWidth / 2.0f) { diff.x = kMaxStageWidth / 2.0f - kWinWidth / 2.0f; }
		if (diff.y <= kStage.min.y + kWinHeight / 2.0f) { diff.y = -kMaxStageHeight / 2.0f + kWinHeight / 2.0f; }
		if (diff.y >= kStage.max.y - kWinHeight / 2.0f) { diff.y = kMaxStageHeight / 2.0f - kWinHeight / 2.0f; }
		return Vector2::Lerp(s.value, diff, 0.05f);
	}

	void DrawSprite(const RenderData &s)
	{
		Vector2 localVertecies[4]{};
		localVertecies[0] = { -s.size.x,  +s.size.y };
		localVertecies[1] = { +s.size.x,  +s.size.y };
		localVertecies[2] = { -s.size.x,  -s.size.y };
		localVertecies[3] = { +s.size.x,  -s.size.y };

		Vector2 rotatedVertecies[4]{};
		rotatedVertecies[0] = {
			localVertecies[0].x * cosf(s.angle) - localVertecies[0].y * sinf(s.angle) + s.center.x,
			localVertecies[0].x * sinf(s.angle) + localVertecies[0].y * cosf(s.angle) + s.center.y
		};
		rotatedVertecies[1] = {
			localVertecies[1].x * cosf(s.angle) - localVertecies[1].y * sinf(s.angle) + s.center.x,
			localVertecies[1].x * sinf(s.angle) + localVertecies[1].y * cosf(s.angle) + s.center.y
		};
		rotatedVertecies[2] = {
			localVertecies[2].x * cosf(s.angle) - localVertecies[2].y * sinf(s.angle) + s.center.x,
			localVertecies[2].x * sinf(s.angle) + localVertecies[2].y * cosf(s.angle) + s.center.y
		};
		rotatedVertecies[3] = {
			localVertecies[3].x * cosf(s.angle) - localVertecies[3].y * sinf(s.angle) + s.center.x,
			localVertecies[3].x * sinf(s.angle) + localVertecies[3].y * cosf(s.angle) + s.center.y
		};


		for (Vector2 &v : rotatedVertecies)
		{
			v.x += kOrigin.x;
			v.y -= kOrigin.y;
			v.y *= -1.0f;
		}

		Novice::DrawQuad(
			static_cast<int>(rotatedVertecies[0].x),
			static_cast<int>(rotatedVertecies[0].y),

			static_cast<int>(rotatedVertecies[1].x),
			static_cast<int>(rotatedVertecies[1].y),

			static_cast<int>(rotatedVertecies[2].x),
			static_cast<int>(rotatedVertecies[2].y),

			static_cast<int>(rotatedVertecies[3].x),
			static_cast<int>(rotatedVertecies[3].y),

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

	void DrawLine(const Line &l, unsigned int color)
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

}