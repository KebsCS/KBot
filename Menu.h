#pragma once

#ifndef _MENU_H_
#define _MENU_H_

#include "Definitions.h"

struct Menu
{
    Menu()
    {};

    //globals
    //todo make a global namespace
    bool MenuOpen = 0;
    bool ExitBot = 0;
    bool ConsoleOpen = 0;
    float GameTime = 0;
    DirectX::XMMATRIX Matrix;

    std::string Champion;
    std::string ScoreboardNames[10]; //for GUI scoreboard


    struct
    {
        int AntiLag = 1;
        int MenuKey = VK_INSERT;
    }Misc;
    int Configs = 0;

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
        bool Master = 0;
        bool DmgCalc = 0;
        bool Jumps = 0;
        int JumpsKey = VK_LSHIFT;
        //0-drake 1-blue raptors 2-botlane 3-toplane 4-red raptors
        bool JumpsType[5];
       
    }Talon;

};

extern Menu M;

#endif //!_MENU_H_