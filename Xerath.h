#pragma once

#ifndef _XERATH_H
#define _XERATH_H

#include "Script.h"
#include <Windows.h>

class Xerath : public IScript {
private:
	Champion champion;

	virtual void OnKeyDown(int vKey);

	virtual void OnKeyUp(int vKey);

	virtual void Harass();

	virtual bool Evade(Vector3 evadePos);

	virtual void Tick();

	virtual void GUI();

public:

	bool chargeQ = false;
	DWORD qTick;
	float qRange = 0;

	Xerath() : champion(Champion::Xerath)
	{
	}
};

#endif