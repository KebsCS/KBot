#include "Visuals.h"

LPDIRECT3D9              g_pD3D = NULL;
LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
D3DPRESENT_PARAMETERS    g_d3dpp = {};
ID3DXLine* g_Line = NULL;
LPDIRECT3DVERTEXBUFFER9  g_pVB = NULL;

RECT rc;
ID3DXFont* pFont;

Visuals* vis;

extern std::vector<CObject>minionList;

extern DWORD GetFirst();
extern DWORD GetNext(DWORD a2);
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
	return ReadMatrix(ClientAddress + oViewMatrix);
}
DirectX::XMMATRIX Direct3D9Render::GetProjectionMatrix()
{
	//DWORD Renderer = Memory.Read<DWORD>(ClientAddress + oRenderer, sizeof(DWORD));
	return ReadMatrix(ClientAddress + oViewMatrix + 0x40);
}

void Direct3D9Render::GetViewProjectionMatrix()
{
#ifndef NOLEAGUE

	M.Matrix = DirectX::XMMatrixMultiply(GetViewMatrix(), GetProjectionMatrix());

#endif // !NOLEAGUE
}

bool Direct3D9Render::DirectXInit(HWND hWnd)
{
	if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
	{
		MessageBoxA(0, " Direct3DCreate9 Failed", 0, 0);

		return false;
	}

	ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
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
		MessageBoxA(0, " CreateDevice Failed", 0, 0);

		return false;
	}

	if (FAILED(D3DXCreateLine(g_pd3dDevice, &g_Line)))
	{
		MessageBoxA(0, " D3DXCreateLine Failed", 0, 0);
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
		clog.AddLog(XorStr("[start] Initialized textures"));
	else clog.AddLog(XorStr("[error] Failed to initialize textures"));

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

//bool IsTroyFunc(DWORD a1)todo
//{
//	int v1; // esi
//	byte v3; // edi
//	int v5; // eax
//	uint8_t v6; // al
//	unsigned int v7; // ecx
//	char v8; // edx
//	char v9; // al
//
//	v1 = a1;
//	if (!a1)
//		return 0;
//	v3 = Memory.Read<uint8_t>(a1 + 5);
//	a1 = Memory.Read<DWORD>(a1 + 4 * Memory.Read<uint8_t>(a1 + 12) + 16);
//	if (v3)
//	{
//		v5 = Memory.Read<int>(v1 + 8);
//		a1 ^= ~v5;
//	}
//	v6 = Memory.Read<byte>(v1 + 6);
//	if (v6)
//	{
//		v7 = 4 - v6;
//		if (v7 < 4)
//		{
//			v8 = (char)(v7 + v1 + 8);
//			do
//			{
//				v9 = Memory.Read<char>(v8);
//				v8++;
//				a1 = Memory.Read<DWORD>(Memory.Read<byte>(a1) + v7);
//				a1 ^= ~v9;
//				v7++;
//			} while (v7 < 4);
//		}
//	}
//	return a1 == ClientAddress + 0x3508318;
//}

void Direct3D9Render::HeroLoop()
{
	//hero loop
	if (M.Cooldowns.Master || M.AARange.Master || M.Tracers.Master || M.GankAlerter.Master || M.Talon.DmgCalc)
	{
		for (auto obj : init->herolist)
		{
			if (M.Cooldowns.Master)
			{
				//todo store cooldowns in object class
				if (M.Cooldowns.Type[0] || M.Cooldowns.Type[1])
					vis->CooldownTimers(obj);
				//if master + tab /  scoreboard window + show scoreboard
				if ((M.Cooldowns.Type[2] && ((PressedKey(VK_TAB))) || (M.Cooldowns.ScoreboardWnd && M.Cooldowns.Scoreboard.ShowScoreboard)))
					vis->ScoreBoard(obj);
			}

			if (M.AARange.Master)
				vis->DrawAARanges(obj, M.AARange.Slider[0], M.AARange.Slider[1] / 10.f, RGBA(M.AARange.Color[0], M.AARange.Color[1], M.AARange.Color[2], M.AARange.Color[3]),
					false, RGBA(M.AARange.LocalColor[0], M.AARange.LocalColor[1], M.AARange.LocalColor[2], M.AARange.LocalColor[3]), false);

			if (M.Tracers.Master)
				vis->DrawTracers(obj, M.Tracers.Thickness);

			if (M.GankAlerter.Master)
				vis->GankAlerter(obj);

			if (M.Talon.Master)
			{
				if (M.Talon.DmgCalc)
					vis->TalonDamageCalc(obj);
			}

			//clog.AddLog("%s %i", obj.GetChampName().c_str(), obj.GetNetworkID());

			//float dist = obj.GetDistToMe(Local);
			//clog.AddLog("%s , %f", obj.GetName(), dist);
			//float dmg = Local.GetTotalDamage(&obj);
			//ImVec2 RealPos = Direct3D9.WorldToScreen(obj.GetPosition());
			//std::string str = obj.GetName() + " , " + std::to_string(dmg);
			//draw->String(str, RealPos.x, RealPos.y, centered, RGBA(255, 255, 255), fontTahoma);

			//on spell cast
			if (M.Evade.Master)
				if (SpellCastInfo spell(obj.GetSpellCastInfo()); spell.Address() != 0)
				{
					/*for (int i = 0; i < 2000; i += 4)
					{
						float temp = Memory.Read<float>(spell.GetSpellData() + i);
						clog.AddLog("%x ,  %s : %f , %x", spell.Address(), spell.GetName().c_str(), temp, i);
					}*/

					//data
					//3b4 range
					//268 to 280 cooldown in seconds per lvl
					//518 to 52c mana cost per lvl
					//244 cast time
					//440 maybe speed
					//ac to c4 damage
					//1dc ad/ap scaling
					//c8 to e0 slow maybe

					if (utils->StringContains(spell.GetName(), "basic", true))
						continue;

					clog.AddLog("%d", spell.GetIndex());
					Vector3 StartPos = spell.GetMissileStartPos();
					ImVec2 RealStartPos = WorldToScreen(StartPos);
					if (RealStartPos.x == 0 || RealStartPos.y == 0)
						continue;
					Vector3 EndPos = spell.GetMissileEndPos();

					//todo if bUseList don't draw here but
					//enable missile map for x seconds and draw there
					float SpellRange = -1;
					bool Circular = 0;
					float SpellWidth = -1;
					float Speed = -1;
					bool Targeted = 0;
					bool Cone = 0;
					//todo idk if all champs are correct
					int Slot = spell.GetSlot();
					bool UseLocal = 0;

					if (Slot == 0) // q
					{
						SpellRange = obj.spell.Q.fRange;
						Circular = obj.spell.Q.bCircular;
						SpellWidth = obj.spell.Q.fWidth;
						Speed = obj.spell.Q.fSpeed;
						Targeted = obj.spell.Q.bTargeted;
						Cone = obj.spell.Q.bCone;
						UseLocal = obj.spell.Q.bLocal;
					}
					else if (Slot == 1) // W
					{
						SpellRange = obj.spell.W.fRange;
						Circular = obj.spell.W.bCircular;
						SpellWidth = obj.spell.W.fWidth;
						Speed = obj.spell.W.fSpeed;
						Targeted = obj.spell.W.bTargeted;
						Cone = obj.spell.W.bCone;
						UseLocal = obj.spell.W.bLocal;
					}
					else if (Slot == 2)// e
					{
						SpellRange = obj.spell.E.fRange;
						Circular = obj.spell.E.bCircular;
						SpellWidth = obj.spell.E.fWidth;
						Speed = obj.spell.E.fSpeed;
						Targeted = obj.spell.E.bTargeted;
						Cone = obj.spell.E.bCone;
						UseLocal = obj.spell.E.bLocal;
					}
					else if (Slot == 3)//r
					{
						SpellRange = obj.spell.R.fRange;
						Circular = obj.spell.R.bCircular;
						SpellWidth = obj.spell.R.fWidth;
						Speed = obj.spell.R.fSpeed;
						Targeted = obj.spell.R.bTargeted;
						Cone = obj.spell.R.bCone;
						UseLocal = obj.spell.R.bLocal;
					}
					EndPos = Circular ? EndPos : StartPos.Extend(EndPos, SpellRange == -1 ? spell.GetRange() : SpellRange);
					if (UseLocal)
						EndPos = obj.GetPosition();
					ImVec2 RealEndPos = WorldToScreen(EndPos);
					if (RealEndPos.x == 0 || RealEndPos.y == 0)
						continue;

					/*	std::string str = spell.GetName() + " , " + std::to_string(spell.Address());
						draw->String(str, RealEndPos.x, RealEndPos.y, centered, RGBA(255, 255, 255), Direct3D9.fontTahoma);*/
					if (!Targeted)
						if (Circular)
						{
							if (!UseLocal)
							{
								EndPos.y -= 100;
								SpellWidth -= 50;
							}
							SpellWidth += 50;
							draw->CircleRange(EndPos, 12, SpellWidth == -1 ? 50/*todo get width*/ : SpellWidth, RGBA(255, 255, 255));
						}
						else
						{
							draw->Line(RealStartPos, RealEndPos, RGBA(255, 255, 255), 1);
							SpellWidth = SpellWidth ? SpellWidth : 100;
							if (!Cone)
							{
								Vector3 direction = EndPos - StartPos;
								Vector3 pos1 = StartPos + Vector3(direction.z * -1.0f, direction.y, direction.x * 1.0f).Normalized() * SpellWidth;
								ImVec2 RealPos1 = Direct3D9.WorldToScreen(pos1);
								//draw->String("1", RealPos1.x, RealPos1.y, centered, RGBA(255, 255, 255), fontTahoma);
								Vector3 pos2 = StartPos + Vector3(direction.z * 1.0f, direction.y, direction.x * -1.0f).Normalized() * SpellWidth;
								ImVec2 RealPos2 = Direct3D9.WorldToScreen(pos2);
								//draw->String("2", RealPos2.x, RealPos2.y, centered, RGBA(255, 255, 255), fontTahoma);
								Vector3 pos3 = EndPos + Vector3(direction.z * -1.0f, direction.y, direction.x * 1.0f).Normalized() * SpellWidth;
								ImVec2 RealPos3 = Direct3D9.WorldToScreen(pos3);
								//draw->String("3", RealPos3.x, RealPos3.y, centered, RGBA(255, 255, 255), fontTahoma);
								Vector3 pos4 = EndPos + Vector3(direction.z * 1.0f, direction.y, direction.x * -1.0f).Normalized() * SpellWidth;
								ImVec2 RealPos4 = Direct3D9.WorldToScreen(pos4);
								//draw->String("4", RealPos4.x, RealPos4.y, centered, RGBA(255, 255, 255), fontTahoma);
								draw->Line(RealPos1, RealPos2, RGBA(255, 255, 255));
								draw->Line(RealPos3, RealPos4, RGBA(255, 255, 255));
								draw->Line(RealPos1, RealPos3, RGBA(255, 255, 255));
								draw->Line(RealPos2, RealPos4, RGBA(255, 255, 255));
							}
							else
							{
								Vector3 direction = EndPos - StartPos;
								Vector3 pos3 = EndPos + Vector3(direction.z * -1.0f, direction.y, direction.x * 1.0f).Normalized() * SpellWidth;
								ImVec2 RealPos3 = Direct3D9.WorldToScreen(pos3);
								//draw->String("3", RealPos3.x, RealPos3.y, centered, RGBA(255, 255, 255), fontTahoma);
								Vector3 pos4 = EndPos + Vector3(direction.z * 1.0f, direction.y, direction.x * -1.0f).Normalized() * SpellWidth;
								ImVec2 RealPos4 = Direct3D9.WorldToScreen(pos4);
								//draw->String("4", RealPos4.x, RealPos4.y, centered, RGBA(255, 255, 255), fontTahoma);
								draw->Line(RealPos3, RealPos4, RGBA(255, 255, 255));
								draw->Line(RealStartPos, RealPos3, RGBA(255, 255, 255));
								draw->Line(RealStartPos, RealPos4, RGBA(255, 255, 255));
							}
						}
				}
		}
	}
}

void Direct3D9Render::TurretLoop()
{
	//turret loop
	if (M.AARange.Turrets)
	{
		for (auto obj : init->turretlist)
		{
			if (M.AARange.Turrets)
				vis->DrawAARanges(obj, M.AARange.Slider[0], M.AARange.Slider[1] / 10.f, RGBA(M.AARange.Color[0], M.AARange.Color[1], M.AARange.Color[2], M.AARange.Color[3]),
					false, RGBA(M.AARange.LocalColor[0], M.AARange.LocalColor[1], M.AARange.LocalColor[2], M.AARange.LocalColor[3]), true);

			//ImVec2 RealPos = Direct3D9.WorldToScreen(obj.GetPosition());
			//std::string str = obj.GetName() + " , " + std::to_string(obj.Address()) +" " + std::to_string(obj.GetPosition().X) + " " + std::to_string(obj.GetPosition().Y) + " " + std::to_string(obj.GetPosition().Z);
			//draw->String(str, RealPos.x, RealPos.y, centered, RGBA(255, 255, 255), fontTahoma);
		}
	}
}

void Direct3D9Render::Loops()
{
	if (M.Debug)
		MissileThread();

	if (M.AARange.Local)
		vis->DrawAARanges(Local, M.AARange.Slider[0], M.AARange.Slider[1] / 10.f, RGBA(M.AARange.Color[0], M.AARange.Color[1], M.AARange.Color[2], M.AARange.Color[3]),
			M.AARange.Local, RGBA(M.AARange.LocalColor[0], M.AARange.LocalColor[1], M.AARange.LocalColor[2], M.AARange.LocalColor[3]), false);

	//std::thread HeroLoopThread(&Direct3D9Render::HeroLoop, this);
	//std::thread TurretLoopThread(&Direct3D9Render::TurretLoop, this);
	HeroLoop();
	TurretLoop();

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
				vis->LastHit(obj, RGBA(M.LastHit.Color[0], M.LastHit.Color[1], M.LastHit.Color[2], M.LastHit.Color[3]));

			//	ImVec2 RealPos = Direct3D9.WorldToScreen(obj.GetPosition());
			//float dmg = Local.GetTotalDamage(obj);
			//std::string str = obj.GetName() + " , " + std::to_string(dmg) + " , " + std::to_string(obj.Address());
			//draw->String(str, RealPos.x, RealPos.y, centered, RGBA(255, 255, 255), fontTahoma);
		}
	}

	//inhib loop
	if (M.Inhibs.Master)
	{
		for (auto obj : init->inhiblist)
		{
			if (M.Inhibs.Master)
			{
				vis->InhibTimers(obj);
			}
		}
	}

	//HeroLoopThread.join();
	//TurretLoopThread.join();
}

