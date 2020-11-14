#include "Visuals.h"


LPDIRECT3D9              g_pD3D = NULL;
LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
D3DPRESENT_PARAMETERS    g_d3dpp = {};
ID3DXLine* g_Line = NULL;
LPDIRECT3DVERTEXBUFFER9  g_pVB = NULL;

RECT rc;
ID3DXFont* pFont;


Visuals *vis;

extern std::list<CObject>minionList;
extern std::list<CObject>missileList;
extern std::vector<DWORD> objList;
DirectX::XMMATRIX Direct3D9Render::ReadMatrix(DWORD address)
{

	DirectX::XMMATRIX tmp;
	for (int i = 0; i < 16; i++) //todo 1 read instaed of 16 (performance)
	{
		tmp.r->m128_f32[i] = Memory.Read<float>((address + sizeof(float) * i), sizeof(float));

	}
	//tmp.r->m128_f32[0] = Memory.Read<float>((address + sizeof(float) * 0), sizeof(float));
	//tmp.r->m128_f32[1] = Memory.Read<float>((address + sizeof(float) * 1), sizeof(float));
	//tmp.r->m128_f32[2] = Memory.Read<float>((address + sizeof(float) * 2), sizeof(float));
	//tmp.r->m128_f32[3] = Memory.Read<float>((address + sizeof(float) * 3), sizeof(float));
	//
	//tmp.r->m128_f32[4] = Memory.Read<float>((address + sizeof(float) * 4), sizeof(float));
	//tmp.r->m128_f32[5] = Memory.Read<float>((address + sizeof(float) * 5), sizeof(float));
	//tmp.r->m128_f32[6] = Memory.Read<float>((address + sizeof(float) * 6), sizeof(float));
	//tmp.r->m128_f32[7] = Memory.Read<float>((address + sizeof(float) * 7), sizeof(float));

	//tmp.r->m128_f32[8] = Memory.Read<float>((address + sizeof(float) * 8), sizeof(float));
	//tmp.r->m128_f32[9] = Memory.Read<float>((address + sizeof(float) * 9), sizeof(float));
	//tmp.r->m128_f32[10] = Memory.Read<float>((address + sizeof(float) * 10), sizeof(float));
	//tmp.r->m128_f32[11] = Memory.Read<float>((address + sizeof(float) * 11), sizeof(float));

	//tmp.r->m128_f32[12] = Memory.Read<float>((address + sizeof(float) * 12), sizeof(float));
	//tmp.r->m128_f32[13] = Memory.Read<float>((address + sizeof(float) * 13), sizeof(float));
	//tmp.r->m128_f32[14] = Memory.Read<float>((address + sizeof(float) * 14), sizeof(float));
	//tmp.r->m128_f32[15] = Memory.Read<float>((address + sizeof(float) * 15), sizeof(float));

	
	//clog.AddLog("[matrix] %f, %f, %f ,%f", tmp.r->m128_f32[0], tmp.r->m128_f32[1], tmp.r->m128_f32[2], tmp.r->m128_f32[3]);
	//clog.AddLog("[matrix] %f, %f, %f ,%f", tmp.r->m128_f32[4], tmp.r->m128_f32[5], tmp.r->m128_f32[6], tmp.r->m128_f32[7]);
	//clog.AddLog("[matrix] %f, %f, %f ,%f", tmp.r->m128_f32[8], tmp.r->m128_f32[9], tmp.r->m128_f32[10], tmp.r->m128_f32[11]);
	//clog.AddLog("[matrix] %f, %f, %f ,%f", tmp.r->m128_f32[12], tmp.r->m128_f32[13], tmp.r->m128_f32[14], tmp.r->m128_f32[15]);

	return tmp;

}
DirectX::XMMATRIX Direct3D9Render::GetViewMatrix()
{
	

	//DWORD Renderer = Memory.Read<DWORD>(ClientAddress + oRenderer, sizeof(DWORD));
	return ReadMatrix(ClientAddress + oViewMatrix); // 10.21 0x3529CC8
}
DirectX::XMMATRIX Direct3D9Render::GetProjectionMatrix()
{

	//DWORD Renderer = Memory.Read<DWORD>(ClientAddress + oRenderer, sizeof(DWORD));
	return ReadMatrix(ClientAddress + oViewMatrix +0x40);
}

