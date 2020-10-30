#pragma once

#ifndef _MOUSE_H_
#define _MOUSE_H_

#include "Windows.h" //mouse things
#include "stdlib.h" //srand and rand
#include "Definitions.h" //Area
#include <math.h> //sin function

class Mouse
{
private:
	float MouseSpeed = 1.0; //Speed range: 0.1 -> 1.0
	int deviation = RandomInt(240, 260); //amount of arc path deviation  (E.g. 250pixels  will deviate the path in an arc randomly maxing at (-250 to 250)
	LARGE_INTEGER frequency, startCount, endCount; // for timer
	int PrevX, PrevY = 0; // for storing previous coords
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

	//Deviates Randomly!! mouse move - recieves pixel coordinates. change mouse position to those coords EXACTLY (!WARNING! SAME PIXEL CLICK DANGER)
	bool MouseMove(int x, int y);

	//same as above, just takes in a point
	bool MouseMove(POINT coord);

	//STRAIGHT LINE DISTANCE mouse move - recieves pixel coordinates. change mouse position to those coords
	bool MouseMoveSLD(int x, int y);

	//same as above, just takes in a point
	bool MouseMoveSLD(POINT coord);

	//left clicks and holds it
	void MouseLeftDown();

	//Releases left mouse button 
	void MouseLeftUp();

	//right clicks and holds it
	void MouseRightDown();

	//Releases right mouse button 
	void MouseRightUp();

	//Presses scroll and holds it
	void MouseMiddleDown();

	//releases pressing scroll
	void MouseMiddleUp();

	//single right click
	void RightClick();

	//single left click
	void LeftClick();

	//left clicks and holds it for given ammount of time
	void LeftClickHold(int t);

	//holds key and left clicks (e.g shift+click)
	void LeftClickKWithKey(int vk);

	//holds key and right clicks
	void RightClickWithKey(int vk);

	//Uses mouse scroll wheel, positive value scrolls up, negative scrolls down
	//one wheel click = 1
	void Scroll(int ammount);

	//returns x,y of mouse
	POINT GetPos();

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

	//returns execution time im milliseconds
	double GetExecutionTime();

};
#endif