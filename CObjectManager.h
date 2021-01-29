#pragma once

#ifndef _COBJECTMANAGER_H_
#define _COBJECTMANAGER_H_

#include "KInterface.h"
#include "offsets.h"
#include "Definitions.h"
#include "Utils.h"

#include "GameData.h"
#include "CSpellSlot.h"



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


	DWORD AiManager(DWORD addr)
	{
		int v1; // eax
		int8_t v2; // esi
		DWORD v3; // ecx
		DWORD v4; // edi
		int v5; // edx
		DWORD v6; // edx
		int v7; // eax
		int8_t v8; // al
		DWORD v9; // eax
		DWORD v10; // edx
		char v11; // cl
		int v13; // [esp+8h] [ebp-4h]

		v1 = Memory.Read<int>(addr + 12312);

		v2 = addr + 12304;
		v3 = 0;
		v4 = Memory.Read<DWORD>(v2 + 1);
		v5 = Memory.Read<DWORD>(v2 + (4 * v1 + 12));
		v13 = Memory.Read<DWORD>(v2 + (4 * v1 + 12));
		DWORD temp = v13;
		if (v4)
		{
			v6 = (int)(v2 + 4);
			do
			{
				v7 = v6;
				++v6;
				temp = Memory.Read<DWORD>(temp + v3);
				temp ^= ~v7;
				++v3;
			} while (v3 < v4);
			v5 = temp;
		}
		v8 = Memory.Read<DWORD>(v2 + 2);
		if (!v8)
			return Memory.Read<DWORD>(v5 + 8);
		v9 = 4 - v8;
		if (v9 >= 4)
			return Memory.Read<DWORD>(v5 + 8);
		v10 = Memory.Read<DWORD>(v2 + v9 + 4);
		
		do
		{
			v11 = v10++;
			temp = Memory.Read<BYTE>(temp + v9++);
			temp ^= ~v11;
		} while (v9 < 4);
		return Memory.Read<DWORD>(temp + 8);
	}



protected:

	DWORD base = 0;
	std::string name;
	std::string champ;
	std::string summ1;
	std::string summ2;
	short index = 0;
	int team = 0;
	float bounding = -1;

	int wardType = -1;

