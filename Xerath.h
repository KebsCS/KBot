#pragma once

#ifndef _XERATH_H
#define _XERATH_H

#include "Script.h"

class Xerath : public IScript {
private:
	Champion champion;

	virtual void OnKeyDown(int vKey);

	virtual void OnKeyUp(int vKey);

	virtual void Harass();

	virtual bool Evade(Vector3 evadePos);

public:
	Xerath() : champion(Champion::Xerath)
	{
	}
};

#endif