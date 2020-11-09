#include "DirectX.h"

#include "KInterface.h"
#include "offsets.h"
#include "ObjectManager.h"


LPCWSTR overlayClassName = L"ovrl"; // overlay window name

HWND hWnd;
HWND tWnd;
MSG Msg;

Direct3D9Render Direct3D9;

ConsoleLog clog;


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


std::list<CObject>wardList;

static DWORD ObjManager = Memory.Read<DWORD>(ClientAddress + oObjManager, sizeof(DWORD));
void ObjListLoop()
{
    while (true)
    {
        std::list<CObject>currobjList;
        DWORD ObjectArray = Memory.Read<DWORD>(ObjManager + 0x20, sizeof(DWORD));

        for (int i = 0; i < 10000; i++)
        {
            CObject obj(Memory.Read<DWORD>(ObjectArray + (0x4 * i), sizeof(DWORD)));

          //  if (obj.Address() == Local.Address())
          //      continue;
          /*  if (!obj.IsVisible())
                continue;
            if (obj.GetHealth() > 1.f || obj.GetHealth() < -1.f)
                continue;
            Vector3 StartPos = obj.GetMissileStartPos();
            if (StartPos.X == 0 || StartPos.Z == 0 || StartPos.Y == 0)
                continue;
            Vector3 EndPos = obj.GetMissileEndPos();
            if (EndPos.X == 0 || EndPos.Z == 0 || StartPos.Y == 0)
                continue;
            if (obj.GetDistToMe(Local) > 1000)
                continue;*/
           // clog.AddLog("%s %x %i", obj.GetName().c_str(), obj.Address(), obj.GetAlive());
            
              
            if (!obj.IsWard())// || obj.GetMaxHealth() != 4.f)
                continue;
            //clog.AddLog("%s ", obj.GetName().c_str());

            currobjList.emplace_back(obj);

        }
        //objList.clear();
       // if(!currobjList.empty())
        wardList = currobjList;
        Sleep(5000);
    }
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


    float GameTime = Memory.Read<float>(ClientAddress + oGameTime, sizeof(float));

    CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ObjListLoop, 0, 0, 0);

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

        if (M.ExitBot)
            break;

        if (GetAsyncKeyState(VK_INSERT) & 0x8000) //open menu
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
            while (GetAsyncKeyState(VK_INSERT) & 0x8000) { Sleep(1); }
        }
        if (GetAsyncKeyState(VK_F11) & 0x8000) //exit button
        {
            break;
        }
        //GameTime = Memory.Read<float>(ClientAddress + oGameTime, sizeof(float));

      
        Sleep((M.AntiLag) +1);
    }

    Config->Save();
    Direct3D9.Shutdown();
    ::DestroyWindow(hWnd);
    ::UnregisterClass(wc.lpszClassName, wc.hInstance);
    return 0;
}
