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
        return Memory.Read<float>(base+ 0x2A8, sizeof(float));
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
        return Memory.ReadString(base + 0x3B50);
    }
    std::string SummonerSpell2()
    {
        return Memory.ReadString(base + 0x3B5C);
    }
    std::string KeystoneName()
    {
        return Memory.ReadString(base + 0x3B7C);
    }

    int GetLevel() 
    {
        return Memory.Read<int>(base + 0x3694, sizeof(int)); //0x36BC - skillpoints avaiable
    }
    bool IsDead()
    {
        return this->GetHealth() <= 0.01f;
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


        if(target->GetArmor() >=0)
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
    bool IsWard()
    {
        return this->GetMaxHealth() == 3.f || this->GetName() == "JammerDevice";
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
    DWORD Address()
    {
        return base;
    }


};


static DWORD LocalPlayer = Memory.Read<DWORD>(ClientAddress + oLocalPlayer, sizeof(DWORD));
static CObject Local(LocalPlayer);


#endif // !_OBJECTMANAGER_H_