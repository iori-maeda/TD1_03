#include <Novice.h>
#include <numbers>
#include <cmath>
#include "Random.h"
#include "Vector2.h"

const char kWindowTitle[] = "no title";
const float kWinWidth = 1280.0f;
const float kWinHeight = 720.0f;

struct Triangle
{
	Vector2 forward{};
	Vector2 backLeft{};
	Vector2 backRight{};
};

Triangle ConvertTriangle(const Vector2 &center, float span, float width, float angle = 0.0f)
{
	Triangle local{
		.forward = {+span / 2.0f, 0.0f},
		.backLeft = {-span / 2.0f, +width / 2.0f},
		.backRight = {-span / 2.0f, -width / 2.0f}
	};

	Triangle result{
		.forward = {
			local.forward.x * cosf(angle) - local.forward.y * sinf(angle) + center.x,
			local.forward.x * sinf(angle) + local.forward.y * cosf(angle) + center.y
		},

		.backLeft = {
			local.backLeft.x * cosf(angle) - local.backLeft.y * sinf(angle) + center.x,
			local.backLeft.x * sinf(angle) + local.backLeft.y * cosf(angle) + center.y
		},

		.backRight = {
			local.backRight.x * cosf(angle) - local.backRight.y * sinf(angle) + center.x,
			local.backRight.x * sinf(angle) + local.backRight.y * cosf(angle) + center.y
		}
	};

	return result;
}

void DrawTriangle(const Triangle &triangle, unsigned int color = 0xffffffff, bool isSolid = true)
{
	Novice::DrawTriangle(
		static_cast<int>(triangle.forward.x),
		static_cast<int>(triangle.forward.y),
		static_cast<int>(triangle.backLeft.x),
		static_cast<int>(triangle.backLeft.y),
		static_cast<int>(triangle.backRight.x),
		static_cast<int>(triangle.backRight.y),
		color, isSolid ? kFillModeSolid : kFillModeWireFrame
	);
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, static_cast<int>(kWinWidth), static_cast<int>(kWinHeight));
	// 初期化(メルセンヌツイスター)
	Random::Initialize();

	struct Fish
	{
		Vector2 position{};
		Vector2 dir{};
		Vector2 velocity{};
		Vector2 size{ 32.0f, 32.0f };
		float angle = 0.0f;
		unsigned int color = 0xffffffff;
	};

	Fish fish{};
	fish.position = Vector2(kWinWidth / 2.0f, kWinHeight / 2.0f);
	fish.size.x = 48.0f;
	fish.velocity = Vector2(3.0f, 0.0f);

	const float kMaxSpeed = 3.0f;

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };



	//Vector2 amplitude{};
	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0)
	{
		// フレームの開始
		Novice::BeginFrame();
		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		Vector2 circleCenter = fish.velocity;
		const float kFutureLength = 100.0f;
		circleCenter += Vector2::Normalize(circleCenter) * kFutureLength;

		// 振れ幅
		const float kDegree = 15.0f;
		const float kOffsetTheta = kDegree * 180.0f / std::numbers::pi_v<float>;

		// 半径
		const float kRadius = kMaxSpeed * 2.0f;

		// 円周上の点
		const float kNewAngle = Random::GetRandom(-kOffsetTheta, +kOffsetTheta) + fish.angle;
		Vector2 displacement = Vector2(cosf(kNewAngle), sinf(kNewAngle))* kRadius;

		
		Vector2 addForce = circleCenter + displacement;
		Vector2 accelaration = addForce * 0.9f;
		fish.velocity += accelaration;

		// 定速に設定
		fish.velocity = Vector2::Normalize(fish.velocity) * kMaxSpeed;

		// 移動と描画
		fish.position += fish.velocity;
		fish.angle = atan2f(fish.velocity.y, fish.velocity.x);
		Triangle drawTriangle = ConvertTriangle(fish.position, fish.size.x, fish.size.y, fish.angle);
		DrawTriangle(drawTriangle, 0x00ffffff, false);

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
