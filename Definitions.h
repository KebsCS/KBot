#pragma once

#ifndef _DEFINITIONS_H_
#define _DEFINITIONS_H_

#pragma warning(disable:4996)

#include <cmath>
#include <vector>
#include <map>
#include <thread>
#include <chrono>


#include "xor.h"

#define ENABLE_XOR

#ifdef ENABLE_XOR
#define XorStr _xor_ 
#else
#define XorStr
#endif

// default screen size
#define SCREENWIDTH ::GetSystemMetrics(SM_CXSCREEN)
#define SCREENHEIGHT ::GetSystemMetrics(SM_CYSCREEN)


#define M_PI 3.14159265358979323846	// pi
#define M_PI_F ((float)(M_PI))	// Shouldn't collide with anything.


//rand() isnt really random, todo
//use bind(uniform_int_distribution<>{min,man},default_random_engine{})
//from <random>

#define RandomInt(min, max) (rand() % (max - min + 1) + min) // inclusive random int (e.g 0 to 100)

#define INRANGE(x,a,b)    (x >= a && x <= b) //if a<=x<=b

//calculate x coordinates for SendInput mouse movement
#define SENDINPUTX(x) (x * 65536 / (SCREENWIDTH)+1) 
//calculate y coordinates for SendInput mouse movement
#define SENDINPUTY(y) (y * 65536 / (SCREENHEIGHT)+1) 

#ifndef RAD2DEG
#define RAD2DEG( x  )  ( (float)(x) * (float)(180.f / M_PI_F) ) // Radians to degrees
#endif

#ifndef DEG2RAD
#define DEG2RAD( x  )  ( (float)(x) * (float)(M_PI_F / 180.f) ) // degrees to radians
#endif

#define PressedKey( x ) ( GetAsyncKeyState (x) & 0x8000 )

#define HUMANIZER

#ifdef HUMANIZER

#define VERYFAST_REACTION_TIME RandomInt(1,10)
#define FAST_REACTION_TIME RandomInt(9,11)
#define AVERAGE_REACTION_TIME RandomInt(40,60)
#define MEDIUM_REACTION_TIME RandomInt(90,110)
#define SLOW_REACTION_TIME RandomInt(900,1100)

#else
#define VERYFAST_REACTION_TIME 5
#define FAST_REACTION_TIME 10
#define AVERAGE_REACTION_TIME 50
#define MEDIUM_REACTION_TIME 100
#define SLOW_REACTION_TIME 1000

#endif

enum text_alignment : int 
{ 
	lefted = 0, 
	centered = 1, 
	righted = 2 
};

enum Arrow_Keys : int
{
	LEFT = 1,
	RIGHT = 2,
	UP = 3,
	DOWN = 4


};

enum WardType : int
{
	NormalWard = 1,
	ControlWard = 2,
	BlueWard = 3,
};

enum SpellSlotID : int
{
	Q = 0,
	W = 1,
	E = 2,
	R = 3,
	Summoner1 = 4,
	Summoner2 = 5,
	Item1 = 6,
	Item2 = 7,
	Item3 = 8,
	Item4 = 9,
	Item5 = 10,
	Item6 = 11,
	Trinket = 12,
	Recall = 13,
	Passive = 63, 
};



enum ObjectType : int
{
	//x << y = x*pow(2,y)
	//x >> y = x/pow(2,y)
	GameObject = (1 << 0),  //0x1
	NeutralCamp = (1 << 1),  //0x2
	DeadObject = (1 << 4),  //0x10
	InvalidObject = (1 << 5),  //0x20
	AIBaseCommon = (1 << 7),  //0x80
	AttackableUnit = (1 << 9),  //0x200
	AI = (1 << 10), //0x400
	Minion = (1 << 11), //0x800
	Hero = (1 << 12), //0x1000
	Turret = (1 << 13), //0x2000
	Unknown0 = (1 << 14), //0x4000
	Missile = (1 << 15), //0x8000
	Unknown1 = (1 << 16), //0x10000
	Building = (1 << 17), //0x20000
	Unknown2 = (1 << 18), //0x40000
};

