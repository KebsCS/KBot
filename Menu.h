#pragma once

#ifndef _MENU_H_
#define _MENU_H_

#include "Definitions.h"

struct Menu
{
	Menu()
	{};

	//globals
	bool bMenuOpen = 1;
	bool bConsoleOpen = 0;
	bool bDebug = 0;

	bool bExitBot = 0;

	float fGameTime = 0;
	DWORD dwStartTime = 0;
	DirectX::XMMATRIX Matrix;

	std::string sServerInfo;

	std::string sChampion;
	std::pair<std::string, DWORD> ScoreBoard[10]; //for GUI scoreboard

	//minimap
	float fMinimapPos[2] = { 1653.f,812.f };
	float fMinimapSize[2] = { 257.f,257.f };

	//renderer
	int nRendererWidth = 1920;
	int nRendererHeight = 1080;

	int nPatch = 0;

	struct
	{
		int AntiLag = 1;
		int MenuKey = VK_INSERT;
	}Misc;
	int Configs = 0;

	struct
	{
		bool Master = 0;
		bool Missile = 0;
		bool Draw = 0;
		bool LimitRange = 0;
		float LR = 2500.f;
		bool Smooth = false;
		int GP = 0;
		int DS = 0;
		int DC = 0;
		bool OnKey = false;
		int EvadeKey = VK_SPACE;
		bool MouseBack = true;
		bool Force = 1;
	}Evade;

	struct
	{
		bool Master = 0;
		int HoldKey = 0;
	}Orbwalker;

	struct
	{
		bool Master = 0;
		bool Local = 0;
		bool Turrets = 0;
		int Slider[2] = { 16,10 };
		float Color[4] = { 255.0f / 255.0f, 85.0f / 255.0f, 50.0f / 255.0f, 255.0f / 255.0f };
		float LocalColor[4] = { 83.0f / 255.0f, 85.0f / 255.0f, 251.0f / 255.0f, 255.0f / 255.0f };
	}AARange;

	struct
	{
		bool Master = 0;
		float Thickness = 1.f;
	}Tracers;

	struct
	{
		bool Master = 0;
		//0-text on champ 1-img on champ 2-scoreboard
		bool Type[3];
		bool ScoreboardWnd = 0;
		struct
		{
			bool ShowScoreboard = 0;
			int Pos[2] = { 985,280 }; //x,y
			bool Summs = 1;
			bool Ults = 1;
			bool Exp = 1;
			bool Items = 0;
		}Scoreboard;
	}Cooldowns;

	struct
	{
		bool Master = 0;
		float Color[4] = { 255.0f / 255.0f, 85.0f / 255.0f, 50.0f / 255.0f, 255.0f / 255.0f };
	}LastHit;

	struct
	{
		bool Master = 0;
	}GankAlerter;

	struct
	{
		bool Master = 0;
		int Slot = 1;
		int Mode = 0;
		float MouseSpeed = 1.f;
	}AutoSmite;

	struct
	{
		bool Master = 0;
	}Wards;

	struct
	{
		bool Master = 0;
	}Inhibs;

	struct
	{
		bool MouseClicks = 0;
	}Enemies;

	struct
	{
		bool Master = 0;
		bool DmgCalc = 0;
		bool Jumps = 0;
		int JumpsKey = VK_LSHIFT;
		//0-drake 1-blue raptors 2-botlane 3-toplane 4-red raptors
		bool JumpsType[5];
	}Talon;

	struct
	{
		bool DrawExecute = 0;
	}Pantheon;
};

extern Menu M;

#endif //!_MENU_H_