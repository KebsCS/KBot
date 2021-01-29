#pragma once

#ifndef  _VISUALS_H_
#define _VISUALS_H_

#include "Draw.h"

class Visuals
{
private:
	//std::string getSpellImg(std::string name);
	//LPDIRECT3DTEXTURE9 GetSpellImg(std::string name);

public:

	void CooldownTimers(CObject obj);

	void ScoreBoard(CObject obj);

	void DrawAARanges(CObject obj, int points, float thickness, RGBA color, bool local, RGBA localcolor, bool turret);

	void DrawTracers(CObject obj, float thickness);

	void AutoSmite(CObject obj, int slot, int mode, float mouseSpeed);

	void LastHit(CObject obj, RGBA color);

	void InhibTimers(CObject obj);

	void WardsRange(CObject obj);

	void GankAlerter(CObject obj);

	void TalonDamageCalc(CObject obj);
};

#endif // ! _VISUALS_H_
