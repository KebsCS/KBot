#include "Visuals.h"




//std::string Visuals::getSpellImg(std::string name)
//{
//	std::string ret = "";
//	if (name.find("boost") != std::string::npos)
//		ret = "Cleanse";
//	else if (name.find("exhaust") != std::string::npos)
//		ret = "Exhaust";
//	else if (name.find("flash") != std::string::npos)
//		ret = "Flash";
//	else if (name.find("haste") != std::string::npos)
//		ret = "Ghost";
//	else if (name.find("heal") != std::string::npos)
//		ret = "Heal";
//	else if (name.find("smite") != std::string::npos)
//		ret = "Smite";
//	else if (name.find("teleport") != std::string::npos)
//		ret = "Teleport";
//	else if (name.find("dot") != std::string::npos)
//		ret = "Ignite";
//	else if (name.find("barrier") != std::string::npos)
//		ret = "Barrier";
//	else
//		ret = "Clarity";
//
//	ret += ".png";
//	return ret;
//}
//

LPDIRECT3DTEXTURE9 Visuals::GetSpellImg(std::string name)
{
	LPDIRECT3DTEXTURE9 ret = NULL;
	if (name.find("boost") != std::string::npos)
		ret = draw->textureCleanse;
	else if (name.find("exhaust") != std::string::npos)
		ret = draw->textureExhaust;
	else if (name.find("flash") != std::string::npos)
		ret = draw->textureFlash;
	else if (name.find("haste") != std::string::npos)
		ret = draw->textureGhost;
	else if (name.find("heal") != std::string::npos)
		ret = draw->textureHeal;
	else if (name.find("smite") != std::string::npos)
		ret = draw->textureSmite;
	else if (name.find("teleport") != std::string::npos)
		ret = draw->textureTeleport;
	else if (name.find("dot") != std::string::npos)
		ret = draw->textureIgnite;
	else if (name.find("barrier") != std::string::npos)
		ret = draw->textureBarrier;
	else
		ret = draw->textureClarity;

	return ret;
}

