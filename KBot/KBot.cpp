﻿#include "DirectX.h"

#include <chrono>
#include "Utils.h"
#include "Config.h"

#include "Evade.h"

#include "Mouse.h"

#include "HTTP.h"
#include "LoLAPI.h"

#include "Evade2.h"

#include "Orbwalker.h"

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

HWND hwnd;
ConsoleLog clog;

Visuals* vis;

Direct3D9Render Direct3D9;

void Direct3D9Render::HeroLoop()
{
	//hero loop
	if (M.Cooldowns.Master || M.AARange.Master || M.Tracers.Master || M.GankAlerter.Master || M.Talon.DmgCalc || M.Evade.Master)
	{
		for (auto& obj : init->herolist)
		{
			if (M.Cooldowns.Master)
			{
				//todo store cooldowns in object class
				if (M.Cooldowns.Type[0] || M.Cooldowns.Type[1])
					vis->CooldownTimers(obj);
				//if master + tab /  scoreboard window + show scoreboard
				if ((M.Cooldowns.Type[2] && ((PressedKey(VK_TAB))) || (M.Cooldowns.ScoreboardWnd && M.Cooldowns.Scoreboard.ShowScoreboard)))
					vis->ScoreBoard(obj);
			}

			if (M.AARange.Master)
				vis->DrawAARanges(obj, M.AARange.Slider[0], M.AARange.Slider[1] / 10.f, RGBA(M.AARange.Color[0], M.AARange.Color[1], M.AARange.Color[2], M.AARange.Color[3]),
					false, RGBA(M.AARange.LocalColor[0], M.AARange.LocalColor[1], M.AARange.LocalColor[2], M.AARange.LocalColor[3]), false);

			if (M.Tracers.Master)
				vis->DrawTracers(obj, M.Tracers.Thickness);

			if (M.GankAlerter.Master)
				vis->GankAlerter(obj);

			if (M.Talon.Master)
			{
				if (M.Talon.DmgCalc)
					vis->TalonDamageCalc(obj);
			}

			if (M.Enemies.MouseClicks)
			{
				if (!obj.IsDead() && obj.GetTeam() != Local.GetTeam())
				{
					ImVec2 Pos = WorldToMinimap(obj.GetPosition());
					draw->String(obj.GetChampName().substr(0, 1), Pos.x - 4, Pos.y, RGBA(255, 255, 255));
				}
				if (obj.IsVisible() && !obj.IsDead() && obj.CheckState(CanMove) && (obj.GetTeam() != Local.GetTeam()))
				{
					Vector3 clickPos = obj.GetAiManager()->GetNavEnd();
					if (!clickPos.IsZero())
					{
						ImVec2 RealClickPos = WorldToScreen(clickPos);
						if (draw->IsOnScreen(RealClickPos))
						{
							if (clickPos.Distance(obj.GetPosition()) > 10.f)
							{
								draw->CircleRange(clickPos, 16, 65, RGBA(COLOR_AQUA));
								draw->String(obj.GetChampName(), RealClickPos, RGBA(COLOR_AQUA));
							}
						}
					}
				}
			}

			//clog.AddLog("%s %i", obj.GetChampName().c_str(), obj.GetNetworkID());

			//float dist = obj.GetDistToMe(Local);
			//clog.AddLog("%s , %f", obj.GetName(), dist);
			//float dmg = Local.GetTotalDamage(&obj);
			//ImVec2 RealPos = Direct3D9.WorldToScreen(obj.GetPosition());
			//std::string str = obj.GetName() + " , " + std::to_string(dmg);
			//draw->String(str, RealPos.x, RealPos.y, centered, RGBA(255, 255, 255), fontTahoma);
		}
	}
}

