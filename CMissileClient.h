#pragma once

#ifndef _CMISSILECLIENT_H_
#define _CMISSILECLIENT_H_

#include "CObjectManager.h"
#include "CSpellInfo.h"

class CMissileClient : public CObject
{
private:
	DWORD base = 0;
	DWORD dwSpellInfo = 0;

	Vector3 startPos, endPos;
public:

	SpellInfo* spellInfo = GameData::UnknownSpell;

	CMissileClient()
		:base{ 0 }
	{
		CObject::base = 0;
		std::string temp = utils->ToLower(GetSpellInfo()->GetName());
		spellInfo = GameData::GetSpellInfoByName(temp);
	}
	CMissileClient(DWORD addy)
		:base{ addy }
	{
		CObject::base = addy;
		std::string temp = utils->ToLower(GetSpellInfo()->GetName());
		spellInfo = GameData::GetSpellInfoByName(temp);
	}
	DWORD Address() const
	{
		return base;
	}

	Vector3 GetMissileStartPos()
	{
		if (startPos.IsZero())
		{
			startPos = Vector3(Memory.Read<float>(base + oMissileStartPos, sizeof(float)),
				Memory.Read<float>(base + oMissileStartPos + 0x4, sizeof(float)),
				Memory.Read<float>(base + oMissileStartPos + 0x8, sizeof(float)));
			//startPos.y += spellInfo->height;
		}
		return startPos;
	}

	Vector3 GetMissileEndPos()
	{
		if (endPos.IsZero())
		{
			endPos = Vector3(Memory.Read<float>(base + oMissileEndPos, sizeof(float)),
				Memory.Read<float>(base + oMissileEndPos + 0x4, sizeof(float)),
				Memory.Read<float>(base + oMissileEndPos + 0x8, sizeof(float)));
			//endPos.y += spellInfo->height;
		}
		return endPos;
	}

	short GetMissileSourceIndex()
	{
		return Memory.Read<short>(base + oMissileSourceIndex, sizeof(short));
	}

	CObject GetSource()
	{
		return CObjectManager::GetObjByIndex(GetMissileSourceIndex());
	}

	short GetMissileTargetIndex()
	{
		return Memory.Read<short>(base + oMissileTargetIndex, sizeof(short));
	}

	CObject GetTarget()
	{
		return CObjectManager::GetObjByIndex(GetMissileTargetIndex());
	}

	CSpellInfo* GetSpellInfo()
	{
		if (!dwSpellInfo)
		{
			dwSpellInfo = Memory.Read<DWORD>(base + oMissileSpellInfo);
		}
		if (!dwSpellInfo)
			return 0;
		return (CSpellInfo*)(dwSpellInfo);
	}

	bool HasSpellFlags(SpellFlags flags) const
	{
		return (spellInfo->flags & flags) == flags;
	}

	//void SetObjConsts()
	//{
	//	std::string temp = utils->ToLower(champ);
	//	spellInfo = GameData::GetSpellInfoByName(temp);
	//	// Some spells require their end position to be projected using the range of the spell
	//	if (spellInfo != GameData::UnknownSpell && HasSpellFlags(ProjectedDestination)) {
	//		startPos.y += spellInfo->height;

	//		// Calculate direction vector and normalize
	//		endPos = Vector3(endPos.x - startPos.x, 0, endPos.z - startPos.z);
	//		endPos = endPos.Normalized();

	//		// Update endposition using the height of the current position
	//		endPos.x = endPos.x * spellInfo->castRange + startPos.x;
	//		endPos.y = startPos.y;
	//		endPos.z = endPos.z * spellInfo->castRange + startPos.z;
	//	}
	//}
};

#endif // !_CMISSILECLIENT_H_