void Visuals::CooldownTimers(CObject obj)
{



	if (obj.GetTeam() == Local.GetTeam())
		return;

	if (obj.IsDead())
		return;

	if (!obj.IsVisible())
		return;

	Vector3 Position = obj.GetPosition();

	ImVec2 RealPos = Direct3D9.WorldToScreen(Position);

	if (RealPos.x == 0.f && RealPos.y == 0.f)
		return;

	if (!((RealPos.x <= SCREENWIDTH * 1.2) && (RealPos.x >= SCREENWIDTH / 2 * (-1)) && (RealPos.y <= SCREENHEIGHT * 1.5) && (RealPos.y >= SCREENHEIGHT / 2 * (-1))))
		return;



	//todo clean this up and summoner icons instead of text 
	//also spellclass with spell offsets
	DWORD QSpell = obj.GetSpellByID(SpellSlotID::Q);
	float GetQCooldownExpire = Memory.Read<float>(QSpell + oSpellSlotTime, sizeof(float));
	int GetQLevel = Memory.Read<int>(QSpell + oSpellSlotLevel, sizeof(int));

	DWORD WSpell = obj.GetSpellByID(SpellSlotID::W);
	float GetWCooldownExpire = Memory.Read<float>(WSpell + oSpellSlotTime, sizeof(float));
	int GetWLevel = Memory.Read<int>(WSpell + oSpellSlotLevel, sizeof(int));

	DWORD ESpell = obj.GetSpellByID(SpellSlotID::E);
	float GetECooldownExpire = Memory.Read<float>(ESpell + oSpellSlotTime, sizeof(float));
	int GetELevel = Memory.Read<int>(ESpell + oSpellSlotLevel, sizeof(int));

	DWORD RSpell = obj.GetSpellByID(SpellSlotID::R);
	float GetRCooldownExpire = Memory.Read<float>(RSpell + oSpellSlotTime, sizeof(float));
	int GetRLevel = Memory.Read<int>(RSpell + oSpellSlotLevel, sizeof(int));

	DWORD Summ1 = obj.GetSpellByID(SpellSlotID::Summoner1);
	DWORD Summ2 = obj.GetSpellByID(SpellSlotID::Summoner2);

	float GetDCooldownExpire = Memory.Read<float>(Summ1 + oSpellSlotTime, sizeof(float));
	float GetFCooldownExpire = Memory.Read<float>(Summ2 + oSpellSlotTime, sizeof(float));


	float GameTime = M.GameTime;

	RGBA colorReady(255, 25, 25);
	RGBA colorDown(25, 255, 25);
	int Qcooldown = GetQCooldownExpire - GameTime + 1;
	std::string sQcd = std::to_string(Qcooldown);
	RGBA Qcolor = colorDown;
	if (GetQLevel < 1)
	{
		sQcd = "-";
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
		sWcd = "-";
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
		sEcd = "-";
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
		sRcd = "-";
	}
	else if (Rcooldown <= 0.f)
	{
		sRcd = "R";
		Rcolor = colorReady;
	}

	if (M.Cooldowns.Type[0]) //text on champ
	{
		draw->String(sQcd, RealPos.x - 20, RealPos.y + 15, centered, Qcolor, Direct3D9.fontTahoma);
		draw->String(sWcd, RealPos.x - 0, RealPos.y + 15, centered, Wcolor, Direct3D9.fontTahoma);
		draw->String(sEcd, RealPos.x + 20, RealPos.y + 15, centered, Ecolor, Direct3D9.fontTahoma);
		draw->String(sRcd, RealPos.x + 40, RealPos.y + 15, centered, Rcolor, Direct3D9.fontTahoma);
	}


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



	

	if (M.Cooldowns.Type[0]) //text on champ
	{
		draw->String(sDcd, RealPos.x - 10, RealPos.y + 30, centered, Dcolor, Direct3D9.fontTahoma);
		draw->String(sFcd, RealPos.x + 30, RealPos.y + 30, centered, Fcolor, Direct3D9.fontTahoma);

		

	if (obj.SummonerSpell1() == "summonersmite")
	{
		float SmiteDamage = Memory.Read<float>(Summ1 + oSpellSlotDamage);
		draw->String(std::to_string((int)SmiteDamage), RealPos.x - 7, RealPos.y + 28, lefted, RGBA(0, 200, 255), Direct3D9.fontTahomaSmall);
		//int SmiteStacks = Memory.Read<int>(Summ1 + 0x58);

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
		//int SmiteStacks = Memory.Read<int>(Summ2 + oSpellSlotRemainingCharge);
		float SmiteDamage = Memory.Read<float>(Summ2 + oSpellSlotDamage);
		//clog.AddLog("%i", SmiteStacks);
		draw->String(std::to_string((int)SmiteDamage), RealPos.x + 33, RealPos.y + 28, lefted, RGBA(0, 200, 255), Direct3D9.fontTahomaSmall);
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
	}

	if (M.Cooldowns.Type[1])//image on champ
	{
	//todo drawing over image
		std::string imgsumm1 = obj.SummonerSpell1();
		std::string imgsumm2 = obj.SummonerSpell2();
		draw->ImageFromMemory(GetSpellImg(imgsumm1), RealPos.x + 60, RealPos.y - 170, sDcd, obj.Address(), 32, 32, false);
		draw->ImageFromMemory(GetSpellImg(imgsumm2), RealPos.x + 60, RealPos.y - 170 + 32, sFcd, obj.Address(), 32, 32, false);
	}

	
	
	

}

void Visuals::ScoreBoard(CObject obj)
{

	DWORD RSpell = obj.GetSpellByID(SpellSlotID::R);
	float GetRCooldownExpire = Memory.Read<float>(RSpell + oSpellSlotTime, sizeof(float));
	int GetRLevel = Memory.Read<int>(RSpell + oSpellSlotLevel, sizeof(int));

	DWORD Summ1 = obj.GetSpellByID(SpellSlotID::Summoner1);
	DWORD Summ2 = obj.GetSpellByID(SpellSlotID::Summoner2);

	float GetDCooldownExpire = Memory.Read<float>(Summ1 + oSpellSlotTime, sizeof(float));
	float GetFCooldownExpire = Memory.Read<float>(Summ2 + oSpellSlotTime, sizeof(float));


	float GameTime = M.GameTime;

	RGBA colorReady(255, 25, 25);
	RGBA colorDown(25, 255, 25);

	int Rcooldown = GetRCooldownExpire - GameTime + 1;
	std::string sRcd = std::to_string(Rcooldown);
	RGBA Rcolor = colorDown;
	if (GetRLevel < 1)
	{
		sRcd = "-";
	}
	else if (Rcooldown <= 0.f)
	{
		sRcd = "R";
		Rcolor = colorReady;
	}


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


	int playerLevel = obj.GetLevel();
	float playerExp = obj.GetEXP();
	float requiredForNext = LevelEXP[playerLevel] - LevelEXP[playerLevel - 1];
	float currentExp = playerExp - LevelEXP[playerLevel - 1];
	float percentLevel = currentExp / requiredForNext * 100;
	//clog.AddLog("%s ,fornext %f ,currexp %f , percentLev %f , required %f", obj.GetChampName().c_str(), LevelEXP[playerLevel], playerExp, percentLevel, requiredForNext);
	std::string spercentLevel = std::to_string((int)percentLevel) + "%";
	//clog.AddLog("%s , %d", obj.GetChampName().c_str(), (int)percentLevel);


	//todo moveable instead of fixed scoreboard pos
	//fix when there are 2 or more same champions (blind picks)
	ImVec2 FirstSumm = ImVec2(985, 280);
	std::string ChampName = obj.GetChampName();
	int Spacing = 38;
	for (int i = 0; i < 10; i++)
	{

		if (ChampName == M.ScoreboardNames[i])
		{

			if (i % 2 == 0)
			{
				FirstSumm.x -= 575;
				FirstSumm.y += Spacing;
			}

			//draw->String(ChampName, FirstSumm.x+100, FirstSumm.y + Spacing * i, centered, RGBA(255,255,255), Direct3D9.fontTahoma);

			
			draw->String(sRcd, FirstSumm.x + 83, FirstSumm.y + 40 + Spacing * i, centered, Rcolor, Direct3D9.fontTahoma);
			draw->String(spercentLevel, FirstSumm.x + 133, FirstSumm.y + 40 + Spacing * i, lefted, RGBA(0, 200, 255), Direct3D9.fontTahoma);


			draw->String(sDcd, FirstSumm.x, FirstSumm.y + Spacing * i, centered, Dcolor, Direct3D9.fontTahoma);
			draw->String(sFcd, FirstSumm.x, FirstSumm.y + 30 + Spacing * i, centered, Fcolor, Direct3D9.fontTahoma);


			if (obj.SummonerSpell1() == "summonersmite")
			{
				float SmiteDamage = Memory.Read<float>(Summ1 + oSpellSlotDamage);
				draw->String(std::to_string((int)SmiteDamage), FirstSumm.x + 5, FirstSumm.y + Spacing * i, lefted, RGBA(0, 200, 255), Direct3D9.fontTahomaSmall);
			}
			else if (obj.SummonerSpell2() == "summonersmite")
			{
				float SmiteDamage = Memory.Read<float>(Summ2 + oSpellSlotDamage);
				draw->String(std::to_string((int)SmiteDamage), FirstSumm.x + 5, FirstSumm.y + 30 + Spacing * i, lefted, RGBA(0, 200, 255), Direct3D9.fontTahomaSmall);
			}

			if (i % 2 == 0)
			{
				FirstSumm.x += 575;
				FirstSumm.y -= Spacing;
			}

		}


	}

	

}

void Visuals::DrawAARanges(CObject obj, int points, float thickness, RGBA color, bool local, RGBA localcolor, bool turret)
{

	if (obj.IsDead())
		return;


	if (local && obj.Address() == Local.Address())
	{

		Vector3 Position = Local.GetPosition();
		draw->CircleRange(Position, points, Local.GetAARange() + Local.GetBoundingRadius(), localcolor, thickness);
		return;

	}

	if (local)
		return;

	if (obj.GetTeam() == Local.GetTeam())
		return;


	if (turret)
	{

		Vector3 Position = obj.GetPosition();

		if (Position == Vector3(105.0, 33.0, 134.0) || Position == Vector3(14576.0, 466.0, 14693.0)) //if fountain
			return;
		draw->CircleRange(Position, points, 750+130, color, thickness);

		return;
	}

	if (obj.IsVisible())
	{
		Vector3 Position = obj.GetPosition();

		draw->CircleRange(Position, points, obj.GetAARange() + obj.GetBoundingRadius(), color, thickness);
	}
}

void Visuals::DrawTracers(CObject obj, float thickness)
{

	if (obj.GetTeam() == Local.GetTeam())
		return;

	if (obj.IsDead() || Local.IsDead())
		return;

	Vector3 Position = obj.GetPosition();
	ImVec2 RealPos = Direct3D9.WorldToScreen(Position);

	Vector3 LocalPosition = Local.GetPosition();
	ImVec2 LocalRealPos = Direct3D9.WorldToScreen(LocalPosition);


	if (RealPos.x == 0.f && RealPos.y == 0.f)
		return;

	if (LocalRealPos.x == 0.f && LocalRealPos.y == 0.f)
		return;

	float distance = sqrtf((LocalRealPos.x - RealPos.x) * (LocalRealPos.x - RealPos.x) + (LocalRealPos.y - RealPos.y) * (LocalRealPos.y - RealPos.y));

	int r = 255 - ((distance / 22) * (int)(2.55));
	int g = (distance / 22) * (int)(2.55);
	int b = 0;
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
			draw->String(obj.GetChampName(), RealPos.x, RealPos.y, centered, RGBA(255, 255, 255), Direct3D9.fontTahoma);

	}

	RGBA color(r, g, b);

	
	draw->Line(LocalRealPos.x, LocalRealPos.y, RealPos.x, RealPos.y, color, thickness);

	

}


