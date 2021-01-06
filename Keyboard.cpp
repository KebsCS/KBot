#include "Keyboard.h"

void Keyboard::SetDelay(int ms)
{
	pressTime = ms;
	//clog.AddLog("[keyboard] Set delay to", ms, "ms");
}

void Keyboard::SpecialKeyDown(int vk)
{
	INPUT       Input = { 0 };
	Input.type = INPUT_KEYBOARD;
	Input.ki.dwFlags = KEYEVENTF_SCANCODE;
	Input.ki.wScan = MapVirtualKey(vk, 0);
	::SendInput(1, &Input, sizeof(Input));
}

void Keyboard::SpecialKeyUp(int vk)
{
	INPUT       Input = { 0 };
	Input.type = INPUT_KEYBOARD;
	Input.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
	Input.ki.wScan = MapVirtualKey(vk, 0);
	::SendInput(1, &Input, sizeof(Input));
}

void Keyboard::ScancodeKeyDown(int vk)
{
	KEYBDINPUT  kb = { 0 };
	INPUT       Input = { 0 };
	kb.dwFlags = KEYEVENTF_SCANCODE;
	kb.wVk = 0;
	kb.wScan = vk;
	kb.time = 0;
	kb.dwExtraInfo = 0;
	Input.type = INPUT_KEYBOARD;
	Input.ki = kb;

	::SendInput(1, &Input, sizeof(Input));
}

void Keyboard::ScancodeKeyUp(int vk)
{
	KEYBDINPUT  kb = { 0 };
	INPUT       Input = { 0 };
	ZeroMemory(&kb, sizeof(KEYBDINPUT));
	ZeroMemory(&Input, sizeof(INPUT));
	kb.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
	kb.wVk = 0;
	kb.wScan = vk;
	Input.type = INPUT_KEYBOARD;
	Input.ki = kb;
	::SendInput(1, &Input, sizeof(Input));
}

//https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
//https://www.asciitable.com
void Keyboard::GenerateKey(int vk, BOOL bExtended, bool shift)
{
	KEYBDINPUT  kb = { 0 };
	INPUT       Input = { 0 };

	/* special chars */
	switch (vk)
	{
	case 33: vk = 49; shift = true; break; // !
	case 34: vk = VK_OEM_7; shift = true; break; // "
	case 35: vk = 51; shift = true;  break; // #
	case 36: vk = 52; shift = true; break; // $
	case 37: vk = 53; shift = true; break; // %
	case 38: vk = 55; shift = true; break; // &
	case 39: vk = VK_OEM_7; break; // '
	case 40: vk = 57; shift = true; break; // (
	case 41: vk = 48; shift = true; break; // )
	case 42: vk = 56; shift = true; break; // *
	case 43: vk = VK_OEM_PLUS; shift = true; break; // +
	case 44: vk = VK_OEM_COMMA; break; // ,
	case 45: vk = VK_OEM_MINUS; break; // -
	case 46: vk = VK_OEM_PERIOD; break;	 // .
	case 47: vk = VK_OEM_2; break; // /
	case 58: vk = VK_OEM_1; shift = true; break; // :
	case 59: vk = VK_OEM_1; break; // ;
	case 60: vk = VK_OEM_COMMA; shift = true; break; // <
	case 61: vk = VK_OEM_PLUS; break; // =
	case 62: vk = VK_OEM_PERIOD; shift = true; break; // >
	case 63: vk = VK_OEM_2; shift = true; break; // ?
	case 64: vk = 50; shift = true; break; // @
	case 91: vk = VK_OEM_4; break; // [
	case 92: vk = VK_OEM_5; break; /* \ */
	case 93: vk = VK_OEM_6; break; // ]
	case 94: vk = 54; shift = true; break; // ^
	case 95: vk = VK_OEM_MINUS; shift = true; break; // _
	case 96: vk = VK_OEM_3; break; // `
	case 123: vk = VK_OEM_4; shift = true; break; // {
	case 124: vk = VK_OEM_5; shift = true; break; // |
	case 125: vk = VK_OEM_6; shift = true; break; // }
	case 126: vk = VK_OEM_3; shift = true; break; // ~
	default: break;
	}

	if (shift) // press the shift key
	{
		// Press shift key
		SpecialKeyDown(VK_SHIFT);
	}

	/* Generate a "key down" */
	if (bExtended) { kb.dwFlags = KEYEVENTF_EXTENDEDKEY; }
	else kb.dwFlags = 0;
	kb.wVk = vk;
	Input.type = INPUT_KEYBOARD;
	Input.ki = kb;

	::SendInput(1, &Input, sizeof(Input));

	std::this_thread::sleep_for(std::chrono::milliseconds(FAST_REACTION_TIME));

	/* Generate a "key up" */
	ZeroMemory(&kb, sizeof(KEYBDINPUT));
	ZeroMemory(&Input, sizeof(INPUT));
	kb.dwFlags = KEYEVENTF_KEYUP;
	if (bExtended) { kb.dwFlags |= KEYEVENTF_EXTENDEDKEY; }
	kb.wVk = vk;
	Input.type = INPUT_KEYBOARD;
	Input.ki = kb;
	::SendInput(1, &Input, sizeof(Input));

	if (shift) //release shift key if pressed
	{
		// Release shift key
		SpecialKeyUp(VK_SHIFT);
	}

	return;
}