DirectX::XMMATRIX Direct3D9Render::GetViewProjectionMatrix()
{

	return DirectX::XMMatrixMultiply(GetViewMatrix(), GetProjectionMatrix());
}


bool Direct3D9Render::DirectXInit(HWND hWnd)
{

	if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
	{
		MessageBox(0, L" Direct3DCreate9 Failed", 0, 0);

		return false;
	}


	ZeroMemory(&g_d3dpp, sizeof(D3DPRESENT_PARAMETERS));
	g_d3dpp.Windowed = TRUE;
	g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	g_d3dpp.hDeviceWindow = hWnd;
	g_d3dpp.MultiSampleQuality = D3DMULTISAMPLE_NONE;
	g_d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	g_d3dpp.BackBufferWidth = SCREENWIDTH;
	g_d3dpp.BackBufferHeight = SCREENHEIGHT;
	g_d3dpp.EnableAutoDepthStencil = TRUE;
	g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE; //D3DPRESENT_INTERVAL_ONE - vsync D3DPRESENT_INTERVAL_IMMEDIATE - no vsync
	g_d3dpp.MultiSampleType = D3DMULTISAMPLE_NONMASKABLE;


	if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0)
	{
		MessageBox(0, L" CreateDevice Failed", 0, 0);

		return false;
	}

	if (FAILED(D3DXCreateLine(g_pd3dDevice, &g_Line)))
	{
		MessageBox(0, L" D3DXCreateLine Failed", 0, 0);
		if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
		if (g_pD3D) { g_pD3D->Release(); g_pD3D = NULL; }
		return false;
	}


	g_Line->SetWidth(1.0f);
	g_Line->SetPattern(0xFFFFFFFF);
	g_Line->SetAntialias(false);

	InitializeFonts();
	
	Renderimgui(hWnd);

	if (draw->InitTextures())
		clog.AddLog("[startup] Initialized textures");
	else clog.AddLog("[error] Failed to initialize textures");

	float GameTime = Memory.Read<float>(ClientAddress + oGameTime, sizeof(float));

	while (GameTime < 1) // pause if not in game
	{
		Sleep(1);
	}

	init->AddObjects();

	Sleep(1000);
	
	return true;
}

// Helper to display a little (?) mark which shows a tooltip when hovered.
// In your own code you may want to display an actual icon if you are using a merged icon fonts (see docs/FONTS.md)
static void HelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}


