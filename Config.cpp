#include "Config.h"


void CConfig::Setup()
{
	SetupValue(M.AntiLag, 0, "Main", "AntiLag");
	SetupValue(M.ConsoleOpen, 0, "Main", "ConsoleOpen");


	SetupValue(M.AARange.Master, 1, "AARange", "Master");
	SetupValue(M.AARange.Local, 0, "AARange", "Local");
	SetupValue(M.AARange.Turrets, 0, "AARange", "Turrets");
	SetupValue(M.AARange.Slider[0], 16, "AARange", "SliderPoints");
	SetupValue(M.AARange.Slider[1], 10, "AARange", "SliderThickness");

	SetupValue(M.AARange.Color[0], 0.f, "AARange", "ColorR");
	SetupValue(M.AARange.Color[1], 1.f, "AARange", "ColorG");
	SetupValue(M.AARange.Color[2], 1.f, "AARange", "ColorB");
	SetupValue(M.AARange.Color[3], 1.f, "AARange", "ColorA");

	SetupValue(M.AARange.LocalColor[0], 0.f, "AARange", "LocalColorR");
	SetupValue(M.AARange.LocalColor[1], 1.f, "AARange", "LocalColorG");
	SetupValue(M.AARange.LocalColor[2], 1.f, "AARange", "LocalColorB");
	SetupValue(M.AARange.LocalColor[3], 1.f, "AARange", "LocalColorA");



	SetupValue(M.Tracers.Master, 1, "Tracers", "Master");
	SetupValue(M.Tracers.Thickness, 10, "Tracers", "Thickness");



	SetupValue(M.Cooldowns.Master, 1, "Cooldowns", "Master");

	SetupValue(M.LastHit.Master, 1, "LastHit", "Master");
	SetupValue(M.LastHit.Color[0], 0.f, "LastHit", "ColorR");
	SetupValue(M.LastHit.Color[1], 1.f, "LastHit", "ColorG");
	SetupValue(M.LastHit.Color[2], 1.f, "LastHit", "ColorB");
	SetupValue(M.LastHit.Color[3], 1.f, "LastHit", "ColorA");

	SetupValue(M.AutoSmite.Master, 1, "AutoSmite", "Master");
	SetupValue(M.AutoSmite.Slot, 0, "AutoSmite", "Slot");

	SetupValue(M.Wards.Master, 1, "Wards", "Master");

	SetupValue(M.Inhibs.Master, 1, "Inhibs", "Master");

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


void CConfig::Save()
{
	static CHAR path[MAX_PATH];
	std::string folder, file;

//	if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, path)))
//	{
		//folder = std::string(path) + ".\\config\\";
		file = std::string(path) + ".\\cfg.ini";
//	}

	//CreateDirectoryA(folder.c_str(), NULL);


	for (auto value : bools)
		WritePrivateProfileStringA(value->category.c_str(), value->name.c_str(), *value->value ? "true" : "false", file.c_str());

	for (auto value : ints)
		WritePrivateProfileStringA(value->category.c_str(), value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());

	for (auto value : floats)
		WritePrivateProfileStringA(value->category.c_str(), value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());
}

void CConfig::Load()
{
	static CHAR path[MAX_PATH];
	std::string folder, file;

	//if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, path)))
//	{
		//folder = std::string(path) + ".\\config\\";
		file = std::string(path) + ".\\cfg.ini";
//	}

	//CreateDirectoryA(folder.c_str(), NULL);

	char value_l[32] = { '\0' };

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