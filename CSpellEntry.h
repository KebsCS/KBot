#pragma once

#ifndef _CSPELLENTRY_H_
#define _CSPELLENTRY_H_

#include "CObjectManager.h"
#include "CSpellInfo.h"
#include "SpellInfo.h"

//good for non missiles(karthus q, xerath w), but doesn't detect end cast pos of charged spells(xerath q, varus q)
//expect chargeable spells it's usable to almost everything with little bit of work
//like calculating correct endpos from spell range and making a timer so the line doesn't disappear too fast
//todo
class CSpellEntry
{
private:
	DWORD base = 0;
	DWORD dwspellinfo = 0;
public:

	SpellInfo* spellInfo = GameData::UnknownSpell;

	CSpellEntry()
		: base{ 0 }
	{
		std::string temp = utils->ToLower(GetSpellInfo()->GetName()).c_str();
		spellInfo = GameData::GetSpellInfoByName(temp);
	}
	CSpellEntry(DWORD addr)
		: base{ addr }
	{
		std::string temp = utils->ToLower(GetSpellInfo()->GetName()).c_str();
		spellInfo = GameData::GetSpellInfoByName(temp);
	}
	DWORD Address() const
	{
		return base;
	}

	int GetIndex()
	{
		return Memory.Read<int>(base + Offsets::oSpellEntryIndex);
	}

	CSpellInfo* GetSpellInfo()
	{
		if (!dwspellinfo)
		{
			dwspellinfo = Memory.Read<DWORD>(base + Offsets::oSpellEntrySpellInfo);
		}
		if (!dwspellinfo)
			return 0;
		return (CSpellInfo*)(dwspellinfo);
	}

	Vector3 GetMissileStartPos()
	{
		Vector3 startPos = Vector3(Memory.Read<float>(base + Offsets::oSpellEntryStartPos, sizeof(float)),
			Memory.Read<float>(base + Offsets::oSpellEntryStartPos + 0x4, sizeof(float)),
			Memory.Read<float>(base + Offsets::oSpellEntryStartPos + 0x8, sizeof(float)));
		//startPos.y += spellInfo->height;
		return startPos;
	}

	Vector3 GetMissileEndPos()
	{
		Vector3 startPos = Vector3(Memory.Read<float>(base + Offsets::oSpellEntryEndPos, sizeof(float)),
			Memory.Read<float>(base + Offsets::oSpellEntryEndPos + 0x4, sizeof(float)),
			Memory.Read<float>(base + Offsets::oSpellEntryEndPos + 0x8, sizeof(float)));
		//startPos.y += spellInfo->height;
		return startPos;
	}

	int GetSlot()
	{
		return Memory.Read<int>(base + Offsets::oSpellEntrySlot);
	}

	bool IsBasic()
	{
		return Memory.Read<bool>(base + Offsets::oSpellEntryIsBasicAttack);
	}

	float GetWindupTime()
	{
		return Memory.Read<float>(base + Offsets::oSpellEntryWindupTime, sizeof(float));
	}

	float GetCastStartTime()
	{
		return Memory.Read<float>(base + Offsets::oSpellEntryCastStartTime, sizeof(float));
	}

	bool HasSpellFlags(SpellFlags flags) const
	{
		return (spellInfo->flags & flags) == flags;
	}

	inline bool operator ! () const
	{
		if (base == 0)
			return true;
		return false;
	}
};

#endif // !_CSPELLENTRY_H_
