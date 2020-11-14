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
    bool alive;
    float timer;
public:

    int GetTeam()
    {
        return Memory.Read<int>(base + 0x4C);
    }
    bool IsVisible()
    {
        return Memory.Read<bool>(base + 0x0270, sizeof(bool));
    }
    std::string GetName()
    {
        return Memory.ReadString(base + 0x006C);
    }
    float GetHealth()
    {
        return Memory.Read<float>(base + mHP, sizeof(float));
    }
    float GetMaxHealth()
    {
        return Memory.Read<float>(base + mMaxHP, sizeof(float));
    }
    float GetMana()
    {
        return Memory.Read<float>(base + 0x298, sizeof(float));
    }
    float GetMaxMana()
    {
        return Memory.Read<float>(base + 0x2A8, sizeof(float));
    }
    float GetArmor()
    {
        return Memory.Read<float>(base + 0x12B0, sizeof(float));
    }
    float GetMR()
    {
        return Memory.Read<float>(base + 0x12B8, sizeof(float));
    }
    float GetMS()
    {
        return Memory.Read<float>(base + 0x12C8, sizeof(float));
    }
    float GetBaseAD()
    {
        return Memory.Read<float>(base + 0x1288, sizeof(float));
    }
    float GetBonusAD()
    {
        return Memory.Read<float>(base + 0x1208, sizeof(float));
    }
    float GetBonusAS()
    {
        return Memory.Read<float>(base + 0x1284, sizeof(float));
    }
    bool IsAttacking()
    {
        return Memory.Read<int>(base + 0x2740) > 0; // changes when player is attacking/casting spell
    }
    float GetTotalAD()
    {
        return GetBaseAD() + GetBonusAD();
    }
    float GetAP()
    {
        return Memory.Read<float>(base + 0x1218, sizeof(float));
    }
    float GetAARange()
    {
        return Memory.Read<float>(base + oObjAttackRange, sizeof(float));
    }
    Vector3 GetPosition()
    {
        return Vector3(Memory.Read<float>(base + 0x220, sizeof(float)),
            Memory.Read<float>(base + 0x220 + 0x4, sizeof(float)),
            Memory.Read<float>(base + 0x220 + 0x8, sizeof(float)));
    }
    DWORD GetSpellByID(int id)
    {
        return Memory.Read<DWORD>(base + (0x2720 + 0x478) + (0x4 * id), sizeof(DWORD));
    }
    std::string GetChampName()
    {
        return Memory.ReadString(base + 0x2F84); //0x3340
    }
    std::string SummonerSpell1()
    {
        return Memory.ReadString(base + mSummonerSpell1);
    }
    std::string SummonerSpell2()
    {
        return Memory.ReadString(base + mSummonerSpell2);
    }
    std::string KeystoneName()
    {
        return Memory.ReadString(base + mKeystone1);
    }

    int GetLevel()
    {
        return Memory.Read<int>(base + 0x3694, sizeof(int)); //0x36BC - skillpoints avaiable
    }
    bool IsDead()
    {
        return this->GetHealth() < 0.01f;
    }
    float GetGold()
    {
        return Memory.Read<float>(base + mGoldTotal, sizeof(float));
    }
    float GetLethality()
    {
        return Memory.Read<float>(base + 0x11CC, sizeof(float));
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
    float GetCrit()
    {
        return Memory.Read<float>(base + 0x12AC, sizeof(float)); //0x1848
    }

    // Return the Distance to your player
    float GetDistToMe(CObject me)
    {
        return this->GetPosition().DistTo(me.GetPosition());
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
            else if (maxHP == 1.f && this->GetHealth() == 1.f && name.find("Plant") == std::string::npos)
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
    bool GetAlive()
    {
        return this->alive;
    }
    void SetAlive(bool state)
    {
        this->alive = state;
    }
    float GetTimer()
    {
        return this->timer;
    }
    void SetTimer(float value)
    {
        this->timer = value;
    }

    CObject(DWORD base)
    {
        this->timer = 0;
        this->alive = true;
        this->base = base;
    }
    int GetNetworkID()
    {
        return Memory.Read<int>(base + oObjNetworkID);
    }
    DWORD Address()
    {
        return base;
    }
    CObject() {}


        //	GameObject = (1 << 0),  //0x1
        //	NeutralCamp = (1 << 1),  //0x2
        //	DeadObject = (1 << 4),  //0x10
        //	InvalidObject = (1 << 5),  //0x20
        //	AIBaseCommon = (1 << 7),  //0x80
        //	AttackableUnit = (1 << 9),  //0x200
        //	AI = (1 << 10), //0x400
        //	Minion = (1 << 11), //0x800
        //	Hero = (1 << 12), //0x1000
        //	Turret = (1 << 13), //0x2000
        //	Unknown0 = (1 << 14), //0x4000
        //	Missile = (1 << 15), //0x8000
        //	Unknown1 = (1 << 16), //0x10000
        //	Building = (1 << 17), //0x20000
        //	Unknown2 = (1 << 18), //0x40000

    bool IsGameObject()
    {
        return IsFunc(base, 0x1);
    }
    bool IsNeutralCamp()
    {
        return IsFunc(base, 0x2);
    }
    bool IsDeadObject()
    {
        return IsFunc(base, 0x10);
    }
    bool IsInvalidObject()
    {
        return IsFunc(base, 0x20);
    }
    bool IsAIBaseCommon()
    {
        return IsFunc(base, 0x80);
    }
    bool IsAI()
    {
        return IsFunc(base, 0x400);
    }
    bool IsMinion()
    {
        return IsFunc(base, 0x800);
    }
    bool IsHero()
    {
        return IsFunc(base, 0x1000);
    }
    bool IsTurret()
    {
        return IsFunc(base, 0x2000);
    }
    bool IsUnknown0()
    {
        return IsFunc(base, 0x4000);
    }
    bool IsMissile()
    {
        return IsFunc(base, 0x8000);
    }
    bool IsUnknown1()
    {
        return IsFunc(base, 0x10000);
    }
    bool IsBuilding()
    {
        return IsFunc(base, 0x20000);
    }
    bool IsUnknown2()
    {
        return IsFunc(base, 0x40000);
    }

    static int GetUnderMouseObject() //todo apparently a pointer to pointer
    {
        int address = Memory.Read<DWORD>(ClientAddress + oUnderMouseObject);
        CObject under_mouse_object;

        if (address > 0)
            under_mouse_object = Memory.Read<DWORD>(address);


        return address;
    }

   

};


const DWORD LocalPlayer = Memory.Read<DWORD>(ClientAddress + oLocalPlayer, sizeof(DWORD));
static CObject Local(LocalPlayer);




#endif // !_OBJECTMANAGER_H_