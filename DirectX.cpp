#include "DirectX.h"


#include "Draw.h"
#include "Visuals.h"


LPDIRECT3D9              g_pD3D = NULL;
LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
D3DPRESENT_PARAMETERS    g_d3dpp = {};
ID3DXLine* g_Line = NULL;
LPDIRECT3DVERTEXBUFFER9  g_pVB = NULL;

RECT rc;
ID3DXFont* pFont;


Draw* draw;
Visuals vis;


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
	//in IDA
	//B9 ? ? ? ? E8 ? ? ? ? B9 ? ? ? ? E9 ? ? ? ? CC CC CC CC CC CC CC CC 

	//DWORD Renderer = Memory.Read<DWORD>(ClientAddress + oRenderer, sizeof(DWORD));
	return ReadMatrix(ClientAddress + 0x353DF38); // 10.21 0x3529CC8
}
DirectX::XMMATRIX Direct3D9Render::GetProjectionMatrix()
{

	//DWORD Renderer = Memory.Read<DWORD>(ClientAddress + oRenderer, sizeof(DWORD));
	return ReadMatrix(ClientAddress + 0x353DF38 +0x40);
}

DirectX::XMMATRIX Direct3D9Render::GetViewProjectionMatrix()
{

	return DirectX::XMMatrixMultiply(GetViewMatrix(), GetProjectionMatrix());
}


