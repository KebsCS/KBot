

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
            while (PressedKey(M.Misc.MenuKey)) { std::this_thread::sleep_for(std::chrono::milliseconds(1)); }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
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


#ifndef NOLEAGUE

    LPCSTR windowName = XorStr("League of Legends (TM) Client");
    if (!FindWindowA(0, windowName))
    {
        MessageBox(0, L"Game not found", L"Window creation failed", 0);
        ::UnregisterClass(wc.lpszClassName, wc.hInstance);
        return 0;
    }

#endif // !NOLEAGUE

    hWnd = CreateWindowEx(WS_EX_TOPMOST | WS_EX_LAYERED, overlayClassName, overlayClassName, WS_POPUP, 1, 1, SCREENWIDTH, SCREENHEIGHT, 0, 0, 0, 0);
    SetLayeredWindowAttributes(hWnd, 0, 0, LWA_ALPHA);
    SetLayeredWindowAttributes(hWnd, 0, 0, LWA_COLORKEY);
    //SetLayeredWindowAttributes(hWnd, RGB(255, 255, 255),100, /* 0=transparent, 255=completely solid*/LWA_COLORKEY);
    //SetWindowPos(hWnd, HWND_TOPMOST, 0 ,0 , SCREENWIDTH, SCREENHEIGHT, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
    //ShowWindow(hWnd, SW_SHOW);



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


#ifndef NOLEAGUE



    std::thread MinionListThread{ MinionListLoop };
    MinionListThread.detach();

#endif // !NOLEAGUE

    std::thread MenuHandlerThread{ MenuHandler };
    MenuHandlerThread.detach();


   // CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ObjListLoop, 0, 0, 0); //todo loop obj only when needed, not in different thread and c++11 threads
    //CreateThread(0, 0, (LPTHREAD_START_ROUTINE)OrbwalkThread, 0, 0, 0); 
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));


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
        

        if (M.ExitBot || PressedKey(VK_F11))// || !FindWindowA(0, windowName)) //exit
            break;
#ifndef NOLEAGUE



        M.GameTime = Memory.Read<float>(ClientAddress + oGameTime, sizeof(float));

#endif // !NOLEAGUE


        Direct3D9.Render();

        //clog.AddLog("%f", Local.GetEXP());
 

        //todo twisted fate cards bound seperatly+visual indicator
       


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


        //clog.AddLog("%f, %f, %f", Local.GetPosition().X, Local.GetPosition().Y, Local.GetPosition().Z);
        //clog.AddLog("%d , %d", mouse->GetPos().x, mouse->GetPos().y);


        //MissileThread.join();
        
        std::this_thread::sleep_for(std::chrono::milliseconds(M.Misc.AntiLag));
    }

    Config->Save("default");
    Direct3D9.Shutdown();
    ::DestroyWindow(hWnd);
    ::UnregisterClass(wc.lpszClassName, wc.hInstance);
    return 0;
}
