#include <Novice.h>
#include <numbers>
#include <cmath>
#include <algorithm>
#include <vector>
#include "Vector2.h"

struct CircleCollider
{
	Vector2 center{};
	float radius = 1.0f;
};

struct BoxCollider
{
	Vector2 min{};
	Vector2 max{};
};

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

struct Angler : CircleCollider
{
	Vector2 velocity{};
	RenderData renderData{};
	float speed = 10.0f;
};

struct FishingHook : Angler
{
	bool isLineBroken = false;
	float maxLineLength = 0.0f;
};

struct Fish :Angler
{
	bool isActive = false;
	float lifeTime = 0.0f;
};

struct Line
{
	Vector2 start{};
	Vector2 end{};
};

struct FishingLine : Line
{
	float duravility = 100.0f;
	unsigned int color = 0xffffffff;
};

struct Scroll
{
	Vector2* target = nullptr;
	Vector2 startMin{};
	Vector2 startMax{};
	Vector2 value{};
};


#pragma region GrobalVariavle 
const char kWindowTitle[] = "TD1_3回目";

using namespace std;
const float kWinWidth = 1280.0f;
const float kWinHeight = 720.0f;
const float kMaxStageWidth = kWinWidth /** 3.0f*/;
const float kMaxStageHeight = kWinHeight /** 3.0f*/;
const float kDeltaTime = 1.0f / 60.0f;
const Vector2 kOrigin{ 640.0f, 360.0f };
const BoxCollider kStage{
	.min = Vector2(-kMaxStageWidth / 2.0, -kMaxStageHeight / 2.0f),
	.max = Vector2(kMaxStageWidth / 2.0, kMaxStageHeight / 2.0f),
};
#pragma endregion

Vector2 MoveScroll(const Scroll& s)
{
	Vector2 target = *s.target;
	Vector2 diff{};
	if (target.x <= s.startMin.x)
	{
		diff.x = target.x - s.startMin.x;
	}
	if (target.y <= s.startMin.y)
	{
		diff.y = target.y - s.startMin.y;
	}
	if (target.x >= s.startMax.x)
	{
		diff.x = target.x - s.startMax.x;
	}
	if (target.y >= s.startMax.y)
	{
		diff.y = target.y - s.startMax.y;
	}
	return diff;
}

bool IsCollision(const CircleCollider& c1, const CircleCollider& c2)
{
	Vector2 diff = c1.center - c2.center;
	float sumR = c1.radius + c2.radius;

	return diff.LengthSquared() <= sumR * sumR;
}

bool IsCollision(const BoxCollider& b, const Vector2& p)
{
	bool isHitX = p.x >= b.min.x && p.x <= b.max.x;
	bool isHitY = p.y >= b.min.y && p.y <= b.max.y;
	return isHitX && isHitY;
}

