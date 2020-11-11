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

DWORD GetFirst(DWORD objManager)
{
    int v1; // eax
    int v2; // edx

    v1 = Memory.Read<int>(objManager + 20);
    v2 = Memory.Read<int>(objManager + 24);
    if (v1 == v2)
        return 0;
    while (Memory.Read<BYTE>(v1) & 1 || !Memory.Read<DWORD>(v1))
    {
        v1 += 4;
        if (v1 == v2)
            return 0;
    }
    return Memory.Read<DWORD>(v1);
}

static DWORD OBJManager = Memory.Read<DWORD>(ClientAddress + oObjManager, sizeof(DWORD));
DWORD GetNext(DWORD objManager, DWORD a2)
{
    int v2; // eax
    uint16_t v3; // edx
    uint16_t v4; // esi
    int v5; // eax

    v2 = Memory.Read<int>(objManager + 20);
    v3 = Memory.Read<uint16_t>(a2 + 32) + 1;
    v4 = (Memory.Read<int>(objManager + 24) - v2) >> 2;
    if (v3 >= v4)
        return 0;
    v5 = v2 + 4 * v3;
    while (Memory.Read<BYTE>(v5) & 1 || !Memory.Read<DWORD>(v5))
    {
        ++v3;
        v5 += 4;
        if (v3 >= v4)
            return 0;
    }
    return Memory.Read<DWORD>(v5);
}

std::list<CObject>missileList;

std::vector<DWORD> GetObjectList()
{

    std::vector<DWORD> list;
    auto obj = GetFirst(OBJManager);

    while (obj)
    {
        //missileList.clear();

        if (obj)
        {
            CObject xd(obj);
            if (xd.Address() != Local.Address())
            {
                if (xd.GetTeam() == 100 || xd.GetTeam() == 200 || xd.GetTeam() == 300)
                {
                    if (xd.IsVisible())
                    {
                        if (!(xd.GetHealth() > 1.f || xd.GetHealth() < -1.f))
                        {
                            Vector3 StartPos = xd.GetMissileStartPos();
                            if (!(StartPos.X == 0 || StartPos.Z == 0 || StartPos.Y == 0))
                            {
                                Vector3 EndPos = xd.GetMissileEndPos();
                                if (!(EndPos.X == 0 || EndPos.Z == 0 || StartPos.Y == 0))
                                {
                                    if (xd.GetDistToMe(Local) < 1500.f)
                                    {
                                        missileList.emplace_back(xd);
                                    }
                                }
                            }

                        }
                    }
                }
            }
        
            // clog.AddLog("%x", obj);
            list.push_back(obj);
        }

        obj = GetNext(OBJManager, obj);
    }

    return list;
}

std::list<CObject>minionList;

void MinionListLoop()
{
    DWORD MinionList = Memory.Read<DWORD>(ClientAddress + oMinionList);
    while (true)
    {
        std::list<CObject>currobjList;
        DWORD MinionArray = Memory.Read<DWORD>(MinionList + 0x4);
        int MinionArrayLength = Memory.Read<int>(MinionList + 0x8);
        for (int i = 0; i < MinionArrayLength * 4; i += 4)
        {
            CObject obj(Memory.Read<DWORD>(MinionArray + i));

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
            
            //clog.AddLog("%s ", obj.GetName().c_str());

            currobjList.emplace_back(obj);

        }
        minionList = currobjList;
        Sleep(1000);
    }
}

std::vector<DWORD>objList;

void ObjListLoop()
{
    while (true)
    {
        objList = GetObjectList();

        Sleep(10);
    }
}

