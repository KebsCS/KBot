#pragma once

#ifndef _UTILS_H_
#define _UTILS_H_


#include "Definitions.h"
#include "Pixel.h"


class Utils
{
private:

public:

	Utils()
	{

	}

	~Utils() = default;

	//returns an Area struct including a box of radius R around x and y
	Area areaBox(int x, int y, int r);

	//Overload
	Area areaBox(POINT p, int r);

	//returns an Area struct including a box with differing heights and widths around x and y
	Area areaBox(int x, int y, int height, int width);

	//returns an Area struct including a box with differing heights and widths around x and y
	Area areaBox(POINT p, int height, int width);

	//Given three pixels, checks the environment to see if those pixels are where they should be
	bool CheckLocation(Pixel pix1, Pixel pix2, Pixel pix3);

	//retruns an Area struct with screen size
	Area GetDesktopArea() const;

	//retruns an Area struct with foreground window size
	Area GetForegroundWindowArea() const;

	//converts whole string to lowercase
	std::string ToLower(std::string str);

	//converts whole string to uppercase
	std::string ToUpper(std::string str);

	//check if strA contains strB
	bool StringContains(std::string strA, std::string strB, bool ignore_case = false);
};

extern Utils* utils;

#endif