//todo reverse bounding radius and check it within smite range
//https://www.unknowncheats.me/forum/league-of-legends/327917-incoming-damage-minions.html
void Visuals::AutoSmite(CObject obj, int slot, int mode, float mouseSpeed)
{
	//clog.AddLog("%s , %x , %f ", obj.GetName().c_str(), obj.Address(), obj.GetDistToMe(Local));
	if (obj.GetTeam() == Local.GetTeam())
		return;

	if (obj.IsDead() || Local.IsDead())
		return;

	if (!obj.IsVisible())
		return;

	if (obj.GetDistTo(Local) > 560.f)
		return;

	
	DWORD SmiteSlot = 0;
	int SpellKey = 0;
	if (slot == 0)
	{
		SmiteSlot = Local.GetSpellByID(SpellSlotID::Summoner1);
		SpellKey = DIK_D;
	}
	else if (slot == 1)
	{

		SmiteSlot = Local.GetSpellByID(SpellSlotID::Summoner2);
		SpellKey = DIK_F;
	}
	else return;

	float SmiteCooldownExpire = Memory.Read<float>(SmiteSlot + oSpellSlotRemainingCD, sizeof(float));
	int SmiteStacks = Memory.Read<int>(SmiteSlot + oSpellSlotRemainingCharge);
	float SmiteDamage = Memory.Read<float>(SmiteSlot + oSpellSlotDamage);

	if (!SmiteStacks)
		return;

	float cd = SmiteCooldownExpire - M.GameTime + 1;
	//clog.AddLog("%f", cd);
	if (cd > 0.f)
		return;
	std::string objName = obj.GetName();

	if (!(objName.find("SRU_Baron") != std::string::npos || objName.find("SRU_Dragon") != std::string::npos || objName.find("SRU_Gromp") != std::string::npos
		|| objName.find("SRU_Razorbeak3") != std::string::npos || objName.find("SRU_Razorbeak9") != std::string::npos || objName.find("SRU_Blue") != std::string::npos || objName.find("SRU_Red") != std::string::npos
		|| objName.find("SRU_Murkwolf2") != std::string::npos || objName.find("SRU_Murkwolf8") != std::string::npos || objName.find("SRU_RiftHerald") != std::string::npos
		|| objName.find("Sru_Crab") != std::string::npos || objName.find("SRU_Krug1") != std::string::npos || objName.find("SRU_Krug5") != std::string::npos))
		return;


	Vector3 Position = obj.GetPosition();
	ImVec2 RealPos = Direct3D9.WorldToScreen(Position);

	if (RealPos.x == 0 && RealPos.y == 0)
		return;

	draw->BoxFilled(RealPos.x, RealPos.y, 10, 10, RGBA(255, 0, 0));

	if (obj.GetHealth() <= SmiteDamage)
	{
		clog.AddLog("a");
	
		BlockInput(1);	

		mouse->ChangeSpeed(mouseSpeed);

		mouse->StoreCurrentPos();
		int tries = 0;

		while (!obj.IsDead())
		{
			if (mode == 0)
				mouse->MouseMoveInstant(RealPos.x, RealPos.y);
			else if (mode == 1)
				mouse->MouseMoveSLD(RealPos.x, RealPos.y);
			else
				mouse->MouseMove(RealPos.x, RealPos.y);
			keyboard->GenerateKeyScancode(SpellKey, false);

			POINT LastMousePos = mouse->GetStoredPos();

			if (mode == 0)
				mouse->MouseMoveInstant(LastMousePos.x, LastMousePos.y);
			else if (mode == 1)
				mouse->MouseMoveSLD(LastMousePos.x, LastMousePos.y);
			else
				mouse->MouseMove(LastMousePos.x, LastMousePos.y);

			mouse->ChangeSpeed(1.f);
			BlockInput(0);
			tries++;
			if (tries == 3 && !obj.IsDead()) //so it doesnt get stuck when failed to smite
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			}
		}
	}

}

