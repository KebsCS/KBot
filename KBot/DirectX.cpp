#include <chrono>

#include "DirectX.h"
#include "Config.h"

bool Direct3D9Render::DirectXInit(HWND hWnd)
{
	if (D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_BGRA_SUPPORT, nullptr, 0, D3D11_SDK_VERSION, &g_pd3dDevice, nullptr, &g_pd3dDeviceContext) != S_OK)
		return false;

	IDXGIDevice* dxgiDevice;
	if (g_pd3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice) != S_OK)
		return false;
	IDXGIAdapter* dxgiAdapter = 0;
	if (dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter) != S_OK)
		return false;

	IDXGIFactory2* dxgiFactory = 0;
	if (dxgiAdapter->GetParent(__uuidof(IDXGIFactory2), (void**)&dxgiFactory) != S_OK)
		return false;

	// Create swap chain with alpha mode premultiplied
	DXGI_SWAP_CHAIN_DESC1 description = {};
	description.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	description.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	description.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	description.BufferCount = 2;
	description.SampleDesc.Count = 1;
	description.AlphaMode = DXGI_ALPHA_MODE_PREMULTIPLIED;
	description.Scaling = DXGI_SCALING_STRETCH;
	RECT rect = {};
	GetClientRect(hWnd, &rect);
	description.Width = rect.right - rect.left;
	description.Height = rect.bottom - rect.top;

	if (dxgiFactory->CreateSwapChainForComposition(dxgiDevice,
		&description,
		nullptr,
		&g_pSwapChain) != S_OK)
		return false;

	// Create Direct Composition layer
	IDCompositionDevice* dcompDevice;
	DCompositionCreateDevice(
		dxgiDevice,
		__uuidof(dcompDevice),
		reinterpret_cast<void**>(&dcompDevice));

	IDCompositionTarget* target;
	dcompDevice->CreateTargetForHwnd(hWnd,
		true,
		&target);

	IDCompositionVisual* visual;
	dcompDevice->CreateVisual(&visual);
	visual->SetContent(g_pSwapChain);
	target->SetRoot(visual);
	dcompDevice->Commit();

	if (!CreateRenderTarget())
		return false;

	Renderimgui(hWnd);

	if (!init->Start())
		return false;

	return true;
}

// Helper to display a little (?) mark which shows a tooltip when hovered.
// In your own code you may want to display an actual icon if you are using a merged icon fonts (see docs/FONTS.md)
void Direct3D9Render::HelpMarker(const char* desc)
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

void Direct3D9Render::HotkeyButton(int& key, bool mouse)
{
	char buf[128];

	sprintf(buf, XorStr("[ %s ]"), key ? keyNames[key] : "key");

	ImGui::Button(buf, ImVec2(90, 20));

	if (!ImGui::IsItemHovered())
		return;

	ImGui::SetTooltip("Press any key to change keybind");
	ImGuiIO& io = ImGui::GetIO();
	for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++)
		if (ImGui::IsKeyPressed(i) && i != M.Misc.MenuKey)
			key = i != VK_ESCAPE ? i : 0;

	if (mouse)
	{
		for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++)
			if (ImGui::IsMouseDown(i) && i + (i > 1 ? 2 : 1) != M.Misc.MenuKey)
				key = i + (i > 1 ? 2 : 1);
	}
}

void Direct3D9Render::ArrowButtonDisabled(const char* id, ImGuiDir dir)
{
	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
	ImGui::ArrowButton(id, dir);
	ImGui::PopStyleVar();
}

