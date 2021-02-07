#include "Xerath.h"
#include "DirectX.h"

#include "Mouse.h"
#include "Keyboard.h"
#include "CSpellEntry.h"

void Xerath::OnKeyDown(int)
{
	Harass();
}

void Xerath::OnKeyUp(int)
{
}

Vector3 PredictLinear(CObject target, float range, float missilespeed, float casttime)
{
	float t = Vector3(target.GetPosition() - Local.GetPosition()).Length() / missilespeed;
	t += casttime;
	auto aim = target.GetAiManager();
	auto veloc = aim->GetVelocity();
	veloc.y = 0.f;
	Vector3 orientation = veloc.Normalized();

	if (!(target.Address() && target.GetHealth() > 0.f && target.GetDistTo(Local) < range))
	{
		return Vector3(0.f, 0.f, 0.f);
	}

	if (veloc.x == 0.f && veloc.z == 0.f)
	{
		return target.GetPosition();
	}

	Vector3 result = target.GetPosition() + (orientation * target.GetMS() * t);

	if (result.Distance(Local.GetPosition()) > range) {
		return Vector3(0.f, 0.f, 0.f);
	}

	return result;
}

Vector3 PredictCircular(CObject target, float range, float casttime)
{
	auto aim = target.GetAiManager();
	auto veloc = aim->GetVelocity();
	veloc.y = 0.f;
	Vector3 orientation = veloc.Normalized(); //target->GetAllShield->vVelocity.Normalized();

	if (!(target.Address() && target.GetHealth() > 0.f && target.GetDistTo(Local) < range)) {
		return Vector3(0.f, 0.f, 0.f);
	}

	if (veloc.x == 0.f && veloc.z == 0.f)
	{
		return target.GetPosition();
	}

	Vector3 result = target.GetPosition() + (orientation * target.GetMS() * casttime);

	return result;
}

CObject GetTarget(float range, int type)
{
	std::vector<CObject> possibleTargets;

	for (auto obj : init->herolist)
	{
		if (!obj.IsDead() && obj.GetTeam() != Local.GetTeam())
		{
			if (obj.GetDistTo(Local) > range)
				continue;
			possibleTargets.push_back(obj);
		}
	}
	if (possibleTargets.empty())
		return 0;

	if (possibleTargets.size() > 1)
	{
		//closest to player
		/*std::sort(possibleTargets.begin(), possibleTargets.end(),
			[](CObject pFirst, CObject pSecond)
			{
				return (pFirst.GetDistTo(Local) < pSecond.GetDistTo(Local));
			});*/

			//closest to mouse pos
		std::sort(possibleTargets.begin(), possibleTargets.end(),
			[](CObject pFirst, CObject pSecond)
			{
				Vector3 mpos = mouse->GetWorldPos();
				return (pFirst.GetDistTo(mpos) < pSecond.GetDistTo(mpos));
			});
	}
	return possibleTargets.front();
}

void useQclose(CObject target, Vector3 qPred)
{
	if (Local.GetPosition().Distance(qPred) < 750)//&& /*Game.Timer() - OnWaypoint(target).time > 0.05*/)
	{
		/*ReleaseSpell(HK_Q, qPred, self.Q.range, 75)
		self.lastTarget = target
		self.lastTarget_tick = GetTickCount() + 200*/
	}
}

void Xerath::Harass()
{
	//std::vector<CObject> possibleTargets;

	//for (auto pObject : init->herolist)
	//{
	//	if (!pObject.IsDead() && pObject.GetTeam() != Local.GetTeam())
	//	{
	//		possibleTargets.push_back(pObject);
	//	}
	//}
	//if (possibleTargets.empty())
	//	return;

	//if (possibleTargets.size() > 1)
	//{
	//	//closest to player
	//	/*std::sort(possibleTargets.begin(), possibleTargets.end(),
	//		[](CObject pFirst, CObject pSecond)
	//		{
	//			return (pFirst.GetDistTo(Local) < pSecond.GetDistTo(Local));
	//		});*/

	//		//closest to mouse pos
	//	std::sort(possibleTargets.begin(), possibleTargets.end(),
	//		[](CObject pFirst, CObject pSecond)
	//		{
	//			Vector3 mpos = mouse->GetWorldPos();
	//			return (pFirst.GetDistTo(mpos) < pSecond.GetDistTo(mpos));
	//		});
	//}

	if (chargeQ)
	{
		qRange = 750 + 500 * (GetTickCount() - qTick) / 1000;
		if (qRange > 1500)
			qRange = 1500;
	}

	if (CSpellEntry spell(Local.GetActiveSpellEntry()); spell.Address() != 0)
	{
		//if is casting q
		if (utils->StringContains(spell.GetSpellInfo()->GetName(), "XerathArcanopulse"))
		{
			if (!chargeQ)
			{
				qTick = GetTickCount();
				chargeQ = true;
			}
		}
		else
		{
			chargeQ = false;
			qRange = 750;
		}
	}
	else
	{
		chargeQ = false;
		qRange = 750;
	}

	CObject target = GetTarget(1500, 1);
	if (target.Address() != 0)
	{
		Vector3 qPred = PredictLinear(target, qRange, 999999, 0.35 + M.Evade.GP / 1000);
		Vector3 qPred2 = PredictLinear(target, qRange, 999999, 1);
		draw->Line(Direct3D9.WorldToScreen(qPred), Direct3D9.WorldToScreen(Local.GetPosition()), RGBA(COLOR_CRIMSON));
		draw->Line(Direct3D9.WorldToScreen(qPred2), Direct3D9.WorldToScreen(Local.GetPosition()), RGBA(COLOR_CRIMSON));

		if (qPred2.Distance(target.GetPosition()) < 1500)
		{
			keyboard->GenerateKeyScancode(DIK_Q);
		}
		if (chargeQ)
		{
		}
	}

	draw->CircleRange(Local.GetPosition(), 16, qRange, RGBA(COLOR_CRIMSON));
}

bool Xerath::Evade(Vector3 evadePos)
{
	return false;
}