#include <Novice.h>
#include <numbers>
#include <cmath>
#include <algorithm>
#include <vector>
#include "Vector2.h"

struct Circle
{
	Vector2 center{};
	float radius = 1.0f;
	unsigned int color = WHITE;
	FillMode fillMode = kFillModeSolid;
};

struct Planet : public Circle
{
	float speed = 1.0f;
	Vector2 moveDir{};
	float gravity = 0.98f;
	float gravityRange = 100.0f;
};

struct Satellite : public Circle
{
	float speed = 1.0f;
	Vector2 velocity{};
};

struct Segment
{
	Vector2 start{};
	Vector2 end{};
};

void DrawCircle(const Circle &c)
{
	Novice::DrawEllipse(
		static_cast<int>(c.center.x),
		static_cast<int>(c.center.y),
		static_cast<int>(c.radius),
		static_cast<int>(c.radius),
		0.0f,
		c.color,
		c.fillMode
	);
}

bool isCollision(const Circle &c1, const Circle &c2)
{
	Vector2 diff = c1.center - c2.center;
	return diff.LengthSquared() <= (c1.radius + c2.radius) * (c1.radius + c2.radius);
}

bool isCollision(const Circle &c, const Segment &s)
{
	Vector2 toEnd = s.end - s.start;
	Vector2 toCenter = c.center - s.start;
	float toEndDiff = toEnd.LengthSquared();

	if (toEndDiff <= 0.0f) { return toCenter.LengthSquared() <= c.radius * c.radius; }

	float t = Vector2::Dot(toCenter, toEnd) / toEnd.LengthSquared();
	t = std::clamp(t, 0.0f, 1.0f);
	Vector2 closestPoint = s.start + toEnd * t;
	Vector2 diff = c.center - closestPoint;
	return diff.LengthSquared() <= c.radius * c.radius;
}

const char kWindowTitle[] = "TD1_3回目";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
	using namespace std;
	const float kWinWidth = 1280.0f;
	const float kWinHeight = 720.0f;
	const float kDeltaTime = 1.0f / 60.0f;

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, static_cast<int>(kWinWidth), static_cast<int>(kWinHeight));

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	enum PlanetName
	{
		None = -1,
		Earth = 0,
		Sun
	};

	std::vector<Planet> planets;

	Planet earth{};
	earth.center = Vector2(200.0f, 360.0f);
	earth.radius = 40.0f;
	earth.color = 0x00a0faff;
	earth.speed = 0.0f;
	earth.gravityRange = 300.0f;
	//planets.push_back(earth);

	Planet sun{};
	sun.center = Vector2(kWinWidth, kWinHeight) / 2.0f;
	sun.radius = 80.0f;
	sun.color = 0xffaa44ff;
	sun.speed = 0.0f;
	sun.gravityRange = 1000.0f;
	sun.gravity = 3.0f;
	planets.push_back(sun);

	Satellite hayabusa{};
	hayabusa.center = { 100.0f ,100.0f };
	hayabusa.radius = 15.0f;
	hayabusa.color = 0x000088ff;
	hayabusa.fillMode = kFillModeSolid;
	hayabusa.speed = 100.0f;
	hayabusa.velocity = Vector2::Normalize(Vector2(1.0f, 1.0f)) * hayabusa.speed;


	// 相対ベクトル作成
	Vector2 toPlanet{};
	Vector2 toPlanetNorm{};
	// 相対距離の更新
	float minLength{};
	float currentDist{};
	float customDist{};

	Vector2 tangent{};

	Satellite target = hayabusa;
	target.color = 0x000000ff;
	target.fillMode = kFillModeWireFrame;

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0)
	{
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		hayabusa.color = 0x000088ff;
		Vector2 totalAcceleration{};
		for (const Planet &planet : planets)
		{
			// 相対ベクトル作成
			toPlanet = planet.center - hayabusa.center;
			toPlanetNorm = Vector2::Normalize(toPlanet);
			// 相対距離の更新
			minLength = planet.radius + hayabusa.radius;
			currentDist = toPlanet.Length();
			customDist = powf(currentDist / 1500.0f, 2.0f);

			if (currentDist <= hayabusa.radius + planet.gravityRange + planet.radius)
			{
				// 色の変更
				hayabusa.color = RED;

				float currentGravity = planet.gravity / customDist * kDeltaTime;
				Vector2 gravityAcceleration = toPlanetNorm * currentGravity;

				tangent = Vector2(-toPlanet.y, toPlanet.x);
				if (Vector2::Cross(hayabusa.velocity, toPlanet) < 0.0f)
				{
					//tangent = Vector2(-tangent.y, tangent.x);
				}
				if (currentDist <= minLength)
				{
					hayabusa.center = planet.center + -toPlanetNorm * minLength;
				}

				Vector2 targetSurface = planet.center + Vector2::Normalize(tangent) * minLength;
				Vector2 targetDir = targetSurface - hayabusa.center;

				hayabusa.velocity = Vector2::Normalize(targetDir) * hayabusa.velocity.Length();
				if(isCollision(planet, Segment(hayabusa.center, hayabusa.center + hayabusa.velocity)))
				{
					hayabusa.velocity += Vector2::Normalize(tangent) * hayabusa.speed;
				}
				hayabusa.velocity += gravityAcceleration;

				target.center = planet.center + Vector2::Normalize(tangent) * minLength;
			}
		}

		hayabusa.center += hayabusa.velocity * kDeltaTime;

		if (hayabusa.center.x + hayabusa.radius <= 0.0f)
		{
			hayabusa.center.x = kWinWidth;
		}
		if (hayabusa.center.x - hayabusa.radius >= kWinWidth)
		{
			hayabusa.center.x = 0.0f;
		}

		if (hayabusa.center.y + hayabusa.radius <= 0.0f)
		{
			hayabusa.center.y = kWinHeight;
		}
		if (hayabusa.center.y - hayabusa.radius >= kWinHeight)
		{
			hayabusa.center.y = 0.0f;
		}


		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		for (const Planet &planet : planets)
		{
			DrawCircle(planet);
		}
		DrawCircle(hayabusa);
		DrawCircle(target);
		Vector2 drawDir = Vector2::Normalize(hayabusa.velocity);
		Novice::DrawLine(
			static_cast<int>(hayabusa.center.x),
			static_cast<int>(hayabusa.center.y),
			static_cast<int>(hayabusa.center.x + drawDir.x * 100.0f),
			static_cast<int>(hayabusa.center.y + drawDir.y * 100.0f),
			0x000000ff
		);
		Novice::DrawLine(
			static_cast<int>(hayabusa.center.x),
			static_cast<int>(hayabusa.center.y),
			static_cast<int>(hayabusa.center.x + toPlanet.x),
			static_cast<int>(hayabusa.center.y + toPlanet.y),
			0x000000ff
		);;

#ifdef _DEBUG
		Novice::ScreenPrintf(10, 10, "pos (x:%.2f, y:%.2f)", hayabusa.center.x, hayabusa.center.y);
		Novice::ScreenPrintf(10, 30, "velo(x:%.2f, y:%.2f)", hayabusa.velocity.x, hayabusa.velocity.y);
		Novice::ScreenPrintf(10, 50, "spd %.2f", hayabusa.velocity.Length());
#endif
		///
		/// ↑描画処理ここまで
		///

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