void Direct3D9Render::TurretLoop()
{
	//turret loop
	if (M.AARange.Turrets)
	{
		for (auto& obj : init->turretlist)
		{
			if (M.AARange.Turrets)
				vis->DrawAARanges(obj, M.AARange.Slider[0], M.AARange.Slider[1] / 10.f, RGBA(M.AARange.Color[0], M.AARange.Color[1], M.AARange.Color[2], M.AARange.Color[3]),
					false, RGBA(M.AARange.LocalColor[0], M.AARange.LocalColor[1], M.AARange.LocalColor[2], M.AARange.LocalColor[3]), true);

			//ImVec2 RealPos = Direct3D9.WorldToScreen(obj.GetPosition());
			//std::string str = obj.GetName() + " , " + std::to_string(obj.Address()) +" " + std::to_string(obj.GetPosition().X) + " " + std::to_string(obj.GetPosition().Y) + " " + std::to_string(obj.GetPosition().Z);
			//draw->String(str, RealPos.x, RealPos.y, centered, RGBA(255, 255, 255), fontTahoma);
		}
	}
}

void Direct3D9Render::Loops()
{
	//if (M.Debug)
	//MissileThread();

	if (M.AARange.Local)
		vis->DrawAARanges(Local, M.AARange.Slider[0], M.AARange.Slider[1] / 10.f, RGBA(M.AARange.Color[0], M.AARange.Color[1], M.AARange.Color[2], M.AARange.Color[3]),
			M.AARange.Local, RGBA(M.AARange.LocalColor[0], M.AARange.LocalColor[1], M.AARange.LocalColor[2], M.AARange.LocalColor[3]), false);

	//std::thread HeroLoopThread(&Direct3D9Render::HeroLoop, this);
	//std::thread TurretLoopThread(&Direct3D9Render::TurretLoop, this);
	HeroLoop();
	TurretLoop();

	//minions/monsters/wards loop
	if (M.Wards.Master || M.LastHit.Master || M.AutoSmite.Master)
	{
		for (auto& obj : g_MinionList)
		{
			//clog.AddLog("%s", obj.GetName().c_str());
			if (M.Wards.Master)
				vis->WardsRange(obj);

			if (M.AutoSmite.Master)
				vis->AutoSmite(obj, M.AutoSmite.Slot, M.AutoSmite.Mode, M.AutoSmite.MouseSpeed);

			if (M.LastHit.Master)
				vis->LastHit(obj, RGBA(M.LastHit.Color[0], M.LastHit.Color[1], M.LastHit.Color[2], M.LastHit.Color[3]));

			//	ImVec2 RealPos = Direct3D9.WorldToScreen(obj.GetPosition());
			//float dmg = Local.GetTotalDamage(obj);
			//std::string str = obj.GetName() + " , " + std::to_string(dmg) + " , " + std::to_string(obj.Address());
			//draw->String(str, RealPos.x, RealPos.y, centered, RGBA(255, 255, 255), fontTahoma);
		}
	}

	//inhib loop
	if (M.Inhibs.Master)
	{
		for (auto& obj : init->inhiblist)
		{
			if (M.Inhibs.Master)
			{
				vis->InhibTimers(obj);
			}
		}
	}

	//HeroLoopThread.join();
	//TurretLoopThread.join();
}