int Direct3D9Render::Render()
{


	// Start the Dear ImGui frame
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();


	if (M.MenuOpen)
	{
		
		static int counter = 0;
		char buf[128];
		sprintf(buf, "KBot %.1f FPS ###AnimatedTitle", ImGui::GetIO().Framerate);
		ImGui::SetNextWindowPos(ImVec2(903, 349), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(350, 350), ImGuiCond_FirstUseEver);
		ImGui::Begin(buf, &M.MenuOpen);// , ImGuiWindowFlags_AlwaysAutoResize);
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Open Console"))
				M.ConsoleOpen = true;
			if (ImGui::MenuItem("Exit"))
				M.ExitBot = true;
			ImGui::EndPopup();
		}


		ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_Reorderable;
		ImGuiTreeNodeFlags collapsing_header_flags = ImGuiTreeNodeFlags_DefaultOpen;
		if (ImGui::BeginTabBar("TabBar", tab_bar_flags))
		{
			if (ImGui::BeginTabItem("Main"))
			{
				ImGui::Separator();
				ImGui::Columns(2, 0, false);
				draw->ImageFromMemory(draw->textureFlash, 0, 0, "", 99, 32, 32, true);
				draw->ImageFromMemory(draw->textureSmite, 0, 0, "", 99, 32, 32, true);
				draw->ImageFromMemory(draw->textureIgnite, 0, 0, "", 99, 32, 32, true);
				draw->ImageFromMemory(draw->textureTeleport, 0, 0, "", 99, 32, 32, true);
				draw->ImageFromMemory(draw->textureHeal, 0, 0, "", 99, 32, 32, true);
				ImGui::NextColumn();
				draw->ImageFromMemory(draw->textureExhaust, 0, 0, "", 99, 32, 32, true);
				draw->ImageFromMemory(draw->textureBarrier, 0, 0, "", 99, 32, 32, true);
				draw->ImageFromMemory(draw->textureCleanse, 0, 0, "", 99, 32, 32, true);
				draw->ImageFromMemory(draw->textureGhost, 0, 0, "", 99, 32, 32, true);
				draw->ImageFromMemory(draw->textureClarity, 0, 0, "", 99, 32, 32, true);
				ImGui::Columns(1);
				
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Visuals"))
			{
				if (ImGui::CollapsingHeader("Enemies", collapsing_header_flags))
				{
					ImGui::Checkbox("AA Range", &M.AARange.Master);
					ImGui::SameLine();
					ImGui::ColorEdit4("AARangeColor##3", (float*)&M.AARange.Color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoAlpha);
					ImGui::SameLine();
					
					


					ImGui::SliderInt2("", M.AARange.Slider, 10, 60, "%d");
					ImGui::Separator();

					ImGui::Checkbox("Tracers\t", &M.Tracers.Master);
					ImGui::SameLine();
					ImGui::SliderFloat("##TracersThickness", &M.Tracers.Thickness, 0.1f, 10.f, "Thickness: %lg", 1.f);
					ImGui::Separator();

					ImGui::Checkbox("Cooldowns", &M.Cooldowns.Master);

				
				}
				
				if (ImGui::CollapsingHeader("Local", collapsing_header_flags))
				{
					ImGui::Checkbox("AA Range##Local", &M.AARange.Local);
					ImGui::SameLine();
					ImGui::ColorEdit4("AARangeLocalColor##3", (float*)&M.AARange.LocalColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoAlpha);
					ImGui::Separator();

					ImGui::Checkbox("LastHit Helper", &M.LastHit.Master);
					ImGui::SameLine();
					ImGui::ColorEdit4("LastHitColor##3", (float*)&M.LastHit.Color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoAlpha);


				}

				if (ImGui::CollapsingHeader("Structures", collapsing_header_flags))
				{
					
					ImGui::Checkbox("Turrets Range", &M.AARange.Turrets);
					ImGui::Separator();

					ImGui::Checkbox("Wards Range", &M.Wards.Master);
					ImGui::Separator();

					ImGui::Checkbox("Inhib Respawn Time", &M.Inhibs.Master);


				}

				

				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Evade"))
			{

				draw->ImageFromMemory(draw->textureKEKW, 0, 0, "", 99, 256, 256, true);

				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Orbwalker"))
			{
				ImGui::Combo("Hold Key", &M.Orbwalker.HoldKey, keyNames, ARRAYSIZE(keyNames));
				draw->ImageFromMemory(draw->textureSks, 0, 0, "", 99, 256*1.25, 256*1.25, true);

				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Misc"))
			{
				if (ImGui::CollapsingHeader("Auto Smite", collapsing_header_flags))
				{
					ImGui::Columns(2, 0, false);
					ImGui::Checkbox("Enable ##AutoSmite", &M.AutoSmite.Master);
					

					ImGui::NextColumn();
					ImGui::Text("Slot:");
					ImGui::SameLine();
					ImGui::RadioButton("D", &M.AutoSmite.Slot, 0); ImGui::SameLine();
					ImGui::RadioButton("F", &M.AutoSmite.Slot, 1);
					ImGui::Columns(1);
					ImGui::Text("Mode:\t");
					ImGui::SameLine();
					ImGui::RadioButton("Instant", &M.AutoSmite.Mode, 0); ImGui::SameLine();
					ImGui::RadioButton("Legit", &M.AutoSmite.Mode, 1); ImGui::SameLine();
					ImGui::RadioButton("Extra legit", &M.AutoSmite.Mode, 2);
					
					ImGui::SliderFloat("##AutoSmiteMouseSpeed", &M.AutoSmite.MouseSpeed, 0.1f, 1.f, "Mouse Speed: %lg", 1.f);

				}

				ImGui::Separator();

				ImGui::SliderInt("AntiLag", &M.Misc.AntiLag, 0, 50);
				ImGui::SameLine(); HelpMarker("Higher for slower PCs");

				ImGui::Separator();
				ImGui::Combo("Menu Key", &M.Misc.MenuKey, keyNames, ARRAYSIZE(keyNames));

				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Config"))
			{
				ImGui::Separator();
				static char bufcfg[32] = "default";
				ImGui::InputText("File name", bufcfg, IM_ARRAYSIZE(bufcfg));
				ImGui::Columns(3, 0, false);

				char value_l[32] = { '\0' };
				if (ImGui::Button("Save", ImVec2(100.f, 40.f)))
				{
					if (bufcfg[0] != '\0' && bufcfg[0] != ' ') // if is not empty
					{
						bool exists = false;
						for (int i = 0; i < M.Configs; i++) // loop through config list size
						{
							std::string configName = "fileName" + std::to_string(i);
							GetPrivateProfileStringA("Config", configName.c_str(), "", value_l, 32, ".\\configs\\default.ini"); //get every config saved
							std::string returned = value_l;
							if (returned == bufcfg) //check if config already exists
							{
								exists = true;
								Config->Save(bufcfg); //save to it 
								break;
							}
						}
						if (!exists) // if already exists dont write new configname to file
						{
							std::string configName = "fileName" + std::to_string(M.Configs++);
							WritePrivateProfileStringA("Config", configName.c_str(), bufcfg, ".\\configs\\default.ini");
							Config->Save(bufcfg);
						}
					}
				}
				ImGui::NextColumn();
				if (ImGui::Button("Load", ImVec2(100.f, 40.f))) //todo looking for already existing config in files and adding it to the list
					Config->Load(bufcfg);
				ImGui::NextColumn();
				if (ImGui::Button("Default", ImVec2(100.f, 40.f)))
					Config->Setup();
				ImGui::Columns(1);
				ImGui::Separator();
				
				for (int i = 0; i < M.Configs; i++) // print every config
				{
					std::string configName = "fileName" + std::to_string(i);
					GetPrivateProfileStringA("Config", configName.c_str(), "", value_l, 32, ".\\configs\\default.ini");
					std::string returned = value_l;
					ImGui::Columns(2, 0, false);
					ImGui::Text(returned.c_str());
					ImGui::NextColumn();
					std::string savebuttonName = "Save to##" + returned;
					if (ImGui::Button(savebuttonName.c_str()))
						Config->Save(returned);
					ImGui::SameLine();
					std::string loadbuttonName = "Load##" + returned;
					if (ImGui::Button(loadbuttonName.c_str()))
						Config->Load(returned);
					ImGui::Columns(1);
					ImGui::Separator();
					
				}

				

				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}


		ImGui::End();

		//ImGui::ShowDemoWindow();
		
	}

	if (M.ConsoleOpen)
	{
		clog.Draw("Console Log", &M.ConsoleOpen);
	}


	// Rendering
	
	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
	if (g_pd3dDevice->BeginScene() >= 0)
	{
		
		if (M.AARange.Local)
			vis->DrawAARanges(Local, M.AARange.Slider[0], M.AARange.Slider[1] / 10.f, RGBA(M.AARange.Color[0] * 255, M.AARange.Color[1] * 255, M.AARange.Color[2] * 255, M.AARange.Color[3] * 255),
				M.AARange.Local, RGBA(M.AARange.LocalColor[0] * 255, M.AARange.LocalColor[1] * 255, M.AARange.LocalColor[2] * 255, M.AARange.LocalColor[3] * 255));

		//hero loop
		if (M.Cooldowns.Master || M.AARange.Master || M.Tracers.Master)
		{
			for (auto obj : init->herolist)
			{
				if (M.Cooldowns.Master)
					vis->CooldownTimers(obj, 0);

				if (M.AARange.Master)
					vis->DrawAARanges(obj, M.AARange.Slider[0], M.AARange.Slider[1] / 10.f, RGBA(M.AARange.Color[0] * 255, M.AARange.Color[1] * 255, M.AARange.Color[2] * 255, M.AARange.Color[3] * 255),
						false, RGBA(M.AARange.LocalColor[0] * 255, M.AARange.LocalColor[1] * 255, M.AARange.LocalColor[2] * 255, M.AARange.LocalColor[3] * 255));
				

				if (M.Tracers.Master)
					vis->DrawTracers(obj, M.Tracers.Thickness);

				
				//float dist = obj.GetDistToMe(Local);
				//clog.AddLog("%s , %f", obj.GetName(), dist);
				//float dmg = Local.GetTotalDamage(&obj);
				//ImVec2 RealPos = Direct3D9.WorldToScreen(obj.GetPosition());
				//std::string str = obj.GetName() + " , " + std::to_string(dmg);
				//draw->String(str, RealPos.x, RealPos.y, centered, RGBA(255, 255, 255), fontTahoma);

			}
		}
		ImGui::EndFrame();
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

		//turret loop
		if (M.AARange.Turrets)
		{
			for (auto obj : init->turretlist)
			{
				if (M.AARange.Turrets)
					vis->DrawAARanges(obj, M.AARange.Slider[0], M.AARange.Slider[1] / 10.f, RGBA(M.AARange.Color[0] * 255, M.AARange.Color[1] * 255, M.AARange.Color[2] * 255, M.AARange.Color[3] * 255),
						false, RGBA(M.AARange.LocalColor[0] * 255, M.AARange.LocalColor[1] * 255, M.AARange.LocalColor[2] * 255, M.AARange.LocalColor[3] * 255));
			}
		}

		//minions/monsters/wards loop
		if (M.Wards.Master || M.LastHit.Master || M.AutoSmite.Master)
		{
			for (auto obj : minionList)
			{
				//clog.AddLog("%s", obj.GetName().c_str());
				if (M.Wards.Master)
					vis->WardsRange(obj);

				if (M.AutoSmite.Master)
					vis->AutoSmite(obj, M.AutoSmite.Slot, M.AutoSmite.Mode, M.AutoSmite.MouseSpeed);

				if (M.LastHit.Master)
					vis->LastHit(obj, RGBA(M.LastHit.Color[0] * 255, M.LastHit.Color[1] * 255, M.LastHit.Color[2] * 255, M.LastHit.Color[3] * 255));

				/*	ImVec2 RealPos = Direct3D9.WorldToScreen(obj.GetPosition());
				float dmg = Local.GetTotalDamage(&obj);
				std::string str = obj.GetName() + " , " + std::to_string(dmg) + " , " + std::to_string(obj.Address());
				draw->String(str, RealPos.x, RealPos.y, centered, RGBA(255, 255, 255), fontTahoma);*/

			}
		}

		//inhib loop
		if (M.Inhibs.Master)
		{
			for (auto obj : init->inhiblist)
			{
				if (M.Inhibs.Master)
				{
					vis->InhibTimers(&obj);
				}

			}
		}

		
		if (!missileList.empty())
		{
			for (auto xdd : missileList)
			{
				CObject a(xdd);
				ImVec2 RealPos = Direct3D9.WorldToScreen(a.GetPosition());
				std::string str = a.GetName() + " , " + std::to_string(a.Address());
				//clog.AddLog("%i , %s", (IsFunc(a.Address(), 0x1000)) ,a.GetName().c_str());
				//if(a.IsBuilding())
				if (RealPos.x == 0 && RealPos.y == 0)
					continue;

				Vector3 StartPos = a.GetMissileStartPos();
				Vector3 EndPos = a.GetMissileEndPos();
				//clog.AddLog("%s", obj.GetName().c_str());
				ImVec2 RealStartPos = WorldToScreen(StartPos);
				ImVec2 RealEndPos = WorldToScreen(EndPos);

				if (RealStartPos.x == 0 && RealStartPos.y == 0)
					continue;

				if (RealEndPos.x == 0 && RealEndPos.y == 0)
					continue;


				draw->Line(RealStartPos.x, RealStartPos.y, RealEndPos.x, RealEndPos.y, RGBA(255, 255, 255));


				draw->String(str, RealPos.x, RealPos.y, centered, RGBA(255, 255, 255), fontTahoma);
				//clog.AddLog("%i", IsFunc(a.Address(), 0x1000));
			}
		}

		//float GameTime = Memory.Read<float>(ClientAddress + oGameTime, sizeof(float));
		//if (xd)
		//{
		//	xd = false;
		//	xdtimer = GameTime + 300;

		//}
		//draw->String(std::to_string(xdtimer - GameTime), 100, 100, centered, RGBA(255, 255, 255),fontTahoma);
		


		//drawings test
		//draw->BoxFilled(200, 500, 100, 100, RGBA(255, 0, 0));
		//draw->BoxBordered(500, 500, 100, 100,RGBA(255, 0, 0));
		//draw->BoxOutlined(800, 500, 100, 100, RGBA(255, 0, 0));
		//draw->StringBoxed("asdfsdgSVX123!_", 700, 200, lefted, RGBA(255, 255, 255), fontTahoma, RGBA(1,0,0),RGBA(255,0,0));
		//draw->Circle(1100, 500, 100, RGBA(255, 0, 0));
		//draw->CircleFilled(1400, 500, 100, RGBA(255, 0, 0));
		

		g_pd3dDevice->EndScene();
	}
	HRESULT result = g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

	// Handle loss of D3D9 device
	if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
		ResetDevice();

	return 0;
}



void Direct3D9Render::Shutdown()
{
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
    if (g_pD3D) { g_pD3D->Release(); g_pD3D = NULL; }
	if (g_Line != NULL) { g_Line->Release(); }
	if (g_pVB != NULL) { g_pVB->Release(); }
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

}


void Direct3D9Render::InitializeFonts()
{

	D3DXCreateFontA(g_pd3dDevice, 14, 0, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial", &fontArial);
	D3DXCreateFontA(g_pd3dDevice, 16, 0, FW_BOLD, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Tahoma", &fontTahoma);
	D3DXCreateFontA(g_pd3dDevice, 12, 0, FW_BOLD, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Tahoma", &fontTahomaSmall);
}


void Direct3D9Render::Renderimgui(HWND hWnd)
{

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	
	// Setup Dear ImGui style
	MenuInit();

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX9_Init(g_pd3dDevice);

}

void Direct3D9Render::ResetDevice()
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
	HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
	if (hr == D3DERR_INVALIDCALL)
		IM_ASSERT(0);
	ImGui_ImplDX9_CreateDeviceObjects();
}




ImVec2 Direct3D9Render::WorldToScreen(Vector3 pos)
{
	DirectX::XMMATRIX matrix = GetViewProjectionMatrix();
	
	ImVec2 returnVec = ImVec2(0,0);

	Vector4 clipCoords;
	clipCoords.X = pos.X * matrix.r->m128_f32[0] + pos.Y * matrix.r->m128_f32[4] + pos.Z * matrix.r->m128_f32[8] + matrix.r->m128_f32[12];
	clipCoords.Y = pos.X * matrix.r->m128_f32[1] + pos.Y * matrix.r->m128_f32[5] + pos.Z * matrix.r->m128_f32[9] + matrix.r->m128_f32[13];
	clipCoords.Z = pos.X * matrix.r->m128_f32[2] + pos.Y * matrix.r->m128_f32[6] + pos.Z * matrix.r->m128_f32[10] + matrix.r->m128_f32[14];
	clipCoords.W = pos.X * matrix.r->m128_f32[3] + pos.Y * matrix.r->m128_f32[7] + pos.Z * matrix.r->m128_f32[11] + matrix.r->m128_f32[15];


	//clipCoords.X = pos.X * matrix.r->m128_f32[0] + pos.Y * matrix.r->m128_f32[1] + pos.Z * matrix.r->m128_f32[2] + matrix.r->m128_f32[3];
	//clipCoords.Y = pos.X * matrix.r->m128_f32[4] + pos.Y * matrix.r->m128_f32[5] + pos.Z * matrix.r->m128_f32[6] + matrix.r->m128_f32[7];
	//clipCoords.Z = pos.X * matrix.r->m128_f32[8] + pos.Y * matrix.r->m128_f32[9] + pos.Z * matrix.r->m128_f32[10] + matrix.r->m128_f32[11];
	//clipCoords.W = pos.X * matrix.r->m128_f32[12] + pos.Y * matrix.r->m128_f32[13] + pos.Z * matrix.r->m128_f32[14] + matrix.r->m128_f32[15];

	if (clipCoords.W < 0.1f) return returnVec;

	Vector3 M;
	M.X = clipCoords.X / clipCoords.W;
	M.Y = clipCoords.Y / clipCoords.W;
	M.Z = clipCoords.Z / clipCoords.W;

	returnVec.x = (SCREENWIDTH / 2.0f * M.X) + (M.X + SCREENWIDTH / 2.0f);
	returnVec.y = -(SCREENHEIGHT / 2.0f * M.Y) + (M.Y + SCREENHEIGHT / 2.0f);

	return returnVec;
}

void Direct3D9Render::MenuInit()
{

	ImGuiIO& io = ImGui::GetIO();
	io.IniFilename = ".\\configs\\imgui.ini";
	io.IniSavingRate = 7.f;
	io.ConfigWindowsResizeFromEdges = 0;

	ImGuiStyle& style = ImGui::GetStyle();

	//Main
	style.WindowPadding = ImVec2(4.f,4.f);
	style.FramePadding = ImVec2(3.f, 3.f);
	style.ItemSpacing = ImVec2(5.f, 5.f);
	style.ItemInnerSpacing = ImVec2(5.f, 5.f);
	style.TouchExtraPadding = ImVec2(0.f, 0.f);
	style.ScrollbarSize = 15.f;
	style.GrabMinSize = 15.f;
	//Borders
	style.WindowBorderSize = 1.f;
	style.ChildBorderSize = 1.f;
	style.PopupBorderSize = 1.f;
	style.FrameBorderSize = 1.f;
	style.TabBorderSize = 1.f;
	//Rounding
	style.WindowRounding = 0.f;
	style.ChildRounding = 0.f;
	style.FrameRounding = 0.f;
	style.PopupRounding = 0.f;
	style.ScrollbarRounding = 0.f;
	style.GrabRounding = 0.f;
	style.LogSliderDeadzone = 5.f;
	style.TabRounding = 0.f;
	//Aligment
	style.WindowTitleAlign = ImVec2(0.f, 0.f);
	style.WindowMenuButtonPosition = 0;
	style.ColorButtonPosition = 1;
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.SelectableTextAlign = ImVec2(0.5f, 0.5f);


	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.01f, 0.01f, 0.01f, 1.00f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.01f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.01f, 0.01f, 0.01f, 1.00f);
	colors[ImGuiCol_Border] = ImVec4(1.00f, 1.00f, 1.00f, 0.20f);
	colors[ImGuiCol_BorderShadow] = ImVec4(1.00f, 1.00f, 1.00f, 0.04f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.01f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.39f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.39f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.01f, 0.01f, 0.01f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.01f, 0.01f, 0.01f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.01f, 0.00f, 0.00f, 0.51f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.01f, 0.01f, 0.01f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.01f, 0.01f, 0.01f, 1.00f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.39f, 0.39f, 0.39f, 0.39f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.63f);
	colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.39f, 0.39f, 0.39f, 0.39f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.39f, 0.39f, 0.39f, 0.39f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.63f);
	colors[ImGuiCol_Header] = ImVec4(0.39f, 0.39f, 0.39f, 0.39f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	colors[ImGuiCol_HeaderActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.63f);
	colors[ImGuiCol_Separator] = ImVec4(0.39f, 0.39f, 0.39f, 0.39f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.63f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.39f, 0.39f, 0.39f, 0.39f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.63f);
	colors[ImGuiCol_Tab] = ImVec4(0.39f, 0.39f, 0.39f, 0.39f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	colors[ImGuiCol_TabActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.63f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);


}