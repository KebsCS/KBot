#pragma once

#ifndef _CSPELLSLOT_H_
#define _CSPELLSLOT_H_

#include "CSpellInfo.h"


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
