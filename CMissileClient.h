#pragma once

#ifndef _CMISSILECLIENT_H_
#define _CMISSILECLIENT_H_

#include "ObjectManager.h"

class MissileClient : public CObject
{
private:
	DWORD base = 0;
	DWORD spelldata = 0;
public:

	MissileClient()
		:base{ 0 }
	{
		CObject::base = 0;
	}
	MissileClient(DWORD addy)
		:base{ addy }
	{
		CObject::base = addy;
	}
	DWORD Address() const
	{
		return base;
	}

	Vector3 GetMissileStartPos()
	{
		Vector3 startPos = Vector3(Memory.Read<float>(base + oMissileStartPos, sizeof(float)),
			Memory.Read<float>(base + oMissileStartPos + 0x4, sizeof(float)),
			Memory.Read<float>(base + oMissileStartPos + 0x8, sizeof(float)));
		startPos.Y += 100;
		return startPos;
	}

	Vector3 GetMissileEndPos()
	{
		Vector3 startPos = Vector3(Memory.Read<float>(base + oMissileEndPos, sizeof(float)),
			Memory.Read<float>(base + oMissileEndPos + 0x4, sizeof(float)),
			Memory.Read<float>(base + oMissileEndPos + 0x8, sizeof(float)));
		startPos.Y += 100;
		return startPos;
	}

	short GetMissileSourceIndex()
	{
		return Memory.Read<short>(base + oMissileSourceIndex, sizeof(short));
	}

	CObject GetSource()
	{
		return ObjManager->GetObjByIndex(GetMissileSourceIndex());
	}

	short GetMissileTargetIndex()
	{
		return Memory.Read<short>(base + oMissileTargetIndex, sizeof(short));
	}

	CObject GetTarget()
	{
		return ObjManager->GetObjByIndex(GetMissileTargetIndex());
	}

	DWORD GetSpellData()
	{
		if (!spelldata)
		{
			DWORD spellinfo = Memory.Read<DWORD>(base + oMissileSpellInfo);
			spelldata = Memory.Read<DWORD>(spellinfo + oSpellInfoSpellData);
		}
		else return spelldata;
	}
};

#endif // !_CMISSILECLIENT_H_
