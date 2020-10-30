#include "Pixel.h"


//Constructor, set up for handling pixel operations
PixelHandler::PixelHandler()
{
	if (!Initialize())
		//LOG("!ERROR! could not initialize Pixel Checking!");

	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&startCount);
}

//destructor
PixelHandler::~PixelHandler()
{
	DeleteDC(hdc);
}

bool PixelHandler::Initialize()
{
	//_hDC = GetDC(NULL);      			//deprecated windows getpixel hdc   
	hdc = GetDC(HWND_DESKTOP);
	if (hdc == NULL)
		return false;
	return true;
}


/* INPUT: color - HEX (RGBA) color to be searched
upper left boundary of search box
bottom right boundary of search box
OUTPUT:	returns TRUE if it finds color in specified area
!warning! returns true on first instance found
reading top left to bottom right - horizontally
otherwise returns FALSE  */
bool PixelHandler::SearchPixelArea(unsigned int color, int UpperLeftX, int UpperLeftY, int BottomRightX, int BottomRightY)
{

	//LOG("Searching for", color, "color");

	int red, green, blue, alpha;
	unsigned int _color = 0;

	int MAX_WIDTH = (BottomRightX - UpperLeftX) + 1; //area to be searched
	int MAX_HEIGHT = (BottomRightY - UpperLeftY) + 1; //area to be searched

	hdcTemp = CreateCompatibleDC(hdc);
	BITMAPINFO bitmap;
	bitmap.bmiHeader.biSize = sizeof(bitmap.bmiHeader);
	bitmap.bmiHeader.biWidth = MAX_WIDTH;
	bitmap.bmiHeader.biHeight = MAX_HEIGHT;
	bitmap.bmiHeader.biPlanes = 1;
	bitmap.bmiHeader.biBitCount = 32;
	bitmap.bmiHeader.biCompression = BI_RGB;
	bitmap.bmiHeader.biSizeImage = MAX_WIDTH * 4 * MAX_HEIGHT;
	bitmap.bmiHeader.biClrUsed = 0;
	bitmap.bmiHeader.biClrImportant = 0;
	HBITMAP hBitmap2 = CreateDIBSection(hdcTemp, &bitmap, DIB_RGB_COLORS, (void**)(&bitPointer), NULL, NULL);
	SelectObject(hdcTemp, hBitmap2);
	BitBlt(hdcTemp, 0, 0, MAX_WIDTH, MAX_HEIGHT, hdc, UpperLeftX, UpperLeftY, SRCCOPY);

	//Check the bitmap for color iteratively
	for (int i = 0; i < (MAX_WIDTH * 4 * MAX_HEIGHT); i += 4)
	{
		_color = 0;
		blue = (int)bitPointer[i];
		green = (int)bitPointer[i + 1];
		red = (int)bitPointer[i + 2];
		alpha = (int)bitPointer[i + 3];
		_color |= red << 24;
		_color |= green << 16;
		_color |= blue << 8;

		if (color == _color)//check if color matches some pixel in area
		{
			DeleteDC(hdcTemp);
			DeleteObject(hBitmap2); //release memory

			//LOG("Found color", color);

			return true;
		}
	}
	DeleteDC(hdcTemp);
	DeleteObject(hBitmap2);

	//LOG("Didn't find color", color);

	return false;
}


//searches a given area for a color [HEX - RGBA] - returns true or false if found
bool PixelHandler::SearchPixelArea(unsigned int color, Area region)
{
	return SearchPixelArea(color, region.x1, region.y1, region.x2, region.y2);
}

