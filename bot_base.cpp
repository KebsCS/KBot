
//#include "Orbwalker.h"
#include "DirectX.h"
#include "API.h"



LPCSTR overlayClassName = "ovrl"; // overlay window name

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



DWORD GetNext( DWORD a2)
{
    int v2; // eax
    uint16_t v3; // edx
    uint16_t v4; // esi
    int v5; // eax

    v2 = OBJManagerArray;
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




//
//std::vector<CObject>missileList;
//
//std::vector<DWORD> GetObjectList()
//{
//
//    std::vector<DWORD> list;
//    auto obj = GetFirst();
//
//    std::vector<CObject>currmissileList;
//
//    while (obj)
//    {
//        //missileList.clear();
//
//        if (obj)
//        {
//            CObject currobj(obj);
//            bool addMissile = true;
//
//            auto itminion = std::find(minionList.begin(), minionList.end(), currobj);
//            if (itminion != minionList.end())
//                addMissile = false;
//
//            if (addMissile)
//            {
//                auto ithero = std::find(init->herolist.begin(), init->herolist.end(), currobj);
//                if (ithero != init->herolist.end())
//                    addMissile = false;
//            }
//
//            if (addMissile)
//            {
//                auto ittest = std::find(init->objlisttest.begin(), init->objlisttest.end(), currobj);
//                if (ittest != init->objlisttest.end())
//                    addMissile = false;
//            }
//
//            if (addMissile)
//                if (!currobj.IsMissile())//&& !xd.IsTroy())
//                    addMissile = false;
//            if(addMissile)
//                currmissileList.emplace_back(currobj);
//        
//            // clog.AddLog("%x", obj);
//            list.push_back(obj);
//        }
//
//        obj = GetNext(obj);
//        
//    }
//    if(!currmissileList.empty())
//        missileList = currmissileList;
//    return list;
//}

//std::vector<DWORD>objList;
//
////object manager loop
//void ObjListLoop()
//{
//    std::this_thread::sleep_for(std::chrono::milliseconds(100));
//    while (true)
//    {
//
//        objList = GetObjectList();
//
//        std::this_thread::sleep_for(std::chrono::milliseconds(1));
//    }
//}
//



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
        if(!currobjList.empty())
            minionList = currobjList;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
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
#ifndef NOLEAGUE

            if (!FindWindowA(0, XorStr("League of Legends (TM) Client")))
            {
                M.ExitBot = true;
                break;
            }
                
#endif // !NOLEAGUE
            while (PressedKey(M.Misc.MenuKey)) { std::this_thread::sleep_for(std::chrono::milliseconds(1)); }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

}



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

    WNDCLASSEXA wc;
    //Registering the Window Class
    wc.cbSize = sizeof(WNDCLASSEXA); //
    wc.style = CS_VREDRAW | CS_HREDRAW | CS_NOCLOSE;
    wc.lpfnWndProc = WndProc; //
    wc.cbClsExtra = 0; // 
    wc.cbWndExtra = 0; // 
    wc.hInstance = hInstance; // 
    wc.hIcon = 0;// LoadIconA(NULL, IDI_APPLICATION);
    wc.hIconSm = 0;// LoadIconA(NULL, IDI_APPLICATION); // 
    wc.hCursor = 0;// LoadCursorA(NULL, IDC_ARROW); // 
    wc.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0)); // 
    wc.lpszMenuName = overlayClassName; //
    wc.lpszClassName = overlayClassName; //



    if (!RegisterClassExA(&wc))
    {
        MessageBoxA(0, XorStr("Window Registration Failed"), std::to_string(GetLastError()).c_str(), 0);
        ::UnregisterClassA(wc.lpszClassName, wc.hInstance);
        return 0;
    }


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
 
    if (NowVersion != XorStr("1.0.0") || NowPatch != XorStr("10.25"))
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
        M.DrawServerInfo = true;
    }
    clog.AddLog(XorStr("[start] Version: %s"), NowVersion.c_str());
    clog.AddLog(XorStr("[start] Patch: %s"), NowPatch.c_str());




    htmlData = api->GET(XorStr("https://24timezones.com"), XorStr("/time-zone/cet"));
    nPos = htmlData.find(XorStr(" January ")) + strlen(XorStr(" January "));
    if (nPos == std::string::npos)
    {
        M.ExitBot = true;
        ::UnregisterClassA(wc.lpszClassName, wc.hInstance);
        return 0;

    }
    std::string Today = htmlData.substr(nPos, htmlData.find(XorStr(", 2021</p>"), nPos) - nPos).c_str();
    try
    {
        if (std::stoi(Today) > 31) //end of month
        {
            M.ExitBot = true;
            ::UnregisterClassA(wc.lpszClassName, wc.hInstance);
            return 0;

        }
    }
    catch (...)
    {
        M.ExitBot = true;
        ::UnregisterClassA(wc.lpszClassName, wc.hInstance);
        return 0;
    }