void Visuals::LastHit(CObject obj, RGBA color)
{

	if (obj.GetTeam() == Local.GetTeam())
		return;
	if (obj.IsDead() || Local.IsDead())
		return;
	if (!obj.IsVisible())
		return;
	if (obj.GetDistTo(Local) > 1500)
		return;
	if (obj.GetName().find("Minion_") == std::string::npos)
		return;

	Vector3 Position = obj.GetPosition();
	ImVec2 RealPos = Direct3D9.WorldToScreen(Position);
	if (RealPos.x == 0 && RealPos.y == 0)
		return;
	if ((RealPos.x <= SCREENWIDTH * 1.2) && (RealPos.x >= SCREENWIDTH / 2 * (-1)) && (RealPos.y <= SCREENHEIGHT * 1.5) && (RealPos.y >= SCREENHEIGHT / 2 * (-1)))
	{
		float dmg = Local.GetTotalDamage(obj);
		float critChance = Local.GetCrit();
		//draw->String(std::to_string(dmg), RealPos.x, RealPos.y, centered, RGBA(255, 255, 255), Direct3D9.fontTahoma);
		//todo draw over hp bar instead of circles
		float xd = dmg / obj.GetHealth();
		if (critChance == 1.f)
			xd = dmg * 1.75 / obj.GetHealth();
		//else if (critChance > 0.f && xd <0.5)
		//	draw->Circle(RealPos.x, RealPos.y, 11 * 0.5, RGBA(255, 255, 0));

		if (xd <= 1)
			draw->Circle(RealPos.x, RealPos.y, 11 * xd, RGBA(255, 255, 255));
		draw->Circle(RealPos.x, RealPos.y, 11, color);
	}

}