/* INPUT: color - HEX (RGBA) color to be searched
upper left boundary of search box
bottom right boundary of search box
OUTPUT:	returns TRUE if it finds color in specified area
!warning! returns true on first instance found
reading bottom left to top right - horizontally
otherwise returns FALSE  */
bool PixelHandler::SearchPixelArea(unsigned int color, int UpperLeftX, int UpperLeftY, int BottomRightX, int BottomRightY, int tolerance)
{

	//LOG("Searching for", color, "color");

	int red, green, blue, alpha;
	unsigned char _red, _blue, _green;
	unsigned int _color = 0;

	int MAX_WIDTH = (BottomRightX - UpperLeftX) + 1; //area to be searched
	int MAX_HEIGHT = (BottomRightY - UpperLeftY) + 1; //area to be searched

	hdcTemp = CreateCompatibleDC(hdc);
	BITMAPINFO bitmap;
	bitmap.bmiHeader.biSize = sizeof(bitmap.bmiHeader);
	bitmap.bmiHeader.biWidth = MAX_WIDTH;
	bitmap.bmiHeader.biHeight = MAX_HEIGHT;
	bitmap.bmiHeader.biPlanes = 1;
	bitmap.bmiHeader.biBitCount = 32;
	bitmap.bmiHeader.biCompression = BI_RGB;
	bitmap.bmiHeader.biSizeImage = MAX_WIDTH * 4 * MAX_HEIGHT;
	bitmap.bmiHeader.biClrUsed = 0;
	bitmap.bmiHeader.biClrImportant = 0;
	HBITMAP hBitmap2 = CreateDIBSection(hdcTemp, &bitmap, DIB_RGB_COLORS, (void**)(&bitPointer), NULL, NULL);
	SelectObject(hdcTemp, hBitmap2);
	BitBlt(hdcTemp, 0, 0, MAX_WIDTH, MAX_HEIGHT, hdc, UpperLeftX, UpperLeftY, SRCCOPY);

	//Check the bitmap for color iteratively
	for (int i = 0; i < (MAX_WIDTH * 4 * MAX_HEIGHT); i += 4)
	{
		_color = 0;
		blue = (int)bitPointer[i];
		green = (int)bitPointer[i + 1];
		red = (int)bitPointer[i + 2];
		alpha = (int)bitPointer[i + 3];

		_red = (color & 0xff000000) >> 24;
		_green = (color & 0x00ff0000) >> 16;
		_blue = (color & 0x0000ff00) >> 8;

		if (((_red < (red + tolerance)) && (_red > (red - tolerance))) && ((_green < (green + tolerance)) && (_green > (green - tolerance))) && ((_blue < (blue + tolerance)) && (_blue > (blue - tolerance)))) //check if color matches some pixel in area
		{
			DeleteDC(hdcTemp);
			DeleteObject(hBitmap2); //release memory

			//LOG("Found color", color);

			return true;
		}
	}
	DeleteDC(hdcTemp);
	DeleteObject(hBitmap2);

	//LOG("Didn't find color", color);

	return false;
}

bool PixelHandler::SearchPixelArea(unsigned int color, Area region, int tolerance)
{
	return SearchPixelArea(color, region.x1, region.y1, region.x2, region.y2, tolerance);
}


