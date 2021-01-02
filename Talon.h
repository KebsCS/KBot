#pragma once


#ifndef _TALON_H
#define _TALON_H

#include "Script.h"

class Talon : public IScript {
private:
	Champion champion;

	virtual void OnKeyDown(int vKey);

	virtual void OnKeyUp(int vKey);

	virtual void Harass();

	virtual bool Evade(Vector3 evadePos);

public:
	Talon() : champion(Champion::Talon)
	{

	}


};


#endif