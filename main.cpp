#include <Novice.h>
#include <numbers>
#include <cmath>
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
	earth.speed = 1000.0f;
	earth.gravityRange = 500.0f;
	//planets.push_back(earth);

	Planet sun{};
	sun.center = Vector2(kWinWidth, kWinHeight) / 2.0f;
	sun.radius = 80.0f;
	sun.color = 0xffaa44ff;
	sun.speed = 0.0f;
	sun.gravityRange = 500.0f;
	sun.gravity = 10.0f;
	planets.push_back(sun);

	Satellite hayabusa{};
	hayabusa.center = { 100.0f ,100.0f };
	hayabusa.radius = 15.0f;
	hayabusa.color = 0x000088ff;
	hayabusa.fillMode = kFillModeSolid;
	hayabusa.velocity = Vector2::Normalize(Vector2(1.0f, 1.0f)) * 100.0f;
	hayabusa.speed = 100.0f;

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

		// 入力による移動
		/*earth.moveDir = {};
		if (keys[DIK_W]) { earth.moveDir.y += -1.0f; }
		if (keys[DIK_S]) { earth.moveDir.y += 1.0f; }
		if (keys[DIK_A]) { earth.moveDir.x += -1.0f; }
		if (keys[DIK_D]) { earth.moveDir.x += 1.0f; }
		earth.center += Vector2::Normalize(earth.moveDir) * earth.speed * kDeltaTime;*/

		hayabusa.color = 0x000088ff;
		Vector2 totalAcceleration{};
		for (const Planet &planet : planets)
		{
			// 相対ベクトル作成
			Vector2 toPlanet = planet.center - hayabusa.center;
			Vector2 toPlanetNorm = Vector2::Normalize(toPlanet);
			// 相対距離の更新
			float minLength = planet.radius + hayabusa.radius;
			float currentDist = fmaxf(toPlanet.Length(), minLength) - planet.radius;

			if (currentDist <= hayabusa.radius + planet.gravityRange + planet.radius)
			{
				// 色の変更
				hayabusa.color = RED;

				float currentGravity = planet.gravity / (currentDist * currentDist);
				Vector2 gravityAcceleration = toPlanetNorm * currentGravity;
				/*if(Vector2::Dot(gravityAcceleration, toPlanet) >= 0.9f)
				{
					gravityAcceleration = planet.gravity * toPlanetNorm;
				}*/

				hayabusa.velocity += gravityAcceleration;// * kDeltaTime;
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
		Vector2 drawDir = Vector2::Normalize(hayabusa.velocity);
		Novice::DrawLine(
			static_cast<int>(hayabusa.center.x),
			static_cast<int>(hayabusa.center.y),
			static_cast<int>(hayabusa.center.x + drawDir.x * 100.0f),
			static_cast<int>(hayabusa.center.y + drawDir.y * 100.0f),
			0x000000ff
		);

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
