#include "Visuals.h"
#include "Draw.h"

extern Direct3D9Render Direct3D9;
extern Draw* draw;
extern ConsoleLog clog;



void Visuals::CooldownTimers(CObject obj, int type)
{

	DWORD LocalPlayer = Memory.Read<DWORD>(ClientAddress + oLocalPlayer, sizeof(DWORD));
	CObject Local(LocalPlayer);

	if (obj.GetTeam() == Local.GetTeam())
		return;


	if (obj.GetHealth() <= 0.01f)
		return;

	if (!obj.IsVisible())
		return;

	Vector3 Position = obj.GetPosition();
	ImVec2 RealPos = Direct3D9.WorldToScreen(Position);

	if (RealPos.x == 0.f && RealPos.y == 0.f)
		return;

	if (!((RealPos.x <= SCREENWIDTH * 1.2) && (RealPos.x >= SCREENWIDTH / 2 * (-1)) && (RealPos.y <= SCREENHEIGHT * 1.5) && (RealPos.y >= SCREENHEIGHT / 2 * (-1))))
		return;

	/*
	Level 0x20
	Cooldown 0x78
	CooldownExpire 0x28
	FinalCooldownExpire 0x64
	Charges 0x58
	*/
	int Level = 0x20;
	int CooldownExpire = 0x28;

	//todo clean this up and summoner icons instead of text and text alligning to center
	DWORD QSpell = obj.GetSpellByID(SpellSlotID::Q);
	float GetQCooldownExpire = Memory.Read<float>(QSpell + CooldownExpire, sizeof(float));
	int GetQLevel = Memory.Read<int>(QSpell + Level, sizeof(int));

	DWORD WSpell = obj.GetSpellByID(SpellSlotID::W);
	float GetWCooldownExpire = Memory.Read<float>(WSpell + CooldownExpire, sizeof(float));
	int GetWLevel = Memory.Read<int>(WSpell + Level, sizeof(int));

	DWORD ESpell = obj.GetSpellByID(SpellSlotID::E);
	float GetECooldownExpire = Memory.Read<float>(ESpell + CooldownExpire, sizeof(float));
	int GetELevel = Memory.Read<int>(ESpell + Level, sizeof(int));

	DWORD RSpell = obj.GetSpellByID(SpellSlotID::R);
	float GetRCooldownExpire = Memory.Read<float>(RSpell + CooldownExpire, sizeof(float));
	int GetRLevel = Memory.Read<int>(RSpell + Level, sizeof(int));

	DWORD Summ1 = obj.GetSpellByID(SpellSlotID::Summoner1);
	DWORD Summ2 = obj.GetSpellByID(SpellSlotID::Summoner2);

	float GetDCooldownExpire = Memory.Read<float>(Summ1 + CooldownExpire, sizeof(float));
	float GetFCooldownExpire = Memory.Read<float>(Summ2 + CooldownExpire, sizeof(float));


	float GameTime = Memory.Read<float>(ClientAddress + oGameTime, sizeof(float));

	RGBA colorReady(255, 25, 25);
	RGBA colorDown(25, 255, 25);
	int Qcooldown = GetQCooldownExpire - GameTime + 1;
	std::string sQcd = std::to_string(Qcooldown);
	RGBA Qcolor = colorDown;
	if (GetQLevel < 1)
	{
		sQcd = "Q";
	}
	else if (Qcooldown <= 0.f)
	{
		sQcd = "Q";
		Qcolor = colorReady;
	}

	int Wcooldown = GetWCooldownExpire - GameTime + 1;
	std::string sWcd = std::to_string(Wcooldown);
	RGBA Wcolor = colorDown;
	if (GetWLevel < 1)
	{
		sWcd = "W";
	}
	else if (Wcooldown <= 0.f)
	{
		sWcd = "W";
		Wcolor = colorReady;
	}

	int Ecooldown = GetECooldownExpire - GameTime + 1;
	std::string sEcd = std::to_string(Ecooldown);
	RGBA Ecolor = colorDown;
	if (GetELevel < 1)
	{
		sEcd = "E";
	}
	else if (Ecooldown <= 0.f)
	{
		sEcd = "E";
		Ecolor = colorReady;
	}

	int Rcooldown = GetRCooldownExpire - GameTime + 1;
	std::string sRcd = std::to_string(Rcooldown);
	RGBA Rcolor = colorDown;
	if (GetRLevel < 1)
	{
		sRcd = "R";
	}
	else if (Rcooldown <= 0.f)
	{
		sRcd = "R";
		Rcolor = colorReady;
	}

	draw->StringOutlined(sQcd, RealPos.x - 40, RealPos.y + 15, Qcolor, Direct3D9.fontTahoma);
	draw->StringOutlined(sWcd, RealPos.x - 10, RealPos.y + 15, Wcolor, Direct3D9.fontTahoma);
	draw->StringOutlined(sEcd, RealPos.x + 20, RealPos.y + 15, Ecolor, Direct3D9.fontTahoma);
	draw->StringOutlined(sRcd, RealPos.x + 40, RealPos.y + 15, Rcolor, Direct3D9.fontTahoma);



	int Dcooldown = GetDCooldownExpire - GameTime + 1;
	std::string sDcd = std::to_string(Dcooldown);
	RGBA Dcolor = colorDown;

	if (Dcooldown <= 0.f)
	{
		sDcd = "D";
		Dcolor = colorReady;
	}

	int Fcooldown = GetFCooldownExpire - GameTime + 1;
	std::string sFcd = std::to_string(Fcooldown);
	RGBA Fcolor = colorDown;

	if (Fcooldown <= 0.f)
	{
		sFcd = "F";
		Fcolor = colorReady;
	}

	//todo smite cooldown is always 15s 
	if (obj.SummonerSpell1() == "summonersmite")
	{
		int SmiteStacks = Memory.Read<int>(Summ1 + 0x58);
		float SmiteDamage = Memory.Read<float>(Summ1 + 0x90);
		draw->StringOutlined(std::to_string((int)SmiteDamage), RealPos.x - 10, RealPos.y + 28, RGBA(0, 200, 255), Direct3D9.fontTahomaSmall);
		//if (SmiteStacks == 2)
		//	draw->StringOutlined(std::to_string((int)SmiteDamage), RealPos.x - 10, RealPos.y + 28, RGBA(0, 200, 255), Direct3D9.fontTahomaSmall);
		//else if (SmiteStacks == 0)
		//	Dcolor = colorDown;
		//else if (SmiteStacks == 1)
		//{
		//	draw->StringOutlined(std::to_string((int)SmiteDamage), RealPos.x - 10, RealPos.y + 28, RGBA(0, 200, 255), Direct3D9.fontTahomaSmall);
		//	Dcolor = RGBA(255, 125, 0);
		//}
		
	}
	else if (obj.SummonerSpell2() == "summonersmite")
	{
		int SmiteStacks = Memory.Read<int>(Summ2 + 0x58);
		float SmiteDamage = Memory.Read<float>(Summ2 + 0x90);
		//clog.AddLog("%i", SmiteStacks);
		draw->StringOutlined(std::to_string((int)SmiteDamage), RealPos.x + 27, RealPos.y + 28, RGBA(0, 200, 255), Direct3D9.fontTahomaSmall);
		/*if (SmiteStacks == 2)
			int a;
		else if (SmiteStacks == 0)
			Fcolor = colorDown;
		else if (SmiteStacks == 1)
		{
			draw->StringOutlined(std::to_string((int)SmiteDamage), RealPos.x + 27, RealPos.y + 28, RGBA(0, 200, 255), Direct3D9.fontTahomaSmall);
			Fcolor = RGBA(255, 125, 0);
		}*/
	}




	draw->StringOutlined(sDcd, RealPos.x - 20, RealPos.y + 30, Dcolor, Direct3D9.fontTahoma);
	draw->StringOutlined(sFcd, RealPos.x + 20, RealPos.y + 30, Fcolor, Direct3D9.fontTahoma);

}