bool Direct3D9Render::DirectXInit(HWND hWnd)
{

	if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
	{
		clog.AddLog("[error] Direct3DCreate9Ex failed");

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
	g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	g_d3dpp.MultiSampleType = D3DMULTISAMPLE_NONMASKABLE;


	if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0)
	{
		clog.AddLog("[error] CreateDeviceEx failed");

		return false;
	}

	if (FAILED(D3DXCreateLine(g_pd3dDevice, &g_Line)))
	{
		clog.AddLog("[error] D3DXCreateLine failed");
		return false;
	}


	g_Line->SetWidth(1.0f);
	g_Line->SetPattern(0xFFFFFFFF);
	g_Line->SetAntialias(false);

	InitializeFonts();
	
	Renderimgui(hWnd);

	float GameTime = Memory.Read<float>(ClientAddress + oGameTime, sizeof(float));

	while (GameTime < 1) // pause if not in game
	{
		Sleep(1);
	}
	Sleep(2000);



	init.AddObjects();
	
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

static DWORD MinionList = Memory.Read<DWORD>(ClientAddress + oMinionList);



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
		//ImGui::SetNextWindowPos(ImVec2(100, 300), ImGuiCond_FirstUseEver);
		ImGui::Begin(buf, &M.MenuOpen, ImGuiWindowFlags_AlwaysAutoResize);
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Open Console"))
				bConsoleLog = true;
			ImGui::EndPopup();
		}
		
		ImGui::Checkbox("AA Range", &M.AARange.Master);
		ImGui::SameLine();
		ImGui::ColorEdit4("AARangeColor##3", (float*)&M.AARange.Color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoAlpha);
		ImGui::SameLine();
		ImGui::Checkbox("Local Player", &M.AARange.Local);
		ImGui::SameLine();
		ImGui::ColorEdit4("AARangeLocalColor##3", (float*)&M.AARange.LocalColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoAlpha);
		ImGui::SameLine();
		ImGui::Checkbox("Turrets", &M.AARange.Turrets);

	
		ImGui::SliderInt2("", M.AARange.Slider, 10, 60,"%d");
		ImGui::Separator();

		ImGui::Checkbox("Tracers\t", &M.Tracers.Master);
		ImGui::SameLine();
		ImGui::SliderInt("", &M.Tracers.Thickness, 10, 60, "Thickness: %d ");
		ImGui::Separator();

		ImGui::Checkbox("Cooldowns", &M.Cooldowns.Master);

		ImGui::Separator();


		ImGui::Checkbox("LastHit Helper", &M.LastHit.Master);
		ImGui::SameLine();
		ImGui::ColorEdit4("LastHitColor##3", (float*)&M.LastHit.Color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoAlpha);

		ImGui::Separator();

		ImGui::SliderInt("AntiLag", &M.AntiLag, 0, 50);
		ImGui::SameLine(); HelpMarker("Higher for slower PCs");

		ImGui::Separator();

		ImGui::Checkbox("Auto Smite", &M.AutoSmite.Master);

		ImGui::Separator();


		ImGui::Columns(2, 0, false);
		draw->Image("Flash.png", 0, 0, "", 99, 32, 32, true);
		draw->Image("Smite.png", 0, 0, "", 99, 32, 32, true);
		draw->Image("Ignite.png", 0, 0, "", 99, 32, 32, true);
		draw->Image("Teleport.png", 0, 0, "", 99, 32, 32, true);
		draw->Image("Heal.png", 0, 0, "", 99, 32, 32, true);
		ImGui::NextColumn();
		draw->Image("Exhaust.png", 0, 0, "", 99, 32, 32, true);
		draw->Image("Barrier.png", 0, 0, "", 99, 32, 32, true);
		draw->Image("Cleanse.png", 0, 0, "", 99, 32, 32, true);
		draw->Image("Ghost.png", 0, 0, "", 99, 32, 32, true);
		draw->Image("Clarity.png", 0, 0, "", 99, 32, 32, true);
		ImGui::Columns(1);
		ImGui::End();

		//ImGui::ShowDemoWindow();
		
	}

	if (bConsoleLog)
	{
		clog.Draw("Console Log", &bConsoleLog);
	}



	// Rendering
	ImGui::EndFrame();
	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
	if (g_pd3dDevice->BeginScene() >= 0)
	{
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

		//turret loop
		if (M.AARange.Turrets)
		{
			for (auto obj : init.turretlist)
			{
				if(M.AARange.Turrets)
					vis.DrawAARanges(obj, M.AARange.Slider[0], M.AARange.Slider[1] / 10.f, RGBA(M.AARange.Color[0] * 255, M.AARange.Color[1] * 255, M.AARange.Color[2] * 255, M.AARange.Color[3] * 255),
						false, RGBA(M.AARange.LocalColor[0] * 255, M.AARange.LocalColor[1] * 255, M.AARange.LocalColor[2] * 255, M.AARange.LocalColor[3] * 255));
			}
		}

		//hero loop
		if (M.Cooldowns.Master || M.AARange.Master || M.Tracers.Master)
		{
			for (auto obj : init.herolist)
			{
				if (M.Cooldowns.Master)
					vis.CooldownTimers(obj, 1);

				if (M.AARange.Master)
					vis.DrawAARanges(obj, M.AARange.Slider[0], M.AARange.Slider[1] / 10.f, RGBA(M.AARange.Color[0] * 255, M.AARange.Color[1] * 255, M.AARange.Color[2] * 255, M.AARange.Color[3] * 255),
						M.AARange.Local, RGBA(M.AARange.LocalColor[0] * 255, M.AARange.LocalColor[1] * 255, M.AARange.LocalColor[2] * 255, M.AARange.LocalColor[3] * 255));

				if (M.Tracers.Master)
					vis.DrawTracers(obj, M.Tracers.Thickness / 10.f);

				//float dist = obj.GetDistToMe(Local);
				//clog.AddLog("%s , %f", obj.GetName(), dist);

			}
		}
		
		if (M.LastHit.Master || M.AutoSmite.Master)
		{
			DWORD MinionArray = Memory.Read<DWORD>(MinionList + 0x04);
			int MinionLength = Memory.Read<int>(MinionList + 0x08);
			for (int i = 0; i < MinionLength * 4; i += 4)
			{
				CObject minion(Memory.Read<DWORD>(MinionArray + i)); // finds minions faster when its in loop

				if(M.AutoSmite.Master)
					vis.AutoSmite(minion);
				//clog.AddLog("%s , %x ", minion.GetName().c_str(), minion.Address());
				if (M.LastHit.Master)
					vis.LastHit(minion, RGBA(M.LastHit.Color[0] * 255, M.LastHit.Color[1] * 255, M.LastHit.Color[2] * 255, M.LastHit.Color[3] * 255));

			}
		}

		
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

	ImGui_ImplDX9_Init(g_pd3dDevice);

	ImGuiIO& io = ImGui::GetIO();
	io.IniFilename = "imgui.ini";
	io.IniSavingRate = 10.f;
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