//returns coordinates by value not reference. Values = -1 if not found
POINT PixelHandler::SearchPixelAreaForPoint(unsigned int color, int UpperLeftX, int UpperLeftY, int BottomRightX, int BottomRightY, int tolerance)
{

	//LOG("Searching for", color, "color coordinates");

	POINT pix; //coordinates where the color is found
	int red, green, blue, alpha;
	unsigned char _red, _green, _blue;
	unsigned int _color = 0;

	int MAX_WIDTH = (BottomRightX - UpperLeftX) + 1; //area to be searched
	int MAX_HEIGHT = (BottomRightY - UpperLeftY) + 1; //area to be searched

	hdcTemp = CreateCompatibleDC(hdc);
	BITMAPINFO bitmap;
	bitmap.bmiHeader.biSize = sizeof(bitmap.bmiHeader);
	bitmap.bmiHeader.biWidth = MAX_WIDTH;
	bitmap.bmiHeader.biHeight = MAX_HEIGHT;
	bitmap.bmiHeader.biPlanes = 1;
	bitmap.bmiHeader.biBitCount = 32;
	bitmap.bmiHeader.biCompression = BI_RGB;
	bitmap.bmiHeader.biSizeImage = MAX_WIDTH * 4 * MAX_HEIGHT;
	bitmap.bmiHeader.biClrUsed = 0;
	bitmap.bmiHeader.biClrImportant = 0;
	HBITMAP hBitmap2 = CreateDIBSection(hdcTemp, &bitmap, DIB_RGB_COLORS, (void**)(&bitPointer), NULL, NULL);
	SelectObject(hdcTemp, hBitmap2);
	BitBlt(hdcTemp, 0, 0, MAX_WIDTH, MAX_HEIGHT, hdc, UpperLeftX, UpperLeftY, SRCCOPY);

	//Check the bitmap for color iteratively
	for (int i = 0; i < (MAX_WIDTH * 4 * MAX_HEIGHT); i += 4)
	{
		_color = 0;
		blue = (int)bitPointer[i];
		green = (int)bitPointer[i + 1];
		red = (int)bitPointer[i + 2];
		alpha = (int)bitPointer[i + 3];
		_color |= red << 24;
		_color |= green << 16;
		_color |= blue << 8;

		_red = (color & 0xff000000) >> 24;
		_green = (color & 0x00ff0000) >> 16;
		_blue = (color & 0x0000ff00) >> 8;

		if (((_red < (red + tolerance)) && (_red > (red - tolerance))) && ((_green < (green + tolerance)) && (_green > (green - tolerance))) && ((_blue < (blue + tolerance)) && (_blue > (blue - tolerance)))) //check if color matches some pixel in area
		{
			// compute coords found
			int BotPos = i / 4; //current position built from bottom up
			int RowRemain = (MAX_WIDTH - (BotPos % MAX_WIDTH)) % MAX_WIDTH;//grabs remaining pixels in row
			float antiRows = (BotPos / (MAX_WIDTH));					  //Grab the rows under position
			if (RowRemain == 0)
				antiRows = ((BotPos - 1) / (MAX_WIDTH));
			int result = (MAX_WIDTH * MAX_HEIGHT) - ((int)antiRows * MAX_WIDTH + RowRemain); //it builds pixels left to right, BOTTOM UP. so this ludicrous algorithm is necessary

			pix.x = (i % (4 * MAX_WIDTH)) / 4;
			pix.y = (int)(result / MAX_WIDTH);
			pix.x += UpperLeftX;
			pix.y += UpperLeftY;


			DeleteDC(hdcTemp);
			DeleteObject(hBitmap2); //release memory

			//LOG("Found", color, "color at:", pix.x, pix.y);

			return pix;
		}
	}
	DeleteDC(hdcTemp);
	DeleteObject(hBitmap2);
	pix.x = -1;
	pix.y = -1;

	//LOG("Didn't find", color, "color coordinates");

	return pix;
}

POINT PixelHandler::SearchPixelAreaForPoint(unsigned int color, Area region, int tolerance)
{
	return SearchPixelAreaForPoint(color, region.x1, region.y1, region.x2, region.y2, tolerance);
}