#ifndef NOLEAGUE

    LPCSTR windowName = XorStr("League of Legends (TM) Client");
    if (!FindWindowA(0, windowName))
    {
        MessageBoxA(0, XorStr("Game not found"), XorStr("Window creation failed"), 0);
        M.ExitBot = true;
        ::UnregisterClassA(wc.lpszClassName, wc.hInstance);
        return 0;
    }

#endif // !NOLEAGUE

    hWnd = CreateWindowExA(WS_EX_TOPMOST | WS_EX_LAYERED, overlayClassName, overlayClassName, WS_POPUP, 1, 1, SCREENWIDTH, SCREENHEIGHT, 0, 0, 0, 0);
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

    ::ShowWindow(hWnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hWnd);

  


#ifndef NOLEAGUE



    std::thread MinionListThread{ MinionListLoop };
    MinionListThread.detach();


    M.StartTime = M.GameTime;
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

        if (championScript && PressedKey(VK_SPACE))
            championScript->Harass();


#endif // !NOLEAGUE


        Direct3D9.Render();


        //std::vector<CObject> missiles;

        //int missileMapPtr = Memory.Read<DWORD>(ClientAddress+ 0x34F848C);
       
        //int numMissiles = Memory.Read<int>(missileMapPtr + 0x78);
        //int rootNode = Memory.Read<int>(missileMapPtr + 0x74);
        ////clog.AddLog("%x", rootNode);
        //std::queue<int> nodesToVisit;
        //std::set<int> visitedNodes;
        //nodesToVisit.push(rootNode);

        //int childNode1, childNode2, childNode3, node;
        //while (nodesToVisit.size() > 0 && visitedNodes.size() < numMissiles * 2) 
        //{
        //    node = nodesToVisit.front();
        //    nodesToVisit.pop();
        //    visitedNodes.insert(node);
        //    //clog.AddLog("%x", node);
        //    childNode1 = Memory.Read<int>(node);
        //    childNode2 = Memory.Read<int>(node+4);
        //    childNode3 = Memory.Read<int>(node+8);


        //    if (visitedNodes.find(childNode1) == visitedNodes.end())
        //        nodesToVisit.push(childNode1);
        //    if (visitedNodes.find(childNode2) == visitedNodes.end())
        //        nodesToVisit.push(childNode2);
        //    if (visitedNodes.find(childNode3) == visitedNodes.end())
        //        nodesToVisit.push(childNode3);

        //    unsigned int netId = Memory.Read<int>(node + 0x10);
        //   

        //    // Actual missiles net_id start from 0x40000000 and throught the game this id will be incremented by 1 for each missile.
        //    // So we use this information to check if missiles are valid.
        //    if (netId - (unsigned int)0x40000000 > 0x100000)
        //        continue;

        //    int addr = Memory.Read<int>(node + 0x14);
        //    if (addr == 0)
        //        continue;

        //    // The MissileClient is wrapped around another object
        //    addr = Memory.Read<int>(addr + 0x4); 
        //    if (addr == 0)
        //        continue;

        //    addr = Memory.Read<int>(addr + 0x10);
        //    if (addr == 0)
        //        continue;

        //    // At this point addr is the address of the MissileClient
        //    CObject m(addr); //std::unique_ptr<CObject>(new CObject());
        //   // m->LoadFromMem(addr, hProcess);

        //    // Check one more time that we read a valid missile
        //    if (m.GetNetworkID() != netId)
        //        continue;
        //    //.AddLog("%x", m.Address());
        //    clog.AddLog("%s %x", m.GetName().c_str(), m.Address());
        //    missiles.push_back(m);
        //}
 

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

  /*      clog.AddLog("Mouse: %f, %f, %f", mouse->GetMouseWorldPosition().X, mouse->GetMouseWorldPosition().Y, mouse->GetMouseWorldPosition().Z);
        clog.AddLog("Player: %f, %f, %f", Local.GetPosition().X, Local.GetPosition().Y, Local.GetPosition().Z);
        clog.AddLog("Real Mouse: %d , %d", mouse->GetPos().x, mouse->GetPos().y);*/

        //MissileThread.join();
        
        std::this_thread::sleep_for(std::chrono::milliseconds(M.Misc.AntiLag));
    }

    Config->Save(XorStr("default"));
    Direct3D9.Shutdown();
    ::DestroyWindow(hWnd);
    ::UnregisterClassA(wc.lpszClassName, wc.hInstance);
    return 0;
}
