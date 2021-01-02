#include "Initialize.h"

#include "DirectX.h"


CObject Local;
DWORD OBJManager;
DWORD OBJManagerArray;
DWORD MissileMap;
IScript* championScript;

bool Initialize::Start()
{
#ifndef NOLEAGUE



	M.GameTime = Memory.Read<float>(ClientAddress + oGameTime, sizeof(float));
	int wait = 0;
	while (M.GameTime < 1) // pause if not in game
	{
		if (wait > 30)
			if (MessageBoxA(0, XorStr("Game not starting, continue?"), 0, MB_YESNO) == IDNO) //if no button is pressed 
				return false;
			else
				wait = 0;
		M.GameTime = Memory.Read<float>(ClientAddress + oGameTime, sizeof(float));
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		if (PressedKey(VK_F11))
			return false;
		wait++;
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));


	//globals
	Local = CObject(Memory.Read<DWORD>(ClientAddress + oLocalPlayer, sizeof(DWORD)));
	if (!Local.Address())
		return false;
	Local.SetPlayerConsts();
	OBJManager = Memory.Read<DWORD>(ClientAddress + oObjManager, sizeof(DWORD));
	OBJManagerArray = Memory.Read<DWORD>(OBJManager + 0x14, sizeof(DWORD));
	MissileMap = Memory.Read<DWORD>(ClientAddress + oMissileMap);

	M.Champion = Local.GetChampName();
	championScript = ScriptUtils::GetScriptByChampionName(M.Champion);


	AddObjects();
	CreateChampArray();
	
	if(M.Debug)
		StartupInfo();

	//std::string strAPI = api->GET("https://127.0.0.1", "/liveclientdata/allgamedata", 2999);
	


#elif
	M.Champion = "Empty";

#endif // !NOLEAGUE
	return true;
}

void Initialize::StartupInfo()
{

	clog.AddLog("ProcessId: %d", Memory.ProcessID);
	clog.AddLog("ClientAddress: %x", ClientAddress);
	clog.AddLog("OBJManager: %x", OBJManager);

	clog.AddLog("LocalPlayer: %x", Local.Address());
	clog.AddLog("UnitComponentInfo: %x", Local.GetUnitComponentInfo());
	clog.AddLog("Name: %s", Local.GetName().c_str());
	clog.AddLog("NetworkID: %d", Local.GetNetworkID());
	clog.AddLog("IsVisible: %d", Local.IsVisible());
	clog.AddLog("Bounding: %d", Local.GetBoundingRadius());
	clog.AddLog("GetTeam: %d", Local.GetTeam());
	clog.AddLog("HP: %f", Local.GetHealth());
	clog.AddLog("MaxHP: %f", Local.GetMaxHealth());
	clog.AddLog("Mana: %f", Local.GetMana());
	clog.AddLog("MaxMana: %f", Local.GetMaxMana());
	clog.AddLog("Armor: %f", Local.GetArmor());
	clog.AddLog("MR: %f", Local.GetMR());
	clog.AddLog("MS: %f", Local.GetMS());
	clog.AddLog("BaseAD: %f", Local.GetBaseAD());
	clog.AddLog("BonusAD: %f", Local.GetBonusAD());
	clog.AddLog("AP: %f", Local.GetAP());
	clog.AddLog("AARange: %f", Local.GetAARange());
	clog.AddLog("Lethality: %f", Local.GetLethality());
	clog.AddLog("Crit: %f", Local.GetCrit());
	clog.AddLog("ChampName: %s", Local.GetChampName().c_str());
	clog.AddLog("SummonerSpell1: %s", Local.SummonerSpell1().c_str());
	clog.AddLog("SummonerSpell2: %s", Local.SummonerSpell2().c_str());
	clog.AddLog("KeystoneName: %s", Local.KeystoneName().c_str());
	clog.AddLog("GetLevel: %i", Local.GetLevel());
}

void Initialize::AddObjects()
{


	std::thread MakeHeroListThread(&Initialize::MakeHeroList, this);
	std::thread MakeTurretListThread(&Initialize::MakeTurretList, this);
	std::thread MakeInhibListThread(&Initialize::MakeInhibList, this);
	std::thread MakeStructureListThread(&Initialize::MakeStructureList, this);

	std::thread MakeTestListThread(&Initialize::MakeTestList, this);

	//todo make threads return value


	MakeHeroListThread.join();
	MakeTurretListThread.join();
	MakeInhibListThread.join();
	MakeStructureListThread.join();
	MakeTestListThread.join();

}


