#pragma once

#ifndef _SCRIPTUTILS_H_

#include <string>

#include "Talon.h"
#include "Lux.h"
#include "Xerath.h"

class ScriptUtils {
public:
	static IScript* GetScriptByChampionName(std::string championName)
	{
		if (championName == "Talon")
		{
			M.Talon.Master = true;
			return new Talon();
		}
		else if (championName == "Lux")
		{
			return new Lux();
		}
		else if (championName == "Xerath")
		{
			return new Xerath();
		}

		return nullptr;
	}
};

#endif // !_SCRIPTUTILS_H_
