#include "Config.h"

#include <filesystem>

void CConfig::Setup()
{
	SetupValue(M.bMenuOpen, 1, "Main", "MenuOpen");
	SetupValue(M.bConsoleOpen, 0, "Main", "ConsoleOpen");
	SetupValue(M.nPatch, 0, "Main", "Patch");
	SetupValue(M.bDebug, 0, "Main", "Debug");

	SetupValue(M.Misc.AntiLag, 1, "Misc", "AntiLag");
	SetupValue(M.Misc.MenuKey, VK_INSERT, "Misc", "MenuKey");

	SetupValue(M.Evade.Master, 0, "Evade", "Master");
	SetupValue(M.Evade.Smooth, 0, "Evade", "Smooth");
	SetupValue(M.Evade.Draw, 0, "Evade", "Draw");
	SetupValue(M.Evade.Missile, 0, "Evade", "Missile");
	SetupValue(M.Evade.LimitRange, 0, "Evade", "LimitRange");
	SetupValue(M.Evade.LR, 2500.f, "Evade", "LR");
	SetupValue(M.Evade.GP, 50, "Evade", "GP");
	SetupValue(M.Evade.DS, 20, "Evade", "DS");
	SetupValue(M.Evade.DC, 4, "Evade", "DC");
	SetupValue(M.Evade.MouseBack, 1, "Evade", "MouseBack");
	SetupValue(M.Evade.Force, 1, "Evade", "Force");
	SetupValue(M.Evade.OnKey, 0, "Evade", "OnKey");
	SetupValue(M.Evade.EvadeKey, VK_SPACE, "Evade", "EvadeKey");

	SetupValue(M.Orbwalker.Master, 0, "Orbwalker", "Master");
	SetupValue(M.Orbwalker.HoldKey, 0, "Orbwalker", "HoldKey");

	SetupValue(M.AARange.Master, 0, "AARange", "Master");
	SetupValue(M.AARange.Local, 0, "AARange", "Local");
	SetupValue(M.AARange.Turrets, 0, "AARange", "Turrets");
	SetupValue(M.AARange.Slider[0], 14, "AARange", "SliderPoints");
	SetupValue(M.AARange.Slider[1], 20, "AARange", "SliderThickness");

	SetupValue(M.AARange.Color[0], 0.965517f, "AARange", "ColorR");
	SetupValue(M.AARange.Color[1], 0.207541f, "AARange", "ColorG");
	SetupValue(M.AARange.Color[2], 0.076100f, "AARange", "ColorB");
	SetupValue(M.AARange.Color[3], 1.f, "AARange", "ColorA");

	SetupValue(M.AARange.LocalColor[0], 0.116188f, "AARange", "LocalColorR");
	SetupValue(M.AARange.LocalColor[1], 0.209196f, "AARange", "LocalColorG");
	SetupValue(M.AARange.LocalColor[2], 0.842365f, "AARange", "LocalColorB");
	SetupValue(M.AARange.LocalColor[3], 1.f, "AARange", "LocalColorA");

	SetupValue(M.Tracers.Master, 0, "Tracers", "Master");
	SetupValue(M.Tracers.Thickness, 1.f, "Tracers", "Thickness");

	SetupValue(M.Enemies.MouseClicks, 0, "Enemies", "MouseClicks");

	SetupValue(M.Cooldowns.Master, 0, "Cooldowns", "Master");
	SetupValue(M.Cooldowns.Type[0], 0, "Cooldowns", "Type0");
	SetupValue(M.Cooldowns.Type[1], 0, "Cooldowns", "Type1");
	SetupValue(M.Cooldowns.Type[2], 0, "Cooldowns", "Type2");
	SetupValue(M.Cooldowns.ScoreboardWnd, 0, "Cooldowns", "ScoreboardWnd");

	SetupValue(M.Cooldowns.Scoreboard.Pos[0], 985, "Scoreboard", "X");
	SetupValue(M.Cooldowns.Scoreboard.Pos[1], 280, "Scoreboard", "Y");
	SetupValue(M.Cooldowns.Scoreboard.Summs, 1, "Scoreboard", "Summs");
	SetupValue(M.Cooldowns.Scoreboard.Ults, 1, "Scoreboard", "Ults");
	SetupValue(M.Cooldowns.Scoreboard.Summs, 1, "Scoreboard", "Summs");
	SetupValue(M.Cooldowns.Scoreboard.Exp, 1, "Scoreboard", "Exp");
	SetupValue(M.Cooldowns.Scoreboard.Items, 0, "Scoreboard", "Items");

	SetupValue(M.LastHit.Master, 0, "LastHit", "Master");
	SetupValue(M.LastHit.Color[0], 0.f, "LastHit", "ColorR");
	SetupValue(M.LastHit.Color[1], 1.f, "LastHit", "ColorG");
	SetupValue(M.LastHit.Color[2], 0.f, "LastHit", "ColorB");
	SetupValue(M.LastHit.Color[3], 1.f, "LastHit", "ColorA");

	SetupValue(M.GankAlerter.Master, 0, "GankAlerter", "Master");

	SetupValue(M.AutoSmite.Master, 0, "AutoSmite", "Master");
	SetupValue(M.AutoSmite.Slot, 1, "AutoSmite", "Slot");
	SetupValue(M.AutoSmite.Mode, 0, "AutoSmite", "Mode");
	SetupValue(M.AutoSmite.MouseSpeed, 1.f, "AutoSmite", "MouseSpeed");

	SetupValue(M.Wards.Master, 0, "Wards", "Master");

	SetupValue(M.Inhibs.Master, 0, "Inhibs", "Master");

	SetupValue(M.Talon.DmgCalc, 0, "Talon", "DmgCalc");
	SetupValue(M.Talon.Jumps, 0, "Talon", "Jumps");
	SetupValue(M.Talon.JumpsKey, VK_LSHIFT, "Talon", "JumpsKey");
	for (int i = 0; i < 5; i++)
		SetupValue(M.Talon.JumpsType[i], 1, "Talon", "JumpsType" + std::to_string(i));

	SetupValue(M.Pantheon.DrawExecute, 1, "Pantheon", "DrawExecute");
}

