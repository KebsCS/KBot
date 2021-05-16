#pragma once

#ifndef _COBJECTMANAGER_H_
#define _COBJECTMANAGER_H_

#include "KInterface.h"
#include "offsets.h"
#include "Definitions.h"
#include "Utils.h"

#include "GameData.h"
#include "CSpellSlot.h"

#include "AiManager.h"
#include "LoLAPI.h"

class CObject
{
private:

	int IsFunc(DWORD param_1, int param_2)
	{
		if (!param_1)
			return 0;

		int uVar2;
		int puVar3;
		int uStack4;

		uStack4 = Memory.Read<int>(param_1 + 0x5c + (Memory.Read<byte>(param_1 + 0x58)) * 4);
		puVar3 = param_1 + 0x54;
		uVar2 = Memory.Read<int>(puVar3);
		uStack4 ^= ~uVar2;

		return  ((param_2 & uStack4) != 0);
	}

	DWORD AIMNGR(int param_1)
	{
		if (!param_1)
			return 0;
		int uVar2;
		int puVar3;
		int uStack4;
		int num1 = Offsets::oAiManager;
		uStack4 = Memory.Read<int>(param_1 + (num1 + 8) + (Memory.Read<byte>(param_1 + (num1 + 4))) * 4);
		puVar3 = param_1 + num1;
		uVar2 = Memory.Read<int>(puVar3);
		uStack4 ^= ~uVar2;
		return Memory.Read<DWORD>(uStack4 + 8);
	}

	DWORD aimanager = 0;

protected:

	DWORD base = 0;
	std::wstring name;
	std::string champ;
	std::string summ1;
	std::string summ2;
	short index = 0;
	int team = 0;
	float bounding = -1;

public:

	UnitInfo* unitInfo = GameData::UnknownUnit;
	API::PlayerList* playerAPIInfo = LoLAPI::UnknownPlayer;

	//bool IsTroy()
	//{
	//    return 0;//;IsTroyFunc(base);
	//}
	//ChampSpells spell;

	AiManager* GetAiManager()
	{
		if (!aimanager)
		{
			aimanager = AIMNGR(base);
		}
		if (!aimanager)
			return 0;
		return (AiManager*)(aimanager);
	}

	DWORD Address() const
	{
		return base;
	}
	CObject()
		:base{ 0 }
	{
	}
	CObject(DWORD addr)
		:base{ addr }
	{
	}

	short GetIndex()
	{
		if (!index)
			index = Memory.Read<short>(base + Offsets::oObjIndex, sizeof(short));
		return index;
	}