void Visuals::InhibTimers(CObject obj)
{

	//if (Local.IsDead())
	//{
	//	clog.AddLog("Dead %i , %f", Local.GetAlive(), Local.GetHealth());
	//	float GameTime = Memory.Read<float>(ClientAddress + oGameTime, sizeof(float));
	//	clog.AddLog("%f , %i", Local.GetTimer(), Local.GetAlive());
	//	if (Local.GetAlive())
	//	{
	//		Local.SetTimer(GameTime + 300);
	//		Local.SetAlive(false);
	//	}

	//	ImVec2 RealPos = Direct3D9.WorldToScreen(Local.GetPosition());

	//	if ((RealPos.x <= SCREENWIDTH * 1.2) && (RealPos.x >= SCREENWIDTH / 2 * (-1)) && (RealPos.y <= SCREENHEIGHT * 1.5) && (RealPos.y >= SCREENHEIGHT / 2 * (-1)))
	//	{
	//		float timer = Local.GetTimer() - GameTime;
	//		std::string sTimer = std::to_string(timer);
	//		draw->String(sTimer, RealPos.x, RealPos.y, centered, RGBA(255, 255, 255), Direct3D9.fontTahoma);
	//		clog.AddLog("%f , %f , %i", Local.GetTimer(), timer, Local.GetAlive());
	//	}
	//}
	//else
	//{
	//	clog.AddLog("Alive %i, %f , %x", Local.GetAlive(), Local.GetHealth(), Local.Address());
	//	if (!Local.GetAlive())
	//	{
	//		clog.AddLog("ssdads");
	//		Local.SetAlive(true);
	//	}
	//}




	//if (obj.GetTeam() == Local.GetTeam())
	//	return;
	if (obj.IsDead())
	{
		ImVec2 RealPos = Direct3D9.WorldToScreen(obj.GetPosition());

		if (RealPos.x == 0.f && RealPos.y == 0.f)
			return;

		if ((RealPos.x <= SCREENWIDTH * 1.2) && (RealPos.x >= SCREENWIDTH / 2 * (-1)) && (RealPos.y <= SCREENHEIGHT * 1.5) && (RealPos.y >= SCREENHEIGHT / 2 * (-1)))
		{
			float RespawnTimer = Memory.Read<float>(obj.Address() + oInhiRemainingRespawnTime);
			draw->String(std::to_string((int)RespawnTimer), RealPos.x, RealPos.y, centered, RGBA(255, 255, 255), Direct3D9.fontTahoma);
		}
	}

}

