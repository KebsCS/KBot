#pragma once

#ifndef _SPELLDATA_H_
#define _SPELLDATA_H_

#include "Kinterface.h"
#include "offsets.h"

class CSpellData
{
private:
	//todo using base instead of (DWORD)this
	DWORD base = 0;
public:
	CSpellData()
		:base{ 0 }
	{
	}
	CSpellData(DWORD addr)
		:base{ addr }
	{
	}

	float GetRange()
	{
		return Memory.Read<float>((DWORD)this + Offsets::oSpellDataRange);
	}

	float GetWidth()
	{
		return Memory.Read<float>((DWORD)this + Offsets::oSpellDataWidth);
	}

	float GetSpeed()
	{
		return Memory.Read<float>((DWORD)this + Offsets::oSpellDataSpeed);
	}

	float GetCastRadius()
	{
		return Memory.Read<float>((DWORD)this + Offsets::oSpellDataCastRadius);
	}

	float GetManaCostByLevel(unsigned int level)
	{
		if (level > 4) {
			return 0.f;
		}
		return  Memory.Read<float>((DWORD)this + Offsets::oSpellDataMana + ((level - 1) * 0x4));
	}
};

#endif // !_SPELLDATA_H_
