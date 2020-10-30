#pragma once

#include "KInterface.h"
#include "offsets.h"
#include "Definitions.h"

static KInterface Driver(R"(\\.\kbotl)");




//class CObject;
class Obj_AI_Base;





class CObject
{
private:
    
    DWORD base;
    DWORD ProcessId = Driver.GetProcessId();
public:

   

    int GetIndex()
    {
        return Driver.ReadVirtualMemory<int>(ProcessId, base + 0x0020, sizeof(int));
    }
    int GetTeam()
    {
        return Driver.ReadVirtualMemory<int>(ProcessId, base + 0x4C, sizeof(int));
    }
    bool IsVisible()
    {
        return Driver.ReadVirtualMemory<bool>(ProcessId, base + 0x0270, sizeof(bool));
    }
    std::string GetName()
    {
        return Driver.ReadStringMemory(ProcessId, base + 0x006C, 16);
    }
	float GetHealth()
	{
		return Driver.ReadVirtualMemory<float>(ProcessId,base + 0xDC4   , sizeof(float));
	}
    float GetMaxHealth()
    {
        return Driver.ReadVirtualMemory<float>(ProcessId, base + 0xDD4, sizeof(float));
    }
    float GetMana()
    {
        return Driver.ReadVirtualMemory<float>(ProcessId, base + 0x298, sizeof(float));
    }
    float GetMaxMana()
    {
        return Driver.ReadVirtualMemory<float>(ProcessId, base+ 0x2A8, sizeof(float));
    }
    float GetArmor()
    {
        return Driver.ReadVirtualMemory<float>(ProcessId, base + 0x12B0, sizeof(float));
    }
    float GetMR()
    {
        return Driver.ReadVirtualMemory<float>(ProcessId, base + 0x12B8, sizeof(float));
    }
    float GetMS()
    {
        return Driver.ReadVirtualMemory<float>(ProcessId, base + 0x12C8, sizeof(float));
    }
    float GetBaseAD()
    {
        return Driver.ReadVirtualMemory<float>(ProcessId, base + 0x1288, sizeof(float));
    }
    float GetBonusAD()
    {
        return Driver.ReadVirtualMemory<float>(ProcessId, base + 0x1208, sizeof(float));
    }
    float GetAP()
    {
        return Driver.ReadVirtualMemory<float>(ProcessId, base + 0x1218, sizeof(float));
    }
    float GetAARange()
    {
        return Driver.ReadVirtualMemory<float>(ProcessId, base + oObjAttackRange, sizeof(float));
    }
    Vector3 GetPosition()
    {
        return Vector3(Driver.ReadVirtualMemory<float>(ProcessId, base + 0x220, sizeof(float)),
            Driver.ReadVirtualMemory<float>(ProcessId, base + 0x220 + 0x4, sizeof(float)),
            Driver.ReadVirtualMemory<float>(ProcessId, base + 0x220 + 0x8, sizeof(float)));
    }
    DWORD GetSpellbook()
    {
        return Driver.ReadVirtualMemory<DWORD>(ProcessId, base + 0x2720, sizeof(DWORD));
    }
    char* ChampName()
    {
        //return Driver.ReadStringMemory(ProcessId, base + 0x2F84, 32);  //0x3340
        char NAME[32];
        for (int i = 0; i < 32; i++)
        {
            auto c = Driver.ReadVirtualMemory<char>(ProcessId, base + 0x2F84 + (i) * sizeof(char), sizeof(char)); 
            if (c == '\0')
                NAME[i] = ' '; NAME[i] = c;
        }

        return NAME;

    }
    std::string SummonerSpell1()
    {
        return Driver.ReadStringMemory(ProcessId, base + 0x3B58, 32);
    }
    std::string SummonerSpell2()
    {
        return Driver.ReadStringMemory(ProcessId, base + 0x3B64, 16);
    }
    std::string KeystoneName()
    {
        return Driver.ReadStringMemory(ProcessId, base + 0x3B78, 16);
    }
    int GetLevel() // 1 lower by actual level
    {
        return (Driver.ReadVirtualMemory<int>(ProcessId, base + 0x36BC, sizeof(int)));
    }
    bool IsHero()
    {
        return Driver.ReadVirtualMemory<bool>(ProcessId, base + oIsHero, sizeof(bool));
    }

    bool IsAlive()
    {
        return Driver.ReadVirtualMemory<bool>(ProcessId, base + oIsAlive, sizeof(bool));
    }


	CObject(DWORD base)
	{
		this->base = base;
	}


};

