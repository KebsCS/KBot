#pragma once

#ifndef _PANTHEON_H
#define _PANTHEON_H

#include "Script.h"

class Pantheon : public IScript {
private:
	Champion champion;

	virtual void OnKeyDown(int vKey);

	virtual void OnKeyUp(int vKey);

	virtual void Harass();

	virtual bool Evade(Vector3 evadePos);

	virtual void Tick();

	virtual void GUI();

public:
	Pantheon() : champion(Champion::Pantheon)
	{
	}
};

#endif