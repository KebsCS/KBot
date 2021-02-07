#pragma once

#include <string>
#include <vector>

namespace API
{
	struct Rune
	{
		std::string displayName;
		int id = 0;
		std::string rawDescription;
		std::string rawDisplayName;
	};

	struct Runes
	{
		Rune keystone;
		Rune primaryRuneTree;
		Rune secondaryRuneTree;
	};

	struct Scores
	{
		int assists = 0;
		int creepScore = 0;
		int deaths = 0;
		int kills = 0;
		float wardScore = 0;
	};

	struct SummonerSpell
	{
		std::string displayName;
		std::string rawDescription;
		std::string rawDisplayName;
	};

	struct SummonerSpells
	{
		SummonerSpell summonerSpellOne;
		SummonerSpell summonerSpellTwo;
	};

	struct Item
	{
		bool canUse = false;
		bool consumable = false;
		int	count = 0;
		std::string	displayName;
		int	itemID = 0;
		int	price = 0;
		std::string	rawDescription;
		std::string	rawDisplayName;
		int	slot = 0;
	};

	struct PlayerList
	{
		std::string championName;
		bool isBot = false;
		bool isDead = false;
		std::vector<Item>items;
		int level = 0;
		std::string position;
		std::string rawChampionName;
		std::string rawSkinName;
		float respawnTimer = 0;
		Runes runes;
		Scores scores;
		int skinID = 0;
		std::string skinName;
		std::string summonerName;
		SummonerSpells summonerSpells;
		std::string team;
	};

	struct GameStats
	{
		std::string gameMode;
		float gameTime = 0;
		std::string mapName;
		int mapNumber = 0;
		std::string mapTerrain;
	};

	struct Event
	{
		int EventID = 0;
		std::string EventName;
		float EventTime = 0;
		std::vector<std::string> Assisters;
		int KillStreak = 0;
		std::string KillerName;
		std::string VictimName;
		std::string TurretKilled;
	};

	struct Ability
	{
		int abilityLevel = 0;
		std::string displayName;
		std::string id;
		std::string rawDescription;
		std::string rawDisplayName;
	};

	struct Abilities
	{
		Ability Passive;
		Ability Q;
		Ability W;
		Ability E;
		Ability R;
	};

	struct ChampionStats
	{
		float abilityPower = 0;
		float armor = 0;
		float armorPenetrationFlat = 0;
		float armorPenetrationPercent = 0;
		float attackDamage = 0;
		float attackRange = 0;
		float attackSpeed = 0;
		float bonusArmorPenetrationPercent = 0;
		float bonusMagicPenetrationPercent = 0;
		float cooldownReduction = 0;
		float critChance = 0;
		float critDamage = 0;
		float currentHealth = 0;
		float healthRegenRate = 0;
		float lifeSteal = 0;
		float magicLethality = 0;
		float magicPenetrationFlat = 0;
		float magicPenetrationPercent = 0;
		float magicResist = 0;
		float maxHealth = 0;
		float moveSpeed = 0;
		float physicalLethality = 0;
		float resourceMax = 0;
		float resourceRegenRate = 0;
		std::string resourceType;
		float resourceValue = 0;
		float spellVamp = 0;
		float tenacity = 0;
	};

	struct StatRune
	{
		int id = 0;
		std::string rawDescription;
	};

	struct FullRunes
	{
		std::vector<Rune> generalRunes;
		Rune keystone;
		Rune primaryRuneTree;
		Rune secondaryRuneTree;
		std::vector<StatRune> statRunes;
	};

	struct ActivePlayer
	{
		Abilities abilities;
		ChampionStats championStats;
		float currentGold = 0;
		FullRunes fullRunes;
		int level = 0;
		std::string summonerName;
	};

	struct AllGameData
	{
		ActivePlayer activePlayer;
		std::vector<PlayerList> allPlayers;
		std::vector<Event> events;
		GameStats gameData;
	};
}