	int GetTeam()
	{
		if (!team)
			team = Memory.Read<int>(base + Offsets::oObjTeam);
		return team;
	}
	//bool IsEnemy()
	//{
	//	return this->GetTeam() != Local.GetTeam();
	//}
	bool IsVisible()
	{
		return Memory.Read<bool>(base + Offsets::oObjVisibility, sizeof(bool));
	}
	std::wstring GetName()
	{
		if (name.empty())
			name = Memory.ReadWString(base + Offsets::oObjName);
		return name;
	}
	float GetHealth()
	{
		return Memory.Read<float>(base + Offsets::oObjHealth, sizeof(float));
	}
	float GetMaxHealth()
	{
		return Memory.Read<float>(base + Offsets::oObjMaxHealth, sizeof(float));
	}
	float GetHealthPercent()
	{
		return GetHealth() / GetMaxHealth() * 100;
	}
	float GetMana()
	{
		return Memory.Read<float>(base + Offsets::oObjMana, sizeof(float));
	}
	float GetMaxMana()
	{
		return Memory.Read<float>(base + Offsets::oObjMaxMana, sizeof(float));
	}
	float GetArmor()
	{
		return Memory.Read<float>(base + Offsets::oObjArmor, sizeof(float));
	}
	float GetMR()
	{
		return Memory.Read<float>(base + Offsets::oObjMagicRes, sizeof(float));
	}
	float GetMS()
	{
		return Memory.Read<float>(base + Offsets::oObjMoveSpeed, sizeof(float));
	}
	float GetBaseAD()
	{
		return Memory.Read<float>(base + Offsets::oObjBaseAtk, sizeof(float));
	}
	float GetBonusAD()
	{
		return Memory.Read<float>(base + Offsets::oObjBonusAtk, sizeof(float));
	}
	//bool IsAttacking()
	//{
	//    return Memory.Read<int>(base + Offsets::0x2740) > 0; // changes when player is attacking/casting spell
	//}
	float GetTotalAD()
	{
		return GetBaseAD() + GetBonusAD();
	}
	float GetBonusAS()
	{
		return Memory.Read<float>(base + Offsets::oObjAttackSpeed);
	}
	float GetBaseAS()
	{
		return Memory.Read<float>(GetUCIPropertiesInstance() + Offsets::oBaseAttackSpeed);
	}
	float GetAS()
	{
		return GetBonusAS() * GetBaseAS();
	}
	float GetAP()
	{
		return Memory.Read<float>(base + Offsets::oObjAbilityPower, sizeof(float));
	}
	float GetAARange()
	{
		return Memory.Read<float>(base + Offsets::oObjAtkRange, sizeof(float));
	}
	Vector3 GetPosition()
	{
		return Vector3(Memory.Read<float>(base + Offsets::oObjPos, sizeof(float)),
			Memory.Read<float>(base + Offsets::oObjPos + 0x4, sizeof(float)),
			Memory.Read<float>(base + Offsets::oObjPos + 0x8, sizeof(float)));
	}
	CSpellSlot* GetSpellByID(int id)
	{
		DWORD addr = Memory.Read<DWORD>(base + Offsets::oObjSpellBook + 0x478 + (0x4 * id), sizeof(DWORD));
		return (CSpellSlot*)(addr);
	}
	DWORD GetActiveSpellEntry()
	{
		return Memory.Read<DWORD>(base + Offsets::oObjSpellBook + 0x20);
	}
	std::string GetChampName()
	{
		if (champ.empty())
			champ = Memory.ReadString(base + Offsets::oObjChampionName);
		return champ;
	}
	std::string SummonerSpell1()
	{
		if (summ1.empty())
			summ1 = Memory.ReadString(base + Offsets::oObjSummonerSpell1);
		return summ1;
	}
	std::string SummonerSpell2()
	{
		if (summ2.empty())
			summ2 = Memory.ReadString(base + Offsets::oObjSummonerSpell2);
		return summ2;
	}
	std::string KeystoneName()
	{
		return Memory.ReadString(base + Offsets::oObjKeystone1);
	}

	int GetLevel()
	{
		return Memory.Read<int>(base + Offsets::oObjLevel, sizeof(int));
	}
	bool IsDead()
	{
		return this->GetHealth() <= 0.0f;
	}
	bool IsMoving()
	{
		Vector3 pos = GetPosition();
		return pos.x - std::floor(pos.x) != 0;
	}
	int IsRecalling()
	{
		//6 = recall 16= teleport
		return Memory.Read<int>(base + Offsets::oRecallState, sizeof(int));
	}
	float GetEXP()
	{
		return Memory.Read<float>(base + Offsets::oObjEXP, sizeof(float));
	}
	/* float GetGold()
	 {
		 return Memory.Read<float>(base + Offsets::mGoldTotal, sizeof(float));
	 }*/
	float GetLethality()
	{
		return Memory.Read<float>(base + Offsets::oObjFloatLethality, sizeof(float));
	}

	//1.0 is 0% armor pen 25% armor pen is 0.75
	float GetArmorPen()
	{
		return Memory.Read<float>(base + Offsets::oObjPercentArmorPen, sizeof(float));
	}

