#include "DirectX.h"

#include "KInterface.h"
#include "offsets.h"
#include "ObjectManager.h"
#include "SpellInfo.h"

LPCWSTR overlayClassName = L"ovrl"; // overlay window name

Utils util;

HWND hWnd;
HWND tWnd;
MSG Msg;

Direct3D9Render Direct3D9;
DWORD ProcessId;

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
        return 0;
    }

    tWnd = FindWindow(0, L"League of Legends (TM) Client");

    if (tWnd)
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
        return 0;
    }

    

   

    ::ShowWindow(hWnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hWnd);

    Sleep(1000);

    Direct3D9.DirectXInit(hWnd);// Initialize Direct3D

    ProcessId = Driver.GetProcessId();
    DWORD ClientAddress = Driver.GetClientModule();
    clog.AddLog("ProcessId: %d", ProcessId);
    clog.AddLog("ClientAddress: %x", ClientAddress);

    DWORD LocalPlayer = Driver.ReadVirtualMemory<DWORD>(ProcessId, ClientAddress + oLocalPlayer, sizeof(DWORD));
    CObject Local(Driver.ReadVirtualMemory<DWORD>(ProcessId, ClientAddress + oLocalPlayer, sizeof(DWORD)));
    float GameTime = Driver.ReadVirtualMemory<float>(ProcessId, ClientAddress + oGameTime, sizeof(float));

    DWORD Obj = Driver.ReadVirtualMemory<DWORD>(ProcessId, ClientAddress + oObjManager, sizeof(DWORD));
    
   

    if (GameTime > 0) // if in game
    {


        clog.AddLog("LocalPlayer: %x", LocalPlayer);
        clog.AddLog("Name: %s", Local.GetName());
        clog.AddLog("Index: %d", Local.GetIndex());
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
        clog.AddLog("ChampName: %s", Local.ChampName());
        clog.AddLog("SummonerSpell1: %s", Local.SummonerSpell1());
        clog.AddLog("SummonerSpell2: %s", Local.SummonerSpell2());
        clog.AddLog("KeystoneName: %s", Local.KeystoneName());
        clog.AddLog("GetLevel: %i", Local.GetLevel());


    }
      
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
                clog.AddLog("Menu Opened");


            }
            else
            {
                MenuOpen = !MenuOpen;
                if (winlong != (WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TRANSPARENT))
                    SetWindowLong(hWnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TRANSPARENT);
                clog.AddLog("Menu Closed");

            }
            while (GetAsyncKeyState(VK_INSERT) & 0x8000) {}
        }
        if (GetAsyncKeyState(VK_F11) & 0x8000) //exit button
        {
            break;
        }
        GameTime = Driver.ReadVirtualMemory<float>(ProcessId, ClientAddress + oGameTime, sizeof(float));




        DWORD ObjectArray = Driver.ReadVirtualMemory<DWORD>(ProcessId, Obj + 0x20, sizeof(DWORD));

        for (int i = 0; i < 500; i++)
        {
            CObject obj(Driver.ReadVirtualMemory<DWORD>(ProcessId, ObjectArray + (0x4 * i), sizeof(DWORD)));


           //100 - blue team, 200 - red team, 300 - neutral
            if (obj.GetTeam() == 100 || obj.GetTeam() == 200 || obj.GetTeam() == 300)
            {
                if (obj.IsHero() && obj.IsVisible() && obj.IsAlive() !=0 && obj.GetHealth()>0.f && obj.GetMS() >1.f && obj.GetBaseAD() > 1.f && obj.GetAARange()>1.f && obj.GetArmor()>1.f && obj.GetMaxHealth() >1.f && obj.GetName() != "" && obj.GetName() != " " &&obj.GetMR()>1.f )
                {


                    // if (obj.IsHero() && obj.IsAlive() && )
                   //  {

                         //clog.AddLog("%s : %f, %f, %f ", obj.GetName(), obj.GetPosition().X, obj.GetPosition().Y, obj.GetPosition().Z);
                    Vector3 Position = obj.GetPosition();
                    ImVec2 RealPos = Direct3D9.WorldToScreen(Position);

                    clog.AddLog("%s : %f, %f, %f", obj.GetName(), RealPos.x, RealPos.y, obj.GetBaseAD() );
                    //  }
                }
            }
        }

    

        Sleep(1);
    }
    Direct3D9.Shutdown();
    ::DestroyWindow(hWnd);
    ::UnregisterClass(wc.lpszClassName, wc.hInstance);
    return Msg.wParam;
}
