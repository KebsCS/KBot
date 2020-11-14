#include "Initialize.h"

#include "DirectX.h"

void Initialize::AddObjects() 
{

	std::string str;

	DWORD dwHeroList = Memory.Read<DWORD>(ClientAddress + oHeroList);
	DWORD HeroArray = Memory.Read<DWORD>(dwHeroList + 0x04);
	int HeroArrayLength = Memory.Read<int>(dwHeroList + 0x08);
	for (int i = 0; i < HeroArrayLength * 4; i += 4)
	{

		CObject obj(Memory.Read<DWORD>(HeroArray + i));
		herolist.emplace_back(obj);
		clog.AddLog("%s : %x %s %s ", obj.GetName().c_str(), obj.Address(), obj.SummonerSpell1().c_str(), obj.SummonerSpell2().c_str());
	}
	if (herolist.size() < 10)
		str = "error";
	else str = "startup";
	clog.AddLog("[%s] Added %i/10 heroes", str.c_str(), herolist.size());

	DWORD dwTurretList = Memory.Read<DWORD>(ClientAddress + oTurretList);
	DWORD TurretArray = Memory.Read<DWORD>(dwTurretList + 0x04);
	int TurretArrayLength = Memory.Read<int>(dwTurretList + 0x08);
	for (int i = 0; i < TurretArrayLength * 4; i += 4)
	{

		CObject obj(Memory.Read<DWORD>(TurretArray + i));
		turretlist.emplace_back(obj);
		//clog.AddLog("%s : %x", obj.GetName().c_str(), obj.Address());
	}

	if (turretlist.size() < 22)
		str = "error";
	else str = "startup";
	clog.AddLog("[%s] Added %i/22 turrets", str.c_str(), turretlist.size());


	DWORD dwInhibList = Memory.Read<DWORD>(ClientAddress + oInhibitorList);
	DWORD InhibArray = Memory.Read<DWORD>(dwInhibList + 0x04);
	int InhibArrayLength = Memory.Read<int>(dwInhibList + 0x08);
	for (int i = 0; i < InhibArrayLength * 4; i += 4)
	{

		CObject obj(Memory.Read<DWORD>(InhibArray + i));
		inhiblist.emplace_back(obj);
		//clog.AddLog("%s : %x", obj.GetName().c_str(), obj.Address());
	}
	if (inhiblist.size() < 6)
		str = "error";
	else str = "startup";
	clog.AddLog("[%s] Added %i/6 inhibitors", str.c_str(), inhiblist.size());
	

	//DWORD dwMissileList = Memory.Read<DWORD>(ClientAddress + 0x350B4F8);

	//clog.AddLog("inhib %x", dwInhibList);
	//clog.AddLog("missile %x", dwMissileList);

	//DWORD Obj = Memory.Read<DWORD>(ClientAddress + oObjManager, sizeof(DWORD));

	//DWORD ObjectArray = Memory.Read<DWORD>(Obj + 0x20, sizeof(DWORD));
	//for (int i = 0; i < 10000; i++) // every object to objectlist
	//{
	//	CObject obj(Memory.Read<DWORD>(ObjectArray + (0x4 * i), sizeof(DWORD)));
	//	ObjList.emplace_back(obj);
	//}

	//for (auto obj : ObjList)
	//{

	//	if (obj.GetTeam() == 100 || obj.GetTeam() == 200 || obj.GetTeam() == 300)
	//	{
	//		std::string objName = obj.GetName();
	//		std::string objChampName = obj.GetChampName();
	//		clog.AddLog("%s , %s , %x", objName.c_str(), objChampName.c_str(), obj.Address());
	//		//clog.AddLog("%s", obj.GetName());
	//		if (objName == "Target Dummy")
	//		{
	//			clog.AddLog("[startup] Added hero: %s", objName.c_str());
	//			herolist.emplace_back(obj);
	//			continue;
	//		}//todo grab this from turret list bcs one turret is multiply defined here
	//	/*	if (objName.find("Turret_") != std::string::npos)
	//		{
	//			clog.AddLog("[startup] Added turret: %s", objName.c_str());
	//			herolist.emplace_back(obj);
	//			continue;
	//		}*/
	//		
	//		for (auto name : ChampNames)
	//		{
	//			
	//			if (name == objChampName)
	//			{
	//				clog.AddLog("[startup] Added hero: %s", objChampName.c_str());
	//				herolist.emplace_back(obj);
	//				break;
	//			}
	//		}
	//	}
	//}

}
Initialize* init = new Initialize();

