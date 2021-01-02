#include "Xerath.h"
#include "DirectX.h"

void Xerath::OnKeyDown(int)
{

}

void Xerath::OnKeyUp(int)
{

}

Vector3 Calculate(CObject target, float range, float casttime)
{
	//auto aim = target->GetAIManager();
	auto veloc = Vector3(0, 0, 0); //aim->GetVelocity();
	//veloc.Y = 0.f;
	Vector3 orientation = Vector3(0, 0, 0);//veloc.Normalize(); //target->GetAllShield->vVelocity.Normalized();


	if (!(target.GetHealth() > 0.f && target.GetDistTo(Local) < range)) 
	{
		return Vector3(0.f, 0.f, 0.f);
	}

	if (veloc.X == 0.f && veloc.Z == 0.f)
	{
		return target.GetPosition();
	}

	Vector3 result = target.GetPosition() + (orientation * target.GetMS() * casttime);

	return result;
}

Vector3 PredictCircular(CObject target, SpellSlotID slot, bool checkCharges = false) 
{
	DWORD spell = Local.GetSpellByID(slot);
	//if (checkCharges && (spell->GetCharges() < 1))
	//	return Vector(0.f, 0.f, 0.f);
	//else
	if (!(Memory.Read<int>(spell + oSpellSlotLevel, sizeof(int)) >= 1 && (M.GameTime >= Memory.Read<float>(spell + oSpellSlotTime, sizeof(float)))))
		return Vector3(0.f, 0.f, 0.f);
	//auto spellData = spell->GetSpellInfo()->GetSpellData();
	//if (spellData->GetManaCostByLevel(spell->GetLevel()) > localObj->GetMana())
	//	return Vector(0.f, 0.f, 0.f);
	DWORD spellData = Memory.Read<DWORD>(Memory.Read<DWORD>(spell + 0x134) + 0x44);
	auto vec = Calculate(target, 1000.f/*spellData.GetSpellRange()*/, 0.f);

	return vec;
}

void Xerath::Harass()
{
	std::vector<CObject> possibleTargets;

	for (auto pObject : init->herolist) 
	{
		if (!pObject.IsDead() && pObject.GetTeam() != Local.GetTeam())
		{
			possibleTargets.push_back(pObject);

		}
	}
	if (possibleTargets.empty())
		return;

	if (possibleTargets.size() > 1)
	{
		std::sort(possibleTargets.begin(), possibleTargets.end(),
			[](CObject pFirst, CObject pSecond)
			{
				return (pFirst.GetDistTo(Local) < pSecond.GetDistTo(Local));
			});
	}

	auto vec = PredictCircular(possibleTargets.front(), SpellSlotID::W);
	if (!(vec == Vector3(0.f, 0.f, 0.f)))
	{
		ImVec2 RealPos = Direct3D9.WorldToScreen(vec);
		mouse->StoreCurrentPos();
		mouse->MouseMoveInstant(RealPos.x, RealPos.y);
		keyboard->GenerateKeyScancode(DIK_W, false);
		POINT LastMousePos = mouse->GetStoredPos();
		mouse->MouseMoveInstant(LastMousePos.x, LastMousePos.y);
	}

}

bool Xerath::Evade(Vector3 /*evadePos*/)
{
	return false;
}