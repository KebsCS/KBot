#include "LoLAPI.h"
#include "HTTP.h"
//GetObject is defined in wingdi.h and it causes conflicts with json reader
#undef GetObject
#include <aws/core/utils/json/JsonSerializer.h>

API::PlayerList* LoLAPI::UnknownPlayer = new API::PlayerList();
std::map<std::string, API::PlayerList*>  LoLAPI::Players = {};

void LoLAPI::LoadPlayerListData()
{
	HTTP http;
	std::string s = http.GET("https://127.0.0.1", "/liveclientdata/playerlist", 2999);
	Aws::String aws_s(s.c_str(), s.size());

	Aws::Utils::Json::JsonValue PlayerInfo(aws_s);

	auto players = PlayerInfo.View().AsArray();
	for (size_t i = 0; i < players.GetLength(); ++i)
	{
		auto playerObj = players.GetItem(i).AsObject();

		API::PlayerList* player = new API::PlayerList(); 
		player->championName = std::string(playerObj.GetString("championName").c_str());
		player->isBot = playerObj.GetBool("isBot");
		player->isDead = playerObj.GetBool("isDead");
		auto items = playerObj.GetArray("items");
		for (size_t j = 0; j < items.GetLength(); ++j)
		{
			auto itemObj = items.GetItem(j).AsObject();
			API::Item item;
			item.canUse = itemObj.GetBool("canUse");
			item.consumable = itemObj.GetBool("consumable");
			item.count = itemObj.GetInteger("count");
			item.displayName = std::string(itemObj.GetString("displayName").c_str());
			item.itemID = itemObj.GetInteger("itemID");
			item.price = itemObj.GetInteger("price");
			item.rawDescription = std::string(itemObj.GetString("rawDescription").c_str());
			item.rawDisplayName = std::string(itemObj.GetString("rawDisplayName").c_str());
			item.slot = itemObj.GetInteger("slot");
			player->items.emplace_back(item);
		}
		player->level = playerObj.GetInteger("level");
		player->position = std::string(playerObj.GetString("position").c_str());
		player->rawChampionName = std::string(playerObj.GetString("rawChampionName").c_str());
		player->rawSkinName = std::string(playerObj.GetString("rawSkinName").c_str());
		player->respawnTimer = (float)playerObj.GetDouble("respawnTimer");

		if (player->championName != "TargetDummy")
		{
			auto runesObj = playerObj.GetObject("runes");
			auto keystoneObj = runesObj.GetObject("keystone");
			player->runes.keystone.displayName = std::string(keystoneObj.GetString("displayName").c_str());
			player->runes.keystone.id = keystoneObj.GetInteger("id");
			player->runes.keystone.rawDescription = std::string(keystoneObj.GetString("rawDescription").c_str());
			player->runes.keystone.rawDisplayName = std::string(keystoneObj.GetString("rawDisplayName").c_str());

			auto primaryRuneTreeObj = runesObj.GetObject("primaryRuneTree");
			player->runes.primaryRuneTree.displayName = std::string(primaryRuneTreeObj.GetString("displayName").c_str());
			player->runes.primaryRuneTree.id = primaryRuneTreeObj.GetInteger("id");
			player->runes.primaryRuneTree.rawDescription = std::string(primaryRuneTreeObj.GetString("rawDescription").c_str());
			player->runes.primaryRuneTree.rawDisplayName = std::string(primaryRuneTreeObj.GetString("rawDisplayName").c_str());

			auto secondaryRuneTreeObj = runesObj.GetObject("secondaryRuneTree");
			player->runes.secondaryRuneTree.displayName = std::string(secondaryRuneTreeObj.GetString("displayName").c_str());
			player->runes.secondaryRuneTree.id = secondaryRuneTreeObj.GetInteger("id");
			player->runes.secondaryRuneTree.rawDescription = std::string(secondaryRuneTreeObj.GetString("rawDescription").c_str());
			player->runes.secondaryRuneTree.rawDisplayName = std::string(secondaryRuneTreeObj.GetString("rawDisplayName").c_str());
		}

		auto scoresObj = playerObj.GetObject("scores");
		player->scores.assists = scoresObj.GetInteger("assists");
		player->scores.creepScore = scoresObj.GetInteger("creepScore");
		player->scores.deaths = scoresObj.GetInteger("deaths");
		player->scores.kills = scoresObj.GetInteger("kills");
		player->scores.wardScore = (float)scoresObj.GetDouble("wardScore");

		player->skinID = playerObj.GetInteger("skinID");
		player->skinName = std::string(playerObj.GetString("skinName").c_str());
		player->summonerName = std::string(playerObj.GetString("summonerName").c_str());

		auto summonerSpellsObj = playerObj.GetObject("summonerSpells");
		if (player->championName != "TargetDummy")
		{
			auto summonerSpellOneeObj = summonerSpellsObj.GetObject("summonerSpellOne");
			player->summonerSpells.summonerSpellOne.displayName = std::string(summonerSpellOneeObj.GetString("displayName").c_str());
			player->summonerSpells.summonerSpellOne.rawDescription = std::string(summonerSpellOneeObj.GetString("rawDescription").c_str());
			player->summonerSpells.summonerSpellOne.rawDisplayName = std::string(summonerSpellOneeObj.GetString("rawDisplayName").c_str());

			auto summonerSpellTwoObj = summonerSpellsObj.GetObject("summonerSpellTwo");
			player->summonerSpells.summonerSpellTwo.displayName = std::string(summonerSpellTwoObj.GetString("displayName").c_str());
			player->summonerSpells.summonerSpellTwo.rawDescription = std::string(summonerSpellTwoObj.GetString("rawDescription").c_str());
			player->summonerSpells.summonerSpellTwo.rawDisplayName = std::string(summonerSpellTwoObj.GetString("rawDisplayName").c_str());
		}
		player->team = std::string(playerObj.GetString("team").c_str());

		Players[player->rawChampionName.substr(strlen("game_character_displayname_"))] = player;
	}
}

