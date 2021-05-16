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
	std::string strLower;
	strLower.resize(str.size());

	std::transform(str.begin(),
		str.end(),
		strLower.begin(),
		::tolower);

	return strLower;
	return str;
}

std::wstring Utils::ToLower(std::wstring str)
{
	std::wstring strLower;
	strLower.resize(str.size());

	std::transform(str.begin(),
		str.end(),
		strLower.begin(),
		::tolower);

	return strLower;
	return str;
}

std::string Utils::ToUpper(std::string str)
{
	std::string strLower;
	strLower.resize(str.size());

	std::transform(str.begin(),
		str.end(),
		strLower.begin(),
		::toupper);

	return strLower;
	return str;
}

bool Utils::StringContains(std::string strA, std::string strB, bool ignore_case)
{
	if (strA.empty() || strB.empty())
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

bool Utils::StringContains(std::wstring strA, std::wstring strB, bool ignore_case)
{
	if (strA.empty() || strB.empty())
		return true;

	if (ignore_case)
	{
		strA = ToLower(strA);
		strB = ToLower(strB);
	}

	if (strA.find(strB) != std::wstring::npos)
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
		/*std::stringstream s;
		s << wstr.c_str();
		return s.str();*/
		return "range_error";
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

std::string Utils::RandomString(int size)
{
	std::string str = "";

	for (int i = 0; i < size; i++)
		str += RandomInt(0, 1) ? RandomInt(48, 57) : RandomInt(97, 122);

	return str;
}

std::string Utils::FormatString(const char* c, const char* args...)
{
	char buff[200];
	sprintf_s(buff, c, args);

	return std::string(buff);
}

void Utils::CopyToClipboard(std::string text)
{
	if (!::OpenClipboard(NULL))
		return;
	const int wbuf_length = ::MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, NULL, 0);
	HGLOBAL wbuf_handle = ::GlobalAlloc(GMEM_MOVEABLE, (SIZE_T)wbuf_length * sizeof(WCHAR));
	if (wbuf_handle == NULL)
	{
		::CloseClipboard();
		return;
	}
	WCHAR* wbuf_global = (WCHAR*)::GlobalLock(wbuf_handle);
	::MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, wbuf_global, wbuf_length);
	::GlobalUnlock(wbuf_handle);
	::EmptyClipboard();
	if (::SetClipboardData(CF_UNICODETEXT, wbuf_handle) == NULL)
		::GlobalFree(wbuf_handle);
	::CloseClipboard();
}

bool Utils::DownloadFile(std::string fileName, std::string directory, std::string url)
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

bool Utils::ContainsOnlyASCII(std::string buff)
{
	for (int i = 0; i < buff.size(); ++i)
	{
		if (buff[i] == 0)
			return true;
		if ((unsigned char)buff[i] > 127)
			return false;
	}
	return true;
}

Utils* utils = new Utils();