CObjectManager* ObjManager = new CObjectManager();

void Direct3D9Render::MissileThread()
{
	//auto obj = GetFirst();

	//while (obj)
	//{
	//	CObject cobj(obj);
	//	int team = cobj.GetTeam();
	//	if (team == 100 || team == 200 || team == 300)
	//	{
	//		if (cobj.IsMissile())
	//		{
	//			CObject source = ObjManager->GetObjByIndex(cobj.GetMissileSourceIndex());
	//			//DWORD spellinfo = Memory.Read<DWORD>(cobj.Address() + 0x230);
	//			//DWORD spelldata = Memory.Read<DWORD>(spellinfo + 0x44);
	//			//float speed = Memory.Read<float>(spelldata + 0x428);
	//			//
	//			////clog.AddLog("%x ,  %s  , %d", source.Address(), source.GetChampName().c_str(), cobj.GetMissileSourceIndex());
	//			//clog.AddLog("%x , %s ", cobj.Address(), cobj.GetName().c_str());
	//			if (source.IsHero())
	//			{
	//				if (cobj.GetName().find("basic") == std::string::npos)
	//				{
	//					Vector3 StartPos = cobj.GetMissileStartPos();
	//					ImVec2 RealStartPos = Direct3D9.WorldToScreen(StartPos);
	//					Vector3 EndPos = cobj.GetMissileEndPos();
	//					ImVec2 RealEndPos = Direct3D9.WorldToScreen(EndPos);

	//					/*DWORD spellinfo = Memory.Read<DWORD>(cobj.Address() + 0x230);
	//					DWORD spelldata = Memory.Read<DWORD>(spellinfo + 0x44);
	//					float speed = Memory.Read<float>(spellinfo + 0x450);
	//					std::string desc = Memory.ReadString(spelldata + 0x0088);
	//					clog.AddLog("=============");
	//					for (int i = 0; i < 2000; i += 4)
	//					{
	//						speed = Memory.Read<float>(spelldata + i);
	//						clog.AddLog("%x ,  %s, %f  , %x", cobj.Address(), cobj.GetName().c_str(), speed ,i);
	//					}*/

	//					if (RealStartPos.x != 0 && RealStartPos.y != 0 && RealEndPos.x != 0 && RealEndPos.y != 0)
	//					{
	//						std::string str = cobj.GetName() + " , " + std::to_string(cobj.Address());
	//						draw->String(str, RealEndPos.x, RealEndPos.y, centered, RGBA(255, 255, 255), Direct3D9.fontTahoma);
	//						draw->Line(RealStartPos, RealEndPos, RGBA(255, 255, 255));
	//					}
	//				}
	//			}
	//		}
	//	}
	//	obj = GetNext(obj);
	//}

	//std::vector<CObject> missiles;
	int numMissiles = Memory.Read<int>(MissileMap + 0x78);
	int rootNode = Memory.Read<int>(MissileMap + 0x74);
	//clog.AddLog("%x", rootNode);
	std::queue<int> nodesToVisit;
	std::set<int> visitedNodes;
	nodesToVisit.push(rootNode);

	int childNode1, childNode2, childNode3, node;
	while (nodesToVisit.size() > 0 && visitedNodes.size() < numMissiles * 2)
	{
		node = nodesToVisit.front();
		nodesToVisit.pop();
		visitedNodes.insert(node);
		//clog.AddLog("%x", node);
		childNode1 = Memory.Read<int>(node);
		childNode2 = Memory.Read<int>(node + 4);
		childNode3 = Memory.Read<int>(node + 8);

		if (visitedNodes.find(childNode1) == visitedNodes.end())
			nodesToVisit.push(childNode1);
		if (visitedNodes.find(childNode2) == visitedNodes.end())
			nodesToVisit.push(childNode2);
		if (visitedNodes.find(childNode3) == visitedNodes.end())
			nodesToVisit.push(childNode3);

		unsigned int netId = Memory.Read<int>(node + 0x10);

		// Actual missiles net_id start from 0x40000000 and throught the game this id will be incremented by 1 for each missile.
		// So we use this information to check if missiles are valid.
		if (netId - (unsigned int)0x40000000 > 0x100000)
			continue;

		int addr = Memory.Read<int>(node + 0x14);
		if (addr == 0)
			continue;

		// The MissileClient is wrapped around another object
		addr = Memory.Read<int>(addr + 0x4);
		if (addr == 0)
			continue;

		addr = Memory.Read<int>(addr + 0x10);
		if (addr == 0)
			continue;

		// At this point addr is the address of the MissileClient
		MissileClient obj(addr); //std::unique_ptr<CObject>(new CObject());
	   // m->LoadFromMem(addr, hProcess);

		// Check one more time that we read a valid missile
		if (obj.GetNetworkID() != netId)
			continue;
		//.AddLog("%x", m.Address());
		//clog.AddLog("%s %x", obj.GetName().c_str(), obj.Address());
		//missiles.push_back(m);

		int team = obj.GetTeam();
		if (team == 100 || team == 200 || team == 300)
		{
			CObject source = obj.GetSource();
			if (source.IsHero())
			{
				clog.AddLog("%x", obj.Address());
				if (!utils->StringContains(obj.GetName(), "basic", true))
				{
					Vector3 StartPos = obj.GetMissileStartPos();
					ImVec2 RealStartPos = WorldToScreen(StartPos);
					if (RealStartPos.x == 0 || RealStartPos.y == 0)
						continue;
					Vector3 EndPos = obj.GetMissileEndPos();
					ImVec2 RealEndPos = WorldToScreen(EndPos);
					if (RealEndPos.x == 0 || RealEndPos.y == 0)
						continue;

					draw->Line(RealStartPos, RealEndPos, RGBA(255, 255, 255), 1);

					Vector3 direction = EndPos - StartPos;
					Vector3 pos1 = StartPos + Vector3(direction.z * -1.0f, direction.y, direction.x * 1.0f).Normalized() * 100;
					ImVec2 RealPos1 = Direct3D9.WorldToScreen(pos1);
					//draw->String("1", RealPos1.x, RealPos1.y, centered, RGBA(255, 255, 255), fontTahoma);
					Vector3 pos2 = StartPos + Vector3(direction.z * 1.0f, direction.y, direction.x * -1.0f).Normalized() * 100;
					ImVec2 RealPos2 = Direct3D9.WorldToScreen(pos2);
					//draw->String("2", RealPos2.x, RealPos2.y, centered, RGBA(255, 255, 255), fontTahoma);
					Vector3 pos3 = EndPos + Vector3(direction.z * -1.0f, direction.y, direction.x * 1.0f).Normalized() * 100;
					ImVec2 RealPos3 = Direct3D9.WorldToScreen(pos3);
					//draw->String("3", RealPos3.x, RealPos3.y, centered, RGBA(255, 255, 255), fontTahoma);
					Vector3 pos4 = EndPos + Vector3(direction.z * 1.0f, direction.y, direction.x * -1.0f).Normalized() * 100;
					ImVec2 RealPos4 = Direct3D9.WorldToScreen(pos4);
					//draw->String("4", RealPos4.x, RealPos4.y, centered, RGBA(255, 255, 255), fontTahoma);
					draw->Line(RealPos1, RealPos2, RGBA(255, 255, 255));
					draw->Line(RealPos3, RealPos4, RGBA(255, 255, 255));
					draw->Line(RealPos1, RealPos3, RGBA(255, 255, 255));
					draw->Line(RealPos2, RealPos4, RGBA(255, 255, 255));
				}
			}
		}
	}
}

