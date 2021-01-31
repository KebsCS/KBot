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
//ItemInfo* GameData::UnknownItem = new ItemInfo();
std::map<std::string, UnitInfo*>  GameData::Units = {};
std::map<std::string, SpellInfo*> GameData::Spells = {};
//std::map<std::string, Texture2D*> GameData::Images = {};
//std::map<int, ItemInfo*>          GameData::Items = {};

void GameData::Load(std::string& dataFolder)
{
	std::string unitData = dataFolder + "/UnitData.json";
	std::string spellData = dataFolder + "/SpellData.json";
	std::string spellDataCustom = dataFolder + "/SpellDataCustom.json";
	std::string itemData = dataFolder + "/ItemData.json";
	std::string spellIcons = dataFolder + "/icons_spells";
	std::string champIcons = dataFolder + "/icons_champs";
	std::string extraIcons = dataFolder + "/icons_extra";

	//LoadItemData(itemData);

	if (utils->DownloadFile(unitData.substr(dataFolder.size(), unitData.size() - dataFolder.size()), dataFolder))
		LoadUnitData(unitData);

	if (utils->DownloadFile(spellData.substr(dataFolder.size(), spellData.size() - dataFolder.size()), dataFolder))
		LoadSpellData(spellData);

	if (utils->DownloadFile(spellDataCustom.substr(dataFolder.size(), spellDataCustom.size() - dataFolder.size()), dataFolder))
		LoadSpellData(spellDataCustom);

	//LoadIcons(spellIcons);
	//LoadIcons(champIcons);
	//LoadIcons(extraIcons);

	clog.AddLog("[start] Loaded json files");
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

//ItemInfo* GameData::GetItemInfoById(int id)
//{
//	auto it = Items.find(id);
//	if (it != Items.end())
//		return it->second;
//	return UnknownItem;
//}

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

//void GameData::LoadIcons(std::string& path)
//{
//	std::string folder(path);
//	WIN32_FIND_DATAA findData;
//	HANDLE hFind;
//
//
//	int nrFiles = std::distance(filesystem::directory_iterator(path), filesystem::directory_iterator());
//	int nrFile = 0;
//	hFind = FindFirstFileA((folder + "\\*.png").c_str(), &findData);
//	do {
//		if (hFind != INVALID_HANDLE_VALUE) {
//			if (nrFile % 100 == 0)
//				printf("\r	Loading %d/%d      ", nrFile, nrFiles);
//
//			std::string filePath = folder + "/" + findData.cFileName;
//			Texture2D* image = Texture2D::LoadFromFile(Overlay::GetDxDevice(), filePath);
//			if (image == nullptr)
//				printf("Failed to load: %s\n", filePath.c_str());
//			else {
//				std::string fileName(findData.cFileName);
//				fileName.erase(fileName.find(".png"), 4);
//				Images[Character::ToLower(fileName)] = image;
//			}
//		}
//		nrFile++;
//	} while (FindNextFileA(hFind, &findData));
//}

//void GameData::LoadItemData(std::string& path)
//{
//	std::ifstream inputItems;
//	inputItems.open(path);
//
//	if (!inputItems.is_open())
//		throw std::runtime_error("Can't open item data file");
//
//	JsonValue itemsData(inputItems);
//
//	auto items = itemsData.View().AsArray();
//	for (size_t i = 0; i < items.GetLength(); ++i) {
//		auto item = items.GetItem(i).AsObject();
//		ItemInfo* info = new ItemInfo();
//
//		info->movementSpeed = (float)item.GetDouble("movementSpeed");
//		info->health = (float)item.GetDouble("health");
//		info->crit = (float)item.GetDouble("crit");
//		info->abilityPower = (float)item.GetDouble("abilityPower");
//		info->mana = (float)item.GetDouble("mana");
//		info->armour = (float)item.GetDouble("armour");
//		info->magicResist = (float)item.GetDouble("magicResist");
//		info->physicalDamage = (float)item.GetDouble("physicalDamage");
//		info->attackSpeed = (float)item.GetDouble("attackSpeed");
//		info->lifeSteal = (float)item.GetDouble("lifeSteal");
//		info->hpRegen = (float)item.GetDouble("hpRegen");
//		info->movementSpeedPercent = (float)item.GetDouble("movementSpeedPercent");
//		info->cost = (float)item.GetDouble("cost");
//		info->id = item.GetInteger("id");
//
//		Items[info->id] = info;
//	}
//}