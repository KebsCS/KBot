#pragma once

#ifndef _PIXEL_H_
#define _PIXEL_H_

#include "Definitions.h"
#include "Windows.h"
#include "stdio.h"
#include <fstream>

class PixelHandler
{
private:
	//HDC _hDC			//deprecated windows getpixel hdc
	HDC hdc, hdcTemp; //handle device connectivity
	BYTE* bitPointer;
	LARGE_INTEGER frequency, startCount, endCount; // for timer
public:
	//Constructor, set up for handling pixel operations
	PixelHandler();

	//Destructor used to free memory after object is deleted
	~PixelHandler();

	//Returns true if successfully initializes objects device controllers, else false
	bool Initialize();

	//searches a given area for a color [HEX - RGBA] - returns true or false if found
	bool SearchPixelArea(unsigned int color, int UpperLeftX, int UpperLeftY, int BottomRightX, int BottomRightY);

	//searches a given area for a color [HEX - RGBA] - returns true or false if found
	bool SearchPixelArea(unsigned int color, Area region);

	//searches a given area for a color [HEX - RGBA] - returns true or false if found, within shade tolerance
	bool SearchPixelArea(unsigned int color, int UpperLeftX, int UpperLeftY, int BottomRightX, int BottomRightY, int tolerance);

	//searches a given area for a color [HEX - RGBA] - returns true or false if found, within shade tolerance
	bool SearchPixelArea(unsigned int color, Area region, int tolerance);

	//searches a given area for a color [HEX - RGBA] - returns Point where color found
	POINT SearchPixelAreaForPoint(unsigned int color, int UpperLeftX, int UpperLeftY, int BottomRightX, int BottomRightY, int tolerance);

	//searches a given area for a color [HEX - RGBA] - returns Point where color found
	POINT SearchPixelAreaForPoint(unsigned int color, Area region, int tolerance);

	//searches a given area for a color [HEX - RGBA] - returns Point where color found
	POINT SearchPixelAreaForPoint(unsigned int color, int UpperLeftX, int UpperLeftY, int BottomRightX, int BottomRightY);

	//searches a given area for a color [HEX - RGBA] - returns Point where color found
	POINT SearchPixelAreaForPoint(unsigned int color, Area region);

	//returns pixel color of a coordinate on screen
	unsigned int GetPixelColor(int x, int y);

	//same as above but with points
	unsigned int GetPixelColor(POINT coord);

	//checks the color [HEX - RGBA] of a single pixel.
	bool VerifyPixelColor(unsigned int color, int x, int y);

	//checks the color [HEX - RGBA] of a single pixel. within shade tolerance
	bool VerifyPixelColor(unsigned int color, int x, int y, int tolerance);

	//checks the color [HEX - RGBA] of a single pixel. (overloaded function for handling pixel struct)
	bool VerifyPixelColor(Pixel pix);

	// Searches a decent pixel area around a certain pixel with a tolerance of 10 for a certain pixel
	// its large because the runescape world shifts over time
	bool VerifyRoughPixel(Pixel pix);

	//prints out color and (x,y) of mouse pointer. infinite while loop CTRL + C to leave.
	unsigned int DEBUG();

	//returns execution time in milliseconds
	double GetExecutionTime();

	//todo
	//bool SearchforImage(LPCWSTR path, int x1, int y1, int x2, int y2);

	//searches given area for image returns true if found, coords of top left corner as parameters
	//bool SearchforImage(LPCWSTR path, POINT coord);

	//screenshots given area and saves it as screenshot.bmp in bot directory
	bool Screenshot(int UpperLeftX, int UpperLeftY, int BottomRightX, int BottomRightY, const char* fileName = "screenshot.bmp");

	//screenshots given area and saves it as screenshot.bmp in bot directory
	bool Screenshot(Area region, const char* fileName = "screenshot.bmp");

	//void SetScreenPixel(int x, int y, int r, int g, int b);

	//void SetScreenPixel(POINT coord, int r, int g, int b);
};

extern PixelHandler* pixel;

#endif