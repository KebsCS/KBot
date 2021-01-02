#pragma once


#ifndef _LUX_H
#define _LUX_H

#include "Script.h"

class Lux : public IScript {
private:
	Champion champion;

	virtual void OnKeyDown(int vKey);

	virtual void OnKeyUp(int vKey);

	virtual void Harass();

	virtual bool Evade(Vector3 evadePos);

public:
	Lux() : champion(Champion::Lux)
	{

	}


};


#endif