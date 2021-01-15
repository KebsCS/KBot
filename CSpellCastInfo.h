#pragma once

#ifndef _CSPELLCASTINFO_H_
#define _CSPELLCASTINFO_H_

#include "ObjectManager.h"

class SpellCastInfo //: public CObject
{
private:
	DWORD base = 0;
	DWORD spelldata = 0;
	DWORD spellinfoptr = 0;
public:

	SpellCastInfo()
		:base{ 0 }
	{
		//CObject::base = 0;
	}
	SpellCastInfo(DWORD addy)
		:base{ addy }
	{
		//CObject::base = addy;
	}
	DWORD Address() const
	{
		return base;
	}

	int GetIndex()
	{
		return Memory.Read<int>(base + 0x14);
	}
	DWORD GetSpellInfoPtr()
	{
		if (!spellinfoptr)
		{
			spellinfoptr = Memory.Read<DWORD>(base + 0x8);
			return spellinfoptr;
		}
		return spellinfoptr;
	}

	std::string GetName()
	{
		return Memory.ReadString(GetSpellInfoPtr() + 0x18);
	}
	std::string GetSlotName()
	{
		return Memory.ReadString(GetSpellData() + 0x1F0);
	}
	int GetSlot()
	{
		return Memory.Read<int>(base + 0xC);
	}

	float GetWindupTime()
	{
		return Memory.Read<float>(base + 0x4C0, sizeof(float));
	}

	float GetCastStartTime()
	{
		return Memory.Read<float>(base + 0x544, sizeof(float));
	}
	//good for non missiles(karthus q, xerath w), but doesn't detect end cast pos of charged spells(xerath q, varus q)
	//expect chargeable spells it's usable to almost everything with little bit of work
	//like calculating correct endpos from spell range and making a timer so the line doesn't disappear too fast
	//todo
	Vector3 GetMissileStartPos()
	{
		Vector3 startPos = Vector3(Memory.Read<float>(base + 0x80, sizeof(float)),
			Memory.Read<float>(base + 0x80 + 0x4, sizeof(float)),
			Memory.Read<float>(base + 0x80 + 0x8, sizeof(float)));
		startPos.y += 100;
		return startPos;
	}

	Vector3 GetMissileEndPos()
	{
		Vector3 startPos = Vector3(Memory.Read<float>(base + 0x8C, sizeof(float)),
			Memory.Read<float>(base + 0x8C + 0x4, sizeof(float)),
			Memory.Read<float>(base + 0x8C + 0x8, sizeof(float)));
		startPos.y += 100;
		return startPos;
	}
	DWORD GetSpellData()
	{
		if (!spelldata)
		{
			spelldata = Memory.Read<DWORD>(GetSpellInfoPtr() + oSpellInfoSpellData);
		}
		return spelldata;
	}

	float GetRange()
	{
		return Memory.Read<float>(GetSpellData() + 0x3b4);
	}

	//short GetMissileSourceIndex()
	//{
	//	return Memory.Read<short>(base + oMissileSourceIndex, sizeof(short));
	//}

	//CObject GetSource()
	//{
	//	return ObjManager->GetObjByIndex(GetMissileSourceIndex());
	//}

	//short GetMissileTargetIndex()
	//{
	//	return Memory.Read<short>(base + oMissileTargetIndex, sizeof(short));
	//}

	//CObject GetTarget()
	//{
	//	return ObjManager->GetObjByIndex(GetMissileTargetIndex());
	//}
};

#endif // !_CSPELLCASTINFO_H_