std::map<DWORD, float>wardTimer;
void Visuals::WardsRange(CObject obj)
{
	if (Local.GetTeam() == obj.GetTeam())
		return;

	if (obj.IsDead())
		return;

	int type = obj.IsWard(); // store ward type for later
	if (!type)
		return;

	Vector3 Pos = obj.GetPosition();
	ImVec2 RealPos = Direct3D9.WorldToScreen(Pos);

	if (RealPos.x == 0.f && RealPos.y == 0.f)
		return;

	if (!((RealPos.x <= SCREENWIDTH * 1.2) && (RealPos.x >= SCREENWIDTH / 2 * (-1)) && (RealPos.y <= SCREENHEIGHT * 1.5) && (RealPos.y >= SCREENHEIGHT / 2 * (-1))))
		return;

	if (type == NormalWard)
	{
		RGBA WardColor(255, 170, 0);
		draw->String("Ward", RealPos.x, RealPos.y, centered, WardColor, Direct3D9.fontTahomaSmall);
		draw->CircleRange(Pos, 14, 900, WardColor);
		if (wardTimer[obj.Address()] - M.GameTime <= 0)
			wardTimer[obj.Address()] = M.GameTime + obj.GetMana();

		draw->String(std::to_string((int)(wardTimer[obj.Address()] - M.GameTime)), RealPos.x, RealPos.y+10, centered, RGBA(255, 255, 255), Direct3D9.fontTahomaSmall);
	}
	else if (type == ControlWard)
	{
		RGBA ControlWardColor(255, 40, 0);
		draw->String("Control Ward", RealPos.x, RealPos.y, centered, ControlWardColor, Direct3D9.fontTahomaSmall);
		draw->CircleRange(Pos, 14, 900, ControlWardColor);
	}
	else if (type == BlueWard)
	{
		RGBA BlueWardColor(0, 65, 255);
		draw->String("Blue Ward", RealPos.x, RealPos.y, centered, BlueWardColor, Direct3D9.fontTahomaSmall);
		draw->CircleRange(Pos, 14, 500, BlueWardColor);
	}
	//todo plants sometimes show up as blue ward
	draw->String(obj.GetName(), RealPos.x, RealPos.y+30, centered, RGBA(255,255,255), Direct3D9.fontTahomaSmall);




}



