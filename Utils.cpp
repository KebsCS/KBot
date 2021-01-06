#include "Utils.h"

#include <locale>
#include <codecvt>
#include <sstream>

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
	result &= pixel->VerifyRoughPixel(pix1);
	result &= pixel->VerifyRoughPixel(pix2);
	result &= pixel->VerifyRoughPixel(pix3);
	return result;
}

Area Utils::GetDesktopArea() const
{
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);
	Area t(desktop.left, desktop.top, desktop.right, desktop.bottom);
	return t;
}

Area Utils::GetForegroundWindowArea() const
{
	RECT desktop;
	const HWND hDesktop = GetForegroundWindow();
	GetWindowRect(hDesktop, &desktop);
	Area t(desktop.left, desktop.top, desktop.right, desktop.bottom);
	return t;
}

std::string Utils::ToLower(std::string str)
{
	std::transform(str.begin(), str.end(), str.begin(),
		[](unsigned char c) { return std::tolower(c); }
	);
	return str;
}

std::string Utils::ToUpper(std::string str)
{
	std::transform(str.begin(), str.end(), str.begin(),
		[](unsigned char c) { return std::toupper(c); }
	);
	return str;
}

bool Utils::StringContains(std::string strA, std::string strB, bool ignore_case)
{
	if (strA == "" || strB == "")
		return true;

	if (ignore_case)
	{
		strA = ToLower(strA);
		strB = ToLower(strB);
	}

	if (strA.find(strB) != std::string::npos)
		return true;

	return false;
}

std::string Utils::WstringToString(std::wstring wstr)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;

	try
	{
		return converter.to_bytes(wstr);
	}
	catch (std::range_error)
	{
		std::stringstream s;
		s << wstr.c_str();
		return s.str();
	}
}

std::wstring Utils::StringToWstring(std::string str)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;

	try
	{
		return converter.from_bytes(str);
	}
	catch (std::range_error)
	{
		std::wostringstream s;
		s << str.c_str();
		return s.str();
	}
}

Utils* utils = new Utils();