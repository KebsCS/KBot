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
    
   


    DWORD base;
    std::string name;
    std::string champ;
    std::string summ1;
    std::string summ2;
    int team;
    float bounding;
    
  //  bool alive;
  //  float timer;
    
public:
    //bool IsTroy()
    //{
    //    return 0;//;IsTroyFunc(base);
    //}

    int GetTeam()
    {
        if (!team || team < 0)
            return Memory.Read<int>(base + oObjTeam);
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
        return Memory.Read<DWORD>(base + (oObjSpellBook + /*0x440*/ 0x478) + (0x4 * id), sizeof(DWORD));
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
            return Memory.ReadString(base + mSummonerSpell1);
        else
            return summ1;
    }
    std::string SummonerSpell2()
    {
        if (summ2.empty())
            return Memory.ReadString(base + mSummonerSpell2);
        else
            return summ2;
    }
    std::string KeystoneName()
    {
        return Memory.ReadString(base + mKeystone1);
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
    float GetFlatAmorPen()
    {
        return Memory.Read<float>(base + oObjPercentArmorPen, sizeof(float));
    }
    float GetTotalDamage(CObject* target)
    {

        float lethality = (this->GetLethality() * (0.6 + 0.4 * this->GetLevel() / 18));
        float reducedArmor;
        if (target->GetArmor() <= 0)
        {
            reducedArmor = target->GetArmor();
        }
        else if (target->GetArmor() - lethality <= 0)
        {
            reducedArmor = 0;
        }
        else
        {
            reducedArmor = target->GetArmor() - lethality;
        }


        if (target->GetArmor() >= 0)
            return this->GetTotalAD() * (100 / (100 + reducedArmor));
        else
            return this->GetTotalAD() * (2 - (100 / (100 - reducedArmor)));
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

    // Return the Distance to your player
    float GetDistTo(CObject obj)
    {
        return this->GetPosition().DistTo(obj.GetPosition());
    }

    bool IsLasthitable(CObject me)
    {

        return this->GetHealth() <= GetTotalDamage(this);
    }
    int IsWard()
    {
        float maxHP = this->GetMaxHealth();
        if (maxHP == 1.f || maxHP == 3.f || maxHP == 4.f)
        {
            std::string name = this->GetName();
            if (maxHP == 3.f)
                return 1;
            else if (name == "JammerDevice")
                return 2;
            else if (maxHP == 1.f && this->GetHealth() == 1.f && name.find("Plant") == std::string::npos && name.find("Shen") == std::string::npos)
                return 3;
            else return 0;
        }
        return 0;
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
   /* bool GetAlive() const
    {
        return this->alive;
    }
    void SetAlive(bool state)
    {
        this->alive = state;
    }
    float GetTimer() const 
    {
        return this->timer;
    }
    void SetTimer(float value)
    {
        this->timer = value;
    }*/
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
        if (!bounding || bounding <0.f)
        {
            float val = Memory.Read<float>(GetUCIPropertiesInstance() + 0x454);
            if (val > 250.f)
                return 65.f;
            else
                return val;
        }
        else
            return bounding;
    }
    float GetSelectionRadius()
    {
        return Memory.Read<float>(GetUCIPropertiesInstance() + 0x44C);
    }

    float GetPathingRadius()
    {
        return Memory.Read<float>(GetUCIPropertiesInstance() + 0x450);
    }


    CObject(DWORD addr) 
        :base{ addr }//, alive{ true }, timer{ 0 }
    {

    }
    CObject(DWORD addr, bool isLocal)
        :base{ addr }
    {
        SetPlayerConsts();
    }
    void SetPlayerConsts()
    {
        name = this->GetName();
        champ = this->GetChampName();
        summ1 = this->SummonerSpell1();
        summ2 = this->SummonerSpell2();
        bounding = this->GetBoundingRadius();
        SetObjConsts();
    }
    void SetObjConsts()
    {
        team = this->GetTeam();
    }
    void SetStructureConsts()
    {
        name = this->GetName();
        SetObjConsts();
    }
    int GetNetworkID()
    {
        return Memory.Read<int>(base + oObjNetworkID);
    }
    DWORD Address() const
    {
        return base;
    }
    CObject() 
        :base{ 0 }//, alive{ true }, timer{ 0 }
    {
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

    //static int GetUnderMouseObject() //todo apparently a pointer to pointer
    //{
    //    int address = Memory.Read<DWORD>(ClientAddress + oUnderMouseObject);
    //    CObject under_mouse_object;

    //    if (address > 0)
    //        under_mouse_object = Memory.Read<DWORD>(address);


    //    return address;
    //}

   

};



static CObject Local(Memory.Read<DWORD>(ClientAddress + oLocalPlayer, sizeof(DWORD)), true);
    






#endif // !_OBJECTMANAGER_H_