API::PlayerList* LoLAPI::GetAPIPlayerByChampName(std::string& name)
{
	auto it = Players.find(name);
	if (it != Players.end())
		return it->second;
	return UnknownPlayer;
}

API::GameStats LoLAPI::GetGameStats()
{
	HTTP http;
	std::string s = http.GET("https://127.0.0.1", "/liveclientdata/gamestats", 2999);
	Aws::String aws_s(s.c_str(), s.size());
	Aws::Utils::Json::JsonValue GameStats(aws_s);

	auto statsObj = GameStats.View().AsObject();

	return {
	std::string(statsObj.GetString("gameMode").c_str()),
	(float)statsObj.GetDouble("gameTime"),
	std::string(statsObj.GetString("mapName").c_str()),
	statsObj.GetInteger("mapNumber"),
	std::string(statsObj.GetString("mapTerrain").c_str())
	};
}

std::vector<API::Event> LoLAPI::GetEvents()
{
	HTTP http;
	std::string s = http.GET("https://127.0.0.1", "/liveclientdata/eventdata", 2999);
	Aws::String aws_s(s.c_str(), s.size());
	Aws::Utils::Json::JsonValue Events(aws_s);

	std::vector<API::Event>temp;
	auto events = Events.View().GetObject("Events").AsArray();
	for (size_t i = 0; i < events.GetLength(); ++i)
	{
		auto eventObj = events.GetItem(i).AsObject();
		API::Event event;
		auto objects = eventObj.GetAllObjects();

		for (auto o : objects)
		{
			if (o.first == "Assisters")
			{
				auto assisters = eventObj.GetArray("Assisters");
				for (size_t j = 0; j < assisters.GetLength(); ++j)
				{
					event.Assisters.emplace_back(assisters.GetItem(j).AsString());
				}
			}
		}
		event.EventID = eventObj.GetInteger("EventID");
		event.EventName = std::string(eventObj.GetString("EventName").c_str());
		event.EventTime = (float)eventObj.GetDouble("EventTime");
		event.KillStreak = eventObj.GetInteger("KillStreak");
		event.KillerName = std::string(eventObj.GetString("KillerName").c_str());
		event.VictimName = std::string(eventObj.GetString("VictimName").c_str());
		temp.emplace_back(event);
	}

	return temp;
}

