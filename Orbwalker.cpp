#include "Orbwalker.h"
#include "DirectX.h"
#include "Mouse.h"
#include "Keyboard.h"

Orbwalker::Orbwalker()
{
	windupTime = Local.unitInfo->basicAttackWindup;
	missileTime = Local.unitInfo->basicAttackMissileSpeed / (Local.GetAARange() / 2);
	startPos = mouse->GetWorldPos();
}

float Orbwalker::GetAttackDelay()
{
	return (int)(1000.0f / Local.GetAS());
}

bool Orbwalker::CanAttack()
{
	return (lastAATick + GetAttackDelay() + (M.Evade.GP / 2)) < GetTickCount();
}

float Orbwalker::GetWindupTime()
{
	return (1 / Local.GetAS() * 1000) * windupTime;
}

bool Orbwalker::CanMove()
{
	return lastTick < GetTickCount();
}

void Orbwalker::Tick()
{
	MyHeroPos = Local.GetPosition();
	std::vector<CObject>canAttack;

	// Get players in aa range
	for (auto& hero : init->herolist)
	{
		if (hero == Local || hero.IsDead() || !hero.IsVisible())
			continue;
		if (hero.GetDistTo(Local) <= Local.GetBoundingRadius() + Local.GetAARange())
		{
			canAttack.emplace_back(hero);
		}
	}
	// Sort so closest player is first
	if (canAttack.size() > 1)
	{
		std::sort(canAttack.begin(), canAttack.end(), [&](CObject& a, CObject& b)
			{
				return MyHeroPos.Distance(a.GetPosition()) < MyHeroPos.DistanceSquared(b.GetPosition());
			});
	}

	if (!PressedKey(M.Orbwalker.HoldKey))
		return;

	if (Local.CheckState(CharacterState::CanAttack) && !canAttack.empty() && CanAttack())
	{
		Vector3 pos = canAttack.front().GetPosition();
		pos.y += 25;
		ImVec2 RealPos = Direct3D9.WorldToScreen(pos);
		ImVec2 OldPos = Direct3D9.WorldToScreen(startPos);
		if (RealPos.x != 0 && RealPos.y != 0)
		{
			mouse->MouseMoveInstant(RealPos.x, RealPos.y);
			mouse->RightClick();
			//keyboard->GenerateKeyScancode(DIK_A);
			//mouse->LeftClick();
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
			while (!mouse->MouseMoveInstant(OldPos.x, OldPos.y))
			{
			}

			lastAATick = GetTickCount() - (int)(1000.f / missileTime) + 35;
			lastTick = GetTickCount() + GetWindupTime() + 35;
		}
	}
	else if (CanMove())
	{
		startPos = mouse->GetWorldPos();
		mouse->RightClick();
		lastTick = GetTickCount() + RandomInt(50, 80);
	}
}