float ftest = 1;

Vector3 Test1;
Vector3 Test2;

int Direct3D9Render::Render()
{
	// Start the Dear ImGui frame
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	std::thread ViewmatrixThread(&Direct3D9Render::GetViewProjectionMatrix, this);

	if (M.MenuOpen)
	{
		char buf[128];
		if (M.Debug)
			sprintf(buf, XorStr("KBot %.1f FPS ###AnimatedTitle"), ImGui::GetIO().Framerate);
		else
			sprintf(buf, XorStr("KBot"));
		ImGui::SetNextWindowPos(ImVec2(904, 349), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(419, 421), ImGuiCond_FirstUseEver);
		ImGui::Begin(buf);// , ImGuiWindowFlags_AlwaysAutoResize);
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem(XorStr("Open Console")))
				M.ConsoleOpen = true;
			if (ImGui::MenuItem(XorStr("Exit")))
				M.ExitBot = true;
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
								ImGui::Button(M.ScoreboardNames[n].c_str(), ImVec2(100, 25));

								// Our buttons are both drag sources and drag targets here!
								if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
								{
									// Set payload to carry the index of our item (could be anything)
									ImGui::SetDragDropPayload("DND_DEMO_CELL", &n, sizeof(int));

									// Display preview (could be anything, e.g. when dragging an image we could decide to display
									// the filename and a small preview of the image, etc.)
									ImGui::Text("Swap %s", M.ScoreboardNames[n].c_str());
									ImGui::EndDragDropSource();
								}
								if (ImGui::BeginDragDropTarget())
								{
									if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_DEMO_CELL"))
									{
										IM_ASSERT(payload->DataSize == sizeof(int));
										int payload_n = *(const int*)payload->Data;
										std::string tmp = M.ScoreboardNames[n];
										M.ScoreboardNames[n] = M.ScoreboardNames[payload_n];
										M.ScoreboardNames[payload_n] = tmp;
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
								keyboard->GenerateKeyScancode(DIK_O, false);
								M.Cooldowns.Scoreboard.ShowScoreboard = !M.Cooldowns.Scoreboard.ShowScoreboard;
							}
							if (ImGui::Button("Swap", ImVec2(200, 50)))
							{
								for (int i = 0; i < 10; i += 2)
									std::swap(M.ScoreboardNames[i], M.ScoreboardNames[i + 1]);
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
				draw->ImageFromMemory(draw->textureKEKW, 0, 0, "", 99, 256, 256, true);

				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Orbwalker"))
			{
				ImGui::Checkbox("Enable - just a placeholder ##Orbwalker", &M.Orbwalker.Master);
				ImGui::Combo("Hold Key", &M.Orbwalker.HoldKey, keyNames, ARRAYSIZE(keyNames));
				draw->ImageFromMemory(draw->textureSks, 0, 0, "", 99, 256 * 1.25, 256 * 1.25, true);

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

				ImGui::Separator();

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem(M.Champion.c_str()))
			{
				if (M.Champion == "Talon")
				{
					ImGui::Checkbox("Damage Calculator", &M.Talon.DmgCalc);
					ImGui::Separator();
					ImGui::Checkbox("Pixel Perfect Jumps", &M.Talon.Jumps);
					ImGui::Combo("Jumps Key", &M.Talon.JumpsKey, keyNames, ARRAYSIZE(keyNames));
					if (M.Talon.Jumps)
					{
						ImGui::Selectable("Drake", &M.Talon.JumpsType[0]);
						ImGui::Selectable("Blue-side Raptors", &M.Talon.JumpsType[1]);
						ImGui::Selectable("Botlane", &M.Talon.JumpsType[2]);
						ImGui::Selectable("Toplane", &M.Talon.JumpsType[3]);
						ImGui::Selectable("Red-side Raptors TODO", &M.Talon.JumpsType[4]);
					}
				}
				else
					ImGui::Text("Only Talon for now");

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
		ViewmatrixThread.join(); // wait for it to execute

#ifndef NOLEAGUE
		//std::thread MissileLoopThread;

		Loops();

		if (championScript)
			championScript->OnKeyDown(1);

		//if(M.Debug)
		//	MissileLoopThread.join();

		if (PressedKey(VK_END))
			Test1 = Local.GetPosition();

		if (PressedKey(VK_HOME))
			Test2 = Local.GetPosition();

		draw->Line(WorldToScreen(Test1), WorldToScreen(Test2), RGBA(255, 255, 255));

		//Vector3 TempVec2 = Test1.Extend(Test2, 2.f);
		auto start_position = Test1;
		auto end_position = Test2;
		//end_position = end_position.Extend(start_position, -fmin(50, 100 - end_position.Distance(start_position)));
		//end_position = end_position.Extend(start_position, 100);
		draw->Line(WorldToScreen(start_position), WorldToScreen(end_position), RGBA(0, 0, 255));

#endif // !NOLEAGUE

		//draw->ImageFromMemory(draw->textureIgnite, mouse->GetPos().x, mouse->GetPos().y, "abc", 3213, 120, 120, false);

		//ImGui::DragFloat("asd", &ftest, 1, 1, 1000);

		ImGui::EndFrame();
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

		//
		//DWORD testList = Memory.Read<DWORD>(ClientAddress + 0x34F848C);
		//clog.AddLog("TestList: %x", testList);
		//DWORD testarray = Memory.Read<DWORD>(testList + 0x74);
		//int testlength = Memory.Read<int>(testList + 0x78);
		//for (int i = 0; i < testlength * 4; i += 4)
		//{
		//	CObject obj(Memory.Read<DWORD>(testarray + i));
		//	ImVec2 RealPos = Direct3D9.WorldToScreen(obj.GetPosition());

		//	//if (RealPos.x == 0 && RealPos.y == 0)
		//	//	continue;

		//	Vector3 StartPos = obj.GetMissileStartPos();
		//	Vector3 EndPos = obj.GetMissileEndPos();
		//	ImVec2 RealStartPos = WorldToScreen(StartPos);
		//	ImVec2 RealEndPos = WorldToScreen(EndPos);

		//	draw->Line(RealStartPos.x, RealStartPos.y, RealEndPos.x, RealEndPos.y, RGBA(255, 255, 255));

		//
		//	clog.AddLog("%s : %x", obj.GetName().c_str(), obj.Address());
		//	std::string str = obj.GetName() + " , " + std::to_string(obj.Address());
		//	draw->String(str, RealPos.x, RealPos.y, centered, RGBA(255, 255, 255), fontTahoma);

		//}

		//float GameTime = Memory.Read<float>(ClientAddress + oGameTime, sizeof(float));
		//if (xd)
		//{
		//	xd = false;
		//	xdtimer = GameTime + 300;

		//}

		if (M.StartTime + 5000 > GetTickCount())
			if (!M.ServerInfo.empty())
				draw->StringBoxed(M.ServerInfo, 10, 10, lefted, RGBA(255, 255, 255), fontTahoma, RGBA(255, 255, 255), RGBA(1, 0, 0));
			else
				M.ServerInfo = "";

		////drawings test
		//draw->Bar(50, 50, ftest);
		//draw->GradientBox(300, 300, 50, 50, RGBA(255, 0, 0), RGBA(0, 0, 255), false);
		//draw->GradientBoxOutlined(400, 300, 50, 50, RGBA(255, 0, 0), RGBA(0, 0, 255), true);
		//draw->BoxFilled(200, 500, 100, 100, RGBA(255, 0, 0));
		//draw->BoxBorder(500, 500, 100, 100, RGBA(255, 0, 0));
		//draw->BoxOutlined(800, 500, 100, 100, RGBA(255, 0, 0));
		//draw->StringBoxed("asdfsdgSVX123!_", 700, 200, lefted, RGBA(255, 255, 255), fontTahoma, RGBA(1, 0, 0), RGBA(255, 0, 0));
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

void Direct3D9Render::MenuInit()
{
	ImGuiIO& io = ImGui::GetIO();
	io.IniFilename = ".\\configs\\imgui.ini";
	io.IniSavingRate = 7.f;
	io.ConfigWindowsResizeFromEdges = 0;

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