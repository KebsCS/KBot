#pragma once

#ifndef _CSPELLSLOT_H_
#define _CSPELLSLOT_H_

#include "CSpellInfo.h"
#include "Menu.h"

//todo reverse ["GetSpellState", FindFuncCall("E8 ? ? ? ? 8B F8 8B CB 89")],
enum class SpellState : int
{
	//Possible flags
	Ready = 0,
	NotAvailable = 4,
	Surpressed = 8,
	NotLearned = 12,
	Cooldown = 32,
	NoMana = 64,
	Unknown
};

class CSpellSlot
{
private:
	DWORD base = 0;
public:
	CSpellSlot()
		: base{ 0 }
	{
	}

	CSpellSlot(DWORD addr)
		: base{ addr }
	{
	}
	bool IsReady(float mana = 99999.f)
	{
		int level = this->GetLevel();
		if ((level > 0) && (this->GetCooldownExpire() - M.fGameTime + 1 < 0)
			&& this->GetSpellInfo()->GetSpellData()->GetManaCostByLevel(level) <= mana)
			return true;
		return false;
	}

	int GetLevel()
	{
		return Memory.Read<int>((DWORD)this + Offsets::oSpellSlotLevel);
	}

	float GetCooldownExpire()
	{
		return Memory.Read<float>((DWORD)this + Offsets::oSpellSlotCooldownExpire);
	}

	int GetRemainingCharge()
	{
		return Memory.Read<int>((DWORD)this + Offsets::oSpellSlotRemainingCharge);
	}

	bool IsActive()
	{
		return Memory.Read<bool>((DWORD)this + Offsets::oSpellSlotIsActive);
	}

	float GetDamage()
	{
		return Memory.Read<float>((DWORD)this + Offsets::oSpellSlotDamage);
	}

	CSpellInfo* GetSpellInfo()
	{
		DWORD addr = Memory.Read<DWORD>((DWORD)this + Offsets::oSpellSlotSpellInfo);

		return (CSpellInfo*)(addr);
	}
};

#endif // !_CSPELLSLOT_H_