//returns coordinates by value not reference. Values = -1 if not found
POINT PixelHandler::SearchPixelAreaForPoint(unsigned int color, int UpperLeftX, int UpperLeftY, int BottomRightX, int BottomRightY)
{

	//LOG("Searching for", color, "color coordinates");

	POINT pix; //coordinates where the color is found
	int red, green, blue, alpha;
	unsigned int _color = 0;

	int MAX_WIDTH = (BottomRightX - UpperLeftX) + 1; //area to be searched
	int MAX_HEIGHT = (BottomRightY - UpperLeftY) + 1; //area to be searched

	hdcTemp = CreateCompatibleDC(hdc);
	BITMAPINFO bitmap;
	bitmap.bmiHeader.biSize = sizeof(bitmap.bmiHeader);
	bitmap.bmiHeader.biWidth = MAX_WIDTH;
	bitmap.bmiHeader.biHeight = MAX_HEIGHT;
	bitmap.bmiHeader.biPlanes = 1;
	bitmap.bmiHeader.biBitCount = 32;
	bitmap.bmiHeader.biCompression = BI_RGB;
	bitmap.bmiHeader.biSizeImage = MAX_WIDTH * 4 * MAX_HEIGHT;
	bitmap.bmiHeader.biClrUsed = 0;
	bitmap.bmiHeader.biClrImportant = 0;
	HBITMAP hBitmap2 = CreateDIBSection(hdcTemp, &bitmap, DIB_RGB_COLORS, (void**)(&bitPointer), NULL, NULL);
	SelectObject(hdcTemp, hBitmap2);
	BitBlt(hdcTemp, 0, 0, MAX_WIDTH, MAX_HEIGHT, hdc, UpperLeftX, UpperLeftY, SRCCOPY);

	//Check the bitmap for color iteratively
	for (int i = 0; i < (MAX_WIDTH * 4 * MAX_HEIGHT); i += 4)
	{
		_color = 0;
		blue = (int)bitPointer[i];
		green = (int)bitPointer[i + 1];
		red = (int)bitPointer[i + 2];
		alpha = (int)bitPointer[i + 3];
		_color |= red << 24;
		_color |= green << 16;
		_color |= blue << 8;

		if (color == _color)//check if color matches some pixel in area
		{
			// compute coords found
			int BotPos = i / 4; //current position built from bottom up
			int RowRemain = (MAX_WIDTH - (BotPos % MAX_WIDTH)) % MAX_WIDTH;//grabs remaining pixels in row
			float antiRows = (BotPos / (MAX_WIDTH));					  //Grab the rows under position
			if (RowRemain == 0)
				antiRows = ((BotPos - 1) / (MAX_WIDTH));
			int result = (MAX_WIDTH * MAX_HEIGHT) - ((int)antiRows * MAX_WIDTH + RowRemain); //it builds pixels left to right, BOTTOM UP. so this ludicrous algorithm is necessary

			pix.x = (i % (4 * MAX_WIDTH)) / 4;
			pix.y = (int)(result / MAX_WIDTH);
			pix.x += UpperLeftX;
			pix.y += UpperLeftY;

			DeleteDC(hdcTemp);
			DeleteObject(hBitmap2); //release memory

			//LOG("Found", color, "color at:", pix.x, pix.y);

			return pix;
		}
	}
	DeleteDC(hdcTemp);
	DeleteObject(hBitmap2);
	pix.x = -1;
	pix.y = -1;

	//LOG("Didn't find", color, "color coordinates");

	return pix;
}

POINT PixelHandler::SearchPixelAreaForPoint(unsigned int color, Area region)
{
	return SearchPixelAreaForPoint(color, region.x1, region.y1, region.x2, region.y2);
}

