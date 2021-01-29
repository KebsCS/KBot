#pragma once
#include <string>
#include "windows.h"
#include <map>
#include "SpellInfo.h"

//TODO change spellslot to DWORD so i can read CSpellSlot spell(obj.getspellbyid) and then this file is useless bcs i can use spelinfo directly from spellslot




/// Type of summoner spell
enum class SummonerSpellType {
	NONE, GHOST, HEAL, BARRIER, EXHAUST, CLARITY, SNOWBALL, FLASH, TELEPORT, CLEANSE, IGNITE, SMITE
};

/// Class that represents a spell some of this data is loaded from disk and the rest is read from memory
class Spell
{

public:
	Spell(SpellSlotID slot) :slot(slot) {}


	float               GetRemainingCooldown(float gameTime);
	const char* GetTypeStr();
	void                LoadFromMem(DWORD base, HANDLE hProcess, bool deepLoad = true);
	void                Trigger();

	bool                HasSpellFlags(SpellFlags flags)   const;
	bool                EqualSpellFlags(SpellFlags flags) const;
	float               GetSpeed()                        const;
	float               GetCastRange()                    const;
	float               GetWidth()                        const;
	float               GetCastRadius()                   const;
	float               GetDelay()                        const;
	float               GetHeight()                       const;
	float               GetTravelTime()                   const;
	std::string         GetIcon()                         const;

public:
	std::string       name;
	SpellSlotID         slot;
	SummonerSpellType summonerSpellType;
	int               level = 0;
	float             readyAt = 0.f;
	float             value = 0.f;

	DWORD             addressSlot;
	SpellInfo* info;

private:
	static BYTE                                        buffer[0x150];
	static const char* spellTypeName[6];
	static const int                                  spellSlotKey[6];
	static std::map<std::string, SummonerSpellType>    summonerSpellTypeDict;
};