void Direct3D9Render::StartFrame()
{
	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void Direct3D9Render::EndFrame()
{
	// Rendering
	ImVec4 clear_color = ImVec4(0.f, 0.f, 0.f, 0.f);
	ImGui::EndFrame();
	ImGui::Render();
	g_pd3dDeviceContext->OMSetRenderTargets(1, &g_pd3dRenderTargetView, NULL);
	g_pd3dDeviceContext->ClearRenderTargetView(g_pd3dRenderTargetView, (float*)&clear_color);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	g_pSwapChain->Present(1, 0); // Present with vsync
	//g_pSwapChain->Present(0, 0); // Present without vsync
}

float processTimeMs = 0;
int Direct3D9Render::Render()
{
	auto timeBefore = std::chrono::high_resolution_clock::now();

	if (M.bConsoleOpen)
	{
		clog.Draw("Console Log", &M.bConsoleOpen);
	}

	if (M.bMenuOpen)
	{
		char buf[128];
		if (M.bDebug)
			sprintf(buf, XorStr("KBot %.1f FPS ###AnimatedTitle"), ImGui::GetIO().Framerate);
		else
			sprintf(buf, XorStr("KBot"));
		ImGui::SetNextWindowPos(ImVec2(904, 349), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(419, 421), ImGuiCond_FirstUseEver);
		ImGui::Begin(buf);// , ImGuiWindowFlags_AlwaysAutoResize);
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem(XorStr("Open Console")))
				M.bConsoleOpen = true;
			if (ImGui::MenuItem(XorStr("Exit")))
				M.bExitBot = true;
			ImGui::EndPopup();
		}

		ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_Reorderable;
		ImGuiTreeNodeFlags collapsing_header_flags = ImGuiTreeNodeFlags_DefaultOpen;
		if (ImGui::BeginTabBar("TabBar", tab_bar_flags))
		{
			//if (ImGui::BeginTabItem("Main"))
			//{
			//	ImGui::Separator();
			//	ImGui::Columns(2, 0, false);
			//	draw->ImageFromMemory(draw->textureFlash, 0, 0, "", 99, 32, 32, true);
			//	draw->ImageFromMemory(draw->textureSmite, 0, 0, "", 99, 32, 32, true);
			//	draw->ImageFromMemory(draw->textureIgnite, 0, 0, "", 99, 32, 32, true);
			//	draw->ImageFromMemory(draw->textureTeleport, 0, 0, "", 99, 32, 32, true);
			//	draw->ImageFromMemory(draw->textureHeal, 0, 0, "", 99, 32, 32, true);
			//	ImGui::NextColumn();
			//	draw->ImageFromMemory(draw->textureExhaust, 0, 0, "", 99, 32, 32, true);
			//	draw->ImageFromMemory(draw->textureBarrier, 0, 0, "", 99, 32, 32, true);
			//	draw->ImageFromMemory(draw->textureCleanse, 0, 0, "", 99, 32, 32, true);
			//	draw->ImageFromMemory(draw->textureGhost, 0, 0, "", 99, 32, 32, true);
			//	draw->ImageFromMemory(draw->textureClarity, 0, 0, "", 99, 32, 32, true);
			//	ImGui::Columns(1);

			//	ImGui::EndTabItem();
			//}
			if (ImGui::BeginTabItem("Visuals"))
			{
				if (ImGui::CollapsingHeader("Enemies", collapsing_header_flags))
				{
					ImGui::Checkbox("AA Range", &M.AARange.Master);
					//if (ImGui::IsItemHovered())
					//	ImGui::SetTooltip("test");
					ImGui::SameLine();
					ImGui::ColorEdit4("AARangeColor##3", (float*)&M.AARange.Color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoAlpha);
					ImGui::SameLine();

					ImGui::SliderInt2("", M.AARange.Slider, 10, 60, "%d");
					ImGui::Separator();

					ImGui::Checkbox("Tracers\t", &M.Tracers.Master);
					ImGui::SameLine();
					ImGui::SliderFloat("##TracersThickness", &M.Tracers.Thickness, 0.1f, 10.f, "Thickness: %lg", 1.f);
					ImGui::Separator();

					ImGui::Checkbox("Show mouse clicks", &M.Enemies.MouseClicks);
					ImGui::Separator();

					ImGui::Checkbox("Gank Alerter", &M.GankAlerter.Master);
					ImGui::Separator();
					ImGui::Columns(2, 0, false);
					ImGui::Checkbox("Cooldowns", &M.Cooldowns.Master);
					ImGui::NextColumn();
					ImGui::Checkbox("Scoreboard Window", &M.Cooldowns.ScoreboardWnd);
					ImGui::Columns(1);

					if (M.Cooldowns.Master)
					{
						ImGui::Selectable("Text on champ", &M.Cooldowns.Type[0]);
						ImGui::Selectable("Image on champ", &M.Cooldowns.Type[1]);
						ImGui::Selectable("Scoreboard", &M.Cooldowns.Type[2]);
					}

					if (M.Cooldowns.ScoreboardWnd)
					{
						ImGui::SetNextWindowPos(ImVec2(1324, 502), ImGuiCond_FirstUseEver);
						ImGui::SetNextWindowSize(ImVec2(437, 301), ImGuiCond_FirstUseEver);

						if (ImGui::Begin("Scoreboard##Window", &M.Cooldowns.ScoreboardWnd))
						{
							ImGui::Columns(2, 0, false);
							ImGui::Checkbox("Summoner spells", &M.Cooldowns.Scoreboard.Summs);
							ImGui::Checkbox("Experience", &M.Cooldowns.Scoreboard.Exp);
							ImGui::NextColumn();
							ImGui::Checkbox("Ultimates", &M.Cooldowns.Scoreboard.Ults);
							ImGui::Checkbox("Items", &M.Cooldowns.Scoreboard.Items);
							ImGui::Columns(1);

							ImGui::Separator();

							ImGui::Text("Alignment:");
							ImGui::Columns(2, 0, false);
							for (int n = 0; n < 10; n++)
							{
								ImGui::PushID(n);
								if ((n % 2) != 0)
									ImGui::SameLine();
								ImGui::Button(M.ScoreBoard[n].first.c_str(), ImVec2(100, 25));

								// Our buttons are both drag sources and drag targets here!
								if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
								{
									// Set payload to carry the index of our item (could be anything)
									ImGui::SetDragDropPayload("DND_DEMO_CELL", &n, sizeof(int));

									// Display preview (could be anything, e.g. when dragging an image we could decide to display
									// the filename and a small preview of the image, etc.)
									ImGui::Text("Swap %s", M.ScoreBoard[n].first.c_str());
									ImGui::EndDragDropSource();
								}
								if (ImGui::BeginDragDropTarget())
								{
									if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_DEMO_CELL"))
									{
										IM_ASSERT(payload->DataSize == sizeof(int));
										int payload_n = *(const int*)payload->Data;
										std::pair<std::string, DWORD> tmp = M.ScoreBoard[n];
										M.ScoreBoard[n] = M.ScoreBoard[payload_n];
										M.ScoreBoard[payload_n] = tmp;
									}
									ImGui::EndDragDropTarget();
								}
								ImGui::PopID();
							}
							ImGui::NextColumn();
							if (ImGui::Button(M.Cooldowns.Scoreboard.ShowScoreboard ? "Hide Scoreboard" : "Show Scoreboard", ImVec2(200, 50)))
							{
								SetForegroundWindow(FindWindowA(0, XorStr("League of Legends (TM) Client")));
								std::this_thread::sleep_for(std::chrono::milliseconds(100));
								//keyboard->GenerateKeyScancode(DIK_O, false);
								M.Cooldowns.Scoreboard.ShowScoreboard = !M.Cooldowns.Scoreboard.ShowScoreboard;
							}
							if (ImGui::Button("Swap", ImVec2(200, 50)))
							{
								for (int i = 0; i < 10; i += 2)
									std::swap(M.ScoreBoard[i], M.ScoreBoard[i + 1]);
							}

							ImGui::Columns(1);

							ImGui::Separator();
							const int u16_one = 1;
							ImGui::InputScalar("Position X", ImGuiDataType_U16, &M.Cooldowns.Scoreboard.Pos[0], &u16_one, NULL, "%u");
							ImGui::InputScalar("Position Y", ImGuiDataType_U16, &M.Cooldowns.Scoreboard.Pos[1], &u16_one, NULL, "%u");
						}
						ImGui::End();
					}
				}

				if (ImGui::CollapsingHeader("Local", collapsing_header_flags))
				{
					ImGui::Checkbox("AA Range##Local", &M.AARange.Local);
					ImGui::SameLine();
					ImGui::ColorEdit4("AARangeLocalColor##3", (float*)&M.AARange.LocalColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoAlpha);
					ImGui::Separator();

					ImGui::Checkbox("LastHit Helper", &M.LastHit.Master);
					ImGui::SameLine(); HelpMarker("Visual indicator, when to lasthit");
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
				ImGui::Checkbox("Enable ##Evade", &M.Evade.Master);
				ImGui::SameLine(); ImGui::Checkbox("Smooth", &M.Evade.Smooth);
				ImGui::SliderInt("Average Game Ping", &M.Evade.GP, 0, 250);
				ImGui::Checkbox("Missile", &M.Evade.Missile);
				ImGui::SameLine(); HelpMarker("Uses missile list for better detection (laggy)");
				ImGui::Checkbox("Draw Spells", &M.Evade.Draw);
				ImGui::Checkbox("Limit Range", &M.Evade.LimitRange);
				ImGui::SameLine(); HelpMarker("Don't scan missiles at long range");
				ImGui::SliderFloat("##EvadeLR", &M.Evade.LR, 500.f, 10000.f, "Max Range: %lg", 1.f);
				ImGui::SliderInt("Diagonal Search Step", &M.Evade.DS, 5, 100);
				ImGui::SameLine(); HelpMarker("How far away are the detected points apart from each other");
				ImGui::SliderInt("Diagonal Points Count", &M.Evade.DC, 1, 8);
				ImGui::SameLine(); HelpMarker("How many evade points to detect");
				ImGui::Checkbox("Return mouse to original position", &M.Evade.MouseBack);
				ImGui::Checkbox("Force ##Evade", &M.Evade.Force);
				ImGui::Checkbox("On Key ##Evade", &M.Evade.OnKey);
				HotkeyButton(M.Evade.EvadeKey);
				ImGui::SameLine(); ImGui::Text("Evade Key");
				//draw->ImageFromMemory(draw->textureKEKW, 0, 0, "", 99, 256, 256, true);

				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Orbwalker"))
			{
				ImGui::Checkbox("Enable##Orbwalker", &M.Orbwalker.Master);
				//ImGui::Combo("Hold Key", &M.Orbwalker.HoldKey, keyNames, ARRAYSIZE(keyNames));
				HotkeyButton(M.Orbwalker.HoldKey);
				ImGui::SameLine(); ImGui::Text("Hold Key");
				//ArrowButtonDisabled("asdas", ImGuiDir_Down);
				//ImGui::ArrowButton("asd", ImGuiDir_Up);
				//draw->ImageFromMemory(draw->textureSks, 0, 0, "", 99, 256 * 1.25, 256 * 1.25, true);

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
				HotkeyButton(M.Misc.MenuKey);
				ImGui::SameLine(); ImGui::Text("Menu Key");

				ImGui::Separator();
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem(M.sChampion.c_str()))
			{
				if (championScript)
					championScript->GUI();
				else
					ImGui::Text(XorStr("No available scripts for this champion"));

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

	if (M.dwStartTime + 6000 > GetTickCount())
	{
		ImGui::SetNextWindowPos(ImVec2(10, 10));
		bool openinfo = true;
		ImGui::Begin("##info", &openinfo, ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs);
		{
			if (!M.sServerInfo.empty())
				ImGui::Text(M.sServerInfo.c_str());
			else
				M.sServerInfo = "";
		}
		ImGui::End();
	}

	std::chrono::duration<float, std::milli> timeDuration = std::chrono::high_resolution_clock::now() - timeBefore;
	processTimeMs = timeDuration.count();

	return 1;
}

void Direct3D9Render::Shutdown()
{
	CleanupRenderTarget();
	if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = NULL; }
	if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = NULL; }
	if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
}

bool Direct3D9Render::CreateRenderTarget()
{
	ID3D11Resource* pBackBuffer;
	if (S_OK != g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer)))
		return false;
	if (S_OK != g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_pd3dRenderTargetView))
		return false;
	pBackBuffer->Release();
	return true;
}

