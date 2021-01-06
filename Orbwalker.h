#pragma once

#ifndef _ORBWALKER_H_
#define _ORBWALKER_H_

#include "Mouse.h"
#include <vector>
#include <map>
#include "DirectX.h"

class Orbwalker
{
private:

public:
	Orbwalker()
	{
	}
	~Orbwalker() = default;

	void Move();
};

extern Orbwalker* orbwalk;

#endif // !_ORBWALKER_H_