/* INPUT:	(x,y) coordinates of pixel to be checked
OUTPUT:	returns the color at said coordinate */
unsigned int PixelHandler::GetPixelColor(int x, int y)
{
	int i = 0;
	int red, green, blue, alpha;
	unsigned int _color = 0;

	int MAX_WIDTH = 1; //area to be searched
	int MAX_HEIGHT = 1; //area to be searched

	hdcTemp = CreateCompatibleDC(hdc);
	BITMAPINFO bitmap;
	bitmap.bmiHeader.biSize = sizeof(bitmap.bmiHeader);
	bitmap.bmiHeader.biWidth = MAX_WIDTH;
	bitmap.bmiHeader.biHeight = MAX_HEIGHT;
	bitmap.bmiHeader.biPlanes = 1;
	bitmap.bmiHeader.biBitCount = 32;
	bitmap.bmiHeader.biCompression = BI_RGB;
	bitmap.bmiHeader.biSizeImage = MAX_WIDTH * 4 * MAX_HEIGHT;
	bitmap.bmiHeader.biClrUsed = 0;
	bitmap.bmiHeader.biClrImportant = 0;
	HBITMAP hBitmap2 = CreateDIBSection(hdcTemp, &bitmap, DIB_RGB_COLORS, (void**)(&bitPointer), NULL, NULL);
	SelectObject(hdcTemp, hBitmap2);
	BitBlt(hdcTemp, 0, 0, MAX_WIDTH, MAX_HEIGHT, hdc, x, y, SRCCOPY);

	_color = 0;
	blue = (int)bitPointer[i];
	green = (int)bitPointer[i + 1];
	red = (int)bitPointer[i + 2];
	alpha = (int)bitPointer[i + 3];
	_color |= red << 24;
	_color |= green << 16;
	_color |= blue << 8;
	//printf("R:%4i, G:%4i, B:%4i  ... (%4i,%4i)  %08x\n", red, green, blue, x, y, _color);

	DeleteDC(hdcTemp);
	DeleteObject(hBitmap2); //release memory
	return _color;

}

/* INPUT:	(x,y) coordinates of pixel to be checked
OUTPUT:	returns the color at said coordinate */
unsigned int PixelHandler::GetPixelColor(POINT coord)
{
	unsigned int color = GetPixelColor(coord.x, coord.y);
	return color;

}


bool PixelHandler::VerifyPixelColor(unsigned int color, int x, int y)
{
	unsigned int pixColor = GetPixelColor(x, y);
	if (pixColor == color)
		return true;
	return false;
}

bool PixelHandler::VerifyPixelColor(unsigned int color, int x, int y, int tolerance)
{
	unsigned char red, green, blue, _red, _blue, _green;
	unsigned int pixColor = GetPixelColor(x, y);

	_red = (pixColor & 0xff000000) >> 24;
	_green = (pixColor & 0x00ff0000) >> 16;
	_blue = (pixColor & 0x0000ff00) >> 8;

	red = (color & 0xff000000) >> 24;
	green = (color & 0x00ff0000) >> 16;
	blue = (color & 0x0000ff00) >> 8;
	//printf("color: %08x, pixColor: %08x\n", color, pixColor);
	//printf("Red:%3i, _Red:%3i\n", red, _red);
	if (((red < (_red + tolerance)) && (red > (_red - tolerance))) && ((green < (_green + tolerance)) && (green > (_green - tolerance))) && ((blue < (_blue + tolerance)) && (blue > (_blue - tolerance))))
		return true;
	return false;

}

bool PixelHandler::VerifyPixelColor(Pixel pix)
{
	unsigned int color;
	color = GetPixelColor(pix._x, pix._y);
	if (color == pix._color)
		return true;
	return false;
}

// Searches just around a certain pixel with a tolerance of 10 for a certain pixel
bool PixelHandler::VerifyRoughPixel(Pixel pix)
{
	return SearchPixelArea(pix._color, pix._x - 8, pix._y - 8, pix._x + 8, pix._y + 8, 10);
}

//USED to find pixels on screen and get informations
unsigned int PixelHandler::DEBUG()
{
	POINT curMouse;
	COLORREF color;
	while (true)
	{
		GetCursorPos(&curMouse);
		color = GetPixelColor(curMouse.x, curMouse.y);
		RGBA colorRGB(color);
		//std::cout << "Color: 0x" << std::hex << color << std::dec << " (" << colorRGB.R << " " << colorRGB.G << " " << colorRGB.B << " " << colorRGB.A << ") found at (" << curMouse.x << ", " << curMouse.y << ")\n";
		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			std::fstream hFile("debug.txt", std::ios::out | std::ios::app);
			if (hFile.is_open())
			{
				hFile << "Color: 0x" << std::hex << color << std::dec << " (" << colorRGB.R << " " << colorRGB.G << " " << colorRGB.B << " " << colorRGB.A << ") found at (" << curMouse.x << ", " << curMouse.y << ")\n";
				hFile.close();
			}
			while (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {}
		}
	}
}