std::map<DWORD, float>lastEXP;
int wait = 0;
std::string Expstr;
std::string currentChampName;
void Visuals::GankAlerter(CObject obj)
{
	wait++;


	if (obj.GetTeam() == Local.GetTeam())
		return;

	if (obj.IsDead())
		return;
	if (!obj.IsVisible())
		return;

	float temp = lastEXP[obj.Address()];

	if (obj.GetEXP() != lastEXP[obj.Address()])
	{
		//Expstr = std::to_string(obj.GetEXP() - lastEXP[obj.Address()]);

		if (INRANGE((obj.GetEXP() - lastEXP[obj.Address()]) * 2, 115.0, 117.0) || INRANGE((obj.GetEXP() - lastEXP[obj.Address()]) * 3, 115.0, 117.0))
			clog.AddLog("[error] someone near %s - cannon", obj.GetChampName().c_str());

		if (INRANGE((obj.GetEXP() - lastEXP[obj.Address()]) * 2, 37.0, 39.0) || INRANGE((obj.GetEXP() - lastEXP[obj.Address()]) * 3, 37.0, 39.0))
			clog.AddLog("[error] someone near %s - caster", obj.GetChampName().c_str());

		if (INRANGE((obj.GetEXP() - lastEXP[obj.Address()]) * 2, 74.0, 77.0) || INRANGE((obj.GetEXP() - lastEXP[obj.Address()]) * 3, 74.0, 77.0))
			clog.AddLog("[error] someone near %s - melee", obj.GetChampName().c_str());

		if (INRANGE((obj.GetEXP() - temp) * 2, 115.0, 117.0)
			|| INRANGE((obj.GetEXP() - temp) * 2, 37.0, 39.0)
			|| INRANGE((obj.GetEXP() - temp) * 2, 74.0, 77.0))
		{
			//ImVec2 RealPos = Direct3D9.WorldToScreen(obj.GetPosition());
			//draw->String("2 PEOPLE NEARBY!", RealPos.x, RealPos.y, centered, RGBA(255, 50, 50), fontTahoma);
			Expstr = "SOMEONE NEAR " + obj.GetChampName();
			currentChampName = obj.GetChampName();
		}


		if (INRANGE((obj.GetEXP() - temp) * 3, 115.0, 117.0)
			|| INRANGE((obj.GetEXP() - temp) * 3, 37.0, 39.0)
			|| INRANGE((obj.GetEXP() - temp) * 3, 74.0, 77.0))
		{
			/*ImVec2 RealPos = Direct3D9.WorldToScreen(obj.GetPosition());
			draw->String("3 PEOPLE NEARBY!", RealPos.x, RealPos.y, centered, RGBA(255, 50, 50), fontTahoma);*/
			Expstr = "2 PEOPLE NEAR " + obj.GetChampName();
			currentChampName = obj.GetChampName();
		}

		
		if (!Expstr.empty())
		{

			bool Open = true;
			std::string wndName = "##gankalert" + obj.GetChampName();
			const auto flags = ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs;

		
			//clog.AddLog("%s got %s xp ", obj.GetChampName().c_str(), std::to_string(obj.GetEXP() - lastEXP[obj.Address()]).c_str());
		}
		if (wait > 1000)
			wait = 0;
		lastEXP[obj.Address()] = obj.GetEXP();
		
	}
	if (wait < 1000)
	{
		if (!Expstr.empty() && obj.GetChampName() == currentChampName)
		{
			//todo better drawings
			ImVec2 RealPos = Direct3D9.WorldToScreen(obj.GetPosition());
			if (RealPos.x == 0.f && RealPos.y == 0.f)
				return;

			if (!((RealPos.x <= SCREENWIDTH * 1.2) && (RealPos.x >= SCREENWIDTH / 2 * (-1)) && (RealPos.y <= SCREENHEIGHT * 1.5) && (RealPos.y >= SCREENHEIGHT / 2 * (-1))))
				return;
			draw->String(Expstr, RealPos.x, RealPos.y, centered, RGBA(255, 50, 50), Direct3D9.fontTahoma);

		}
	}



}