API::ActivePlayer LoLAPI::GetActivePlayer()
{
	HTTP http;
	std::string s = http.GET("https://127.0.0.1", "/liveclientdata/activeplayer", 2999);
	Aws::String aws_s(s.c_str(), s.size());
	Aws::Utils::Json::JsonValue ActivePlayer(aws_s);

	API::ActivePlayer player;

	auto info = ActivePlayer.View().AsObject();
	info.GetAllObjects();
	auto abilities = info.GetObject("abilities");
	abilities.GetAllObjects();
	auto passive = abilities.GetObject("Passive");
	player.abilities.Passive = {
		0,
		std::string(passive.GetString("displayName").c_str()),
		std::string(passive.GetString("id").c_str()),
		std::string(passive.GetString("rawDescription").c_str()),
		std::string(passive.GetString("rawDisplayName").c_str())
	};
	auto Q = abilities.GetObject("Q");
	player.abilities.Q = {
		Q.GetInteger("abilityLevel"),
		std::string(Q.GetString("displayName").c_str()),
		std::string(Q.GetString("id").c_str()),
		std::string(Q.GetString("rawDescription").c_str()),
		std::string(Q.GetString("rawDisplayName").c_str())
	};
	auto W = abilities.GetObject("W");
	player.abilities.W = {
		W.GetInteger("abilityLevel"),
		std::string(W.GetString("displayName").c_str()),
		std::string(W.GetString("id").c_str()),
		std::string(W.GetString("rawDescription").c_str()),
		std::string(W.GetString("rawDisplayName").c_str())
	};
	auto E = abilities.GetObject("E");
	player.abilities.E = {
		E.GetInteger("abilityLevel"),
		std::string(E.GetString("displayName").c_str()),
		std::string(E.GetString("id").c_str()),
		std::string(E.GetString("rawDescription").c_str()),
		std::string(E.GetString("rawDisplayName").c_str())
	};
	auto R = abilities.GetObject("R");
	player.abilities.R = {
		R.GetInteger("abilityLevel"),
		std::string(R.GetString("displayName").c_str()),
		std::string(R.GetString("id").c_str()),
		std::string(R.GetString("rawDescription").c_str()),
		std::string(R.GetString("rawDisplayName").c_str())
	};

	auto championStats = info.GetObject("championStats");
	player.championStats = {
		(float)championStats.GetDouble("abilityPower"),
		(float)championStats.GetDouble("armor"),
		(float)championStats.GetDouble("armorPenetrationFlat"),
		(float)championStats.GetDouble("armorPenetrationPercent"),
		(float)championStats.GetDouble("attackDamage"),
		(float)championStats.GetDouble("attackRange"),
		(float)championStats.GetDouble("attackSpeed"),
		(float)championStats.GetDouble("bonusArmorPenetrationPercent"),
		(float)championStats.GetDouble("bonusMagicPenetrationPercent"),
		(float)championStats.GetDouble("cooldownReduction"),
		(float)championStats.GetDouble("critChance"),
		(float)championStats.GetDouble("critDamage"),
		(float)championStats.GetDouble("currentHealth"),
		(float)championStats.GetDouble("healthRegenRate"),
		(float)championStats.GetDouble("lifeSteal"),
		(float)championStats.GetDouble("magicLethality"),
		(float)championStats.GetDouble("magicPenetrationFlat"),
		(float)championStats.GetDouble("magicPenetrationPercent"),
		(float)championStats.GetDouble("magicResist"),
		(float)championStats.GetDouble("maxHealth"),
		(float)championStats.GetDouble("moveSpeed"),
		(float)championStats.GetDouble("physicalLethality"),
		(float)championStats.GetDouble("resourceMax"),
		(float)championStats.GetDouble("resourceRegenRate"),
		std::string(championStats.GetString("resourceType").c_str()),
		(float)championStats.GetDouble("resourceValue"),
		(float)championStats.GetDouble("spellVamp"),
		(float)championStats.GetDouble("tenacity"),
	};

	player.currentGold = (float)info.GetDouble("currentGold");

	auto runesObj = info.GetObject("fullRunes");
	auto generalRunesObj = runesObj.GetObject("generalRunes").AsArray();
	std::vector<API::Rune> generalRunes;
	for (size_t j = 0; j < generalRunesObj.GetLength(); ++j)
	{
		auto runeObj = generalRunesObj.GetItem(j).AsObject();
		API::Rune rune;
		rune.displayName = std::string(runeObj.GetString("displayName").c_str());
		rune.id = runeObj.GetInteger("id");
		rune.rawDescription = std::string(runeObj.GetString("rawDescription").c_str());
		rune.rawDisplayName = std::string(runeObj.GetString("rawDisplayName").c_str());
		generalRunes.emplace_back(rune);
	}
	player.fullRunes.generalRunes = generalRunes;

	auto keystoneObj = runesObj.GetObject("keystone");
	player.fullRunes.keystone.displayName = std::string(keystoneObj.GetString("displayName").c_str());
	player.fullRunes.keystone.id = keystoneObj.GetInteger("id");
	player.fullRunes.keystone.rawDescription = std::string(keystoneObj.GetString("rawDescription").c_str());
	player.fullRunes.keystone.rawDisplayName = std::string(keystoneObj.GetString("rawDisplayName").c_str());

	auto primaryRuneTreeObj = runesObj.GetObject("primaryRuneTree");
	player.fullRunes.primaryRuneTree.displayName = std::string(primaryRuneTreeObj.GetString("displayName").c_str());
	player.fullRunes.primaryRuneTree.id = primaryRuneTreeObj.GetInteger("id");
	player.fullRunes.primaryRuneTree.rawDescription = std::string(primaryRuneTreeObj.GetString("rawDescription").c_str());
	player.fullRunes.primaryRuneTree.rawDisplayName = std::string(primaryRuneTreeObj.GetString("rawDisplayName").c_str());

	auto secondaryRuneTreeObj = runesObj.GetObject("secondaryRuneTree");
	player.fullRunes.secondaryRuneTree.displayName = std::string(secondaryRuneTreeObj.GetString("displayName").c_str());
	player.fullRunes.secondaryRuneTree.id = secondaryRuneTreeObj.GetInteger("id");
	player.fullRunes.secondaryRuneTree.rawDescription = std::string(secondaryRuneTreeObj.GetString("rawDescription").c_str());
	player.fullRunes.secondaryRuneTree.rawDisplayName = std::string(secondaryRuneTreeObj.GetString("rawDisplayName").c_str());

	auto statRunesObj = runesObj.GetObject("statRunes").AsArray();
	std::vector<API::StatRune> statRunes;
	for (size_t j = 0; j < statRunesObj.GetLength(); ++j)
	{
		auto statruneObj = statRunesObj.GetItem(j).AsObject();
		API::StatRune statrune;
		statrune.id = statruneObj.GetInteger("id");
		statrune.rawDescription = std::string(statruneObj.GetString("rawDescription").c_str());
		statRunes.emplace_back(statrune);
	}
	player.fullRunes.statRunes = statRunes;

	player.level = info.GetInteger("level");
	player.summonerName = std::string(info.GetString("summonerName").c_str());

	return player;
}