double PixelHandler::GetExecutionTime()
{
	QueryPerformanceCounter(&endCount);

	return(endCount.QuadPart - startCount.QuadPart) * 1000.0 / frequency.QuadPart;
}


unsigned int PixelHandler::RGBtoHEX(int r, int g, int b)
{
	return ((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff);
}


unsigned int PixelHandler::RGBtoHEX(int r, int g, int b, int a)
{
	return ((r & 0xff) << 24) + ((g & 0xff) << 16) + ((b & 0xff) << 8) + (a & 0xff);

}


bool PixelHandler::SearchforImage(LPCWSTR path, int x1, int y1, int x2, int y2)
{


}
//
//
//bool PixelHandler::SearchforImage(LPCWSTR path, POINT coord)
//{
//	return SearchforImage(path, coord.x, coord.y);
//}




bool PixelHandler::Screenshot(int UpperLeftX, int UpperLeftY, int BottomRightX, int BottomRightY, const char* fileName)
{

	int MAX_WIDTH = (BottomRightX - UpperLeftX) + 1; //area to be searched
	int MAX_HEIGHT = (BottomRightY - UpperLeftY) + 1; //area to be searched

	hdcTemp = CreateCompatibleDC(hdc);
	BITMAPINFO bitmap;
	BITMAPFILEHEADER Header;
	bitmap.bmiHeader.biSize = sizeof(bitmap.bmiHeader);
	bitmap.bmiHeader.biWidth = MAX_WIDTH;
	bitmap.bmiHeader.biHeight = MAX_HEIGHT;
	bitmap.bmiHeader.biPlanes = 1;
	bitmap.bmiHeader.biBitCount = 32;
	bitmap.bmiHeader.biCompression = BI_RGB;
	bitmap.bmiHeader.biSizeImage = MAX_WIDTH * 4 * MAX_HEIGHT;
	bitmap.bmiHeader.biClrUsed = 0;
	bitmap.bmiHeader.biClrImportant = 0;
	HBITMAP hBitmap2 = CreateDIBSection(hdcTemp, &bitmap, DIB_RGB_COLORS, (void**)(&bitPointer), NULL, NULL);
	SelectObject(hdcTemp, hBitmap2);
	BitBlt(hdcTemp, 0, 0, MAX_WIDTH, MAX_HEIGHT, hdc, UpperLeftX, UpperLeftY, SRCCOPY);
	Header.bfType = 0x4D42;
	Header.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(bitmap.bmiHeader);

	std::fstream hFile(fileName, std::ios::out | std::ios::binary);
	if (hFile.is_open())
	{
		hFile.write((char*)&Header, sizeof(Header));
		hFile.write((char*)&bitmap.bmiHeader, sizeof(bitmap.bmiHeader));
		hFile.write((char*)bitPointer, (((32 * MAX_WIDTH + 31) & ~31) / 8) * MAX_HEIGHT);
		hFile.close();
		DeleteObject(hBitmap2);
		DeleteDC(hdcTemp);

		//LOG("Screenshot saved as", fileName);
		return true;
	}

	DeleteDC(hdcTemp);
	DeleteObject(hBitmap2);

	//LOG("Failed to save screenshot");

	return false;
}

bool PixelHandler::Screenshot(Area region, const char* fileName)
{
	return Screenshot(region.x1, region.y1, region.x2, region.y2, fileName);
}


void PixelHandler::SetScreenPixel(int x, int y, int r, int g, int b)
{
	HWND hWnd = GetDesktopWindow();
	HDC hdc = GetDC(hWnd);
	SetPixel(hdc, x, y, RGBtoHEX(b, g, r));
	ReleaseDC(hWnd, hdc);
}

void PixelHandler::SetScreenPixel(POINT coord, int r, int g, int b)
{
	SetScreenPixel(coord.x, coord.y, r, g, b);
}

