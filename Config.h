#pragma once

#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <vector>
#include <string>
#include <Windows.h>
#include <ShlObj.h>
#include "Menu.h"

template< typename T >
class ConfigValue
{
public:
	ConfigValue(std::string category_, std::string name_, T* value_)
	{
		category = category_;
		name = name_;
		value = value_;
	}

	std::string category, name;
	T* value;
};

class CConfig
{
protected:
	std::vector< ConfigValue< int >* > ints;
	std::vector< ConfigValue< bool >* > bools;
	std::vector< ConfigValue< float >* > floats;
private:
	void SetupValue(int&, int, std::string, std::string);
	void SetupValue(bool&, bool, std::string, std::string);
	void SetupValue(float&, float, std::string, std::string);

public:
	CConfig()
	{
		Setup();
	}
	~CConfig() = default;

	//sets up initial values
	void Setup();

	//saves config to fileName.ini
	void Save(std::string fileName);

	////loads config from fileName.ini
	void Load(std::string fileName);
};

extern CConfig* Config;

#endif // !_CONFIG_H_