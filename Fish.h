#pragma once
#include "RendererUtility.h"
#include "GameObject.h"

class Fish : public GameObject
{
	Fish() = default;



private:

	static Vector2* centerPtr;

	bool isActive = false;
	bool isFishing = false;
	float lifeTime = 0.0f;
	float lifePower = 100.0f;
};

