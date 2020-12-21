

#include "ObjectManager.h"
//#include "Orbwalker.h"
#include "DirectX.h"

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

    v1 = Memory.Read<int>(objManager + 0x14);
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


static DWORD OBJManager = Memory.Read<DWORD>(ClientAddress + oObjManager, sizeof(DWORD));
DWORD GetNext(DWORD objManager, DWORD a2)
{
    int v2; // eax
    uint16_t v3; // edx
    uint16_t v4; // esi
    int v5; // eax

    v2 = Memory.Read<int>(objManager + 0x14);
    v3 = Memory.Read<uint16_t>(a2 + 0x20) + 1; // loops from first to last object index - 1 to a1a
    v4 = 0xbb8;//(Memory.Read<int>(objManager + 0x18) - v2) >> 2; // always bb8 (max possible ammount of objects maybe?)
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


std::vector<CObject>missileList;

std::vector<DWORD> GetObjectList()
{

    std::vector<DWORD> list;
    auto obj = GetFirst(OBJManager);

    std::vector<CObject>currmissileList;

    while (obj)
    {
        //missileList.clear();

        if (obj)
        {
            CObject currobj(obj);
            bool addMissile = true;

            auto itminion = std::find(minionList.begin(), minionList.end(), currobj);
            if (itminion != minionList.end())
                addMissile = false;

            if (addMissile)
            {
                auto ithero = std::find(init->herolist.begin(), init->herolist.end(), currobj);
                if (ithero != init->herolist.end())
                    addMissile = false;
            }

            if (addMissile)
            {
                auto ittest = std::find(init->objlisttest.begin(), init->objlisttest.end(), currobj);
                if (ittest != init->objlisttest.end())
                    addMissile = false;
            }

            if (addMissile)
                if (!currobj.IsMissile())//&& !xd.IsTroy())
                    addMissile = false;
            if(addMissile)
                currmissileList.emplace_back(currobj);
        
            // clog.AddLog("%x", obj);
            list.push_back(obj);
        }

        obj = GetNext(OBJManager, obj);
        
    }
    if(!currmissileList.empty())
        missileList = currmissileList;
    return list;
}




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
        if(!currobjList.empty())
            minionList = currobjList;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

std::vector<DWORD>objList;

//object manager loop
void ObjListLoop()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    while (true)
    {

        objList = GetObjectList();

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
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



//bool FUN_0155b3b0(float param_1)
//
//{   
//    if (((Memory.Read<int>(param_1) < Memory.Read<int>(param_1 + 12) || Memory.Read<int>(param_1) == Memory.Read<int>(param_1 + 12)) &&
//        (Memory.Read<int>(param_1 + 4) < Memory.Read<int>(param_1 + 16) || Memory.Read<int>(param_1 + 4) == Memory.Read<int>(param_1 + 16))) &&
//        (Memory.Read<int>(param_1+ 8) < Memory.Read<int>(param_1+20) || Memory.Read<int>(param_1 +8) == Memory.Read<int>(param_1+20))) {
//        return 0;
//    }
//    return 1;
//}

//bool FUN_0155b3b0(float obj)
//{
//    return Memory.Read<int>(obj) > Memory.Read<int>(obj+12) || Memory.Read<int>(obj+4) > Memory.Read<int>(obj+16) || Memory.Read<int>(obj+8) > Memory.Read<int>(obj +20);
//}



//undefined __fastcall FUN_0155b3b0(float* param_1)
//
//{
//    if (((*param_1 < param_1[3] || *param_1 == param_1[3]) &&
//        (param_1[1] < param_1[4] || param_1[1] == param_1[4])) &&
//        (param_1[2] < param_1[5] || param_1[2] == param_1[5])) {
//        return 0;
//    }
//    return 1;
//}
//

//
//DWORD getDwordFromBytes(byte* b)
//{
//    return (b[3]) | (b[2] << 8) | (b[1] << 16) | (b[0] << 24);
//}
//
//float GetBoundingRadius(DWORD param_1)
//{
//    float pfVar1;
//    bool cVar2;
//    DWORD iVar3;
//    float fVar4;
//    uint_fast64_t uStack36;
//    float fStack28;
//    DWORD uStack16;
//    float fStack4;
//    
//    if (Memory.Read<int>(param_1 + 12024) != 0) {
//        uStack16 = 0xc989b7;
//        iVar3 = Memory.Read<DWORD>((Memory.Read<int>(param_1) + 0xe4)); 
//        uStack16 = 0xc989be;
//        fVar4 = 65.0f;
//        fStack4 = (float)fVar4;
//    }
//    pfVar1 = (float)(param_1 + 0x3c);
//    cVar2 = FUN_0155b3b0(pfVar1);
//    if (cVar2 == 0) {
//        float temp1 = Memory.Read<float>(param_1 + 256) - Memory.Read<float>(param_1 + 244);
//        BYTE btemp1[4];
//        memcpy(&btemp1, (unsigned char*)(&temp1), 4);
//
//        float temp2 = Memory.Read<float>(param_1 + 252) - Memory.Read<float>(param_1 + 240);
//        BYTE btemp2[4];
//        memcpy(&btemp2, (unsigned char*)(&temp2), 4);
//
//
//        DWORD d1 = getDwordFromBytes(btemp1);
//        uint_fast64_t ret = d1 * 0x100000000;
//        ret += getDwordFromBytes(btemp2);
//        uStack36 = ret;
//        fStack28 = Memory.Read<float>(param_1 + 260) - Memory.Read<float>(param_1 + 248);
//    }
//    else {
//        uStack36 = ClientAddress + 0x3539B78;
//        fStack28 = ClientAddress + 0x3539B80;
//    }
//    fStack4 = fStack28;
//    FUN_0155b3b0(pfVar1);
//    return Memory.Read<float>(uStack36);
//}







void OrbwalkThread()
{

    while(true)
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

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // Initialize Direct3D
    if (!Direct3D9.DirectXInit(hWnd))
    {
        Direct3D9.Shutdown();
        ::UnregisterClass(wc.lpszClassName, wc.hInstance);
        return 0;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    ::ShowWindow(hWnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hWnd);

    Config->Setup();
    Config->Load("default");
    clog.AddLog("[startup] Config loaded");

    //rewrite this to work properly and use new verion C++ threads
    std::thread MinionListThread{ MinionListLoop };
   // CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ObjListLoop, 0, 0, 0); //todo loop obj only when needed, not in different thread
    //CreateThread(0, 0, (LPTHREAD_START_ROUTINE)OrbwalkThread, 0, 0, 0); 
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    init->StartupInfo();

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
        //std::thread MissileThread{ GetObjectList };
        Direct3D9.Render();

        if (M.ExitBot)
            break;

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
            while (PressedKey(M.Misc.MenuKey)) { std::this_thread::sleep_for(std::chrono::milliseconds(1)); }
        }
        else if (PressedKey(VK_F11)) //exit button
        {
            break;
        }

        
       // clog.AddLog("%d , %d", mouse->GetPos().x, mouse->GetPos().y);

        //todo twisted fate cards bound seperatly+visual indicator
       
        //GameTime = Memory.Read<float>(ClientAddress + oGameTime, sizeof(float)); //todo gametime as global variable that can be accesed by any func
    
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
        

        //todo wards bounding
        //initialization in different threads
        //turret range in different func

        //MissileThread.join();
        std::this_thread::sleep_for(std::chrono::milliseconds(M.Misc.AntiLag));
    }

    Config->Save("default");
    Direct3D9.Shutdown();
    ::DestroyWindow(hWnd);
    ::UnregisterClass(wc.lpszClassName, wc.hInstance);
    return 0;
}
