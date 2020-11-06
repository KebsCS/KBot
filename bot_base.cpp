#include "DirectX.h"

#include "KInterface.h"
#include "offsets.h"
#include "ObjectManager.h"

LPCWSTR overlayClassName = L"ovrl"; // overlay window name

Utils util;

HWND hWnd;
HWND tWnd;
MSG Msg;

Direct3D9Render Direct3D9;

extern ConsoleLog clog;
bool ExitBot = false;

const MARGINS Margin = { 0, 0, SCREENWIDTH, SCREENHEIGHT };

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);



// Step 4: the Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_CREATE:
        DwmExtendFrameIntoClientArea(hWnd, &Margin);
        break;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
        
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}




int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

    WNDCLASSEX wc;
    //Registering the Window Class
    wc.cbSize = sizeof(WNDCLASSEX); //
    wc.style = CS_VREDRAW | CS_HREDRAW;
    wc.lpfnWndProc = WndProc; //
    wc.cbClsExtra = 0; // 
    wc.cbWndExtra = 0; // 
    wc.hInstance = hInstance; // 
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); // 
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION); // 
    wc.hCursor = LoadCursor(NULL, IDC_ARROW); // 
    wc.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0)); // 
    wc.lpszMenuName = overlayClassName; //
    wc.lpszClassName = overlayClassName; //



    if (!RegisterClassEx(&wc))
    {
       // clog.AddLog("[error] Window Registration Failed");
        MessageBox(0, L" Window Registration Failed", 0, 0);
        ::UnregisterClass(wc.lpszClassName, wc.hInstance);
        return 0;
    }


    if (FindWindow(0, L"League of Legends (TM) Client"))
    {
       // clog.AddLog("Window Found");

        hWnd = CreateWindowEx(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED, overlayClassName, overlayClassName, WS_POPUP, 1, 1, SCREENWIDTH, SCREENHEIGHT, 0, 0, 0, 0);
        SetLayeredWindowAttributes(hWnd, 0, 0, LWA_ALPHA);
        SetLayeredWindowAttributes(hWnd, 0, 0, LWA_COLORKEY);
        //SetLayeredWindowAttributes(hWnd, RGB(255, 255, 255),100, /* 0=transparent, 255=completely solid*/LWA_COLORKEY);
        //SetWindowPos(hWnd, HWND_TOPMOST, 0 ,0 , SCREENWIDTH, SCREENHEIGHT, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
        //ShowWindow(hWnd, SW_SHOW);

    }
    else
    {
       // clog.AddLog("[error] Window Creation Failed");
        MessageBox(0, L" Window Creation Failed", 0, 0);
        ::UnregisterClass(wc.lpszClassName, wc.hInstance);
        return 0;
    }


   
    Sleep(1000);

    // Initialize Direct3D
    //Direct3D9.DirectXInit(hWnd);
    if (!Direct3D9.DirectXInit(hWnd))
    {
        Direct3D9.Shutdown();
        ::UnregisterClass(wc.lpszClassName, wc.hInstance);
        return 0;
    }

    ::ShowWindow(hWnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hWnd);

    

    Config->Load();
    clog.AddLog("[startup] Config loaded");


    clog.AddLog("ProcessId: %d", Memory.ProcessID);
    clog.AddLog("ClientAddress: %x", ClientAddress);

    DWORD LocalPlayer = Memory.Read<DWORD>(ClientAddress + oLocalPlayer, sizeof(DWORD));
    CObject Local(LocalPlayer);
    float GameTime = Memory.Read<float>(ClientAddress + oGameTime, sizeof(float));


    if (GameTime > 0) // if in game
    {
        

        clog.AddLog("LocalPlayer: %x", LocalPlayer);
        clog.AddLog("Name: %s", Local.GetName().c_str());
        clog.AddLog("IsVisible: %d", Local.IsVisible());
        clog.AddLog("GetTeam: %d", Local.GetTeam());
        clog.AddLog("HP: %f", Local.GetHealth());
        clog.AddLog("MaxHP: %f", Local.GetMaxHealth());
        clog.AddLog("Mana: %f", Local.GetMana());
        clog.AddLog("MaxMana: %f", Local.GetMaxMana());
        clog.AddLog("Armor: %f", Local.GetArmor());
        clog.AddLog("MR: %f", Local.GetMR());
        clog.AddLog("MS: %f", Local.GetMS());
        clog.AddLog("BaseAD: %f", Local.GetBaseAD());
        clog.AddLog("BonusAD: %f", Local.GetBonusAD());
        clog.AddLog("AP: %f", Local.GetAP());
        clog.AddLog("AARange: %f", Local.GetAARange());
        clog.AddLog("ChampName: %s", Local.GetChampName().c_str());
        clog.AddLog("SummonerSpell1: %s", Local.SummonerSpell1().c_str());
        clog.AddLog("SummonerSpell2: %s", Local.SummonerSpell2().c_str());
        clog.AddLog("KeystoneName: %s", Local.KeystoneName().c_str());
        clog.AddLog("GetLevel: %i", Local.GetLevel());


    }
    DWORD Obj = Memory.Read<DWORD>(ClientAddress + oObjManager, sizeof(DWORD));

    DWORD ObjectArray = Memory.Read<DWORD>(Obj + 0x20, sizeof(DWORD));

    //message Loop
    ZeroMemory(&Msg, sizeof(Msg));
    while (Msg.message != WM_QUIT)
    {
        if (PeekMessage(&Msg, hWnd, 0, 0, PM_REMOVE))
        {
            ::DispatchMessage(&Msg);
            ::TranslateMessage(&Msg);
            continue;
        }
        Direct3D9.Render();

        if (ExitBot)
            break;

        if (GetAsyncKeyState(VK_INSERT) & 0x8000) //open menu
        {
            long winlong = GetWindowLong(hWnd, GWL_EXSTYLE);
            if (!MenuOpen)
            {
                MenuOpen = !MenuOpen;
                if (winlong != (WS_EX_LAYERED | WS_EX_TOPMOST))
                    SetWindowLong(hWnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TOPMOST);
                SetForegroundWindow(hWnd);
                //clog.AddLog("Menu Opened");


            }
            else
            {
                MenuOpen = !MenuOpen;
                if (winlong != (WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TRANSPARENT))
                    SetWindowLong(hWnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TRANSPARENT);
                //clog.AddLog("Menu Closed");

            }
            while (GetAsyncKeyState(VK_INSERT) & 0x8000) { Sleep(1); }
        }
        if (GetAsyncKeyState(VK_F11) & 0x8000) //exit button
        {
            break;
        }
        //GameTime = Memory.Read<float>(ClientAddress + oGameTime, sizeof(float));


  /*      DWORD MinionList = Memory.Read<DWORD>(ClientAddress + oHeroList);
        DWORD MinionArray = Memory.Read<DWORD>(MinionList + 0x04);
        int MinionLength = Memory.Read<int>(MinionList + 0x08);
        for (int i = 0; i < MinionLength * 4; i += 4)
        {

            CObject minion(Memory.Read<DWORD>(MinionArray + i));
            clog.AddLog("%s , %x , %i", minion.GetName().c_str(), minion.Address(), Memory.Read<bool>(minion.Address()+ oIsHero));

        
        }*/

    //DWORD Obj = Memory.Read<DWORD>(ClientAddress + oObjManager, sizeof(DWORD));

    //DWORD ObjectArray = Memory.Read<DWORD>(Obj + 0x20, sizeof(DWORD));
    //for (int i = 0; i < 1000; i++) 
    //{
    //    CObject obj(Memory.Read<DWORD>(ObjectArray + (0x4 * i), sizeof(DWORD)));
    //    int aa = Memory.Read<int>(obj.Address() + 0x1ADAB0,4);
    //    //if(aa==1)
    //    clog.AddLog("%s , %x , %i", obj.GetName().c_str(), obj.Address(),aa);
    //}

        //clog.AddLog("%x", Local.GetSpellByID(0));

        Sleep(1);
    }

    Config->Save();
    Direct3D9.Shutdown();
    ::DestroyWindow(hWnd);
    ::UnregisterClass(wc.lpszClassName, wc.hInstance);
    return 0;
}
