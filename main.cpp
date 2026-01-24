#include <Novice.h>
#include <numbers>
#include <cmath>
#include <algorithm>
#include <vector>
#include <memory>

#include "Vector2.h"
#include "ColliderCollection.h"
#include "NoviceUtility.h"
#include "Random.h"
#include "Player.h"
#include "Fish.h"
#include "FishingHook.h"
#include "FishingLine.h"

using namespace ColliderCollection;
using namespace NoviceUtility;

struct Angler : CircleCollider
{
	Vector2 velocity{};
	RenderData renderData{};
	float speed = 10.0f;
};
//
//struct FishingHook : Angler
//{
//	bool isLineBroken = false;
//	float maxLineLength = 0.0f;
//	float duravility = 0.0f;
//};

//struct Fish :Angler
//{
//	bool isActive = false;
//	bool mIsFishing = false;
//	float lifeTime = 0.0f;
//	float lifePower = 100.0f;
//};


//struct FishingLine : Line
//{
//	unsigned int color = 0xffffffff;
//};

const char kWindowTitle[] = "TD1_3回目";



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
	const unsigned int kFishTex = Novice::LoadTexture("./Resources/Images/Fish.png");
	Random::Initialize();

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	PlayerConfig playerConfig{};
	playerConfig.collider.radius = 16.0f;

	playerConfig.tag = ObjectTag::Player;
	playerConfig.speed = 200.0f;
	playerConfig.sizeHalf = Vector2(playerConfig.collider.radius, playerConfig.collider.radius);

	RenderData playerRenderData{};
	playerRenderData.size = playerConfig.sizeHalf;
	playerRenderData.texHandle = kDefaultTex;

	unique_ptr<Player> player = make_unique<Player>(playerConfig);


	FishingHookConfig fishingHookConfig{};
	fishingHookConfig.tag = ObjectTag::FishingHook;
	fishingHookConfig.collider.radius = 8.0f;
	fishingHookConfig.sizeHalf = Vector2(fishingHookConfig.collider.radius, fishingHookConfig.collider.radius);
	fishingHookConfig.speed = 15.0f;
	fishingHookConfig.target = player.get();
	fishingHookConfig.centerPosition = player->GetPosition() + Vector2(1.0f, 0.0f) * fishingHookConfig.collider.radius;

	RenderData fishingHookRenderData{};
	fishingHookRenderData.center = fishingHookConfig.centerPosition;
	fishingHookRenderData.color = 0x008888ff;
	fishingHookRenderData.size = fishingHookConfig.sizeHalf;
	fishingHookRenderData.texHandle = kDefaultTex;

	unique_ptr<FishingHook> fishingHook = make_unique<FishingHook>(fishingHookConfig);

	unique_ptr<FishingLine> fishingLine = make_unique<FishingLine>(fishingHook.get(), player.get(), kWinWidth / 2.0f, WHITE);

	//fishingHook.maxLineLength = kWinWidth / 2.0f;
	//fishingHook.duravility = 100.0f;

	/*FishingLine fishingLine{};
	fishingLine.color = WHITE;
	Vector2 toAngler{};
	Vector2 toAnglerNormal{};*/

	int fishingCount = 0;
	int totalFishingCount = 0;

	const float kSpawnFishTime = 1.0f;
	float spawnFishTimer = 0.0f;

	Scroll scroll{};
	scroll.target = player->GetPosition();
	scroll.startPoint = Vector2();


	const int kMaxFishies = 100;
	std::vector <unique_ptr<Fish>> fishies{};
	fishies.resize(kMaxFishies);

	FishConfig fishConfig{};
	fishConfig.collider.radius = 16.0f;
	fishConfig.speed = 50.0f;
	fishConfig.sizeHalf = Vector2(fishConfig.collider.radius, fishConfig.collider.radius);
	fishConfig.target = player.get();

	RenderData fishRenderData{};
	fishRenderData.size = fishConfig.sizeHalf;
	fishRenderData.texHandle = kFishTex;
	fishRenderData.texFrameSize = Vector2(32.0f, 32.0f);
	for (auto &fish : fishies)
	{
		if (fish == nullptr) { fish = make_unique<Fish>(); }

		float spawnRange = Random::GetRandom(100.0f, kStage.max.x);
		float spawnTheta = Random::GetRandom(0.0f, std::numbers::pi_v<float>);

		fishConfig.centerPosition = Vector2(cosf(spawnTheta), sinf(spawnTheta)) * spawnRange;
		//fishConfig.lifePower = Random::GetRandom(5.0f, 10.0f);
		fishConfig.speed = Random::GetRandom(50.0f, 80.0f);

		fish->Spawn(fishConfig);
	}

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0)
	{
		// フレームの開始
		Novice::BeginFrame();
		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		/// Player Move
		Vector2 inputDir{};
		if (keys[DIK_W]) { player->MoveUp(); }
		if (keys[DIK_S]) { player->MoveDown(); }
		if (keys[DIK_A]) { player->MoveLeft(); }
		if (keys[DIK_D]) { player->MoveRight(); }
		player->Update();

		if (!fishingLine->CheckLineBroken())
		{
			fishingHook->MoveUpdate();

			if (keys[DIK_SPACE])
			{
				fishingHook->AddVelocity(fishingLine->GetToAnglerNorm() * fishingHook->GetSpeed() * kDeltaTime);
			}
		}
		else
		{
			for (auto& fish : fishies)
			{
				if(!fish->IsActive()){continue;}
				if(!fish->IsFishing()){continue;}
				fish->Escape();
			}
		}

		fishingHook->Update();

		if(IsCollision(player->GetCollider(), fishingHook->GetCollider()))
		{
			fishingHook->OnCollision(*player.get());
		}

		
		// Fish Spawn
		{
			spawnFishTimer += kDeltaTime;
			if (spawnFishTimer >= kSpawnFishTime)
			{
				for (auto &fish : fishies)
				{
					if (fish->IsActive()) { continue; }

					float spawnRange = Random::GetRandom(100.0f, kStage.max.x);
					float spawnTheta = Random::GetRandom(0.0f, std::numbers::pi_v<float>);

					fishConfig.centerPosition = Vector2(cosf(spawnTheta), sinf(spawnTheta)) * spawnRange;
					//fishConfig.lifePower = Random::GetRandom(5.0f, 10.0f);
					fishConfig.speed = Random::GetRandom(5.0f, 8.0f);

					fish->Spawn(fishConfig);
					break;
				}

				spawnFishTimer = 0.0f;
			}
		}

		// Fish Move
		{
			for (auto &fish : fishies)
			{
				if (fish == nullptr) { continue; }
				if (!fish->IsActive()) { continue; }
				/*if (fish.mIsFishing)
				{
					fish.center = fishingHook.GetPosition() - fishingHook.radius * Vector2::Normalize(-fish.velocity);
					continue;
				}*/
				fish->Update();

				if(IsCollision(fishingHook->GetCollider(), fish->GetCollider()))
				{
					fish->OnCollision(*fishingHook.get());
					if(fish->IsFishing())
					{
						fish->SetPosition(fishingHook->GetPosition());
					}
				}
				//Vector2 toFish = fish.center - player->GetPosition();
				//Vector2 tangentialDir = Vector2::Normalize(Vector2(-toFish.y, toFish.x));
				//fish.velocity = tangentialDir * fish.speed * kDeltaTime;
				//fish.center += fish.velocity;

				//fish.renderData.angle = atan2f(fish.velocity.y, fish.velocity.x) + sinf((fish.center.x - fish.center.y) / 2.0f) / 5.0f;

				//if (!fishingHook.isLineBroken)
				//{
				//	if (IsCollision(fish, fishingHook))
				//	{
				//		float hookPower = fishingHook.velocity.Length();
				//		if (hookPower < fish.lifePower)
				//		{
				//			fish.lifePower -= hookPower * 0.3f;
				//			continue;
				//		}
				//		//Sleep(50);
				//		fish.lifePower = 0.0f;
				//		fish.mIsFishing = true;
				//		fishingCount++;
				//	}
				//}

				//fish.center.x = std::clamp(fish.center.x, kStage.min.x + fish.radius, kStage.max.x - fish.radius);
				//fish.center.y = std::clamp(fish.center.y, kStage.min.y + fish.radius, kStage.max.y - fish.radius);
			}
		}

		/// Scroll Update
		{
			scroll.target = player->GetPosition();
			scroll.value = MoveScroll(scroll);

			// scroll clamp map min
			scroll.value.x = std::clamp(
				scroll.value.x,
				kStage.min.x,
				kStage.max.x
			);

			playerRenderData.center = player->GetPosition() - scroll.value;
			fishingHookRenderData.center = fishingHook->GetPosition() - scroll.value;
			/*for (auto &fish : fishies)
			{
				if (!fish->IsActive()) { continue; }
				fish.renderData.center = fish.center - scroll.value;
			}*/
		}


		/// Draw
		{
			/*fishingLine.start = player->GetPosition() - scroll.value;
			fishingLine.end = fishingHook.GetPosition() - scroll.value;
			{
				if (fishingHook.isLineBroken)
				{
					DrawLine(fishingLine, BLACK);
				}
				else
				{
					DrawLine(fishingLine, fishingLine.color);
				}
			}*/
			DrawLine(fishingLine->GetLine(), fishingLine->GetColor());
			DrawSprite(playerRenderData);
			DrawSprite(fishingHookRenderData);
			for (auto &fish : fishies)
			{
				if (!fish->IsActive()) { continue; }
				fishRenderData.center = fish->GetPosition() - scroll.value;
				fishRenderData.angle = atan2f(fish->GetMoveDir().y, fish->GetMoveDir().x) + sinf((fish->GetPosition().x - fish->GetPosition().y) / 2.0f) / 5.0f;
				DrawSprite(fishRenderData);
			}
		}

#ifdef _DEBUG
		//Novice::ScreenPrintf(10, 10, "position(x:%.2f, y:%.2f) velocity(x:%.2f, y:%.2f) length(%.2f)", player->GetPosition().x, player->GetPosition().y, angler.velocity.x, angler.velocity.y, angler.velocity.Length());
		//Novice::ScreenPrintf(10, 30, "position(x:%.2f, y:%.2f) velocity(x:%.2f, y:%.2f) length(%.2f)", fishingHook->GetPosition().x, fishingHook^GetPosition().y, fishingHook.velocity.x, fishingHook.velocity.y, fishingHook.velocity.Length());
		Novice::ScreenPrintf(10, 50, "scrollVa(x:%.2f, y:%.2f)", scroll.value.x, scroll.value.y);
		Novice::ScreenPrintf(10, 70, "FishCount:%02d, nowFishCount:%02d", totalFishingCount, fishingCount);

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
