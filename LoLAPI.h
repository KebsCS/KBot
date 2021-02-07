#pragma once

#include "API.h"
#include <map>

class LoLAPI
{
private:

public:
	static void LoadPlayerListData();
	static API::PlayerList* UnknownPlayer;
	static std::map<std::string, API::PlayerList*>  Players;
	static API::PlayerList* GetAPIPlayerByChampName(std::string& name);
	static API::GameStats GetGameStats();
	static std::vector<API::Event> GetEvents();
	static API::ActivePlayer GetActivePlayer();
	static API::AllGameData GetAllGameData();
};