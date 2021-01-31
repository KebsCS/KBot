#include "Visuals.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Utils.h"

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
//
//LPDIRECT3DTEXTURE9 Visuals::GetSpellImg(std::string name)
//{
//	LPDIRECT3DTEXTURE9 ret = NULL;
//	if (name.find("boost") != std::string::npos)
//		ret = draw->textureCleanse;
//	else if (name.find("exhaust") != std::string::npos)
//		ret = draw->textureExhaust;
//	else if (name.find("flash") != std::string::npos)
//		ret = draw->textureFlash;
//	else if (name.find("haste") != std::string::npos)
//		ret = draw->textureGhost;
//	else if (name.find("heal") != std::string::npos)
//		ret = draw->textureHeal;
//	else if (name.find("smite") != std::string::npos)
//		ret = draw->textureSmite;
//	else if (name.find("teleport") != std::string::npos)
//		ret = draw->textureTeleport;
//	else if (name.find("dot") != std::string::npos)
//		ret = draw->textureIgnite;
//	else if (name.find("barrier") != std::string::npos)
//		ret = draw->textureBarrier;
//	else
//		ret = draw->textureClarity;
//
//	return ret;
//}

void Visuals::CooldownTimers(CObject obj)
{
	if ((obj.GetTeam() == Local.GetTeam()) && !M.bDebug)
		return;

	if (obj.IsDead())
		return;

	if (!obj.IsVisible())
		return;

	Vector3 Position = obj.GetPosition();

	ImVec2 RealPos = Direct3D9.WorldToScreen(Position);

	if (RealPos.x == 0.f && RealPos.y == 0.f)
		return;

	if (!draw->IsOnScreen(RealPos))
		return;

	//todo clean this up and summoner icons instead of text
	CSpellSlot* QSpell = obj.GetSpellByID(SpellSlotID::Q);
	float GetQCooldownExpire = QSpell->GetCooldownExpire();
	int GetQLevel = QSpell->GetLevel();

	CSpellSlot* WSpell = obj.GetSpellByID(SpellSlotID::W);
	float GetWCooldownExpire = WSpell->GetCooldownExpire();
	int GetWLevel = WSpell->GetLevel();

	CSpellSlot* ESpell = obj.GetSpellByID(SpellSlotID::E);
	float GetECooldownExpire = ESpell->GetCooldownExpire();
	int GetELevel = ESpell->GetLevel();

	CSpellSlot* RSpell = obj.GetSpellByID(SpellSlotID::R);
	float GetRCooldownExpire = RSpell->GetCooldownExpire();
	int GetRLevel = WSpell->GetLevel();

	CSpellSlot* Summ1 = obj.GetSpellByID(SpellSlotID::D);
	CSpellSlot* Summ2 = obj.GetSpellByID(SpellSlotID::F);

	float GetDCooldownExpire = Summ1->GetCooldownExpire();
	float GetFCooldownExpire = Summ2->GetCooldownExpire();

	float GameTime = M.fGameTime;

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
			float SmiteDamage = Summ1->GetDamage();
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
			float SmiteDamage = Summ2->GetDamage();
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
		/*	draw->ImageFromMemory(GetSpellImg(imgsumm1), RealPos.x + 70, RealPos.y - 170, sDcd, obj.Address(), 32, 32, false);
			draw->ImageFromMemory(GetSpellImg(imgsumm2), RealPos.x + 70, RealPos.y - 170 + 32, sFcd, obj.Address(), 32, 32, false);*/
	}
}

