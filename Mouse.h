#pragma once

#ifndef _MOUSE_H_
#define _MOUSE_H_

#include "Windows.h" //mouse things
#include "stdlib.h" //srand and rand
#include "Definitions.h" //Area
#include <math.h> //sin function
#include "CObjectManager.h" //hudinstance and memory reads

class Mouse
{
private:
	float MouseSpeed = 1.0; //Speed range: 0.1 -> 1.0
	int deviation = RandomInt(240, 260); //amount of arc path deviation  (E.g. 250pixels  will deviate the path in an arc randomly maxing at (-250 to 250)
	LARGE_INTEGER frequency, startCount, endCount; // for timer
	int PrevX = 0, PrevY = 0; // for storing previous coords
public:

	Mouse()
	{
		QueryPerformanceFrequency(&frequency);
		QueryPerformanceCounter(&startCount);
	}
	~Mouse() = default;

	//Moves mouse to an random pixel within area
	bool MouseMoveArea(int x1, int y1, int x2, int y2);

	//Moves mouse to an random pixel within area
	bool MouseMoveArea(Area region);

	//Deviates Randomly!! mouse move - receives pixel coordinates. change mouse position to those coords EXACTLY (!WARNING! SAME PIXEL CLICK DANGER)
	bool MouseMove(int x, int y);

	//same as above, just takes in a point
	bool MouseMove(POINT coord);

	//STRAIGHT LINE DISTANCE mouse move - receives pixel coordinates. change mouse position to those coords
	bool MouseMoveSLD(int x, int y);

	//instantly moves mouse
	bool MouseMoveInstant(int x, int y);

	//same as above, just takes in a point
	bool MouseMoveSLD(POINT coord);

	//left clicks and holds it
	bool MouseLeftDown();

	//Releases left mouse button
	bool MouseLeftUp();

	//right clicks and holds it
	bool MouseRightDown();

	//Releases right mouse button
	bool MouseRightUp();

	//Presses scroll and holds it
	bool MouseMiddleDown();

	//releases pressing scroll
	bool MouseMiddleUp();

	//single right click
	bool RightClick();

	//single left click
	bool LeftClick();

	//left clicks and holds it for given amount of time
	bool LeftClickHold(int t);

	//holds key and left clicks (e.g shift+click)
	bool LeftClickKWithKey(int vk);

	//holds key and right clicks
	bool RightClickWithKey(int vk);

	//Uses mouse scroll wheel, positive value scrolls up, negative scrolls down
	//one wheel click = 1
	bool Scroll(int ammount);

	//returns x,y of mouse
	POINT GetPos() const;

	//changes mouse speed
	//Speed range: 0.1 -> 1.0
	void ChangeSpeed(float speed);

	//changes path deviations. Deviation Min is minimum amount deviated, deviation is the random fluctuations from that minimum point.
	//E.G. deviationMin = 250, deviation = 500. The path will deviate the path from (-250,250) pixels
	void SetDeviation(int dev);

	//Moves mouse to first coordinates, holds down left button and drags it to given coordinates
	void DragAndDrop(int x1, int y1, int x2, int y2, bool SLD = false);

	//Moves mouse to first coordinates, holds down left button and drags it to given coordinates
	void DragAndDrop(POINT start, POINT end, bool SLD = false);

	//Stores current coordinates
	void StoreCurrentPos();

	//Clears saved coordinates
	void ClearStoredPos();

	POINT GetStoredPos() const;

	//returns execution time in milliseconds
	double GetExecutionTime();

	inline Vector3 GetWorldPos()
	{
		DWORD MousePtr = ClientAddress + oHudInstance;
		auto aux1 = Memory.Read<DWORD>(MousePtr);
		aux1 += 0x14;
		auto aux2 = Memory.Read<DWORD>(aux1);
		aux2 += 0x1C;

		float X = Memory.Read<float>(aux2 + 0x0);
		float Y = Memory.Read<float>(aux2 + 0x4);
		float Z = Memory.Read<float>(aux2 + 0x8);

		return Vector3{ X, Y, Z };
	}
};

extern Mouse* mouse;

#endif