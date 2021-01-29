#pragma once

#ifndef _UTILS_H_
#define _UTILS_H_

#include "Definitions.h"
#include "Pixel.h"
#include "KInterface.h"

#include <filesystem>

//URLDownloadToFileA
#include <urlmon.h>
#pragma comment(lib, "urlmon.lib")

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

	//returns an Area struct with screen size
	Area GetDesktopArea() const;

	//returns an Area struct with foreground window size
	Area GetForegroundWindowArea() const;

	//converts whole string to lowercase
	std::string ToLower(std::string str);

	//converts whole string to uppercase
	std::string ToUpper(std::string str);

	//check if strA contains strB
	bool StringContains(std::string strA, std::string strB, bool ignore_case = false);

	//string to wstring
	std::wstring StringToWstring(std::string str);

	std::string WstringToString(std::wstring wstr);

	std::string RandomString(int size);

	std::string FormatString(const char* c, const char* args...);

	void CopyToClipboard(std::string text);


	bool DownloadFile(std::string fileName, std::string directory = "Data", std::string url = "https://raw.githubusercontent.com/y3541599/test/main/")
	{
		// Create folder if it doesn't exists
		if (!std::filesystem::exists(directory))
			std::filesystem::create_directory(directory);
		if (fileName[0] == '\\')
			fileName.erase(0);
		std::string file = directory + "\\" + fileName;

		// Don't download if file already exists
		if (std::filesystem::exists(file))
			return true;

		std::string fullPath = std::filesystem::current_path().string() + "\\" + file;
		std::string toDownload = url + fileName;

		// Download file
		HRESULT result = URLDownloadToFileA(NULL, toDownload.c_str(), fullPath.c_str(), 0, NULL);

		if (result != S_OK)
			return false;
		return true;
	}

	//bool IsLeagueForeGround()
	//{
	//	HWND handle = GetForegroundWindow();
	//	DWORD pid;
	//	GetWindowThreadProcessId(handle, &pid);
	//	if (Memory.Process() != pid)
	//	{
	//	//	CloseHandle(handle);
	//		return false;
	//	}
	//	//CloseHandle(handle);
	//	return true;
	//}
	//bool IsScriptForeGround()
	//{
	//	HWND handle = GetForegroundWindow();
	//	DWORD pid;
	//	GetWindowThreadProcessId(handle, &pid);
	//	if (GetCurrentProcessId() != pid)
	//	{
	//	//	CloseHandle(handle);
	//		return false;
	//	}
	//	//CloseHandle(handle);
	//	return true;
	//}

};

extern Utils* utils;

#endif
