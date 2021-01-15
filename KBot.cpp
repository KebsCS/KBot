//#include "Orbwalker.h"

#define _WINSOCKAPI_

#include "DirectX.h"
#include "API.h"
#include "IRC.h"
#include "KBot.h"

HWND hWnd;

Direct3D9Render Direct3D9;

ConsoleLog clog;

const MARGINS Margin = { 0, SCREENWIDTH, 0, SCREENHEIGHT };

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Step 4: the Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_CREATE:
		DwmExtendFrameIntoClientArea(hwnd, &Margin);
		break;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
		break;
	case WM_DESTROY:
		M.ExitBot = true;
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

DWORD GetFirst()
{
	DWORD objManager = OBJManager;
	int v1; // eax
	int v2; // edx

	v1 = OBJManagerArray;
	v2 = Memory.Read<int>(objManager + 0x18);
	if (v1 == v2)
		return 0;
	while (Memory.Read<BYTE>(v1) & 1 || !Memory.Read<DWORD>(v1)) // never enters loop
	{
		v1 += 4;
		if (v1 == v2)
			return 0;
	}
	return Memory.Read<DWORD>(v1); // returns first address in objlist
}

DWORD GetNext(DWORD a2)
{
	int v2; // eax
	uint16_t v3; // edx
	uint16_t v4; // esi
	int v5; // eax

	v2 = OBJManagerArray;
	v3 = Memory.Read<uint16_t>(a2 + 0x20) + 1; // loops from first to last object index - 1 to a1a
	v4 = 0xbb8;//(Memory.Read<int>(objManager + 0x18) - v2) >> 2; // always bb8 (max possible amount of objects maybe?)
	if (v3 >= v4)
		return 0;
	v5 = v2 + 4 * v3; // every obj address before pointer
	while (Memory.Read<BYTE>(v5) & 1 || !Memory.Read<DWORD>(v5))
	{
		++v3;
		v5 += 4;
		if (v3 >= v4)
			return 0;
	}
	DWORD temp = Memory.Read<DWORD>(v5);
	// CObject xd(temp);
   //  clog.AddLog("%x , %x , %s", v5, temp, xd.GetName().c_str());
	return temp; // object address pointer
}