struct RGBA
{
	int R, G, B, A;
	inline RGBA()
	{
		R = G = B = A = 255;
	}
	inline RGBA(const int r, const int g, const int b, const int a=255)
	{
		R = r;
		G = g;
		B = b;
		A = a;
	}
	inline RGBA(const float r, const float g, const float b, const float a = 1.f)
	{
		R = r * 255;
		G = g * 255;
		B = b * 255;
		A = a * 255;
	}
	inline RGBA(const unsigned hex) // HEX to RGB
	{
		R = ((hex >> 24) & 0xFF);
		G = ((hex >> 16) & 0xFF);
		B = ((hex >> 8) & 0xFF);
		A = ((hex) & 0xFF);
	}
	inline unsigned HEX() const // RGB to HEX
	{
		return ((R & 0xff) << 24) + ((G & 0xff) << 16) + ((B & 0xff) << 8) + (A & 0xff);
	}
	inline bool operator==(RGBA& rgba) const
	{
		if (R == rgba.R && G == rgba.G && B == rgba.B && A == rgba.A)
			return true;
	}

};

struct Area
{
	int x1;
	int x2;
	int y1;
	int y2;
	Area()
	{
		x1 = x2 = y1 = y2 = 0;
	}
	//left, top, right, bottom
	Area(int _x1, int _y1, int _x2, int _y2)
	{
		x1 = _x1;
		x2 = _x2;
		y1 = _y1;
		y2 = _y2;
	}

};

struct Pixel
{
	unsigned int _color;   //assign colors in HEX (RR GG BB AA)  -  RGBA format (typically alpha is 00)
	int _x;
	int _y;

	Pixel()
	{
		_x = _y = _color = 0;
	}
	Pixel(unsigned int color, int x, int y)
	{
		_x = x;
		_y = y;
		_color = color;
	}
	void Set(unsigned int color, int x, int y)
	{
		_x = x;
		_y = y;
		_color = color;
	}
};


struct Vector3
{
	float X, Y, Z;

	inline Vector3() 
	{
		X = Y = Z = 0;
	}
	inline Vector3(const float x, const float y, const float z)
	{
		X = x; Y = y; Z = z;
	}
	inline void Zero()
	{
		X = Y = Z = 0;
	}

	inline bool operator == (const Vector3& A) const
	{
		if (X == A.X && Y == A.Y && Z == A.Z)
			return true;
	}

	inline Vector3 operator + (const Vector3& A) const
	{
		return Vector3(X + A.X, Y + A.Y, Z + A.Z);
	}

	inline Vector3 operator + (const float A) const
	{
		return Vector3(X + A, Y + A, Z + A);
	}

	inline Vector3 operator * (const float A) const
	{
		return Vector3(A * X, A * Y, A * Z);
	}

	inline Vector3 operator * (const Vector3& A) const
	{
		return Vector3(A.X * X, A.Y * Y, A.Z * Z);
	}

	inline Vector3 operator - (const float A) const
	{
		return Vector3(A * X, A * Y, A * Z);
	}

	inline Vector3 operator - (const Vector3& A) const
	{
		return Vector3(A.X - X, A.Y - Y, A.Z - Z);
	}

	inline Vector3 operator / (const float A) const
	{
		return Vector3(A / X, A / Y, A / Z);
	}

	inline Vector3 operator / (const Vector3& A) const
	{
		return Vector3(A.X / X, A.Y / Y, A.Z / Z);
	}

	float dot(const Vector3& vec) const
	{
		return X * vec.X + Y * vec.Y + Z * vec.Z;
	}

	inline float lengthSquared()
	{
		return X * X + Y * Y + Z * Z;
	}

	inline float length()
	{
		return (float)sqrt(lengthSquared());
	}

	inline Vector3 perpendicularTo()
	{
		return Vector3(Z, Y, -X);
	}

	inline Vector3 Normalize()
	{
		float length = this->length();
		if (length != 0)
		{
			float inv = 1.0f / length;
			X *= inv;
			Y *= inv;
			Z *= inv;
		}
		return Vector3(X, Y, Z);
	}

	inline float DistTo(const Vector3& A) {
		float out = sqrtf(powf(X - A.X, 2) + powf(Y - A.Y, 2) + powf(Z - A.Z, 2));
		return out < 0 ? out * -1 : out;
	}
};