	float GetTotalDamage(CObject target, float rawdamage = -1)
	{
		float dmg = rawdamage == -1 ? this->GetTotalAD() : rawdamage;
		float lethality = (this->GetLethality() * (0.6 + 0.4 * this->GetLevel() / 18));
		float armorPen = this->GetArmorPen();
		float reducedArmor;
		if (target.GetArmor() <= 0)
		{
			reducedArmor = target.GetArmor();
		}
		else if (target.GetArmor() * armorPen - lethality <= 0)
		{
			reducedArmor = 0;
		}
		else
		{
			reducedArmor = target.GetArmor() * armorPen - lethality;
		}

		if (reducedArmor >= 0)
			return dmg * (100 / (100 + reducedArmor));
		else
			return dmg * (2 - (100 / (100 - reducedArmor)));
	}
	float GetTotalAPDamage(CObject target)
	{
		if (target.GetMR() >= 0)
			return this->GetAP() * (100 / (100 + target.GetMR()));
		else
			return this->GetAP() * (2 - (100 / (100 - target.GetMR())));
	}
	//0.0 to 1.0
	float GetCrit()
	{
		return Memory.Read<float>(base + Offsets::oObjCritChance, sizeof(float));
	}

	// Return the distance from object to another
	float GetDistTo(CObject obj)
	{
		return this->GetPosition().Distance(obj.GetPosition());
	}
	float GetDistTo(Vector3 pos)
	{
		return this->GetPosition().Distance(pos);
	}
	int IsWard()
	{
		if (this->HasUnitTags(Unit_Ward) && !this->HasUnitTags(Unit_Plant))
			return true;
		return false;
	}

	bool CheckState(int state)
	{
		return  ((Memory.Read<int>(base + Offsets::oObjActionState) & state) != 0);
	}

	DWORD GetUnitComponentInfo()
	{
		return Memory.Read<DWORD>(base + Offsets::UnitComponentInfo);
	}
	DWORD GetUCIPropertiesInstance()
	{
		return Memory.Read<DWORD>(this->GetUnitComponentInfo() + Offsets::UCIPropertiesInstance);
	}

	float GetBoundingRadius()
	{
		if (bounding == -1)
		{
			float val = Memory.Read<float>(GetUCIPropertiesInstance() + Offsets::oBoundingRadius);
			if (val > 250.f)
			{
				bounding = 65.f;
				return bounding;
			}
			else
			{
				bounding = val;
				return bounding;
			}
		}
		else
			return bounding;
	}
	float GetSelectionRadius()
	{
		return Memory.Read<float>(GetUCIPropertiesInstance() + Offsets::oSelectionRadius);
	}

	float GetPathingRadius()
	{
		return Memory.Read<float>(GetUCIPropertiesInstance() + Offsets::oPathingRadius);
	}

	bool HasUnitTags(const UnitTag& type1) const
	{
		return unitInfo->tags.test(type1);
	}

	void SetPlayerConsts()
	{
		name = this->GetName();
		champ = this->GetChampName();
		summ1 = this->SummonerSpell1();
		summ2 = this->SummonerSpell2();
		bounding = this->GetBoundingRadius();
		team = this->GetTeam();
		std::string temp = utils->ToLower(champ);
		unitInfo = GameData::GetUnitInfoByName(temp);
		playerAPIInfo = LoLAPI::GetAPIPlayerByChampName(champ);
	}
	void SetObjConsts()
	{
		//team = this->GetTeam();
		champ = this->GetChampName();
		std::string temp = utils->ToLower(champ);
		unitInfo = GameData::GetUnitInfoByName(temp);
	}
	void SetStructureConsts()
	{
		name = this->GetName();
	}
	void SetTurretConsts()
	{
		SetStructureConsts();
		std::string temp = utils->ToLower(Memory.ReadString(base + Offsets::oTurretName));
		unitInfo = GameData::GetUnitInfoByName(temp);
	}
	float GetHpBarHeight() const
	{
		return unitInfo->healthBarHeight;
	}

	unsigned int GetNetworkID()
	{
		return Memory.Read<unsigned int>(base + Offsets::oObjNetworkID);
	}