//http://www.philipstorr.id.au/pcbook/book3/scancode.htm
void Keyboard::GenerateKeyScancode(int vk, bool shift)
{
	KEYBDINPUT  kb = { 0 };
	INPUT       Input = { 0 };

	//todo shift work with scancodes
	//if (shift) // press the shift key
	//{
	//	// Press shift key
	//	SpecialKeyDown(VK_SHIFT);

	//}

	/* Generate a "key down" */
	kb.dwFlags = KEYEVENTF_SCANCODE;
	kb.wVk = 0;
	kb.wScan = vk;
	kb.time = 0;
	kb.dwExtraInfo = 0;
	Input.type = INPUT_KEYBOARD;
	Input.ki = kb;

	::SendInput(1, &Input, sizeof(Input));

	std::this_thread::sleep_for(std::chrono::milliseconds(RandomInt(50, 70)));

	/* Generate a "key up" */
	ZeroMemory(&kb, sizeof(KEYBDINPUT));
	ZeroMemory(&Input, sizeof(INPUT));
	kb.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
	kb.wVk = 0;
	kb.wScan = vk;
	Input.type = INPUT_KEYBOARD;
	Input.ki = kb;
	::SendInput(1, &Input, sizeof(Input));

	//if (shift) //release shift key if pressed
	//{
	//	// Release shift key
	//	SpecialKeyUp(VK_SHIFT);

	//}

	return;
}

void Keyboard::Type(std::string phrase)
{
	bool Uppercase = false;
	char letter;
	size_t length = phrase.length();
	for (int i = 0; i < length; i++)
	{
		if ((phrase[i] >= 65) && (phrase[i] <= 90)) //if in the uppercase range
			Uppercase = true;
		else
			Uppercase = false;

		letter = phrase[i];

		GenerateKey(toupper(letter), false, Uppercase);
		std::this_thread::sleep_for(std::chrono::milliseconds(RandomInt(50, 100)));
	}
	//LOG("Typed:", phrase);
	return;
}

//	1 = left
//	2 = right
//	3 = up
//	4 = down
//not working not sure why
void Keyboard::Arrow(int arrow)
{
	std::string sarrow = "";
	switch (arrow)
	{
		//left
	case LEFT:	SpecialKeyDown(VK_LEFT); sarrow = "left"; break;
		//right
	case RIGHT:	SpecialKeyDown(VK_RIGHT); sarrow = "right"; break;
		//up
	case UP:	SpecialKeyDown(VK_UP); sarrow = "up"; break;
		//down
	case DOWN:	SpecialKeyDown(VK_DOWN); sarrow = "down"; break;
	}
	//LOG("Pressed:", sarrow, "arrow");
}

void Keyboard::HitSpecialKey(int vk)
{
	SpecialKeyDown(vk);
	std::this_thread::sleep_for(std::chrono::milliseconds(FAST_REACTION_TIME));
	SpecialKeyUp(vk);
	//LOG("Pressed ", vk);
	return;
}

void Keyboard::TypeNum(int number)
{
	Type(std::to_string(number));
}

void Keyboard::HitKey(int vk)
{
	GenerateKey(vk, false, false);
	//LOG("Pressed:", vk);
}

double Keyboard::GetExecutionTime()
{
	QueryPerformanceCounter(&endCount);

	return(endCount.QuadPart - startCount.QuadPart) * 1000.0 / frequency.QuadPart;
}

Keyboard* keyboard = new Keyboard();