struct Vector4
{
	float X, Y, Z, W;

	inline Vector4()
	{
		X = Y = Z = W = 0;
	}
	inline Vector4(const float x, const float y, const float z, const float w)
	{
		X = x; Y = y; Z = z; W = w;
	}
};

//https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
const char* const keyNames[] =
{
"",
"Left mouse button",
"Right mouse button",
"Control-break processing",
"Middle mouse button",
"X1 mouse button",
"X2 mouse button",
"",//"Undefined",
"BACKSPACE key",
"TAB key",
"",//"Reserved",
"",
"CLEAR key",
"ENTER key",
"",//"Undefined",
"",
"SHIFT key",
"CTRL key",
"ALT key",
"PAUSE key",
"CAPS LOCK key",
"",//"IME Kana mode","",//"IME Hanguel mode (maintained for compatibility; use VK_HANGUL)","",//"IME Hangul mode",
"",//"IME On",
"",//"IME Junja mode",
"",//"IME final mode",
"",//"IME Hanja mode","",//"IME Kanji mode",
"",//IME Off",
"ESC key",
"",//"IME convert",
"",//"IME nonconvert",
"",//"IME accept",
"",//"IME mode change request",
"SPACEBAR",
"PAGE UP key",
"PAGE DOWN key",
"END key",
"HOME key",
"LEFT ARROW key",
"UP ARROW key",
"RIGHT ARROW key",
"DOWN ARROW key",
"SELECT key",
"PRINT key",
"EXECUTE key",
"PRINT SCREEN key",
"INS key",
"DEL key",
"HELP key",
"0 key",
"1 key",
"2 key",
"3 key",
"4 key",
"5 key",
"6 key",
"7 key",
"8 key",
"9 key",
"",//"Undefined",
"",
"",
"",
"",
"",
"",
"A key",
"B key",
"C key",
"D key",
"E key",
"F key",
"G key",
"H key",
"I key",
"J key",
"K key",
"L key",
"M key",
"N key",
"O key",
"P key",
"Q key",
"R key",
"S key",
"T key",
"U key",
"V key",
"W key",
"X key",
"Y key",
"Z key",
"Left Windows key",
"Right Windows key",
"Applications key",
"",//"Reserved",
"Computer Sleep key",
"Numeric keypad 0 key",
"Numeric keypad 1 key",
"Numeric keypad 2 key",
"Numeric keypad 3 key",
"Numeric keypad 4 key",
"Numeric keypad 5 key",
"Numeric keypad 6 key",
"Numeric keypad 7 key",
"Numeric keypad 8 key",
"Numeric keypad 9 key",
"Multiply key",
"Add key",
"Separator key",
"Subtract key",
"Decimal key",
"Divide key",
"F1 key",
"F2 key",
"F3 key",
"F4 key",
"F5 key",
"F6 key",
"F7 key",
"F8 key",
"F9 key",
"F10 key",
"F11 key",
"F12 key",
"F13 key",
"F14 key",
"F15 key",
"F16 key",
"F17 key",
"F18 key",
"F19 key",
"F20 key",
"F21 key",
"F22 key",
"F23 key",
"F24 key",
"",//"Unassigned",
"",
"",
"",
"",
"",
"",
"",
"NUM LOCK key",
"SCROLL LOCK key",
"OEM specific",
"",
"",
"",
"",
"", //"Unassigned",
"",
"",
"",
"",
"",
"",
"",
"",
"Left SHIFT key",
"Right SHIFT key",
"Left CONTROL key",
"Right CONTROL key",
"Left MENU key",
"Right MENU key",
"Browser Back key",
"Browser Forward key",
"Browser Refresh key",
"Browser Stop key",
"Browser Search key",
"Browser Favorites key",
"Browser Start and Home key",
"Volume Mute key",
"Volume Down key",
"Volume Up key",
"Next Track key",
"Previous Track key",
"Stop Media key",
"Play/Pause Media key",
"Start Mail key",
"Select Media key",
"Start Application 1 key",
"Start Application 2 key",
"",//"Reserved",
"",
";:",//"Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the ';:' key",
"+",//"For any country/region, the '+' key",
",",//"For any country/region, the ',' key",
"-",//"For any country/region, the '-' key",
".",//"For any country/region, the '.' key",
"/?",//"Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '/?' key",
"`~",//"Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '`~' key",
"",//"Reserved",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",//"Unassigned",
"",
"",
"[{",//"Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '[{' key",
"\\|",//"Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '\\|' key",
"]}",//"Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the ']}' key",
"\'\"",//"Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the 'single-quote/double-quote' key",
"",//"Used for miscellaneous characters; it can vary by keyboard.",
"",//"Reserved",
"",//"OEM specific",
"",//"Either the angle bracket key or the backslash key on the RT 102-key keyboard",
"",//"OEM specific",
"",
"",//"IME PROCESS key",
"",//"OEM specific",
"",//"Used to pass Unicode characters as if they were keystrokes. The VK_PACKET key is the low word of a 32-bit Virtual Key value used for non-keyboard input methods.",
"",//"Unassigned",
"",//"OEM specific",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"Attn key",
"CrSel key",
"ExSel key",
"Erase EOF key",
"Play key",
"Zoom key",
"Reserved",
"PA1 key",
"Clear key",

};