void DrawSprite(const RenderData& s)
{
	Vector2 vertecies[4]{};
	vertecies[0] = { s.center.x - s.size.x, s.center.y + s.size.y };
	vertecies[1] = { s.center.x + s.size.x, s.center.y + s.size.y };
	vertecies[2] = { s.center.x - s.size.x, s.center.y - s.size.y };
	vertecies[3] = { s.center.x + s.size.x, s.center.y - s.size.y };

	for (Vector2& v : vertecies)
	{
		v.x += kOrigin.x;
		v.y -= kOrigin.y;
		v.y *= -1.0f;
	}

	Novice::DrawQuad(
		static_cast<int>(vertecies[0].x),
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

#ifdef _DEBUG
	Vector2 renderPos = s.center;
	renderPos.x += kOrigin.x;
	renderPos.y -= kOrigin.y;
	renderPos.y *= -1.0f;
	Novice::DrawEllipse(
		static_cast<int>(renderPos.x),
		static_cast<int>(renderPos.y),
		static_cast<int>(s.size.x),
		static_cast<int>(s.size.y),
		s.angle,
		(0xffffffff - s.color) | 0xff,
		kFillModeWireFrame
	);
#endif 
}

void DrawLine(const Line& l, unsigned int color = 0xffffffff)
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

unsigned int LerpColor(unsigned int startColor, unsigned int endColor, float t)
{
	unsigned char startR = (startColor >> 24) & 0xff;
	unsigned char startG = (startColor >> 16) & 0xff;
	unsigned char startB = (startColor >> 8) & 0xff;
	unsigned char startA = (startColor >> 0) & 0xff;
	unsigned char endR = (endColor >> 24) & 0xff;
	unsigned char endG = (endColor >> 16) & 0xff;
	unsigned char endB = (endColor >> 8) & 0xff;
	unsigned char endA = (endColor >> 0) & 0xff;
	unsigned char lerpR = static_cast<unsigned char>(startR + (endR - startR) * t);
	unsigned char lerpG = static_cast<unsigned char>(startG + (endG - startG) * t);
	unsigned char lerpB = static_cast<unsigned char>(startB + (endB - startB) * t);
	unsigned char lerpA = static_cast<unsigned char>(startA + (endA - startA) * t);
	return (lerpR << 24) | (lerpG << 16) | (lerpB << 8) | (lerpA << 0);
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, static_cast<int>(kWinWidth), static_cast<int>(kWinHeight));

	const unsigned int kDefaultTex = Novice::LoadTexture("white1x1.png");

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	Angler angler{};
	angler.radius = 16.0f;
	angler.renderData.center = angler.center;
	angler.renderData.size = Vector2(angler.radius, angler.radius);
	angler.renderData.texHandle = kDefaultTex;
	angler.speed = 200.0f;

	FishingHook fishingHook{};

	fishingHook.radius = 8.0f;
	fishingHook.center = angler.center + Vector2(1.0f, 0.0f) * fishingHook.radius;
	fishingHook.renderData.center = fishingHook.center;
	fishingHook.renderData.size = Vector2(fishingHook.radius, fishingHook.radius);
	fishingHook.renderData.texHandle = kDefaultTex;
	fishingHook.renderData.color = 0x008888ff;
	fishingHook.maxLineLength = kWinHeight / 2.0f;

	FishingLine fishingLine{};
	fishingLine.color = WHITE;

	Fish fish{};
	fish.center = Vector2(kWinWidth, kWinHeight);
	fish.radius = 16.0f;
	fish.renderData.center = fish.center;
	fish.renderData.size = Vector2(fish.radius, fish.radius);
	fish.renderData.texHandle = kDefaultTex;
	fish.speed = 200.0f;
	fish.renderData.color = 0x0066aaff;

	Scroll scroll{};
	scroll.target = &angler.center;
	scroll.startMin = Vector2(-30.0f, -30.0f);
	scroll.startMax = Vector2(+30.0f, +30.0f);

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0)
	{
		// フレームの開始
		Novice::BeginFrame();
		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		/// Player Move
		{
			Vector2 inputDir{};
			if (keys[DIK_W]) { inputDir.y += 1.0f; }
			if (keys[DIK_S]) { inputDir.y -= 1.0f; }
			if (keys[DIK_A]) { inputDir.x -= 1.0f; }
			if (keys[DIK_D]) { inputDir.x += 1.0f; }

			angler.velocity = Vector2::Normalize(inputDir) * angler.speed * kDeltaTime;
			angler.center += angler.velocity;

			angler.center.x = std::clamp(angler.center.x, kStage.min.x + angler.radius, kStage.max.x - angler.radius);
			angler.center.y = std::clamp(angler.center.y, kStage.min.y + angler.radius, kStage.max.y - angler.radius);
		}

		/// Hook Move
		{
			Vector2 toAngler = angler.center - fishingHook.center;
			Vector2 toAnglerNormal = Vector2::Normalize(toAngler);

			// ラインが切れていないとき
			if (!fishingHook.isLineBroken)
			{
				// push space key
				if (keys[DIK_SPACE])
				{
					// if hook dir not pull dir
					if (Vector2::Dot(Vector2::Normalize(fishingHook.velocity), -toAnglerNormal) < 0.0f)
					{
						fishingHook.velocity += toAnglerNormal * fishingHook.speed * 2.0f * kDeltaTime;
					}
				}
				if (toAngler.Length() >= fishingHook.maxLineLength)
				{
					fishingHook.isLineBroken = true;
				}
				fishingHook.velocity += toAnglerNormal * fishingHook.speed * kDeltaTime;
			}
			if (IsCollision(angler, fishingHook))
			{
				fishingHook.center = angler.center + -toAnglerNormal * (angler.radius + fishingHook.radius);
				fishingHook.velocity = {};
				fishingHook.isLineBroken = false;
			}
			fishingHook.center += fishingHook.velocity;

			float ratio = std::clamp(toAngler.Length() / fishingHook.maxLineLength, 0.0f, 1.0f);
			fishingLine.color = LerpColor(WHITE, RED, ratio);

			if (fishingHook.center.y + fishingHook.radius > kStage.max.y)
			{
				fishingHook.center.y = kStage.max.y - fishingHook.radius;
				fishingHook.velocity.y *= -1.0f;
			}
			if (fishingHook.center.y - fishingHook.radius < kStage.min.y)
			{
				fishingHook.center.y = kStage.min.y + fishingHook.radius;
				fishingHook.velocity.y *= -1.0f;
			}

			if (fishingHook.center.x + fishingHook.radius > kStage.max.x)
			{
				fishingHook.center.x = kStage.max.x - fishingHook.radius;
				fishingHook.velocity.x *= -1.0f;
			}
			if (fishingHook.center.x - fishingHook.radius < kStage.min.x)
			{
				fishingHook.center.x = kStage.min.x + fishingHook.radius;
				fishingHook.velocity.x *= -1.0f;
			}

			fishingHook.velocity *= 0.999f;
		}

		// Fish Move
		{
			if (IsCollision(fish, fishingHook))
			{
				Sleep(100);
			}
		}

		/// Scroll Update
		{
			scroll.value = MoveScroll(scroll);
			angler.renderData.center = angler.center - scroll.value;
			fishingHook.renderData.center = fishingHook.center - scroll.value;
			fish.renderData.center = fish.center - scroll.value;

			// scroll clamp map min
			scroll.value.x = std::clamp(
				scroll.value.x,
				kStage.min.x,
				kStage.max.x
			);
		}


		/// Draw
		{
			fishingLine.start = angler.center - scroll.value;
			fishingLine.end = fishingHook.center - scroll.value;
			{
				if (fishingHook.isLineBroken)
				{
					DrawLine(fishingLine, BLACK);
				}
				else
				{
					DrawLine(fishingLine, fishingLine.color);
				}
			}
			DrawSprite(angler.renderData);
			DrawSprite(fishingHook.renderData);
			DrawSprite(fish.renderData);
		}

#ifdef _DEBUG
		Novice::ScreenPrintf(10, 10, "velocity(x:%.2f, y:%.2f) length(%.2f)", angler.velocity.x, angler.velocity.y, angler.velocity.Length());
		Novice::ScreenPrintf(10, 30, "velocity(x:%.2f, y:%.2f) length(%.2f)", fishingHook.velocity.x, fishingHook.velocity.y, fishingHook.velocity.Length());
		Novice::ScreenPrintf(10, 50, "scrollVa(x:%.2f, y:%.2f)", scroll.value);

		DrawLine(Line(scroll.startMin, scroll.startMax), BLACK);

		for (int i = -10; i < 10; i++)
		{
			float width = kWinWidth / 10.0f * i;
			Line line = { Vector2(width, kStage.min.y), Vector2(width, kStage.max.y) };
			line.start -= scroll.value;
			line.end -= scroll.value;
			DrawLine(line, 0x008800ff);
		}

		for (int i = -10; i <= 10; i++)
		{
			float height = kWinHeight / 10.0f * i;
			Line line = { Vector2(kStage.min.x,height), Vector2(kStage.max.x, height) };
			line.start -= scroll.value;
			line.end -= scroll.value;
			DrawLine(line, 0x880000ff);
		}
#endif

		// フレームの終了
		Novice::EndFrame();
		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0)
		{
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