	inline bool operator == (const CObject& A) const
	{
		if (base == A.base)
			return true;
		else return false;
	}

	inline bool operator == (const DWORD& A) const
	{
		if (base == A)
			return true;
		else return false;
	}

	inline bool operator != (const DWORD& A) const
	{
		if (base != A)
			return true;
		else return false;
	}

	inline bool operator ! () const
	{
		if (base == 0)
			return true;
		return false;
	}

	inline DWORD operator + (DWORD offset) const
	{
		return base + offset;
	}

	bool IsGameObject()
	{
		return IsFunc(base, ObjectType::GameObject);
	}
	bool IsNeutralCamp()
	{
		return IsFunc(base, ObjectType::NeutralCamp);
	}
	bool IsDeadObject()
	{
		return IsFunc(base, ObjectType::DeadObject);
	}
	bool IsInvalidObject()
	{
		return IsFunc(base, ObjectType::InvalidObject);
	}
	bool IsAIBaseCommon()
	{
		return IsFunc(base, ObjectType::AIBaseCommon);
	}
	bool IsAI()
	{
		return IsFunc(base, ObjectType::AI);
	}
	bool IsMinion()
	{
		return IsFunc(base, ObjectType::Minion);
	}
	bool IsHero()
	{
		return IsFunc(base, ObjectType::Hero);
	}
	bool IsTurret()
	{
		return IsFunc(base, ObjectType::Turret);
	}
	bool IsUnknown0()
	{
		return IsFunc(base, ObjectType::Unknown0);
	}
	bool IsMissile()
	{
		return IsFunc(base, ObjectType::Missile);
	}
	bool IsUnknown1()
	{
		return IsFunc(base, ObjectType::Unknown1);
	}
	bool IsBuilding()
	{
		return IsFunc(base, ObjectType::Building);
	}
	bool IsUnknown2()
	{
		return IsFunc(base, ObjectType::Unknown2);
	}
};

inline CObject Local;// (Memory.Read<DWORD>(ClientAddress + oLocalPlayer, sizeof(DWORD)), true);
inline DWORD OBJManager; //OBJManager = Memory.Read<DWORD>(ClientAddress + oObjManager, sizeof(DWORD));
inline DWORD OBJManagerArray; //Memory.Read<DWORD>(OBJManager + 0x14, sizeof(DWORD));
inline DWORD MissileMap;
inline DWORD Renderer;

inline int minimapObj;
inline int minimapHud;

class CObjectManager
{
public:
	static CObject GetObjByIndex(int index)
	{
		if (OBJManagerArray)
		{
			if (index > 3000 && index < 0) //invalid index
				return 0;
			CObject cobj(Memory.Read<DWORD>(OBJManagerArray + (0x4 * index), sizeof(DWORD)));
			if (cobj == 0)
				return 0;

			return cobj;
		}
		return 0;
	}
};

inline std::vector<CObject>g_MinionList;
inline void MinionListLoop()
{
	DWORD dwMinionList = Memory.Read<DWORD>(ClientAddress + Offsets::oMinionList);
	while (true)
	{
		//Read minions to temporary array
		std::vector<CObject>currobjList;
		DWORD dwMinionArray = Memory.Read<DWORD>(dwMinionList + 0x4);
		int nMinionArrayLength = Memory.Read<int>(dwMinionList + 0x8);
		for (int i = 0; i < nMinionArrayLength * 4; i += 4)
		{
			CObject obj(Memory.Read<DWORD>(dwMinionArray + i));
			obj.SetObjConsts();
			// clog.AddLog("%s %x %i", obj.GetName().c_str(), obj.Address(), obj.GetAlive());

			currobjList.emplace_back(obj);
		}
		//We don't want an empty list
		if (!currobjList.empty())
			g_MinionList = currobjList;

		//1s sleep, since the odds of someone doing anything with objects
		//in the first second the spawn is very unlikely
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}

#endif // !_COBJECTMANAGER_H_