public:

	UnitInfo* unitInfo = GameData::UnknownUnit;

	//bool IsTroy()
	//{
	//    return 0;//;IsTroyFunc(base);
	//}
	//ChampSpells spell;


	DWORD GetAiManager()
	{
		return AiManager(base);
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
			index = Memory.Read<short>(base + oObjIndex, sizeof(short));
		return index;
	}

	int GetTeam()
	{
		if (!team)
			team = Memory.Read<int>(base + oObjTeam);
		return team;
	}
	//bool IsEnemy()
	//{
	//	return this->GetTeam() != Local.GetTeam();
	//}
	bool IsVisible()
	{
		return Memory.Read<bool>(base + oObjVisibility, sizeof(bool));
	}
	std::string GetName()
	{
		if (name.empty())
			name = Memory.ReadString(base + oObjName);
		return name;
	}
	float GetHealth()
	{
		return Memory.Read<float>(base + oObjHealth, sizeof(float));
	}
	float GetMaxHealth()
	{
		return Memory.Read<float>(base + oObjMaxHealth, sizeof(float));
	}
	float GetHealthPercent()
	{
		return GetHealth() / GetMaxHealth() * 100;
	}
	float GetMana()
	{
		return Memory.Read<float>(base + oObjMana, sizeof(float));
	}
	float GetMaxMana()
	{
		return Memory.Read<float>(base + oObjMaxMana, sizeof(float));
	}
	float GetArmor()
	{
		return Memory.Read<float>(base + oObjArmor, sizeof(float));
	}
	float GetMR()
	{
		return Memory.Read<float>(base + oObjMagicRes, sizeof(float));
	}
	float GetMS()
	{
		return Memory.Read<float>(base + oObjMoveSpeed, sizeof(float));
	}
	float GetBaseAD()
	{
		return Memory.Read<float>(base + oObjBaseAtk, sizeof(float));
	}
	float GetBonusAD()
	{
		return Memory.Read<float>(base + oObjBonusAtk, sizeof(float));
	}
	float GetBonusAS()
	{
		return Memory.Read<float>(base + 0x1284, sizeof(float));
	}
	//bool IsAttacking()
	//{
	//    return Memory.Read<int>(base + 0x2740) > 0; // changes when player is attacking/casting spell
	//}
	float GetTotalAD()
	{
		return GetBaseAD() + GetBonusAD();
	}
	float GetAP()
	{
		return Memory.Read<float>(base + oObjAbilityPower, sizeof(float));
	}
	float GetAARange()
	{
		return Memory.Read<float>(base + oObjAtkRange, sizeof(float));
	}
	Vector3 GetPosition()
	{
		return Vector3(Memory.Read<float>(base + oObjPos, sizeof(float)),
			Memory.Read<float>(base + oObjPos + 0x4, sizeof(float)),
			Memory.Read<float>(base + oObjPos + 0x8, sizeof(float)));
	}
	CSpellSlot* GetSpellByID(int id)
	{
		DWORD addr = Memory.Read<DWORD>(base + oObjSpellBook + 0x478 + (0x4 * id), sizeof(DWORD));
		return (CSpellSlot*)(addr);
	}
	DWORD GetActiveSpellEntry()
	{
		return Memory.Read<DWORD>(base + oObjSpellBook + 0x20);
	}
	std::string GetChampName()
	{
		if (champ.empty())
			champ = Memory.ReadString(base + oObjChampionName); 
		return champ;
	}
	std::string SummonerSpell1()
	{
		if (summ1.empty())
			summ1 = Memory.ReadString(base + oObjSummonerSpell1);
		return summ1;
	}
	std::string SummonerSpell2()
	{
		if (summ2.empty())
			summ2 = Memory.ReadString(base + oObjSummonerSpell2);
		return summ2;
	}
	std::string KeystoneName()
	{
		return Memory.ReadString(base + oObjKeystone1);
	}

	int GetLevel()
	{
		return Memory.Read<int>(base + oObjLevel, sizeof(int));
	}
	bool IsDead()
	{
		return this->GetHealth() <= 0.01f;
	}
	bool IsMoving()
	{
		Vector3 pos = GetPosition();
		return pos.x - std::floor(pos.x) != 0;
	}
	int IsRecalling()
	{
		//6 = recall 16= teleport
		return Memory.Read<int>(base + oRecallState, sizeof(int));
	}
	float GetEXP()
	{
		return Memory.Read<float>(base + oObjEXP, sizeof(float));
	}
	/* float GetGold()
	 {
		 return Memory.Read<float>(base + mGoldTotal, sizeof(float));
	 }*/
	float GetLethality()
	{
		return Memory.Read<float>(base + oObjFloatLethality, sizeof(float));
	}

	//1.0 is 0 armor pen 25% armor pen is 0.75
	float GetArmorPen()
	{
		return Memory.Read<float>(base + oObjPercentArmorPen, sizeof(float));
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
		return Memory.Read<float>(base + oObjCritChance, sizeof(float));
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
		if (wardType == -1)
		{
			float maxHP = this->GetMaxHealth();
			if (maxHP == 1.f || maxHP == 3.f || maxHP == 4.f)
			{
				std::string sName = this->GetName();
				if (maxHP == 3.f)
				{
					wardType = NormalWard;
					return wardType;
				}
				else if (sName == "JammerDevice")
				{
					wardType = ControlWard;
					return wardType;
				}
				else if (maxHP == 1.f && this->GetHealth() == 1.f && sName.find("Plant") == std::string::npos
					&& sName.find("Shen") == std::string::npos && sName.find("Unused") == std::string::npos && sName.find("Honey") == std::string::npos
					&& sName.find("Bard") == std::string::npos && sName.find("Chime") == std::string::npos && sName.find("Nunu") == std::string::npos
					&& sName.find("Ivern") == std::string::npos)
				{
					wardType = BlueWard;
					return wardType;
				}
				else
				{
					wardType = 0; //not a ward
					return wardType;
				}
			}
			else
			{
				wardType = 0; //not a ward
				return wardType;
			}
		}
		else
			return wardType;
	}

	bool CheckState(CharacterState state)
	{
		return  ((Memory.Read<int>(base + oObjActionState) & state) != 0);
	}

	DWORD GetUnitComponentInfo()
	{
		return Memory.Read<DWORD>(base + UnitComponentInfo);
	}
	DWORD GetUCIPropertiesInstance()
	{
		return Memory.Read<DWORD>(this->GetUnitComponentInfo() + UCIPropertiesInstance);
	}

	float GetBoundingRadius()
	{
		if (bounding == -1)
		{
			float val = Memory.Read<float>(GetUCIPropertiesInstance() + oBoundingRadius);
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
		return Memory.Read<float>(GetUCIPropertiesInstance() + oSelectionRadius);
	}

	float GetPathingRadius()
	{
		return Memory.Read<float>(GetUCIPropertiesInstance() + oPathingRadius);
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
		std::string temp = utils->ToLower(Memory.ReadString(base + oTurretName));
		unitInfo = GameData::GetUnitInfoByName(temp);
	}
	float GetHpBarHeight() const
	{
		return unitInfo->healthBarHeight;
	}

	unsigned int GetNetworkID()
	{
		return Memory.Read<unsigned int>(base + oObjNetworkID);
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
	DWORD dwMinionList = Memory.Read<DWORD>(ClientAddress + oMinionList);
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