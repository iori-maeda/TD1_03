#include <Novice.h>
#include <numbers>
#include <cmath>
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

	Planet Planets[2]{};
	Planets[Earth].center = Vector2(200.0f, 360.0f);
	Planets[Earth].radius = 40.0f;
	Planets[Earth].color = 0x00a0faff;
	Planets[Earth].speed = 1000.0f;
	Planets[Earth].gravityRange = 1000.0f;

	Planets[Sun].center = Vector2(kWinWidth, kWinHeight) / 2.0f;
	Planets[Sun].radius = 80.0f;
	Planets[Sun].color = 0xffaa44ff;
	Planets[Sun].speed = 0.0f;
	Planets[Sun].gravityRange = 2000.0f;

	Satellite hayabusa{};
	hayabusa.center = { 100.0f ,100.0f };
	hayabusa.radius = 15.0f;
	hayabusa.color = 0x000088ff;
	hayabusa.fillMode = kFillModeSolid;
	hayabusa.velocity = Vector2::Normalize(Vector2(1.0f, 1.0f)) * 10.0f;
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
		for (const Planet &planet : Planets)
		{
			// 相対ベクトル作成
			Vector2 toPlanet = planet.center - hayabusa.center;
			// 相対距離の更新
			float minLength = planet.radius + hayabusa.radius;
			float currentLength = fmaxf(toPlanet.Length(), minLength);

			if (currentLength <= hayabusa.radius + planet.gravityRange)
			{
				float dot = Vector2::Dot(hayabusa.velocity, toPlanet);
				if (dot <= 0.0f) { continue; }
				// 距離の割合
				float lengthRatio = planet.gravityRange / fmaxf(currentLength, minLength);
				// 計算に反映する割合
				float currentRatio = lengthRatio * lengthRatio;
				// 引力計算
				float currentGravity = planet.gravity * currentRatio;
				Vector2 gravityDir = Vector2::Normalize(toPlanet);
				totalAcceleration += gravityDir * currentGravity;

				//if (toPlanet.Length() <= planet.radius + hayabusa.radius)
				//{
				//	// 現在の速度
				//	float currentSpeed = hayabusa.velocity.Length();
				//	currentSpeed = currentSpeed <= hayabusa.speed ? hayabusa.speed : currentSpeed;
				//	float cross = Vector2::Cross(hayabusa.velocity, toPlanet);
				//	Vector2 tangent = Vector2::Normalize(Vector2(-toPlanet.y, toPlanet.x));
				//	if (cross > 0.0f)
				//	{
				//		tangent = tangent;
				//	}
				//	else
				//	{
				//		tangent = tangent;
				//	}
				//	totalAcceleration += tangent * currentSpeed;
				//	hayabusa.center = planet.center + Vector2::Normalize(-toPlanet) * (planet.radius + hayabusa.radius);
				//}

				hayabusa.color = RED;
			}
		}

		hayabusa.velocity += totalAcceleration * kDeltaTime;
		hayabusa.center += hayabusa.velocity * kDeltaTime;

		float currentSpeed = hayabusa.velocity.Length();
		currentSpeed--;
		if (currentSpeed <= hayabusa.speed)
		{
			currentSpeed = hayabusa.speed;
		}
		hayabusa.velocity = Vector2::Normalize(hayabusa.velocity) * currentSpeed;

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
		for (const Planet &planet : Planets)
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