void Initialize::MakeHeroList()
{

	DWORD dwHeroList = Memory.Read<DWORD>(ClientAddress + oHeroList);
	DWORD HeroArray = Memory.Read<DWORD>(dwHeroList + 0x04);
	int HeroArrayLength = Memory.Read<int>(dwHeroList + 0x08);
	for (int i = 0; i < HeroArrayLength * 4; i += 4)
	{
		CObject obj(Memory.Read<DWORD>(HeroArray + i));
		obj.SetPlayerConsts();
		herolist.emplace_back(obj);
		if (M.Debug)
			clog.AddLog("%s : %x %s %s ", obj.GetName().c_str(), obj.Address(), obj.SummonerSpell1().c_str(), obj.SummonerSpell2().c_str());
	}

	std::string str;
	if (herolist.size() < 10)
		str = "error";
	else str = "start";
	clog.AddLog("[%s] Added %i/10 heroes", str.c_str(), herolist.size());

}


void Initialize::MakeTurretList()
{

	DWORD dwTurretList = Memory.Read<DWORD>(ClientAddress + oTurretList);
	DWORD TurretArray = Memory.Read<DWORD>(dwTurretList + 0x04);
	int TurretArrayLength = Memory.Read<int>(dwTurretList + 0x08);
	for (int i = 0; i < TurretArrayLength * 4; i += 4)
	{

		CObject obj(Memory.Read<DWORD>(TurretArray + i));
		obj.SetStructureConsts();
		turretlist.emplace_back(obj);
		//clog.AddLog("%s : %x", obj.GetName().c_str(), obj.Address());
	}
	std::string str;
	if (turretlist.size() < 22)
		str = "error";
	else str = "start";
	clog.AddLog("[%s] Added %i/22 turrets", str.c_str(), turretlist.size());

}

void Initialize::MakeInhibList()
{


	DWORD dwInhibList = Memory.Read<DWORD>(ClientAddress + oInhibitorList);
	DWORD InhibArray = Memory.Read<DWORD>(dwInhibList + 0x04);
	int InhibArrayLength = Memory.Read<int>(dwInhibList + 0x08);
	for (int i = 0; i < InhibArrayLength * 4; i += 4)
	{

		CObject obj(Memory.Read<DWORD>(InhibArray + i));
		obj.SetStructureConsts();
		inhiblist.emplace_back(obj);
		//clog.AddLog("%s : %x", obj.GetName().c_str(), obj.Address());
	}
	std::string str;
	if (inhiblist.size() < 6)
		str = "error";
	else str = "start";
	clog.AddLog("[%s] Added %i/6 inhibitors", str.c_str(), inhiblist.size());

}


void Initialize::MakeStructureList()
{

	DWORD dwStructureList = Memory.Read<DWORD>(ClientAddress + oStructureList);
	DWORD StructureArray = Memory.Read<DWORD>(dwStructureList + 0x04);
	int StructureArrayLength = Memory.Read<int>(dwStructureList + 0x08);
	for (int i = 0; i < StructureArrayLength * 4; i += 4)
	{
		CObject obj(Memory.Read<DWORD>(StructureArray + i));
		obj.SetStructureConsts();
		structurelist.emplace_back(obj);
		//clog.AddLog("%s : %x", obj.GetName().c_str(), obj.Address());
	}

}

void Initialize::MakeTestList()
{


	//DWORD testList = Memory.Read<DWORD>(ClientAddress + 0x28BCCC0); //0x28BCC98
	//clog.AddLog("TestList: %x", testList);
	//DWORD testarray = Memory.Read<DWORD>(testList + 0x04);
	//int testlength = Memory.Read<int>(testList + 0x08);
	//for (int i = 0; i < testlength * 4; i += 4)
	//{
	//	CObject obj(Memory.Read<DWORD>(testarray + i));

	//	objlisttest.emplace_back(obj);
	//	//clog.AddLog("%s : %x", obj.GetName().c_str(), obj.Address());

	//}




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
	//			clog.AddLog("[start] Added hero: %s", objName.c_str());
	//			herolist.emplace_back(obj);
	//			continue;
	//		}
	//	/*	if (objName.find("Turret_") != std::string::npos)
	//		{
	//			clog.AddLog("[start] Added turret: %s", objName.c_str());
	//			herolist.emplace_back(obj);
	//			continue;
	//		}*/
	//		
	//		for (auto name : ChampNames)
	//		{
	//			
	//			if (name == objChampName)
	//			{
	//				clog.AddLog("[start] Added hero: %s", objChampName.c_str());
	//				herolist.emplace_back(obj);
	//				break;
	//			}
	//		}
	//	}
	//}
}

