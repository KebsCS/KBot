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
		return Memory.Read<float>((DWORD)this + oMissileRange);
	}

	float GetWidth()
	{
		return Memory.Read<float>((DWORD)this + oMissileWidth);
	}

	float GetSpeed()
	{
		return Memory.Read<float>((DWORD)this + oMissileSpeed);
	}

	float GetCastRadius()
	{
		return Memory.Read<float>((DWORD)this + oMissileCastRadius);
	}

	float GetManaCostByLevel(unsigned int level)
	{
		if (level > 4) {
			return 0.f;
		}
		return  Memory.Read<float>((DWORD)this + 0x518 + ((level - 1) * 0x4));
	}
};

#endif // !_SPELLDATA_H_