std::vector<CObject>minionList;
void MinionListLoop()
{
	DWORD MinionList = Memory.Read<DWORD>(ClientAddress + oMinionList);
	while (true)
	{
		std::vector<CObject>currobjList;
		DWORD MinionArray = Memory.Read<DWORD>(MinionList + 0x4);
		int MinionArrayLength = Memory.Read<int>(MinionList + 0x8);
		for (int i = 0; i < MinionArrayLength * 4; i += 4)
		{
			CObject obj(Memory.Read<DWORD>(MinionArray + i));
			obj.SetObjConsts();
			// clog.AddLog("%s %x %i", obj.GetName().c_str(), obj.Address(), obj.GetAlive());

			currobjList.emplace_back(obj);
		}
		if (!currobjList.empty())
			minionList = currobjList;
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}

void OrbwalkThread()
{
	while (true)
	{
		if (M.Orbwalker.Master)
		{
			if (PressedKey(M.Orbwalker.HoldKey))
			{
				//orbwalk->Move();
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}

void MenuHandler()
{
	while (true)
	{
		if (PressedKey(M.Misc.MenuKey)) //open menu
		{
			long winlong = GetWindowLong(hWnd, GWL_EXSTYLE);
			if (!M.MenuOpen)
			{
				M.MenuOpen = !M.MenuOpen;
				if (winlong != (WS_EX_LAYERED | WS_EX_TOPMOST))
					SetWindowLong(hWnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TOPMOST);
				SetForegroundWindow(hWnd);
				//clog.AddLog("Menu Opened");
			}
			else
			{
				M.MenuOpen = !M.MenuOpen;
				if (winlong != (WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TRANSPARENT))
					SetWindowLong(hWnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TRANSPARENT);
				//clog.AddLog("Menu Closed");
			}

#ifndef NOLEAGUE

			if (!FindWindowA(0, XorStr("League of Legends (TM) Client")))
			{
				M.ExitBot = true;
				break;
			}

#endif // !NOLEAGUE
			while (PressedKey(M.Misc.MenuKey)) {
				CheckHWID();
				CheckWindows();
				AntiDebug();
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

void Secure()
{
	while (true)
	{
		AntiDebug();
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEXA wc;
	//Registering the Window Class
	ZeroMemory(&wc, sizeof(WNDCLASSEXA));
	wc.cbSize = sizeof(WNDCLASSEXA); //
	wc.style = /*CS_VREDRAW | CS_HREDRAW | */ CS_NOCLOSE;
	wc.lpfnWndProc = WndProc; //
	wc.cbClsExtra = 0; //
	wc.cbWndExtra = 0; //
	wc.hInstance = hInstance; //
	wc.hIcon = 0;// LoadIconA(NULL, IDI_APPLICATION);
	wc.hIconSm = 0;// LoadIconA(NULL, IDI_APPLICATION); //
	wc.hCursor = 0;// LoadCursorA(NULL, IDC_ARROW); //
	wc.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0)); //

	srand(time(0));

	if (dwNtGlobalFlag & NT_GLOBAL_FLAG_DEBUGGED)
	{
		M.ExitBot = true;
		abort();
	}

	std::string className = utils->RandomString(RandomInt(5, 10));
	LPCSTR overlayClassName = className.c_str();

	InitAntiDebug();

	wc.lpszMenuName = overlayClassName; //
	wc.lpszClassName = overlayClassName; //

	{
		char szExeFileName[MAX_PATH];
		GetModuleFileNameA(NULL, szExeFileName, MAX_PATH);
		std::string path = std::string(szExeFileName);
		std::string exe = path.substr(path.find_last_of("\\") + 1, path.size());
		std::string newname;
		newname = utils->RandomString(RandomInt(5, 10));
		newname += XorStr(".exe");
		rename(exe.c_str(), newname.c_str());
	}

	if (!::RegisterClassExA(&wc))
	{
		MessageBoxA(0, XorStr("Window Registration Failed"), std::to_string(GetLastError()).c_str(), 0);
		::UnregisterClassA(wc.lpszClassName, wc.hInstance);
		return 0;
	}

	{
		std::string htmlData = api->GET(XorStr("https://raw.githubusercontent.com"), XorStr("/y3541599/test/main/README.md"));
		size_t nPos = htmlData.find(XorStr("#Version#")) + strlen(XorStr("#Version#"));
		if (nPos == std::string::npos)
		{
			M.ExitBot = true;
			::UnregisterClassA(wc.lpszClassName, wc.hInstance);
			return 0;
		}
		std::string NowVersion = htmlData.substr(nPos, htmlData.find(XorStr("$Version$"), nPos) - nPos);
		nPos = htmlData.find(XorStr("#Patch#")) + strlen(XorStr("#Patch#"));
		if (nPos == std::string::npos)
		{
			M.ExitBot = true;
			::UnregisterClassA(wc.lpszClassName, wc.hInstance);
			return 0;
		}
		std::string NowPatch = htmlData.substr(nPos, htmlData.find(XorStr("$Patch$"), nPos) - nPos);

		if (NowVersion != XorStr("1.0.0") || NowPatch != XorStr("11.1"))
		{
			MessageBoxA(0, XorStr("Outdated version"), 0, 0);
			M.ExitBot = true;
			::UnregisterClassA(wc.lpszClassName, wc.hInstance);
			return 0;
		}

		nPos = htmlData.find(XorStr("#Info#")) + strlen(XorStr("#Info#"));
		if (nPos == std::string::npos)
		{
			M.ExitBot = true;
			::UnregisterClassA(wc.lpszClassName, wc.hInstance);
			return 0;
		}
		std::string ServerInfo = htmlData.substr(nPos, htmlData.find(XorStr("$Info$"), nPos) - nPos);

		if (!ServerInfo.empty())
		{
			clog.AddLog(XorStr("[info] %s"), ServerInfo.c_str());
			M.ServerInfo = ServerInfo;
		}
		clog.AddLog(XorStr("[start] Version: %s"), NowVersion.c_str());
		clog.AddLog(XorStr("[start] Patch: %s"), NowPatch.c_str());

		//htmlData = api->GET(XorStr("https://24timezones.com"), XorStr("/time-zone/cet"));
		//nPos = htmlData.find(XorStr(" January ")) + strlen(XorStr(" January "));
		//if (nPos == std::string::npos)
		//{
		//    M.ExitBot = true;
		//    ::UnregisterClassA(wc.lpszClassName, wc.hInstance);
		//    return 0;

		//}
		//std::string Today = htmlData.substr(nPos, htmlData.find(XorStr(", 2021</p>"), nPos) - nPos).c_str();
		//try
		//{
		//    if (std::stoi(Today) > 31) //end of month
		//    {
		//        M.ExitBot = true;
		//        ::UnregisterClassA(wc.lpszClassName, wc.hInstance);
		//        return 0;

		//    }
		//}
		//catch (...)
		//{
		//    M.ExitBot = true;
		//    ::UnregisterClassA(wc.lpszClassName, wc.hInstance);
		//    return 0;
		//}
	}

	//todo fix failed to connect
	/*std::thread IRCThread(&IRC::Run, irc);
	IRCThread.detach();*/

#ifndef NOLEAGUE

	LPCSTR windowName = XorStr("League of Legends (TM) Client");
	if (!::FindWindowA(0, windowName))
	{
		MessageBoxA(0, XorStr("Game not found"), XorStr("Window creation failed"), 0);
		M.ExitBot = true;
		::UnregisterClassA(wc.lpszClassName, wc.hInstance);
		return 0;
	}

	if (!Memory.Process())
	{
		MessageBoxA(0, XorStr("Driver not found"), 0, 0);
		M.ExitBot = true;
		::UnregisterClassA(wc.lpszClassName, wc.hInstance);
		return 0;
	}

#endif // !NOLEAGUE

	hWnd = ::CreateWindowExA(WS_EX_TOPMOST | WS_EX_LAYERED, overlayClassName, overlayClassName, WS_POPUP, 0, 0, SCREENWIDTH, SCREENHEIGHT, 0, 0, wc.hInstance, 0);
	SetLayeredWindowAttributes(hWnd, 0, 0, LWA_ALPHA);
	SetLayeredWindowAttributes(hWnd, 0, 0, LWA_COLORKEY);
	//SetLayeredWindowAttributes(hWnd, RGB(255, 255, 255),100, /* 0=transparent, 255=completely solid*/LWA_COLORKEY);
	//SetWindowPos(hWnd, HWND_TOPMOST, 0 ,0 , SCREENWIDTH, SCREENHEIGHT, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	//ShowWindow(hWnd, SW_SHOW);

	Config->Setup();
	Config->Load(XorStr("default"));
	clog.AddLog(XorStr("[start] Config loaded"));

	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	// Initialize Direct3D
	if (!Direct3D9.DirectXInit(hWnd))
	{
		Direct3D9.Shutdown();
		::UnregisterClassA(wc.lpszClassName, wc.hInstance);
		return 0;
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(200));
	AntiDebug();
	::ShowWindow(hWnd, SW_SHOWDEFAULT);
	::UpdateWindow(hWnd);

#ifndef NOLEAGUE

	std::thread MinionListThread{ MinionListLoop };
	MinionListThread.detach();

#endif // !NOLEAGUE

	std::thread MenuHandlerThread{ MenuHandler };
	MenuHandlerThread.detach();

	// CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ObjListLoop, 0, 0, 0); //todo loop obj only when needed, not in different thread and c++11 threads
	 //CreateThread(0, 0, (LPTHREAD_START_ROUTINE)OrbwalkThread, 0, 0, 0);
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	for (char a = 65; a <= 90; a++)
	{
		std::string sm = ":";
		std::string temp = "\\\\.\\";
		std::string final = temp + a + sm;

		HANDLE hCdRom = CreateFile(utils->StringToWstring(final).c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
		if (hCdRom == INVALID_HANDLE_VALUE)
		{
			//clog.AddLog("Error: %x", GetLastError());
			continue;
		}
		else
		{
			//DWORD dwBytes;
			// Open the door:
			//DeviceIoControl(hCdRom, IOCTL_STORAGE_EJECT_MEDIA, NULL, 0, NULL, 0, &dwBytes, NULL);

			//Sleep(1000);

			// Close the door:
			//DeviceIoControl(hCdRom, IOCTL_STORAGE_LOAD_MEDIA, NULL, 0, NULL, 0, &dwBytes, NULL);

			//clog.AddLog("cd drive: %s", final);

			CloseHandle(hCdRom);
			break;
		}
	}

	//DWORD a = utils->GetStringHash(utils->GetUserNameFunc() + utils->GetComputerNameFunc() + utils->GetGUID());

	CheckHWID();

	std::thread SecureThread{ Secure };
	SecureThread.detach();

	//message Loop
	MSG Msg;
	ZeroMemory(&Msg, sizeof(Msg));
	while (Msg.message != WM_QUIT)
	{
		if (::PeekMessage(&Msg, hWnd, 0, 0, PM_REMOVE))
		{
			::DispatchMessage(&Msg);
			::TranslateMessage(&Msg);
			continue;
		}
		//std::thread MissileThread{ GetObjectList };

		if (M.ExitBot || PressedKey(VK_F11))// || !FindWindowA(0, windowName)) //exit
			break;
#ifndef NOLEAGUE

		M.GameTime = Memory.Read<float>(ClientAddress + oGameTime, sizeof(float));

		if (championScript && PressedKey(VK_SPACE))
			championScript->Harass();

		//todo mem reads in cobfuscation i guess
		//clog.AddLog("aimgr: %x", (Memory.DeobfuscateMember(Local.Address() + 0x3018)));
		/*clog.AddLog("aimgr1: %x", Memory.DeobfuscateMember(Local.Address() + 0x3018));
		clog.AddLog("aimgr2: %x", Memory.DeobfuscateMember2(Local.Address() + 0x3018));*/

#endif // !NOLEAGUE

		Direct3D9.Render();

		/*	DWORD buffStart = Memory.Read<DWORD>(Local.Address() + 0x2160 + 0x10);
			DWORD buffEnd = Memory.Read<DWORD>(Local.Address() + 0x2160 + 0x14);

			for (DWORD buffPtr = buffStart; buffPtr != buffEnd; buffPtr += 0x04)
			{
				DWORD buffCurr = Memory.Read<DWORD>(buffPtr);
				DWORD temp = Memory.Read<DWORD>(buffCurr + 0x08);
				if (!temp)
					continue;
				clog.AddLog("%s %x", Memory.ReadString(temp +0x08, true).c_str(), buffCurr);
			}*/
			//clog.AddLog("%x",Memory.Read<DWORD>(Local.Address()+0x6c));

			//todo twisted fate cards bound separately+visual indicator

			//clog.AddLog("%i", GetObjectList().size());
		   // CObject undermouseObj(CObject::GetUnderMouseObject());
			//int xd = CObject::GetUnderMouseObject();
		   // DWORD address = Memory.Read<DWORD>(ClientAddress + oUnderMouseObject);
		  //  clog.AddLog("under mouse: %x", address);

			//clog.AddLog("%lg", GetBoundingRadius(Local.Address()));
			//int i = 0;
			//DWORD buffmgr = (LocalPlayer + 0x2174);
			//for (DWORD pBuffPtr = Memory.Read<DWORD>(buffmgr +0x10); pBuffPtr != Memory.Read<DWORD>(buffmgr + 0x14); pBuffPtr = Memory.Read<DWORD>(pBuffPtr + 0x8 *i))
			//{
			//    auto pBuff = Memory.Read<DWORD>(pBuffPtr);
			//    i++;
			//    if (!pBuff) continue;
			//    //if (!pBuff->IsValid()) continue;
			//    //if (pBuff->IsAlive()) {
			//    std::string buffname = Memory.ReadString(pBuff + 0x08);
			//    clog.AddLog("%s", buffname);
			//  //  }

			////}
			//Vector3 start = Vector3(11579, 51, 7013);
			//Vector3 end = Vector3(11802, 51, 7264);

			//clog.AddLog("Mouse: %f, %f, %f", mouse->GetMouseWorldPosition().X, mouse->GetMouseWorldPosition().Y, mouse->GetMouseWorldPosition().Z);
			//clog.AddLog("Player: %f, %f, %f", Local.GetPosition().X, Local.GetPosition().Y, Local.GetPosition().Z);
			//clog.AddLog("Real Mouse: %d , %d", mouse->GetPos().x, mouse->GetPos().y);

			//MissileThread.join();

		std::this_thread::sleep_for(std::chrono::milliseconds(M.Misc.AntiLag));
	}
	Config->Save(XorStr("default"));
	WSACleanup();
	Direct3D9.Shutdown();
	::DestroyWindow(hWnd);
	::UnregisterClassA(wc.lpszClassName, wc.hInstance);
	return 0;
}