void Visuals::DrawAARanges(CObject obj, int points, int thickness, RGBA color, bool local, RGBA localcolor)
{
	DWORD LocalPlayer = Memory.Read<DWORD>(ClientAddress + oLocalPlayer, sizeof(float));
	CObject Local(LocalPlayer);


	if (obj.GetHealth() <= 0.01f)
		return;

	if (local && obj.Address() == Local.Address())
	{

		Vector3 Position = Local.GetPosition();
		draw->DrawCircleRange(Position, points, Local.GetAARange(), localcolor, thickness);
		return;
	 
	}

	if (obj.GetTeam() == Local.GetTeam())
		return;

	if (obj.GetName().find("Turret_T") != std::string::npos)
	{

		Vector3 Position = obj.GetPosition();
		draw->DrawCircleRange(Position, points, 750, color, thickness);

		return;
	}

	if (obj.IsVisible())
	{
		Vector3 Position = obj.GetPosition();

		draw->DrawCircleRange(Position, points, obj.GetAARange(), color, thickness);
	}
}

void Visuals::DrawTracers(CObject obj, int thickness)
{
	CObject Local(Memory.Read<DWORD>(ClientAddress + oLocalPlayer, sizeof(DWORD)));

	if (obj.GetTeam() == Local.GetTeam())
		return;

	if (obj.GetHealth() != 0.00f && Local.GetHealth() != 0.00f)
	{

		Vector3 Position = obj.GetPosition();
		ImVec2 RealPos = Direct3D9.WorldToScreen(Position);

		Vector3 LocalPosition = Local.GetPosition();
		ImVec2 LocalRealPos = Direct3D9.WorldToScreen(LocalPosition);


		if (RealPos.x == 0.f && RealPos.y == 0.f)
			return;

		float distance = sqrtf((LocalRealPos.x - RealPos.x) * (LocalRealPos.x - RealPos.x) + (LocalRealPos.y - RealPos.y) * (LocalRealPos.y - RealPos.y));

		float r = 255 - ((distance / 22) * (int)(2.55));
		float g = (distance / 22) * (int)(2.55);
		float b = 0;
		if (distance > 2500)
		{
			g = 255;
			r = 0;
		}

		if (!obj.IsVisible())
		{
			r = g = 0;
			b = 255;

			if ((RealPos.x <= SCREENWIDTH * 1.2) && (RealPos.x >= SCREENWIDTH / 2 * (-1)) && (RealPos.y <= SCREENHEIGHT * 1.5) && (RealPos.y >= SCREENHEIGHT / 2 * (-1)))
				draw->StringOutlined(obj.GetChampName(), RealPos.x, RealPos.y, RGBA(255, 255, 255), Direct3D9.fontTahoma);

		}

		RGBA color(r, g, b);

		if (LocalRealPos.x != 0.f && LocalRealPos.y != 0.f)
			draw->Line(LocalRealPos.x, LocalRealPos.y, RealPos.x, RealPos.y, color, thickness);

	}
	
}


void Visuals::AutoSmite(CObject obj)
{
	CObject Local(Memory.Read<DWORD>(ClientAddress + oLocalPlayer, sizeof(DWORD)));
	DWORD SmiteSlot;
	if (Local.SummonerSpell1().c_str() == "summonersmite")
	{
		SmiteSlot = Local.GetSpellByID(SpellSlotID::Summoner1);
	}
	else if (Local.SummonerSpell2().c_str() == "summonersmite")
	{
		SmiteSlot = Local.GetSpellByID(SpellSlotID::Summoner2);
	}
	else return;

	if (obj.Address() == Local.Address())
		return;

	if (obj.GetTeam() == Local.GetTeam())
		return;

	

	float Smite = Memory.Read<float>(SmiteSlot + 0x28, sizeof(float));

	int SmiteStacks = Memory.Read<int>(Smite + 0x58);
	float SmiteDamage = Memory.Read<float>(Smite + 0x90);
	/*
	0x58 Stacks

	*/

	//todo


}