void Visuals::ScoreBoard(CObject obj)
{
	CSpellSlot* RSpell = obj.GetSpellByID(SpellSlotID::R);
	float GetRCooldownExpire = RSpell->GetCooldownExpire();

	CSpellSlot* Summ1 = obj.GetSpellByID(SpellSlotID::D);
	CSpellSlot* Summ2 = obj.GetSpellByID(SpellSlotID::F);

	float GetDCooldownExpire = Summ1->GetCooldownExpire();
	float GetFCooldownExpire = Summ2->GetCooldownExpire();

	float GameTime = M.fGameTime;

	RGBA colorReady(255, 25, 25);
	RGBA colorDown(25, 255, 25);

	int Rcooldown = GetRCooldownExpire - GameTime + 1;
	std::string sRcd = std::to_string(Rcooldown);
	RGBA Rcolor = colorDown;
	int GetRLevel = RSpell->GetLevel();
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

	//CSpellSlot* Item1 = obj.GetSpellByID(SpellSlotID::Item1);
	//float GetItem1CooldownExpire = Item1
	//int GetItem1Level = Memory.Read<int>(Item1 + oSpellSlotLevel, sizeof(int)); //if something in itemslot
	//int Item1cooldown = GetItem1CooldownExpire - GameTime + 1;
	//std::string sItem1cd = std::to_string(Item1cooldown);
	//RGBA Item1color = colorDown;

	//if (Item1cooldown <= 0.f || GetItem1Level != 1)
	//{
	//	sItem1cd = "1";
	//	Item1color = colorReady;
	//}

	std::vector<std::string>sItemsCD(6);
	std::vector<RGBA>ItemsColor(6);
	if (M.Cooldowns.Scoreboard.Items)
	{
		std::vector<CSpellSlot*>Items(6);
		std::vector<float>GetItemsCooldownExpire(6);
		std::vector<int>Itemscooldown(6);
		for (int i = 0; i < 6; i++)
		{
			Items[i] = obj.GetSpellByID(SpellSlotID::Item1 + i);
			GetItemsCooldownExpire[i] = Items[i]->GetCooldownExpire();
			Itemscooldown[i] = GetItemsCooldownExpire[i] - GameTime + 1;
			sItemsCD[i] = std::to_string(Itemscooldown[i]);
			ItemsColor[i] = colorDown;
		}

		for (int i = 0; i < 6; i++)
		{
			if (Itemscooldown[i] <= 0.f)
			{
				sItemsCD[i] = std::to_string(i + 1) + ".";
				ItemsColor[i] = colorReady;
			}
		}
	}

	int playerLevel = obj.GetLevel();
	float playerExp = obj.GetEXP();
	float requiredForNext = LevelEXP[playerLevel] - LevelEXP[playerLevel - 1];
	float currentExp = playerExp - LevelEXP[playerLevel - 1];
	float percentLevel = playerLevel != 18 ? currentExp / requiredForNext * 100 : 100;
	//clog.AddLog("%s ,fornext %f ,currexp %f , percentLev %f , required %f", obj.GetChampName().c_str(), LevelEXP[playerLevel], playerExp, percentLevel, requiredForNext);
	std::string spercentLevel = std::to_string((int)percentLevel) + "%";
	//clog.AddLog("%s , %d", obj.GetChampName().c_str(), (int)percentLevel);

	//todo moveable instead of fixed scoreboard pos
	//fix when there are 2 or more same champions (blind picks)
	ImVec2 FirstSumm = ImVec2(M.Cooldowns.Scoreboard.Pos[0], M.Cooldowns.Scoreboard.Pos[1]);
	std::string ChampName = obj.GetChampName();
	int Spacing = 38;
	for (int i = 0; i < 10; i++)
	{
		if (ChampName == M.sScoreboardNames[i])
		{
			if (i % 2 == 0)
			{
				FirstSumm.x -= 575 + 5; //+ since I don't have text centering yet
				FirstSumm.y += Spacing;
			}

			//draw->String(ChampName, FirstSumm.x+100, FirstSumm.y + Spacing * i, centered, RGBA(255,255,255), Direct3D9.fontTahoma);
			if (M.Cooldowns.Scoreboard.Items)
			{
				for (int item = 0; item < 6; item++)
				{
					draw->String(sItemsCD[item], FirstSumm.x + 318 + item * 35, FirstSumm.y + 40 + Spacing * i, centered, ItemsColor[item], Direct3D9.fontTahoma);
				}
			}

			//todo when disabled dont rpm
			if (M.Cooldowns.Scoreboard.Ults)
				draw->String(sRcd, FirstSumm.x + 83, FirstSumm.y + 40 + Spacing * i, centered, Rcolor, Direct3D9.fontTahoma);

			if (M.Cooldowns.Scoreboard.Exp)
				draw->String(spercentLevel, FirstSumm.x + 133, FirstSumm.y + 40 + Spacing * i, lefted, RGBA(0, 200, 255), Direct3D9.fontTahoma);

			if (M.Cooldowns.Scoreboard.Summs)
			{
				draw->String(sDcd, FirstSumm.x, FirstSumm.y + Spacing * i, centered, Dcolor, Direct3D9.fontTahoma);
				draw->String(sFcd, FirstSumm.x, FirstSumm.y + 30 + Spacing * i, centered, Fcolor, Direct3D9.fontTahoma);

				if (obj.SummonerSpell1() == "summonersmite")
				{
					float SmiteDamage = Summ1->GetDamage();
					draw->String(std::to_string((int)SmiteDamage), FirstSumm.x + 5, FirstSumm.y + Spacing * i, lefted, RGBA(0, 200, 255), Direct3D9.fontTahomaSmall);
				}
				else if (obj.SummonerSpell2() == "summonersmite")
				{
					float SmiteDamage = Summ2->GetDamage();
					draw->String(std::to_string((int)SmiteDamage), FirstSumm.x + 5, FirstSumm.y + 30 + Spacing * i, lefted, RGBA(0, 200, 255), Direct3D9.fontTahomaSmall);
				}
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

	Vector3 Position = obj.GetPosition();

	if (Position.IsZero())
		return;

	if (!draw->IsOnScreen(Position))
		return;

	if (local && obj.Address() == Local.Address())
	{
		draw->CircleRange(Position, points, Local.GetAARange() + Local.GetBoundingRadius(), localcolor, thickness);
		return;
	}

	if (local)
		return;

	if ((obj.GetTeam() == Local.GetTeam()) && !M.bDebug)
		return;

	if (turret)
	{
		if (Position == Vector3(105.0, 33.0, 134.0) || Position == Vector3(14576.0, 466.0, 14693.0)) //if fountain
			return;
		draw->CircleRange(Position, points, obj.unitInfo->baseAttackRange + obj.unitInfo->selectionRadius, color, thickness);

		//draw->String(Direct3D9.WorldToScreen(Position), Memory.ReadString(obj.Address() + 0x1014), RGBA(255, 255, 255));
		return;
	}

	if (obj.IsVisible())
	{
		draw->CircleRange(Position, points, obj.GetAARange() + obj.GetBoundingRadius(), color, thickness);
	}
}

void Visuals::DrawTracers(CObject obj, float thickness)
{
	if ((obj.GetTeam() == Local.GetTeam()) && !M.bDebug)
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

//todo bounding radius within smite range
//https://www.unknowncheats.me/forum/league-of-legends/327917-incoming-damage-minions.html
void Visuals::AutoSmite(CObject obj, int slot, int mode, float mouseSpeed)
{
	//clog.AddLog("%s , %x , %f ", obj.GetName().c_str(), obj.Address(), obj.GetDistToMe(Local));

	if (Local.SummonerSpell1() != "summonersmite" && Local.SummonerSpell2() != "summonersmite")
		return;

	if (obj.GetNetworkID() - (unsigned int)0x40000000 > 0x100000)
		return;

	if (obj.GetTeam() == Local.GetTeam())
		return;

	if (!obj.IsVisible())
		return;

	if (obj.IsDead() || Local.IsDead())
		return;

	if (obj.GetDistTo(Local) > 560.f)
		return;

	CSpellSlot* SmiteSlot = 0;
	int SpellKey = 0;
	if (slot == 0)
	{
		SmiteSlot = Local.GetSpellByID(SpellSlotID::D);
		SpellKey = DIK_D;
	}
	else if (slot == 1)
	{
		SmiteSlot = Local.GetSpellByID(SpellSlotID::F);
		SpellKey = DIK_F;
	}
	else return;

	float SmiteCooldownExpire = SmiteSlot->GetCooldownExpire();
	int SmiteStacks = SmiteSlot->GetRemainingCharge();
	float SmiteDamage = SmiteSlot->GetDamage();

	if (!SmiteStacks)
		return;

	float cd = SmiteCooldownExpire - M.fGameTime + 1;
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

	draw->Circle(RealPos.x, RealPos.y, 10, RGBA(255, 0, 0));

	if (obj.GetHealth() <= SmiteDamage)
	{
		//clog.AddLog("a");

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
	if (obj.GetNetworkID() - (unsigned int)0x40000000 > 0x100000)
		return;
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
	if (!obj.HasUnitTags(Unit_Minion_Lane))
		return;

	Vector3 Position = obj.GetPosition();
	ImVec2 RealPos = Direct3D9.WorldToScreen(Position);
	if (RealPos.x == 0 && RealPos.y == 0)
		return;
	if (draw->IsOnScreen(RealPos))
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

		/*	if (dmg >= obj.GetHealth())
			{
				ImVec2 p = Direct3D9.GetHpBarPos(obj);
				draw->Rect(Vector4(p.x - 34, p.y - 9, p.x + 32, p.y + 1), RGBA(255, 25, 25), 0, 2);
			}*/
	}
}

void Visuals::InhibTimers(CObject obj)
{
	//draw->String(Direct3D9.WorldToScreen(obj.GetPosition()), std::to_string(obj.Address()), RGBA(255, 255, 255));

	if (obj.IsDead())
	{
		Vector3 Position = obj.GetPosition();
		ImVec2 RealPos = Direct3D9.WorldToScreen(Position);
		ImVec2 MapPos = Direct3D9.WorldToMinimap(Position);

		float RespawnTimer = Memory.Read<float>(obj.Address() + oInhiRemainingRespawnTime);

		draw->String(std::to_string((int)RespawnTimer), MapPos.x, MapPos.y, centered, RGBA(255, 255, 255), Direct3D9.fontTahoma);

		//clog.AddLog("%x", obj.Address());

		if (RealPos.x == 0.f && RealPos.y == 0.f)
			return;

		if ((RealPos.x <= SCREENWIDTH * 1.2) && (RealPos.x >= SCREENWIDTH / 2 * (-1)) && (RealPos.y <= SCREENHEIGHT * 1.5) && (RealPos.y >= SCREENHEIGHT / 2 * (-1)))
		{
			draw->String(std::to_string((int)RespawnTimer), RealPos.x, RealPos.y, centered, RGBA(255, 255, 255), Direct3D9.fontTahoma);
		}
	}
}

std::map<int, float>wardTimer;
void Visuals::WardsRange(CObject obj)
{
	if (obj.GetNetworkID() - (unsigned int)0x40000000 > 0x100000)
		return;
	if ((obj.GetTeam() == Local.GetTeam()) && !M.bDebug)
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
		if (!wardTimer.count(obj.GetIndex()) || (wardTimer[obj.GetIndex()] - M.fGameTime) < -100) // if timer for a ward doesnt exist
		{
			wardTimer[obj.GetIndex()] = M.fGameTime + obj.GetMana();
			//todo wards sometimes start with around 60 mana and its random
			//clog.AddLog("%f , %d", obj.GetMaxMana(),obj.Address());
		}
		if (wardTimer[obj.GetIndex()] - M.fGameTime > 0) //draw only if timer above 0
		{
			draw->String(std::to_string((int)(wardTimer[obj.GetIndex()] - M.fGameTime)), RealPos.x, RealPos.y + 10, centered, RGBA(255, 255, 255), Direct3D9.fontTahomaSmall);
			RGBA WardColor(255, 170, 0);
			draw->String("Ward", RealPos.x, RealPos.y, centered, WardColor, Direct3D9.fontTahomaSmall);
			//draw->String(std::to_string(obj.Address()), RealPos.x, RealPos.y, centered, WardColor, Direct3D9.fontTahomaSmall);
			draw->CircleRange(Pos, 14, 900, WardColor);
		}
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
	//for testing
	std::string testName = obj.GetChampName() + " " + obj.GetName();
	draw->String(testName, RealPos.x, RealPos.y + 30, centered, RGBA(255, 255, 255), Direct3D9.fontTahomaSmall);
}

std::map<int, float>lastEXP;
std::map<int, float>expTimer;
std::map<int, int>howManyNearby;
void Visuals::GankAlerter(CObject obj)
{
	if ((obj.GetTeam() == Local.GetTeam()) && !M.bDebug)
		return;

	if (obj.IsDead())
		return;
	if (!obj.IsVisible())
		return;

	float temp = lastEXP[obj.GetIndex()];

	//solo exp:
	//~60 melee ~30 caster

	//todo when 3 nearby and melee is killed it shows as 1 nearby(caster)
	if (obj.GetEXP() != lastEXP[obj.GetIndex()])
	{
		//clog.AddLog("%s got %f xp",obj.GetChampName(), obj.GetEXP() - lastEXP[obj.Address()]);

		//if (INRANGE((obj.GetEXP() - lastEXP[obj.Address()]) * 2, 115.0, 117.0) || INRANGE((obj.GetEXP() - lastEXP[obj.Address()]) * 3, 115.0, 117.0)
		//	|| INRANGE((obj.GetEXP() - lastEXP[obj.Address()]) * 4, 115.0, 117.0) || INRANGE((obj.GetEXP() - lastEXP[obj.Address()]) * 5, 115.0, 117.0))
		//	clog.AddLog("[error] someone near %s - cannon", obj.GetChampName().c_str());

		//else if (INRANGE((obj.GetEXP() - lastEXP[obj.Address()]) * 2, 37.0, 39.0) || INRANGE((obj.GetEXP() - lastEXP[obj.Address()]) * 3, 37.0, 39.0)
		//	|| INRANGE((obj.GetEXP() - lastEXP[obj.Address()]) * 4, 37.0, 39.0) || INRANGE((obj.GetEXP() - lastEXP[obj.Address()]) * 5, 37.0, 39.0))
		//	clog.AddLog("[error] someone near %s - caster", obj.GetChampName().c_str());

		//else if (INRANGE((obj.GetEXP() - lastEXP[obj.Address()]) * 2, 74.0, 77.0) || INRANGE((obj.GetEXP() - lastEXP[obj.Address()]) * 3, 74.0, 77.0)
		//	 || INRANGE((obj.GetEXP() - lastEXP[obj.Address()]) * 4, 74.0, 77.0) || INRANGE((obj.GetEXP() - lastEXP[obj.Address()]) * 5, 74.0, 77.0))
		//	clog.AddLog("[error] someone near %s - melee", obj.GetChampName().c_str());

		if ((float)((obj.GetEXP() - temp) * 2) >= 115.f && (float)((obj.GetEXP() - temp) * 2) <= 117.f
			|| (float)((obj.GetEXP() - temp) * 2) >= 37.f && (float)((obj.GetEXP() - temp) * 2) <= 39.f
			|| (float)((obj.GetEXP() - temp) * 2) >= 74.f && (float)((obj.GetEXP() - temp) * 2) <= 77.f)
		{
			howManyNearby[obj.GetIndex()] = 1;
			expTimer[obj.GetIndex()] = M.fGameTime + 5;
		}
		else if (INRANGE((obj.GetEXP() - temp) * 3, 115.0, 117.0)
			|| INRANGE((obj.GetEXP() - temp) * 3, 37.0, 39.0)
			|| INRANGE((obj.GetEXP() - temp) * 3, 74.0, 77.0))
		{
			howManyNearby[obj.GetIndex()] = 2;
			expTimer[obj.GetIndex()] = M.fGameTime + 5;
		}
		else if (INRANGE((obj.GetEXP() - temp) * 4, 115.0, 117.0)
			|| INRANGE((obj.GetEXP() - temp) * 4, 37.0, 39.0)
			|| INRANGE((obj.GetEXP() - temp) * 4, 74.0, 77.0))
		{
			howManyNearby[obj.GetIndex()] = 3;
			expTimer[obj.GetIndex()] = M.fGameTime + 5;
		}
		else if (INRANGE((obj.GetEXP() - temp) * 5, 115.0, 117.0)
			|| INRANGE((obj.GetEXP() - temp) * 5, 37.0, 39.0)
			|| INRANGE((obj.GetEXP() - temp) * 5, 74.0, 77.0))
		{
			howManyNearby[obj.GetIndex()] = 4;
			expTimer[obj.GetIndex()] = M.fGameTime + 5;
		}

		lastEXP[obj.GetIndex()] = obj.GetEXP();
	}
	if (expTimer[obj.GetIndex()] - M.fGameTime > 0)
	{
		//todo better drawings
		ImVec2 RealPos = Direct3D9.WorldToScreen(obj.GetPosition());
		if (RealPos.x == 0.f && RealPos.y == 0.f)
			return;

		if (!((RealPos.x <= SCREENWIDTH * 1.2) && (RealPos.x >= SCREENWIDTH / 2 * (-1)) && (RealPos.y <= SCREENHEIGHT * 1.5) && (RealPos.y >= SCREENHEIGHT / 2 * (-1))))
			return;

		std::string str = std::to_string(howManyNearby[obj.GetIndex()]) + (howManyNearby[obj.GetIndex()] == 1 ? " CHAMPION" : " CHAMPIONS") + " NEARBY!";
		draw->String(str, RealPos.x, RealPos.y, centered, RGBA(255, 50, 50), Direct3D9.fontTahoma);
	}
}

//todo calculate runes items etc, more options like both W's hitting etc
void Visuals::TalonDamageCalc(CObject obj)
{
	//if (obj.GetTeam() == Local.GetTeam())
	//	return;

	//if (obj.IsDead())
	//	return;

	//if (!obj.IsVisible())
	//	return;

	//ImVec2 RealPos = Direct3D9.WorldToScreen(obj.GetPosition());
	//if (RealPos.x == 0.f && RealPos.y == 0.f)
	//	return;

	//if (!((RealPos.x <= SCREENWIDTH * 1.2) && (RealPos.x >= SCREENWIDTH / 2 * (-1)) && (RealPos.y <= SCREENHEIGHT * 1.5) && (RealPos.y >= SCREENHEIGHT / 2 * (-1))))
	//	return;

	//int playerLevel = Local.GetLevel();
	//CSpellSlot* QSpell = Local.GetSpellByID(SpellSlotID::Q);
	//int QLevel = QSpell->GetLevel();
	//CSpellSlot* WSpell = Local.GetSpellByID(SpellSlotID::W);
	//int WLevel = WSpell->GetLevel();
	//CSpellSlot* RSpell = Local.GetSpellByID(SpellSlotID::R);
	//int RLevel = RSpell->GetLevel();

	//float PDamage = TalonPDmg[playerLevel] + Local.GetBonusAD() * 2.0;
	//int PDamageTotal = Local.GetTotalDamage(obj, PDamage);
	////adds an autoattack thats required to proc passive
	//PDamageTotal += Local.GetTotalDamage(obj);

	//float QDamage = QLevel > 0 ? TalonQDmg[QLevel] + Local.GetBonusAD() * 1.1 : 0;
	//int QDamageTotal = Local.GetTotalDamage(obj, QDamage);

	//float WDamage = WLevel > 0 ? TalonW1Dmg[WLevel] + Local.GetBonusAD() * 0.4 : 0;
	//int WDamageTotal = Local.GetTotalDamage(obj, WDamage);

	//float RDamage = RLevel > 0 ? TalonRDmg[RLevel] + Local.GetBonusAD() * 1.0 : 0;
	//int RDamageTotal = Local.GetTotalDamage(obj, RDamage);

	////clog.AddLog("%s , %d , %d , %d , %d ,%d", obj.GetChampName().c_str(), (int)Local.GetTotalDamage(obj), PDamageTotal, QDamageTotal, WDamageTotal, RDamageTotal);
	//int TotalDamage = PDamageTotal + QDamageTotal + WDamageTotal + RDamageTotal;

	//int iperecentDamage = TotalDamage / obj.GetHealth() * 100;
	//std::string spercentDamage = 100 - iperecentDamage > 0 ? std::to_string(100 - iperecentDamage) + "%" : "Dead";
	//std::string str = std::to_string(TotalDamage) + " ->" + spercentDamage;
	//draw->String(str, RealPos.x, RealPos.y - 20, centered, RGBA(255, 255, 255), Direct3D9.fontTahoma);
}