enum DirectInputKeys : int
{
	DIK_ESCAPE = 0x01,
	DIK_1 = 0x02,
	DIK_2 = 0x03,
	DIK_3 = 0x04,
	DIK_4 = 0x05,
	DIK_5 = 0x06,
	DIK_6 = 0x07,
	DIK_7 = 0x08,
	DIK_8 = 0x09,
	DIK_9 = 0x0A,
	DIK_0 = 0x0B,
	DIK_MINUS = 0x0C,    /* - on main keyboard */
	DIK_EQUALS = 0x0D,
	DIK_BACK = 0x0E,    /* backspace */
	DIK_TAB = 0x0F,
	DIK_Q = 0x10,
	DIK_W = 0x11,
	DIK_E = 0x12,
	DIK_R = 0x13,
	DIK_T = 0x14,
	DIK_Y = 0x15,
	DIK_U = 0x16,
	DIK_I = 0x17,
	DIK_O = 0x18,
	DIK_P = 0x19,
	DIK_LBRACKET = 0x1A,
	DIK_RBRACKET = 0x1B,
	DIK_RETURN = 0x1C,    /* Enter on main keyboard */
	DIK_LCONTROL = 0x1D,
	DIK_A = 0x1E,
	DIK_S = 0x1F,
	DIK_D = 0x20,
	DIK_F = 0x21,
	DIK_G = 0x22,
	DIK_H = 0x23,
	DIK_J = 0x24,
	DIK_K = 0x25,
	DIK_L = 0x26,
	DIK_SEMICOLON = 0x27,
	DIK_APOSTROPHE = 0x28,
	DIK_GRAVE = 0x29,    /* accent grave */
	DIK_LSHIFT = 0x2A,
	DIK_BACKSLASH = 0x2B,
	DIK_Z = 0x2C,
	DIK_X = 0x2D,
	DIK_C = 0x2E,
	DIK_V = 0x2F,
	DIK_B = 0x30,
	DIK_N = 0x31,
	DIK_M = 0x32,
	DIK_COMMA = 0x33,
	DIK_PERIOD = 0x34,    /* . on main keyboard */
	DIK_SLASH = 0x35,    /* / on main keyboard */
	DIK_RSHIFT = 0x36,
	DIK_MULTIPLY = 0x37,    /* * on numeric keypad */
	DIK_LMENU = 0x38,    /* left Alt */
	DIK_SPACE = 0x39,
	DIK_CAPITAL = 0x3A,
	DIK_F1 = 0x3B,
	DIK_F2 = 0x3C,
	DIK_F3 = 0x3D,
	DIK_F4 = 0x3E,
	DIK_F5 = 0x3F,
	DIK_F6 = 0x40,
	DIK_F7 = 0x41,
	DIK_F8 = 0x42,
	DIK_F9 = 0x43,
	DIK_F10 = 0x44,
	DIK_NUMLOCK = 0x45,
	DIK_SCROLL = 0x46,    /* Scroll Lock */
	DIK_NUMPAD7 = 0x47,
	DIK_NUMPAD8 = 0x48,
	DIK_NUMPAD9 = 0x49,
	DIK_SUBTRACT = 0x4A,    /* - on numeric keypad */
	DIK_NUMPAD4 = 0x4B,
	DIK_NUMPAD5 = 0x4C,
	DIK_NUMPAD6 = 0x4D,
	DIK_ADD = 0x4E,    /* + on numeric keypad */
	DIK_NUMPAD1 = 0x4F,
	DIK_NUMPAD2 = 0x50,
	DIK_NUMPAD3 = 0x51,
	DIK_NUMPAD0 = 0x52,
	DIK_DECIMAL = 0x53,    /* . on numeric keypad */
	DIK_OEM_102 = 0x56,    /* <> or \| on RT 102-key keyboard (Non-U.S.) */
	DIK_F11 = 0x57,
	DIK_F12 = 0x58,
	DIK_F13 = 0x64,    /*                     (NEC PC98) */
	DIK_F14 = 0x65,    /*                     (NEC PC98) */
	DIK_F15 = 0x66,    /*                     (NEC PC98) */
	DIK_KANA = 0x70,    /* (Japanese keyboard)            */
	DIK_ABNT_C1 = 0x73,    /* /? on Brazilian keyboard */
	DIK_CONVERT = 0x79,    /* (Japanese keyboard)            */
	DIK_NOCONVERT = 0x7B,    /* (Japanese keyboard)            */
	DIK_YEN = 0x7D,    /* (Japanese keyboard)            */
	DIK_ABNT_C2 = 0x7E,    /* Numpad . on Brazilian keyboard */
	DIK_NUMPADEQUALS = 0x8D,    /* = on numeric keypad (NEC PC98) */
	DIK_PREVTRACK = 0x90,    /* Previous Track (DIK_CIRCUMFLEX on Japanese keyboard) */
	DIK_AT = 0x91,    /*                     (NEC PC98) */
	DIK_COLON = 0x92,    /*                     (NEC PC98) */
	DIK_UNDERLINE = 0x93,    /*                     (NEC PC98) */
	DIK_KANJI = 0x94,    /* (Japanese keyboard)            */
	DIK_STOP = 0x95,    /*                     (NEC PC98) */
	DIK_AX = 0x96,    /*                     (Japan AX) */
	DIK_UNLABELED = 0x97,    /*                        (J3100) */
	DIK_NEXTTRACK = 0x99,    /* Next Track */
	DIK_NUMPADENTER = 0x9C,    /* Enter on numeric keypad */
	DIK_RCONTROL = 0x9D,
	DIK_MUTE = 0xA0,    /* Mute */
	DIK_CALCULATOR = 0xA1,    /* Calculator */
	DIK_PLAYPAUSE = 0xA2,    /* Play / Pause */
	DIK_MEDIASTOP = 0xA4,    /* Media Stop */
	DIK_VOLUMEDOWN = 0xAE,    /* Volume - */
	DIK_VOLUMEUP = 0xB0,    /* Volume + */
	DIK_WEBHOME = 0xB2,    /* Web home */
	DIK_NUMPADCOMMA = 0xB3,    /* , on numeric keypad (NEC PC98) */
	DIK_DIVIDE = 0xB5,    /* / on numeric keypad */
	DIK_SYSRQ = 0xB7,
	DIK_RMENU = 0xB8,    /* right Alt */
	DIK_PAUSE = 0xC5,    /* Pause */
	DIK_HOME = 0xC7,    /* Home on arrow keypad */
	DIK_UP = 0xC8,    /* UpArrow on arrow keypad */
	DIK_PRIOR = 0xC9,    /* PgUp on arrow keypad */
	DIK_LEFT = 0xCB,    /* LeftArrow on arrow keypad */
	DIK_RIGHT = 0xCD,    /* RightArrow on arrow keypad */
	DIK_END = 0xCF,    /* End on arrow keypad */
	DIK_DOWN = 0xD0,    /* DownArrow on arrow keypad */
	DIK_NEXT = 0xD1,    /* PgDn on arrow keypad */
	DIK_INSERT = 0xD2,    /* Insert on arrow keypad */
	DIK_DELETE = 0xD3,    /* Delete on arrow keypad */
	DIK_LWIN = 0xDB,    /* Left Windows key */
	DIK_RWIN = 0xDC,    /* Right Windows key */
	DIK_APPS = 0xDD,    /* AppMenu key */
	DIK_POWER = 0xDE,    /* System Power */
	DIK_SLEEP = 0xDF,    /* System Sleep */
	DIK_WAKE = 0xE3,    /* System Wake */
	DIK_WEBSEARCH = 0xE5,    /* Web Search */
	DIK_WEBFAVORITES = 0xE6,    /* Web Favorites */
	DIK_WEBREFRESH = 0xE7,    /* Web Refresh */
	DIK_WEBSTOP = 0xE8,    /* Web Stop */
	DIK_WEBFORWARD = 0xE9,    /* Web Forward */
	DIK_WEBBACK = 0xEA,    /* Web Back */
	DIK_MYCOMPUTER = 0xEB,    /* My Computer */
	DIK_MAIL = 0xEC,    /* Mail */
	DIK_MEDIASELECT = 0xED    /* Media Select */
};