void Direct3D9Render::CleanupRenderTarget()
{
	if (g_pd3dRenderTargetView) { g_pd3dRenderTargetView->Release(); g_pd3dRenderTargetView = NULL; }
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
	ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);
}

void Direct3D9Render::MenuInit()
{
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.IniFilename = ".\\configs\\imgui.ini";
	io.IniSavingRate = 7.f;
	io.ConfigWindowsResizeFromEdges = 0;

	/*if (PWSTR fontPath; SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Fonts, 0, nullptr, &fontPath)))
	{
		const std::filesystem::path path = fontPath;
		CoTaskMemFree(fontPath);

		static constexpr ImWchar ranges[]{ 0x0020, 0xFFFF, 0 };
		ImFontConfig cfg;
		cfg.OversampleV = 3;
		io.FontDefault = NULL;
		fontArial = io.Fonts->AddFontFromFileTTF((path / "arial.ttf").string().c_str(), 32.f, &cfg, ranges);
		fontTahoma = io.Fonts->AddFontFromFileTTF((path / "tahoma.ttf").string().c_str(), 16.f, &cfg, ranges);
		fontTahomaSmall = io.Fonts->AddFontFromFileTTF((path / "tahoma.ttf").string().c_str(), 12.f, &cfg, ranges);
	}*/

	io.FontDefault = NULL;

	ImGuiStyle& style = ImGui::GetStyle();

	//Main
	style.WindowPadding = ImVec2(4.f, 4.f);
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
	//Alignment
	style.WindowTitleAlign = ImVec2(0.f, 0.f);
	style.WindowMenuButtonPosition = 0;
	style.ColorButtonPosition = 1;
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.SelectableTextAlign = ImVec2(0.5f, 0.5f);
	//AntiAliasing
	style.AntiAliasedLines = false;
	style.AntiAliasedLinesUseTex = false;
	style.AntiAliasedFill = false;

	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.50f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.01f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.50f);
	colors[ImGuiCol_Border] = ImVec4(1.00f, 1.00f, 1.00f, 0.20f);
	colors[ImGuiCol_BorderShadow] = ImVec4(1.00f, 1.00f, 1.00f, 0.04f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.01f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.39f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.39f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.50f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 0.00f, 0.00f, 0.50f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.01f, 0.00f, 0.00f, 0.51f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.50f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.50f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.39f, 0.39f, 0.39f, 0.39f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 0.50f);
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

