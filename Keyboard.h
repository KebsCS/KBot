#pragma once

#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include "Definitions.h"
#include "Windows.h"
#include <stdio.h>
#include <string>

class Keyboard
{
private:
	LARGE_INTEGER frequency, startCount, endCount; // for timer
	int pressTime = FAST_REACTION_TIME; //how long key is pressed in milliseconds

public:

	Keyboard()
	{
		QueryPerformanceFrequency(&frequency);
		QueryPerformanceCounter(&startCount);
	}
	~Keyboard() = default;

	//Sets press down time
	void SetDelay(int ms);

	//presses special key, e.g arrows, shift
	void SpecialKeyDown(int vk);

	//releases special key
	void SpecialKeyUp(int vk);

	//presses scan code key
	void ScancodeKeyDown(int vk);

	//releases scan code key
	void ScancodeKeyUp(int vk);

	//expects the key (as a capital letter), whether its held down, and whether to yield a capital letter.
	// example sending ('A',false,false) yields 'a'
	// to achieve 'A', send ('A',false,true)
	void GenerateKey(int vk, BOOL bExtended, bool shift = false);

	void GenerateKeyScancode(int vk, bool shift = false);

	//types given text
	void Type(std::string phrase);

	//	1 = left
	//	2 = right
	//	3 = up
	//	4 = down
	void Arrow(int arrow);

	//hits special key e.g shift, enter, arrows
	void HitSpecialKey(int vk);

	//types a given number & presses enter.
	void TypeNum(int number);

	//hits any single key
	void HitKey(int vk);

	double GetExecutionTime();
};

extern Keyboard* keyboard;

#endif // !_KEYBOARD_H_