#pragma once

#ifndef _MENU_H_
#define _MENU_H_

#include "Definitions.h"

struct Menu
{
    Menu()
    {};

    //globals
    bool MenuOpen;
    bool ExitBot;
    bool ConsoleOpen;

    std::string Champion;
    std::string ScoreboardNames[10]; //for GUI scoreboard


    struct
    {
        int AntiLag;
        int MenuKey;
    }Misc;
    int Configs = 0;

	struct
	{
        bool Master;
		int HoldKey;
	}Orbwalker;

    struct
    {
        bool Master;
        bool Local;
        bool Turrets;
        int Slider[2] = { 16,10 };
        float Color[4] = { 255.0f / 255.0f, 85.0f / 255.0f, 50.0f / 255.0f, 255.0f / 255.0f };
        float LocalColor[4] = { 83.0f / 255.0f, 85.0f / 255.0f, 251.0f / 255.0f, 255.0f / 255.0f };


    }AARange;

    struct
    {
        bool Master = true;
        float Thickness;
       
    }Tracers;

    struct
    {
        bool Master = true;
        bool Type[3];
    }Cooldowns;

    struct
    {
        bool Master;
        float Color[4] = { 255.0f / 255.0f, 85.0f / 255.0f, 50.0f / 255.0f, 255.0f / 255.0f };
    }LastHit;

    struct
    {
        bool Master;
        int Slot = 0;
        int Mode;
        float MouseSpeed;
    }AutoSmite;

    struct
    {
        bool Master;
    }Wards;

    struct
    {
        bool Master;
    }Inhibs;

    struct
    {
        bool Master = false;
        bool Jumps;
    }Talon;

};

extern Menu M;

#endif //!_MENU_H_