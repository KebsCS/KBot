#pragma once

#ifndef _OBJECTMANAGER_H_
#define _OBJECTMANAGER_H_

#include "KInterface.h"
#include "offsets.h"
#include "Definitions.h"

static KInterface Memory(R"(\\.\kbotl)");

static DWORD ClientAddress = Memory.GetClientModule();

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
protected:

	DWORD base = 0;
	DWORD spellbook = 0;
	std::string name;
	std::string champ;
	std::string summ1;
	std::string summ2;
	int team = 0;
	float bounding = -1;

	int wardType = -1;

public:
	//bool IsTroy()
	//{
	//    return 0;//;IsTroyFunc(base);
	//}
	ChampSpells spell;

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

	int GetTeam()
	{
		if (!team)
		{
			team = Memory.Read<int>(base + oObjTeam);
			return team;
		}
		else
			return team;
	}
	bool IsVisible()
	{
		return Memory.Read<bool>(base + oObjVisibility, sizeof(bool));
	}
	std::string GetName()
	{
		if (name.empty())
			return Memory.ReadString(base + oObjName);
		else
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
	DWORD GetSpellByID(int id)
	{
		if (!spellbook)
			spellbook = (oObjSpellBook + 0x478);
		return Memory.Read<DWORD>(base + spellbook + (0x4 * id), sizeof(DWORD));
	}
	DWORD GetSpellCastInfo()
	{
		return Memory.Read<DWORD>(base + oObjSpellBook + 0x20);
	}
	std::string GetChampName()
	{
		if (champ.empty())
			return Memory.ReadString(base + oObjChampionName); //0x3340
		else
			return champ;
	}
	std::string SummonerSpell1()
	{
		if (summ1.empty())
		{
			summ1 = Memory.ReadString(base + oObjSummonerSpell1);
			return summ1;
		}
		else
			return summ1;
	}
	std::string SummonerSpell2()
	{
		if (summ2.empty())
		{
			summ2 = Memory.ReadString(base + oObjSummonerSpell2);
			return summ2;
		}
		else
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
		return this->GetHealth() < 0.01f;
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
				else if (name == "JammerDevice")
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

	void SetPlayerConsts()
	{
		name = this->GetName();
		champ = this->GetChampName();
		summ1 = this->SummonerSpell1();
		summ2 = this->SummonerSpell2();
		bounding = this->GetBoundingRadius();
		team = this->GetTeam();
	}
	void SetObjConsts()
	{
		// team = this->GetTeam();
	}
	void SetStructureConsts()
	{
		name = this->GetName();
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

class CObjectManager
{
public:
	CObject GetObjByIndex(int index)
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

extern CObjectManager* ObjManager;

#endif // !_OBJECTMANAGER_H_