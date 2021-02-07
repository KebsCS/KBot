#pragma once

#ifndef  _INITIALIZE_H_
#define _INITIALIZE_H_

#include <string>
#include <vector>

#include "CObjectManager.h"
#include "CMissileClient.h"
//#include "CSpellInfo.h"
//#include "CSpellEntry.h"

class Initialize
{
public:

	//std::list<CObject>ObjList;
	std::vector<CObject>herolist;
	std::vector<CObject>turretlist;
	std::vector<CObject>inhiblist;
	//std::vector<CObject>structurelist;

	std::vector<CObject>objlisttest;

	bool Start();

	//logs useful info to console
	void StartupInfo();

	//populates object lists
	void AddObjects();

	void MakeHeroList();
	void MakeTurretList();
	void MakeInhibList();
	//void MakeStructureList();

	void MakeTestList();

	void SetMinimapProperties();

	void SetRendererProperties();

	//makes array of champions in game for GUI scoreboard
	void CreateChampArray();
};

extern Initialize* init;

#endif // ! _INITIALIZE_H_