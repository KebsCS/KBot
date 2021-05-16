#pragma once
#include <Windows.h>
#include "Vector.h"

class Orbwalker
{
private:
	Vector3 MyHeroPos;
	DWORD lastTick = 0;
	DWORD lastAATick = 0;
	float windupTime;
	float missileTime;
	Vector3 startPos;

public:
	Orbwalker();

	~Orbwalker()
	{
	}

	void Tick();

	bool CanAttack();

	float GetAttackDelay();

	float GetWindupTime();

	bool CanMove();
};
