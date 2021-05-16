#pragma once

#ifndef _CSPELLCASTINFO_H_
#define _CSPELLCASTINFO_H_

#include "CSpellData.h"
#include "SpellInfo.h"

class CSpellInfo
{
private:
	DWORD base = 0;
	DWORD spelldata = 0;
	std::string name;

public:

	CSpellInfo()
		:base{ 0 }
	{
	}
	CSpellInfo(DWORD addy)
		:base{ addy }
	{
	}
	//DWORD Address() const
	//{
	//	return base;
	//}

	std::string GetName()
	{
		return Memory.ReadString((DWORD)this + Offsets::oSpellInfoSpellName, true);
	}
	//crashes when i try write to var spelldata
	CSpellData* GetSpellData()
	{
		DWORD addr = Memory.Read<DWORD>((DWORD)this + Offsets::oSpellInfoSpellData);

		return (CSpellData*)(addr);
	}
};

#endif // !_CSPELLCASTINFO_H_
