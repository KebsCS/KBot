#include "GameData.h"
#include <fstream>
#include <filesystem>
#include "Utils.h"
#include "DirectX.h"

#include <aws/core/utils/json/JsonSerializer.h>

//using namespace Aws::Utils::Json;
using namespace std::experimental;

UnitInfo* GameData::UnknownUnit = new UnitInfo();
SpellInfo* GameData::UnknownSpell = new SpellInfo();
std::map<std::string, UnitInfo*>  GameData::Units = {};
std::map<std::string, SpellInfo*> GameData::Spells = {};

bool GameData::Load(std::string& dataFolder)
{
	std::string unitData = dataFolder + "/UnitData.json";
	std::string spellData = dataFolder + "/SpellData.json";
	std::string spellDataCustom = dataFolder + "/SpellDataCustom.json";
	std::string itemData = dataFolder + "/ItemData.json";
	std::string spellIcons = dataFolder + "/icons_spells";
	std::string champIcons = dataFolder + "/icons_champs";
	std::string extraIcons = dataFolder + "/icons_extra";

	if (!utils->DownloadFile(unitData.substr(dataFolder.size(), unitData.size() - dataFolder.size()), dataFolder))
		return false;
	LoadUnitData(unitData);

	if (!utils->DownloadFile(spellData.substr(dataFolder.size(), spellData.size() - dataFolder.size()), dataFolder))
		return false;
	LoadSpellData(spellData);

	if (!utils->DownloadFile(spellDataCustom.substr(dataFolder.size(), spellDataCustom.size() - dataFolder.size()), dataFolder))
		return false;
	LoadSpellData(spellDataCustom);

	clog.AddLog("[start] Loaded json files");
	return true;
}

UnitInfo* GameData::GetUnitInfoByName(std::string& name)
{
	auto it = Units.find(name);
	if (it != Units.end())
		return it->second;
	return UnknownUnit;
}

SpellInfo* GameData::GetSpellInfoByName(std::string& name)
{
	auto it = Spells.find(name);
	if (it != Spells.end())
		return it->second;
	return UnknownSpell;
}

void GameData::LoadUnitData(std::string& path)
{
	std::ifstream inputUnitData;
	inputUnitData.open(path);

	if (!inputUnitData.is_open())
		abort();

	Aws::Utils::Json::JsonValue unitData(inputUnitData);

	auto units = unitData.View().AsArray();
	for (size_t i = 0; i < units.GetLength(); ++i) {
		auto unitObj = units.GetItem(i).AsObject();

		UnitInfo* unit = new UnitInfo();
		unit->acquisitionRange = (float)unitObj.GetDouble("acquisitionRange");
		unit->attackSpeedRatio = (float)unitObj.GetDouble("attackSpeedRatio");
		unit->baseAttackRange = (float)unitObj.GetDouble("attackRange");
		unit->baseAttackSpeed = (float)unitObj.GetDouble("attackSpeed");
		unit->baseMovementSpeed = (float)unitObj.GetDouble("baseMoveSpeed");
		unit->basicAttackMissileSpeed = (float)unitObj.GetDouble("basicAtkMissileSpeed");
		unit->basicAttackWindup = (float)unitObj.GetDouble("basicAtkWindup");
		unit->gameplayRadius = (float)unitObj.GetDouble("gameplayRadius");
		unit->healthBarHeight = (float)unitObj.GetDouble("healthBarHeight");
		unit->name = utils->ToLower(std::string(unitObj.GetString("name").c_str()));
		unit->pathRadius = (float)unitObj.GetDouble("pathingRadius");
		unit->selectionRadius = (float)unitObj.GetDouble("selectionRadius");

		auto tags = unitObj.GetArray("tags");
		for (size_t j = 0; j < tags.GetLength(); ++j)
			unit->SetTag(tags.GetItem(j).AsString().c_str());

		Units[unit->name] = unit;
	}
}
void GameData::LoadSpellData(std::string& path)
{
	std::ifstream inputSpellData;
	inputSpellData.open(path);

	if (!inputSpellData.is_open())
		abort();

	Aws::Utils::Json::JsonValue spellData(inputSpellData);

	auto spells = spellData.View().AsArray();
	for (size_t i = 0; i < spells.GetLength(); ++i)
	{
		auto spell = spells.GetItem(i).AsObject();

		SpellInfo* info = new SpellInfo();
		info->flags = (SpellFlags)spell.GetInteger("flags");
		info->delay = (float)spell.GetDouble("delay");
		info->height = (float)spell.GetDouble("height");
		info->icon = utils->ToLower(std::string(spell.GetString("icon").c_str()));
		info->name = utils->ToLower(std::string(spell.GetString("name").c_str()));
		info->width = (float)spell.GetDouble("width");
		info->castRange = (float)spell.GetDouble("castRange");
		info->castRadius = (float)spell.GetDouble("castRadius");
		info->speed = (float)spell.GetDouble("speed");
		info->travelTime = (float)spell.GetDouble("travelTime");
		info->flags = (SpellFlags)(info->flags | (spell.GetBool("projectDestination") ? ProjectedDestination : 0));

		Spells[info->name] = info;
	}
}