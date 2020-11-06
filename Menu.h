#pragma once



struct Menu
{
    Menu()
    {};

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
        int Thickness = 10;
       
    }Tracers;

    struct
    {
        bool Master = true;

    }Cooldowns;
};

extern Menu M;