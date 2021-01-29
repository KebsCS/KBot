#pragma once

#ifndef _CSPELLSLOT_H_
#define _CSPELLSLOT_H_

#include "CSpellInfo.h"

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

	int GetLevel()
	{
		return Memory.Read<int>((DWORD)this + oSpellSlotLevel);
	}

	float GetCooldownExpire()
	{
		return Memory.Read<float>((DWORD)this + oSpellCooldownExpire);
	}

	int GetRemainingCharge()
	{
		return Memory.Read<int>((DWORD)this + oSpellSlotRemainingCharge);
	}

	float GetDamage()
	{
		return Memory.Read<float>((DWORD)this + oSpellSlotDamage);
	}

	CSpellInfo* GetSpellInfo()
	{
		DWORD addr = Memory.Read<DWORD>((DWORD)this + oSpellSlotSpellInfo);

		return (CSpellInfo*)(addr);
	}
};

#endif // !_CSPELLSLOT_H_