API::AllGameData LoLAPI::GetAllGameData()
{
	API::AllGameData AllGameData;

	HTTP http;
	std::string s = http.GET("https://127.0.0.1", "/liveclientdata/allgamedata", 2999);
	Aws::String aws_s(s.c_str(), s.size());
	Aws::Utils::Json::JsonValue allgamedata(aws_s);

	API::ActivePlayer player;

	auto info = allgamedata.View().AsObject();

	{
		API::ActivePlayer player;
		auto activePlayer = info.GetObject("activePlayer");
		auto abilities = activePlayer.GetObject("abilities");
		auto passive = abilities.GetObject("Passive");
		player.abilities.Passive = {
			0,
			std::string(passive.GetString("displayName").c_str()),
			std::string(passive.GetString("id").c_str()),
			std::string(passive.GetString("rawDescription").c_str()),
			std::string(passive.GetString("rawDisplayName").c_str())
		};
		auto Q = abilities.GetObject("Q");
		player.abilities.Q = {
			Q.GetInteger("abilityLevel"),
			std::string(Q.GetString("displayName").c_str()),
			std::string(Q.GetString("id").c_str()),
			std::string(Q.GetString("rawDescription").c_str()),
			std::string(Q.GetString("rawDisplayName").c_str())
		};
		auto W = abilities.GetObject("W");
		player.abilities.W = {
			W.GetInteger("abilityLevel"),
			std::string(W.GetString("displayName").c_str()),
			std::string(W.GetString("id").c_str()),
			std::string(W.GetString("rawDescription").c_str()),
			std::string(W.GetString("rawDisplayName").c_str())
		};
		auto E = abilities.GetObject("E");
		player.abilities.E = {
			E.GetInteger("abilityLevel"),
			std::string(E.GetString("displayName").c_str()),
			std::string(E.GetString("id").c_str()),
			std::string(E.GetString("rawDescription").c_str()),
			std::string(E.GetString("rawDisplayName").c_str())
		};
		auto R = abilities.GetObject("R");
		player.abilities.R = {
			R.GetInteger("abilityLevel"),
			std::string(R.GetString("displayName").c_str()),
			std::string(R.GetString("id").c_str()),
			std::string(R.GetString("rawDescription").c_str()),
			std::string(R.GetString("rawDisplayName").c_str())
		};

		auto championStats = activePlayer.GetObject("championStats");
		player.championStats = {
			(float)championStats.GetDouble("abilityPower"),
			(float)championStats.GetDouble("armor"),
			(float)championStats.GetDouble("armorPenetrationFlat"),
			(float)championStats.GetDouble("armorPenetrationPercent"),
			(float)championStats.GetDouble("attackDamage"),
			(float)championStats.GetDouble("attackRange"),
			(float)championStats.GetDouble("attackSpeed"),
			(float)championStats.GetDouble("bonusArmorPenetrationPercent"),
			(float)championStats.GetDouble("bonusMagicPenetrationPercent"),
			(float)championStats.GetDouble("cooldownReduction"),
			(float)championStats.GetDouble("critChance"),
			(float)championStats.GetDouble("critDamage"),
			(float)championStats.GetDouble("currentHealth"),
			(float)championStats.GetDouble("healthRegenRate"),
			(float)championStats.GetDouble("lifeSteal"),
			(float)championStats.GetDouble("magicLethality"),
			(float)championStats.GetDouble("magicPenetrationFlat"),
			(float)championStats.GetDouble("magicPenetrationPercent"),
			(float)championStats.GetDouble("magicResist"),
			(float)championStats.GetDouble("maxHealth"),
			(float)championStats.GetDouble("moveSpeed"),
			(float)championStats.GetDouble("physicalLethality"),
			(float)championStats.GetDouble("resourceMax"),
			(float)championStats.GetDouble("resourceRegenRate"),
			std::string(championStats.GetString("resourceType").c_str()),
			(float)championStats.GetDouble("resourceValue"),
			(float)championStats.GetDouble("spellVamp"),
			(float)championStats.GetDouble("tenacity"),
		};

		player.currentGold = (float)activePlayer.GetDouble("currentGold");

		auto runesObj = activePlayer.GetObject("fullRunes");
		auto generalRunesObj = runesObj.GetObject("generalRunes").AsArray();
		std::vector<API::Rune> generalRunes;
		for (size_t j = 0; j < generalRunesObj.GetLength(); ++j)
		{
			auto runeObj = generalRunesObj.GetItem(j).AsObject();
			API::Rune rune;
			rune.displayName = std::string(runeObj.GetString("displayName").c_str());
			rune.id = runeObj.GetInteger("id");
			rune.rawDescription = std::string(runeObj.GetString("rawDescription").c_str());
			rune.rawDisplayName = std::string(runeObj.GetString("rawDisplayName").c_str());
			generalRunes.emplace_back(rune);
		}
		player.fullRunes.generalRunes = generalRunes;

		auto keystoneObj = runesObj.GetObject("keystone");
		player.fullRunes.keystone.displayName = std::string(keystoneObj.GetString("displayName").c_str());
		player.fullRunes.keystone.id = keystoneObj.GetInteger("id");
		player.fullRunes.keystone.rawDescription = std::string(keystoneObj.GetString("rawDescription").c_str());
		player.fullRunes.keystone.rawDisplayName = std::string(keystoneObj.GetString("rawDisplayName").c_str());

		auto primaryRuneTreeObj = runesObj.GetObject("primaryRuneTree");
		player.fullRunes.primaryRuneTree.displayName = std::string(primaryRuneTreeObj.GetString("displayName").c_str());
		player.fullRunes.primaryRuneTree.id = primaryRuneTreeObj.GetInteger("id");
		player.fullRunes.primaryRuneTree.rawDescription = std::string(primaryRuneTreeObj.GetString("rawDescription").c_str());
		player.fullRunes.primaryRuneTree.rawDisplayName = std::string(primaryRuneTreeObj.GetString("rawDisplayName").c_str());

		auto secondaryRuneTreeObj = runesObj.GetObject("secondaryRuneTree");
		player.fullRunes.secondaryRuneTree.displayName = std::string(secondaryRuneTreeObj.GetString("displayName").c_str());
		player.fullRunes.secondaryRuneTree.id = secondaryRuneTreeObj.GetInteger("id");
		player.fullRunes.secondaryRuneTree.rawDescription = std::string(secondaryRuneTreeObj.GetString("rawDescription").c_str());
		player.fullRunes.secondaryRuneTree.rawDisplayName = std::string(secondaryRuneTreeObj.GetString("rawDisplayName").c_str());

		auto statRunesObj = runesObj.GetObject("statRunes").AsArray();
		std::vector<API::StatRune> statRunes;
		for (size_t j = 0; j < statRunesObj.GetLength(); ++j)
		{
			auto statruneObj = statRunesObj.GetItem(j).AsObject();
			API::StatRune statrune;
			statrune.id = statruneObj.GetInteger("id");
			statrune.rawDescription = std::string(statruneObj.GetString("rawDescription").c_str());
			statRunes.emplace_back(statrune);
		}
		player.fullRunes.statRunes = statRunes;

		player.level = activePlayer.GetInteger("level");
		player.summonerName = std::string(activePlayer.GetString("summonerName").c_str());

		AllGameData.activePlayer = player;
	}

	{
		auto players = info.GetObject("allPlayers").AsArray();
		for (size_t i = 0; i < players.GetLength(); ++i)
		{
			auto playerObj = players.GetItem(i).AsObject();

			API::PlayerList player;
			player.championName = std::string(playerObj.GetString("championName").c_str());
			player.isBot = playerObj.GetBool("isBot");
			player.isDead = playerObj.GetBool("isDead");
			auto items = playerObj.GetArray("items");
			for (size_t j = 0; j < items.GetLength(); ++j)
			{
				auto itemObj = items.GetItem(j).AsObject();
				API::Item item;
				item.canUse = itemObj.GetBool("canUse");
				item.consumable = itemObj.GetBool("consumable");
				item.count = itemObj.GetInteger("count");
				item.displayName = std::string(itemObj.GetString("displayName").c_str());
				item.itemID = itemObj.GetInteger("itemID");
				item.price = itemObj.GetInteger("price");
				item.rawDescription = std::string(itemObj.GetString("rawDescription").c_str());
				item.rawDisplayName = std::string(itemObj.GetString("rawDisplayName").c_str());
				item.slot = itemObj.GetInteger("slot");
				player.items.emplace_back(item);
			}
			player.level = playerObj.GetInteger("level");
			player.position = std::string(playerObj.GetString("position").c_str());
			player.rawChampionName = std::string(playerObj.GetString("rawChampionName").c_str());
			player.rawSkinName = std::string(playerObj.GetString("rawSkinName").c_str());
			player.respawnTimer = (float)playerObj.GetDouble("respawnTimer");

			auto runesObj = playerObj.GetObject("runes");

			auto keystoneObj = runesObj.GetObject("keystone");
			player.runes.keystone.displayName = std::string(keystoneObj.GetString("displayName").c_str());
			player.runes.keystone.id = keystoneObj.GetInteger("id");
			player.runes.keystone.rawDescription = std::string(keystoneObj.GetString("rawDescription").c_str());
			player.runes.keystone.rawDisplayName = std::string(keystoneObj.GetString("rawDisplayName").c_str());

			auto primaryRuneTreeObj = runesObj.GetObject("primaryRuneTree");
			player.runes.primaryRuneTree.displayName = std::string(primaryRuneTreeObj.GetString("displayName").c_str());
			player.runes.primaryRuneTree.id = primaryRuneTreeObj.GetInteger("id");
			player.runes.primaryRuneTree.rawDescription = std::string(primaryRuneTreeObj.GetString("rawDescription").c_str());
			player.runes.primaryRuneTree.rawDisplayName = std::string(primaryRuneTreeObj.GetString("rawDisplayName").c_str());

			auto secondaryRuneTreeObj = runesObj.GetObject("secondaryRuneTree");
			player.runes.secondaryRuneTree.displayName = std::string(secondaryRuneTreeObj.GetString("displayName").c_str());
			player.runes.secondaryRuneTree.id = secondaryRuneTreeObj.GetInteger("id");
			player.runes.secondaryRuneTree.rawDescription = std::string(secondaryRuneTreeObj.GetString("rawDescription").c_str());
			player.runes.secondaryRuneTree.rawDisplayName = std::string(secondaryRuneTreeObj.GetString("rawDisplayName").c_str());

			auto scoresObj = playerObj.GetObject("scores");
			player.scores.assists = scoresObj.GetInteger("assists");
			player.scores.creepScore = scoresObj.GetInteger("creepScore");
			player.scores.deaths = scoresObj.GetInteger("deaths");
			player.scores.kills = scoresObj.GetInteger("kills");
			player.scores.wardScore = (float)scoresObj.GetDouble("wardScore");

			player.skinID = playerObj.GetInteger("skinID");
			player.skinName = std::string(playerObj.GetString("skinName").c_str());
			player.summonerName = std::string(playerObj.GetString("summonerName").c_str());

			auto summonerSpellsObj = playerObj.GetObject("summonerSpells");
			auto summonerSpellOneeObj = summonerSpellsObj.GetObject("summonerSpellOne");
			player.summonerSpells.summonerSpellOne.displayName = std::string(summonerSpellOneeObj.GetString("displayName").c_str());
			player.summonerSpells.summonerSpellOne.rawDescription = std::string(summonerSpellOneeObj.GetString("rawDescription").c_str());
			player.summonerSpells.summonerSpellOne.rawDisplayName = std::string(summonerSpellOneeObj.GetString("rawDisplayName").c_str());

			auto summonerSpellTwoObj = summonerSpellsObj.GetObject("summonerSpellTwo");
			player.summonerSpells.summonerSpellTwo.displayName = std::string(summonerSpellTwoObj.GetString("displayName").c_str());
			player.summonerSpells.summonerSpellTwo.rawDescription = std::string(summonerSpellTwoObj.GetString("rawDescription").c_str());
			player.summonerSpells.summonerSpellTwo.rawDisplayName = std::string(summonerSpellTwoObj.GetString("rawDisplayName").c_str());

			player.team = std::string(playerObj.GetString("team").c_str());

			AllGameData.allPlayers.emplace_back(player);
		}
	}

	{
		std::vector<API::Event>temp;
		auto events = info.GetObject("events").GetObject("Events").AsArray();
		for (size_t i = 0; i < events.GetLength(); ++i)
		{
			auto eventObj = events.GetItem(i).AsObject();
			API::Event event;
			auto objects = eventObj.GetAllObjects();

			for (auto o : objects)
			{
				if (o.first == "Assisters")
				{
					auto assisters = eventObj.GetArray("Assisters");
					for (size_t j = 0; j < assisters.GetLength(); ++j)
					{
						event.Assisters.emplace_back(assisters.GetItem(j).AsString());
					}
				}
				if (o.first == "KillStreak")
				{
					event.KillStreak = eventObj.GetInteger("KillStreak");
				}
			}
			event.EventID = eventObj.GetInteger("EventID");
			event.EventName = std::string(eventObj.GetString("EventName").c_str());
			event.EventTime = (float)eventObj.GetDouble("EventTime");
			event.KillerName = std::string(eventObj.GetString("KillerName").c_str());
			event.VictimName = std::string(eventObj.GetString("VictimName").c_str());
			temp.emplace_back(event);
		}
		AllGameData.events = temp;
	}

	{
		auto statsObj = info.GetObject("gameData");

		AllGameData.gameData = {
			std::string(statsObj.GetString("gameMode").c_str()),
			(float)statsObj.GetDouble("gameTime"),
			std::string(statsObj.GetString("mapName").c_str()),
			statsObj.GetInteger("mapNumber"),
			std::string(statsObj.GetString("mapTerrain").c_str())
		};
	}

	return AllGameData;
}