//double sub_3A7270(DWORD obj)
//{
//    float v1; // xmm0_4
//    double result; // st7
//
//    v1 = Memory.Read<int>(obj + 1108);
//    if (v1 == 3.4028235e38)
//        result = 65.0;
//    else
//        result = v1;
//    return result;
//}
//
//
//bool sub_BA5B80(DWORD obj)
//{
//    return Memory.Read<int>(obj) > Memory.Read<int>(obj+12) || Memory.Read<int>(obj+4) > Memory.Read<int>(obj+16) || Memory.Read<int>(obj+8) > Memory.Read<int>(obj +20);
//}
//
//double sub_3F76C0(DWORD obj)
//{
//    DWORD v1; // esi
//    __int64 v3; // [esp+4h] [ebp-24h]
//    float v4; // [esp+Ch] [ebp-1Ch]
//    __int64 v5; // [esp+1Ch] [ebp-Ch]
//    float v6; // [esp+24h] [ebp-4h]
//
//    v1 = obj + 60;
//
//    auto xd = Memory.Read<float>(v3);
//    auto xd2 = Memory.Read<float>(v3+1);
//    if ((unsigned __int8)sub_BA5B80(obj + 60))
//    {
//        v3 = ClientAddress + 0x3544B50;
//        v4 = Memory.Read<float>(ClientAddress + 0x3544B58);
//    }
//    else
//    {
//        xd = Memory.Read<int>(v1 + 12) - Memory.Read<int>(v1);
//        xd2 = Memory.Read<int>(v1 + 16) - Memory.Read<int>(v1 + 4);
//        v4 = Memory.Read<int>(v1 + 20) - Memory.Read<int>(v1 + 8);
//        
//    }
//    v5 = v3;
//    v6 = v4;
//    if ((unsigned __int8)sub_BA5B80(v1))
//    {
//        v3 = 0x3544B50;
//        v4 = Memory.Read<float>(0x3544B58);
//    }
//    else
//    {
//        
//        xd = Memory.Read<int>(v1 + 12) - Memory.Read<int>(v1);
//        xd2 = Memory.Read<int>(v1 + 16) - Memory.Read<int>(v1 + 4);
//        v4 = Memory.Read<int>(v1 + 20) - Memory.Read<int>(v1 + 8);
//    }
//    return (xd + v6) * 0.25;
//}
//
//
//__int64 sub_134ACA0(unsigned int _ECX, signed int a2)
//{
//    DWORD _XMM0;
//
//    if ((ClientAddress+ 0x3558A0C) < 6)
//    {
//        _XMM0 = (double)_ECX + (double)a2 * 4294967296.0;
//    }
//    else
//    {
//     /*   __asm
//        {
//            vmovd   xmm0, ecx; Move 32 bits
//            vpinsrd xmm0, xmm0, edx, 1; Insert Dword
//            vcvtqq2pd xmm0, xmm0; Convert Packed Quadword Integers to Packed Double - Precision Floating - Point Values
//        }*/
//    }
//    return _XMM0;
//}
//
//double sub_A4A2C0(DWORD a1, double a2)
//{
//    int v2; // eax
//    unsigned int v3; // esi
//    int v4; // ecx
//
//    
//    
//    v2 = Memory.Read<DWORD>(a1);
//    v3 = Memory.Read<int>(a1 + 16);
//    v4 = Memory.Read<DWORD>(Memory.Read<DWORD>(a1 + 48));
//    if ((Memory.Read<DWORD>(v2 + 52) - v4) >> 7 <= v3)
//        int error = 1;
//    return (float)((sub_134ACA0(Memory.Read<DWORD>(((v3 << 7) + v4)), Memory.Read<DWORD>(((v3 << 7) + v4))))
//        * Memory.Read<double>(ClientAddress + 0x3538B28));
//}
//
//double sub_336C00(DWORD obj, int a2)
//{
//    DWORD v2; // ebp
//    BYTE v3; // ebx
//    BYTE v4; // edi
//    DWORD v5; // esi
//    float v6; // xmm2_4
//    double v7; // st7
//    float v8; // xmm0_4
//    float v10; // [esp+4h] [ebp-Ch]
//    float v11; // [esp+8h] [ebp-8h]
//    DWORD v12; // [esp+Ch] [ebp-4h]
//    float v13; // [esp+14h] [ebp+4h]
//    
//    
//    v2 = Memory.Read<DWORD>(Memory.Read<DWORD>(Memory.Read<int>(obj)));
//    v13 = Memory.Read<float>(a2 + 488);
//
//
//    v12 = Memory.Read<DWORD>(Memory.Read<DWORD>(Memory.Read<int>(obj + 4)));
//    if (Memory.Read<DWORD>(Memory.Read<DWORD>(Memory.Read<int>(obj))) != v12)
//    {
//        do
//        {
//            
//            
//            v3 = Memory.Read<int>((Memory.Read<BYTE>(Memory.Read<DWORD>(v2))) + 4);
//            v4 = Memory.Read<BYTE>(Memory.Read<DWORD>(Memory.Read<DWORD>(v2)));
//            if (v4 != v3)
//            {
//                v5 = Memory.Read<float>(v4 + 8);
//                do
//                {
//                    if (!Memory.Read<DWORD>(v4))
//                    {
//                        
//                        
//                        v6 = Memory.Read<DWORD>(v5 - 1);
//                        if (v6 != Memory.Read<DWORD>(v5))
//                        {
//                            v11 = Memory.Read<int>(v5 + 8) + Memory.Read<int>(v5 + 4);
//                            v7 = sub_A4A2C0(Memory.Read<DWORD>((ClientAddress + 0x1C79A08) + 8),v11);
//                            v8 = Memory.Read<int>(v5 + 4);
//                            v10 = v7;
//                            if (v8 < v10)
//                            {
//                                if (v10 < v11)
//                                    v6 = (float)((float)((float)(v10 - v8) / Memory.Read<int>(v5 + 8)) * (float)(Memory.Read<int>(v5 ) - Memory.Read<DWORD>(v5 - 1))) + Memory.Read<DWORD>(v5 - 1);
//                                else
//                                    v6 = Memory.Read<int>(v5);
//                            }
//                            else
//                            {
//                                v6 = Memory.Read<DWORD>(v5 - 1);
//                            }
//                        }
//                        v13 = v6 + v13;
//                    }
//                    v4 += 20;
//                    v5 += 5;
//                } while (v4 != v3);
//            }
//            ++v2;
//        } while (v2 != v12);
//    }
//    return v13;
//}
//
//double GetBoundingRadius_(DWORD obj)
//{
//    DWORD v1; // esi
//    DWORD v2; // eax
//    double v3; // st7
//    float v4; // ST08_4
//    float v6; // [esp+8h] [ebp-4h]
//   
//    v1 = obj;
//    if (!Memory.Read<DWORD>(obj + 3006))
//        return sub_3F76C0(v1);
//    v2 = Memory.Read<DWORD>(obj + 228);
//    v3 = sub_3A7270(v2);
//    v6 = v3;
//    v4 = v3;
//    if (v4 == 0.0)
//        return sub_3F76C0(v1);
//    if (Memory.Read<BYTE>(v1 + 8240))
//        return Memory.Read<int>(v1 + 8236) * v6;
//    return sub_336C00(Memory.Read<DWORD>(v1) + 1678, (int)(v1 + 1070)) * v6;
//}

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


    if (FindWindowA(0, XorStr("League of Legends (TM) Client")))
    {
       // clog.AddLog("Window Found");

        hWnd = CreateWindowEx(WS_EX_TOPMOST | WS_EX_LAYERED, overlayClassName, overlayClassName, WS_POPUP, 1, 1, SCREENWIDTH, SCREENHEIGHT, 0, 0, 0, 0);
        SetLayeredWindowAttributes(hWnd, 0, 0, LWA_ALPHA);
        SetLayeredWindowAttributes(hWnd, 0, 0, LWA_COLORKEY);
        //SetLayeredWindowAttributes(hWnd, RGB(255, 255, 255),100, /* 0=transparent, 255=completely solid*/LWA_COLORKEY);
        //SetWindowPos(hWnd, HWND_TOPMOST, 0 ,0 , SCREENWIDTH, SCREENHEIGHT, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
        //ShowWindow(hWnd, SW_SHOW);

    }
    else
    {
       // clog.AddLog("[error] Window Creation Failed");
        MessageBox(0, L"Game not found", L"Window creation failed", 0);
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

    Config->Setup();
    Config->Load("cfg");
    clog.AddLog("[startup] Config loaded");


    clog.AddLog("ProcessId: %d", Memory.ProcessID);
    clog.AddLog("ClientAddress: %x", ClientAddress);


    float GameTime = Memory.Read<float>(ClientAddress + oGameTime, sizeof(float));

    Sleep(1000);
    CreateThread(0, 0, (LPTHREAD_START_ROUTINE)MinionListLoop, 0, 0, 0);
   // CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ObjListLoop, 0, 0, 0);
    Sleep(1000);

    if (GameTime > 0) // if in game
    {
        
        clog.AddLog("OBJManager: %x", OBJManager);
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
    
        //clog.AddLog("%i", GetObjectList().size());
        

        Sleep(M.AntiLag );
    }

    Config->Save("cfg");
    Direct3D9.Shutdown();
    ::DestroyWindow(hWnd);
    ::UnregisterClass(wc.lpszClassName, wc.hInstance);
    return 0;
}
