#include "Utils.h"



//returns an Area struct including a box of radius R around x and y
Area Utils::areaBox(int x, int y, int r)
{
	Area t;
	t.x1 = x - r;
	t.y1 = y - r;
	t.x2 = x + r;
	t.y2 = y + r;
	return t;
}

//returns an Area struct including a box of radius R around x and y
Area Utils::areaBox(POINT p, int r)
{
	return areaBox(p.x, p.y, r);

}

//returns an Area struct including a box of radius R around x and y
Area Utils::areaBox(int x, int y, int height, int width)
{
	Area t;
	t.x1 = x - width;
	t.y1 = y - height;
	t.x2 = x + width;
	t.y2 = y + height;
	return t;
}

//returns an Area struct including a box of radius R around x and y
Area Utils::areaBox(POINT p, int height, int width)
{
	return areaBox(p.x, p.y, height, width);
}



/*Given three pixels, checks the environment to see if those pixels are where they should be
Usage:
name1.Set(color,x,y)
*/
bool Utils::CheckLocation(Pixel pix1, Pixel pix2, Pixel pix3)
{
	bool result = true;
	result &= pix.VerifyRoughPixel(pix1);
	result &= pix.VerifyRoughPixel(pix2);
	result &= pix.VerifyRoughPixel(pix3);
	return result;
}

Area Utils::GetDesktopArea()
{
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);
	Area t(desktop.left, desktop.top, desktop.right, desktop.bottom);
	return t;
}


Area Utils::GetWindowArea()
{
	RECT desktop;
	const HWND hDesktop = GetForegroundWindow();
	GetWindowRect(hDesktop, &desktop);
	Area t(desktop.left, desktop.top, desktop.right, desktop.bottom);
	return t;
}