void Initialize::CreateChampArray()
{
	//todo cleanup, maybe sort based on API's position

	//own team = r  
	//enemy team = b 
	std::vector<CObject>r(5);
	std::vector<CObject>b(5);
	int ir = 0;
	int ib = 0;
	for (auto hero : herolist)
	{
		//clog.AddLog("%d %s", hero.GetTeam(), hero.GetChampName().c_str());
		if (hero.GetTeam() == Local.GetTeam() && ir < 5)
			r[ir++] = hero;
		else if (hero.GetTeam() != Local.GetTeam() && ib < 5)
			b[ib++] = hero;

	}

	//pseudo sorting

	std::vector<std::string>scoreboardnames(10);
	//put in place based on summoner spells
	//if (herolist.size() == 10)
	//{
	//	ir = 0;
	//	for (auto a : r)
	//	{

	//		//if has smite
	//		if ((a.SummonerSpell1() == "summonersmite" || a.SummonerSpell2() == "summonersmite") && scoreboardnames[2].empty())
	//		{
	//			scoreboardnames[2] = a.GetChampName();
	//			r[ir++] = 0;
	//			continue;
	//		}
	//		//teleport
	//		if ((a.SummonerSpell1() == "summonerteleport" || a.SummonerSpell2() == "summonerteleport") && scoreboardnames[0].empty())
	//		{
	//			scoreboardnames[0] = a.GetChampName();
	//			r[ir++] = 0;
	//			continue;
	//		}
	//		//heal
	//		if ((a.SummonerSpell1() == "summonerheal" || a.SummonerSpell2() == "summonerheal") && scoreboardnames[6].empty())
	//		{
	//			scoreboardnames[6] = a.GetChampName();
	//			r[ir++] = 0;
	//			continue;
	//		}
	//		ir++;

	//	}
	//	ib = 0;
	//	for (auto a : b)
	//	{
	//		//if has smite
	//		if ((a.SummonerSpell1() == "summonersmite" || a.SummonerSpell2() == "summonersmite") && scoreboardnames[3].empty())
	//		{
	//			scoreboardnames[3] = a.GetChampName();
	//			b[ib++] = 0;
	//			continue;
	//		}
	//		//teleport
	//		if ((a.SummonerSpell1() == "summonerteleport" || a.SummonerSpell2() == "summonerteleport") && scoreboardnames[1].empty())
	//		{
	//			scoreboardnames[1] = a.GetChampName();
	//			b[ib++] = 0;
	//			continue;
	//		}
	//		//heal
	//		if ((a.SummonerSpell1() == "summonerheal" || a.SummonerSpell2() == "summonerheal") && scoreboardnames[7].empty())
	//		{
	//			scoreboardnames[7] = a.GetChampName();
	//			b[ib++] = 0;
	//			continue;
	//		}
	//		ib++;

	//	}
	//}

	//put in place whats left
	for (auto a : r)
	{
		if (a == 0)
			continue;
		for (int i = 0; i < 10; i += 2)
		{
			if (scoreboardnames[i].empty())
			{
				scoreboardnames[i] = a.GetChampName();
				break;
			}

		}
	}
	
	for (auto a : b)
	{
		if (a == 0)
			continue;
		for (int i = 1; i < 10; i += 2)
		{
			if (scoreboardnames[i].empty())
			{
				scoreboardnames[i] = a.GetChampName();
				break;
			}

		}
	}

	//put to global 
	for (int i = 0; i < 10; i++)
	{
		M.ScoreboardNames[i] = scoreboardnames[i];
	}

}


Initialize* init = new Initialize();