void CConfig::SetupValue(int& value, int def, std::string category, std::string name)
{
	value = def;
	ints.push_back(new ConfigValue< int >(category, name, &value));
}

void CConfig::SetupValue(float& value, float def, std::string category, std::string name)
{
	value = def;
	floats.push_back(new ConfigValue< float >(category, name, &value));
}

void CConfig::SetupValue(bool& value, bool def, std::string category, std::string name)
{
	value = def;
	bools.push_back(new ConfigValue< bool >(category, name, &value));
}

void CConfig::Save(std::string fileName)
{
	std::string directory = "configs";

	if (!std::filesystem::exists(directory))
		std::filesystem::create_directory(directory);

	std::string file = directory + "\\" + fileName + ".ini";

	//save number of configs to default cfg
	WritePrivateProfileStringA("Main", "Configs", std::to_string(M.Configs).c_str(), ".\\configs\\default.ini");

	for (auto value : bools)
		WritePrivateProfileStringA(value->category.c_str(), value->name.c_str(), *value->value ? "true" : "false", file.c_str());

	for (auto value : ints)
		WritePrivateProfileStringA(value->category.c_str(), value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());

	for (auto value : floats)
		WritePrivateProfileStringA(value->category.c_str(), value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());
}

void CConfig::Load(std::string fileName)
{
	std::string directory = "configs";

	if (!std::filesystem::exists(directory))
		std::filesystem::create_directory(directory);

	std::string file = directory + "\\" + fileName + ".ini";

	//dont load settings if file doesnt exists
	if (!std::filesystem::exists(file))
		return;

	char value_l[32] = { '\0' };

	//get number of configs on startup
	GetPrivateProfileStringA("Main", "Configs", "", value_l, 32, ".\\configs\\default.ini");
	M.Configs = atoi(value_l);

	for (auto value : ints)
	{
		GetPrivateProfileStringA(value->category.c_str(), value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = atoi(value_l);
	}

	for (auto value : floats)
	{
		GetPrivateProfileStringA(value->category.c_str(), value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = atof(value_l);
	}

	for (auto value : bools)
	{
		GetPrivateProfileStringA(value->category.c_str(), value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = !strcmp(value_l, "true");
	}
}

CConfig* Config = new CConfig();
Menu M;