const std::vector<std::string>ChampNames = {
	"Aatrox", "Ahri", "Akali", "Alistar", "Amumu", "Anivia", "Annie", "Aphelios"
	"Ashe", "Aurelion Sol", "AurelionSol","Azir", "Bard", "Blitzcrank", "Brand", "Braum", "Caitlyn",
	"Camille", "Cassiopeia", "Cho'Gath", "Chogath", "Corki", "Darius", "Diana", "Dr. Mundo", "Drmundo", "Draven",
	"Ekko", "Elise", "Evelynn", "Ezreal", "FiddleSticks", "Fiora", "Fizz", "Galio",
	"Gangplank", "Garen", "Gnar", "Gragas", "Graves", "Hecarim", "Heimerdinger", "Illaoi", "Irelia",
	"Ivern", "Janna", "Jarvan IV","JarvanIV", "Jax", "Jayce", "Jhin", "Jinx", "Kai'sa", "Kaisa", "Kalista", "Karma",
	"Karthus", "Kassadin", "Katarina", "Kayle", "Kayn", "Kennen", "Kha'zix", "Kindred", "Kled", "Kog'maw", "KogMaw",
	"LeBlanc", "Lee Sin", "Leona", "Lillia", "Lissandra", "Lucian", "Lulu", "Lux", "Malphite", "Malzahar", "Maokai",
	"Master Yi", "MasterYi", "Miss Fortune", "MissFortune", "Mordekaiser", "Morgana", "Nami", "Nasus", "Nautilus", "Neeko",
	"Nidalee", "Nocturne", "Nunu" , "Nunu & Willump", "Olaf", "Orianna", "Ornn", "Pantheon", "Poppy", "Pyke", "Qiyana", "Quinn",
	"Rakan", "Rammus", "Rek'Sai", "RekSai", "Renekton", "Rengar", "Riven", "Rumble", "Ryze", "Samira", "Sejuani", "Senna", "Seraphine", "Sett", "Shaco",
	"Shen", "Shyvana", "Singed", "Sion", "Sivir", "Skarner", "Sona", "Soraka", "Swain", "Sylas", "Syndra", "Tahm Kench", "TahmKench",
	"Taliyah", "Talon", "Taric", "Teemo", "Thresh", "Tristana", "Trundle", "Tryndamere", "Twisted Fate", "TwistedFate", "Twitch",
	"Udyr", "Urgot", "Varus", "Vayne", "Veigar", "Vel'Koz", "Velkoz","Vi", "Viktor", "Vladimir", "Volibear",
	"Warwick", "Wukong", "Xayah", "Xerath", "Xin Zhao", "XinZhao","Yasuo", "Yone", "Yorick", "Yuumi", "Zac", "Zed", "Ziggs", "Zilean", "Zoe", "Zyra",
	"MonkeyKing", "Mega Gnar", "MegaGnar", "Target Dummy", "TargetDummy"
};

#endif // !_DEFINITIONS_H_
