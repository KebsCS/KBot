#pragma once

#ifndef _GAMEDATA_H_
#define _GAMEDATA_H_

#include <map>
#include "UnitInfo.h"
#include "SpellInfo.h"

class GameData
{
public:
	static bool Load(std::string& dataFolder);
	static UnitInfo* GetUnitInfoByName(std::string& name);
	static SpellInfo* GetSpellInfoByName(std::string& name);
private:
	static void LoadUnitData(std::string& path);
	static void LoadSpellData(std::string& path);

public:
	static UnitInfo* UnknownUnit;
	static SpellInfo* UnknownSpell;

	static std::map<std::string, UnitInfo*>  Units;
	static std::map<std::string, SpellInfo*> Spells;
};

#endif