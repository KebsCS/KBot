#pragma once

#ifndef _MENU_H_
#define _MENU_H_

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

struct Menu
{
    Menu()
    {};


    bool MenuOpen;
    bool ExitBot;

    bool ConsoleOpen;

    struct
    {
        int AntiLag;
        int MenuKey;
    }Misc;
    int Configs = 0;

	struct
	{
		int HoldKey;
	}Orbwalker;

    struct
    {
        bool Master;
        bool Local;
        bool Turrets;
        int Slider[2] = { 16,10 };
        float Color[4] = { 255.0f / 255.0f, 85.0f / 255.0f, 50.0f / 255.0f, 255.0f / 255.0f };
        float LocalColor[4] = { 83.0f / 255.0f, 85.0f / 255.0f, 251.0f / 255.0f, 255.0f / 255.0f };


    }AARange;

    struct
    {
        bool Master = true;
        float Thickness;
       
    }Tracers;

    struct
    {
        bool Master = true;

    }Cooldowns;

    struct
    {
        bool Master;
        float Color[4] = { 255.0f / 255.0f, 85.0f / 255.0f, 50.0f / 255.0f, 255.0f / 255.0f };
    }LastHit;

    struct
    {
        bool Master;
        int Slot = 0;
        int Mode;
        float MouseSpeed;
    }AutoSmite;

    struct
    {
        bool Master;
    }Wards;

    struct
    {
        bool Master;
    }Inhibs;

};

extern Menu M;

#endif //!_MENU_H_