//todo calculate runes items etc, more options like both W's hitting etc
void Visuals::TalonDamageCalc(CObject obj)
{
	if (obj.GetTeam() == Local.GetTeam())
		return;
	if (obj.IsDead())
		return;
	if (!obj.IsVisible())
		return;

	ImVec2 RealPos = Direct3D9.WorldToScreen(obj.GetPosition());
	if (RealPos.x == 0.f && RealPos.y == 0.f)
		return;

	if (!((RealPos.x <= SCREENWIDTH * 1.2) && (RealPos.x >= SCREENWIDTH / 2 * (-1)) && (RealPos.y <= SCREENHEIGHT * 1.5) && (RealPos.y >= SCREENHEIGHT / 2 * (-1))))
		return;
	

	int playerLevel = Local.GetLevel();
	DWORD QSpell = Local.GetSpellByID(SpellSlotID::Q);
	int QLevel = Memory.Read<int>(QSpell + oSpellSlotLevel, sizeof(int));
	DWORD WSpell = Local.GetSpellByID(SpellSlotID::W);
	int WLevel = Memory.Read<int>(WSpell + oSpellSlotLevel, sizeof(int));
	DWORD RSpell = Local.GetSpellByID(SpellSlotID::R);
	int RLevel = Memory.Read<int>(RSpell + oSpellSlotLevel, sizeof(int));

	float PDamage = TalonPDmg[playerLevel] + Local.GetBonusAD() * 2.0;
	int PDamageTotal = Local.GetTotalDamage(obj, PDamage);
	//adds an autoattack thats required to proc passive
	PDamageTotal += Local.GetTotalDamage(obj);

	float QDamage = QLevel > 0 ? TalonQDmg[QLevel] + Local.GetBonusAD() * 1.1 : 0;
	int QDamageTotal = Local.GetTotalDamage(obj, QDamage);

	float WDamage = WLevel > 0 ? TalonW1Dmg[WLevel] + Local.GetBonusAD() * 0.4 : 0;
	int WDamageTotal = Local.GetTotalDamage(obj, WDamage);

	float RDamage = RLevel > 0 ? TalonRDmg[RLevel] + Local.GetBonusAD() * 1.0 : 0;
	int RDamageTotal = Local.GetTotalDamage(obj, RDamage);

	//clog.AddLog("%s , %d , %d , %d , %d ,%d", obj.GetChampName().c_str(), (int)Local.GetTotalDamage(obj), PDamageTotal, QDamageTotal, WDamageTotal, RDamageTotal);
	int TotalDamage = PDamageTotal + QDamageTotal + WDamageTotal + RDamageTotal;
	
	int iperecentDamage = TotalDamage / obj.GetHealth() * 100;
	std::string spercentDamage = 100 - iperecentDamage > 0 ? std::to_string(100 - iperecentDamage) + "%" : "Dead";
	std::string str = std::to_string(TotalDamage) + " ->" + spercentDamage;
	draw->String(str, RealPos.x, RealPos.y-20, centered, RGBA(255, 255, 255), Direct3D9.fontTahoma);

}