//UI open/close Loop
void MenuHandler()
{
	while (true)
	{
		if (PressedKey(M.Misc.MenuKey))
		{
			//Get window styles
			if (long lWinLong = GetWindowLong(hwnd, GWL_EXSTYLE); !M.bMenuOpen)
			{
#ifndef NOLEAGUE
				//If league or script isn't in foreground, don't open menu
			/*	if (!utils->IsLeagueForeGround() && !utils->IsScriptForeGround())
					continue;*/
#endif // !NOLEAGUE

					//Open the menu, and make the UI clickable
				M.bMenuOpen = !M.bMenuOpen;
				if (lWinLong != (WS_EX_LAYERED | WS_EX_TOPMOST))
					SetWindowLong(hwnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TOPMOST);
				SetForegroundWindow(hwnd);
				//clog.AddLog("Menu Opened");
			}
			else
			{
				//Close the menu, and make the UI pass through clicks, by setting the WS_EX_TRANSPARENT flag
				M.bMenuOpen = !M.bMenuOpen;
				if (lWinLong != (WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TRANSPARENT))
					SetWindowLong(hwnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TRANSPARENT);
				//clog.AddLog("Menu Closed");
			}

			while (PressedKey(M.Misc.MenuKey))
			{
				//These don't need to be in the main loop, or would take too much CPU power
#ifndef NOLEAGUE
				//If Lol is closed, exit
				if (!FindWindowA(0, XorStr("League of Legends (TM) Client")))
				{
					M.bExitBot = true;
					break;
				}

				//CheckHWID();
				//CheckWindows();
				//AntiDebug();

				//Get minimap position and size
				init->SetMinimapProperties();

				//Get League's window width and height (usually 1920x1080)
				init->SetRendererProperties();
#endif // !NOLEAGUE

				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			}
		}
		//Infinite loop sleep
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

//Draw game overlay (used for primitive rendering)
void DrawOverlayWindows(Draw& cdraw)
{
	auto io = ImGui::GetIO();
	ImGui::SetNextWindowSize(io.DisplaySize);
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::Begin("##Overlay", nullptr,
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoInputs |
		ImGuiWindowFlags_NoBackground
	);

	cdraw.overlay = ImGui::GetWindowDrawList();

	ImGui::End();
}

bool RenameExe()
{
	char szExeFileName[MAX_PATH];
	GetModuleFileNameA(NULL, szExeFileName, MAX_PATH);
	std::string path = std::string(szExeFileName);
	std::string exe = path.substr(path.find_last_of("\\") + 1, path.size());
	std::string newname;
	newname = utils->RandomString(RandomInt(5, 10));
	newname += XorStr(".exe");
	if (!rename(exe.c_str(), newname.c_str()))
		return true;
	else return false;
}

bool once123 = true;

std::vector<Vector3>navpath;
void DrawNav(CObject obj)
{
	navpath.clear();
	if (!obj.IsMoving())
		return;
	AiManager* aimgr = obj.GetAiManager();

	DWORD NaviStart = Memory.Read<DWORD>((DWORD)aimgr + 0x1BC);
	DWORD NaviEnd = Memory.Read<DWORD>((DWORD)aimgr + 0x1C0);
	DWORD NavSize = (NaviEnd - NaviStart) / 12;
	clog.AddLog("navsize %d", NavSize);
	if (NavSize > 1)
	{
		Vector3 MyStart = Local.GetPosition();
		ImVec2 screen_start_1 = Direct3D9.WorldToScreen(MyStart);

		Vector3 NavLineEnd = Memory.Read<Vector3>(NaviStart + 12);
		ImVec2 screen_end = Direct3D9.WorldToScreen(NavLineEnd);
		//draw->Line(screen_start_1, screen_end, RGBA(COLOR_WHITE));
		navpath.emplace_back(MyStart);
		navpath.emplace_back(NavLineEnd);

		if (NavSize > 2)
		{
			for (int i = 2; i < NavSize; i++)
			{
				Vector3 NavLineStart = Memory.Read<Vector3>(NaviStart + ((i - 1) * 12));
				ImVec2 screen_start = Direct3D9.WorldToScreen(NavLineStart);

				NavLineEnd = Memory.Read<Vector3>(NaviStart + i * 12);
				screen_end = Direct3D9.WorldToScreen(NavLineEnd);

				navpath.emplace_back(NavLineStart);
				navpath.emplace_back(NavLineEnd);
				//draw->Line(screen_start, screen_end, RGBA(COLOR_WHITE));
			}
		}
	}
}

// Main code
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//Randomize using current time, todo swap with recent c++ random
	srand(time(0));

	//Random class name, since some apps, check for known names.
	//League doesn't do that for this script, but just to be sure they don't implement that anytime soon
	std::string sClassName = utils->RandomString(RandomInt(5, 10));
	LPCSTR lpszOverlayClassName = sClassName.c_str();

	//Register window class information
	WNDCLASSEXA wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, sClassName.c_str(), NULL };

	// Rename exe to random string
	RenameExe();

	::RegisterClassExA(&wc);

	// Create application window
	hwnd = ::CreateWindowExA(
		WS_EX_TOPMOST /*| WS_EX_NOACTIVATE*/ | WS_EX_LAYERED | WS_EX_TRANSPARENT,
		sClassName.c_str(), lpszOverlayClassName,
		WS_POPUP,
		1, 1, SCREENWIDTH, SCREENHEIGHT,
		nullptr, nullptr, GetModuleHandle(0), nullptr);

	if (hwnd == NULL)
	{
		::UnregisterClassA(wc.lpszClassName, wc.hInstance);
		return 0;
	}
	HTTP http;
	std::string htmlData = http.GET(XorStr("https://raw.githubusercontent.com"), XorStr("/y3541599/test/main/README.md"));
	size_t nPos = htmlData.find(XorStr("#Version#")) + strlen(XorStr("#Version#"));
	if (nPos == std::string::npos)
	{
		M.bExitBot = true;
		::UnregisterClassA(wc.lpszClassName, wc.hInstance);
		return 0;
	}
	std::string NowVersion = htmlData.substr(nPos, htmlData.find(XorStr("$Version$"), nPos) - nPos);
	nPos = htmlData.find(XorStr("#Patch#")) + strlen(XorStr("#Patch#"));
	if (nPos == std::string::npos)
	{
		M.bExitBot = true;
		::UnregisterClassA(wc.lpszClassName, wc.hInstance);
		return 0;
	}

	std::string NowPatch = htmlData.substr(nPos, htmlData.find(XorStr("$Patch$"), nPos) - nPos);

	if (NowVersion != XorStr("1.0.0") || NowPatch != XorStr("11.10"))
	{
		MessageBoxA(0, XorStr("Outdated version"), 0, 0);
		M.bExitBot = true;
		::UnregisterClassA(wc.lpszClassName, wc.hInstance);
		return 0;
	}

	clog.AddLog(XorStr("[start] Version: %s"), NowVersion.c_str());
	clog.AddLog(XorStr("[start] Patch: %s"), NowPatch.c_str());

	// String version to number eg. 11.2 to 112
	NowPatch.erase(std::remove(NowPatch.begin(), NowPatch.end(), '.'), NowPatch.end());
	int nNowPatch = std::stoi(NowPatch);

	nPos = htmlData.find(XorStr("#Info#")) + strlen(XorStr("#Info#"));
	if (nPos == std::string::npos)
	{
		M.bExitBot = true;
		::UnregisterClassA(wc.lpszClassName, wc.hInstance);
		return 0;
	}
	std::string ServerInfo = htmlData.substr(nPos, htmlData.find(XorStr("$Info$"), nPos) - nPos);

	if (!ServerInfo.empty())
	{
		clog.AddLog(XorStr("[info] %s"), ServerInfo.c_str());
		M.dwStartTime = GetTickCount();
		M.sServerInfo = ServerInfo;
	}

#ifndef NOLEAGUE

	//Find if league is running
	LPCSTR lpszWindowName = XorStr("League of Legends (TM) Client");
	if (!::FindWindowA(0, lpszWindowName))
	{
		MessageBoxA(0, XorStr("Game not found"), XorStr("Window creation failed"), 0);
		M.bExitBot = true;
		::UnregisterClassA(wc.lpszClassName, wc.hInstance);
		return 0;
	}

	//If driver isn't running, or there's problem with it we shouldn't be able to get PID
	if (!Memory.Process())
	{
		MessageBoxA(0, XorStr("Driver not found"), 0, 0);
		M.bExitBot = true;
		::UnregisterClassA(wc.lpszClassName, wc.hInstance);
		return 0;
	}

	LoLAPI::LoadPlayerListData();

	//Load default config
	Config->Setup();
	Config->Load(XorStr("default"));
	clog.AddLog(XorStr("[start] Config loaded"));

	std::string sDataPath("Data");
	if (nNowPatch != M.nPatch)
	{
		// delete files so they redownload
		std::filesystem::remove_all(sDataPath);

		//update version
		M.nPatch = nNowPatch;
		//save version
		Config->Save(XorStr("default"));
	}

	//Load static info from files
	if (!GameData::Load(sDataPath))
	{
		MessageBoxA(0, XorStr("Failed to load/download files"), 0, 0);
		M.bExitBot = 1;
	}
	//todo turret/inhib size warning based on map
	/*LoLAPI::GetGameStats();
	std::vector<API::Event>Events = LoLAPI::GetEvents();*/
#endif // !NOLEAGUE

	//Initialize Direct3D
	if (!Direct3D9.DirectXInit(hwnd))
	{
		Direct3D9.Shutdown();
		::UnregisterClassA(wc.lpszClassName, wc.hInstance);
		return 0;
	}

	// Show the window
	::ShowWindow(hwnd, SW_SHOWDEFAULT);
	::UpdateWindow(hwnd);

	//Start the thread that opens/closes UI
	std::thread MenuHandlerThread{ MenuHandler };
	MenuHandlerThread.detach();

	//Start minion list loop
	std::thread MinionListThread{ MinionListLoop };
	MinionListThread.detach();

	if (M.bDebug)
		init->StartupInfo();

	//Evade* evade = new Evade();
	////evade->MakeWorldMap();
	//evade->InitSpells();
	//evade->InitEvadeSpells();

	Evade2 evade2;
	Orbwalker orb;

	// Main loop
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		// Poll and handle messages (inputs, window resize, etc.)
		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
		// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
		// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
		// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
		if (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
			continue;
		}

		//If script is supposed to exit or F11 pressed, exit
		if (M.bExitBot || PressedKey(VK_F11))// || !FindWindowA(0, windowName)) //exit
			break;

		//Start rendering
		Direct3D9.StartFrame();

		//Render UI
		Direct3D9.Render();

		//Draw contents
		DrawOverlayWindows(*draw);

		//Update Global timer
		M.fGameTime = Memory.Read<float>(ClientAddress + Offsets::oGameTime, sizeof(float));
		try {
			//Update ViewMatrix
			Direct3D9.GetViewProjectionMatrix();

			//Loop through lists
			Direct3D9.Loops();

			//Execute champion scripts
			if (championScript)
				championScript->Tick();

			/*if (M.Evade.Master)
			{
				evade->Tick();
				evade->Draw();
				if (M.bMenuOpen)
					evade->GUI();
			}*/
			/*	DrawNav(Local);
				for (auto a : navpath)
				{
					draw->CircleRange(a, 16, 16, RGBA(COLOR_CYAN));
				}*/

			if (M.Orbwalker.Master)
				orb.Tick();

			evade2.Tick();
			if (M.bMenuOpen)
				evade2.GUI();
		}
		catch (const std::bad_alloc&) {/*xd*/ }
		/*	for (int i = 0; i < 100; i++)
			{
				int ab = 1 << i;
				clog.AddLog("i:%d state:%d", i, Local.CheckState(ab));
			}*/
			//Vector3 mpos = mouse->GetWorldPos();

			//if (PressedKey(VK_XBUTTON2))
			//{
			//	std::string temp = ".Add(Vector3(" + std::to_string(mpos.x) + ", " + std::to_string(mpos.y) + ", " + std::to_string(mpos.z) + "));";
			//	//std::string temp = "Vector3(" + std::to_string(mpos.x) + ", " + std::to_string(mpos.y) + ", " + std::to_string(mpos.z) + ")";
			//	utils->CopyToClipboard(temp);
			//}

			//for (auto& poly : WorldMap)
			//{
			//	if (poly.IsInside(mpos))
			//		draw->Polygon(poly, RGBA(255, 0, 0));
			//	else
			//		draw->Polygon(poly, RGBA(255, 255, 255));
			//}

		/*	DWORD lasterror = GetLastError();
			if (lasterror != 0)
				MessageBoxA(0, std::to_string(lasterror).c_str(), 0, 0);*/

				//if (once123)
				//once123 = false;

		{
			//DrawNav(Local);
			//ImVec2 PathStart = Direct3D9.WorldToScreen(Local.GetAiManager()->GetNavBegin());
			//ImVec2 PathEnd = Direct3D9.WorldToScreen(Local.GetAiManager()->GetNavEnd());
			//draw->Circle(PathStart, 30, RGBA(COLOR_RED));
			//draw->Circle(PathEnd, 30, RGBA(COLOR_BLUE));
			//draw->Line(PathStart, PathEnd, RGBA(COLOR_WHITE));
		}

		//End rendering
		Direct3D9.EndFrame();
	}

	//Save config to default
	Config->Save(XorStr("default"));

	// Cleanup
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	//Exit
	Direct3D9.Shutdown();
	::DestroyWindow(hwnd);
	::UnregisterClassA(wc.lpszClassName, wc.hInstance);

	return 0;
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_SIZE:
		if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
		{
			Direct3D9.CleanupRenderTarget();
			g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
			Direct3D9.CreateRenderTarget();
		}
		return 0;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
		break;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	}
	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}