DirectX::XMMATRIX Direct3D9Render::ReadMatrix(DWORD address)
{
	DirectX::XMMATRIX tmp;
	for (int i = 0; i < 16; i++) //todo 1 read instead of 16 (performance)
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
	return ReadMatrix(ClientAddress + Offsets::oViewMatrix);
}
DirectX::XMMATRIX Direct3D9Render::GetProjectionMatrix()
{
	//DWORD Renderer = Memory.Read<DWORD>(ClientAddress + oRenderer, sizeof(DWORD));
	return ReadMatrix(ClientAddress + Offsets::oViewMatrix + 0x40);
}

void Direct3D9Render::GetViewProjectionMatrix()
{
#ifndef NOLEAGUE

	M.Matrix = DirectX::XMMatrixMultiply(GetViewMatrix(), GetProjectionMatrix());

#endif // !NOLEAGUE
}

ImVec2 Direct3D9Render::WorldToScreen(const Vector3& pos) const
{
	DirectX::XMMATRIX matrix = M.Matrix;

	ImVec2 returnVec = ImVec2(0, 0);

	Vector4 clipCoords;
	clipCoords.x = pos.x * matrix.r->m128_f32[0] + pos.y * matrix.r->m128_f32[4] + pos.z * matrix.r->m128_f32[8] + matrix.r->m128_f32[12];
	clipCoords.y = pos.x * matrix.r->m128_f32[1] + pos.y * matrix.r->m128_f32[5] + pos.z * matrix.r->m128_f32[9] + matrix.r->m128_f32[13];
	clipCoords.z = pos.x * matrix.r->m128_f32[2] + pos.y * matrix.r->m128_f32[6] + pos.z * matrix.r->m128_f32[10] + matrix.r->m128_f32[14];
	clipCoords.w = pos.x * matrix.r->m128_f32[3] + pos.y * matrix.r->m128_f32[7] + pos.z * matrix.r->m128_f32[11] + matrix.r->m128_f32[15];

	//clipCoords.X = pos.X * matrix.r->m128_f32[0] + pos.Y * matrix.r->m128_f32[1] + pos.Z * matrix.r->m128_f32[2] + matrix.r->m128_f32[3];
	//clipCoords.Y = pos.X * matrix.r->m128_f32[4] + pos.Y * matrix.r->m128_f32[5] + pos.Z * matrix.r->m128_f32[6] + matrix.r->m128_f32[7];
	//clipCoords.Z = pos.X * matrix.r->m128_f32[8] + pos.Y * matrix.r->m128_f32[9] + pos.Z * matrix.r->m128_f32[10] + matrix.r->m128_f32[11];
	//clipCoords.W = pos.X * matrix.r->m128_f32[12] + pos.Y * matrix.r->m128_f32[13] + pos.Z * matrix.r->m128_f32[14] + matrix.r->m128_f32[15];

	if (clipCoords.w < 0.1f) return returnVec;

	Vector3 temp;
	temp.x = clipCoords.x / clipCoords.w;
	temp.y = clipCoords.y / clipCoords.w;
	temp.z = clipCoords.z / clipCoords.w;

	returnVec.x = (SCREENWIDTH / 2.0f * temp.x) + (temp.x + SCREENWIDTH / 2.0f);
	returnVec.y = -(SCREENHEIGHT / 2.0f * temp.y) + (temp.y + SCREENHEIGHT / 2.0f);

	return returnVec;
}

ImVec2 Direct3D9Render::WorldToMinimap(const Vector3& pos) const
{
	const ImVec2 minimapPos = ImVec2(M.fMinimapPos[0], M.fMinimapPos[1]);
	const ImVec2 minimapSize = ImVec2(M.fMinimapSize[0], M.fMinimapSize[1]);
	ImVec2 result = { pos.x / 15000.f, pos.z / 15000.f };
	result.x = minimapPos.x + result.x * minimapSize.x;
	result.y = minimapPos.y + minimapSize.y - (result.y * minimapSize.y);

	return result;
}