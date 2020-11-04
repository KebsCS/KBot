#pragma once
#include "DirectX.h"

#ifndef  _VISUALS_H
#define _VISUALS_H


class Visuals
{
private:

public:

	void CooldownTimers(CObject obj, int type);

	void DrawAARanges(CObject obj, int points, int thickness, RGBA color, bool local, RGBA localcolor);
	void DrawTracers(CObject obj, int thickness);

	void AutoSmite(CObject obj);

};

enum SpellSlotID
{
	Q = 0,
	W = 1,
	E = 2,
	R = 3,
	Summoner1 = 4,
	Summoner2 = 5,
	Item1 = 6,
	Item2 = 7,
	Item3 = 8,
	Item4 = 9,
	Item5 = 10,
	Item6 = 11,
	Trinket = 12,
	Recall = 13
};

#endif // ! _VISUALS_H
