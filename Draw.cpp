#include "Draw.h"

void Draw::Line(const ImVec2& start, const ImVec2& end, const RGBA& color, float thickness)
{
	overlay->AddLine((const ImVec2&)start, (const ImVec2&)end, ImColor(color.R, color.G, color.B, color.A), thickness);
}
void Draw::Line(const int& x, const int& y, const int& x2, const int& y2, const RGBA& color, float thickness)
{
	Line(ImVec2(x, y), ImVec2(x2, y2), color, thickness);
}

void Draw::String(const std::string text, const ImVec2& pos, const RGBA& color, ImFont* font)
{
	ImGui::PushFont(font);
	overlay->AddText(ImVec2(pos.x, pos.y), ImColor(color.R, color.G, color.B, color.A), text.c_str());
	ImGui::PopFont();
}
void Draw::String(const std::string text, const int& x, const int& y, const RGBA& color, ImFont* font)
{
	String(text.c_str(), ImVec2(x, y), color);
}

void Draw::CircleRange2(const Vector3& Pos, const float& points, float r, const RGBA& color, float thickness)
{
	float flPoint = M_PI * 2.0f / points;

	//r += 80;
	bool first = true;
	ImVec2 First, Last;
	for (float flAngle = flPoint; flAngle < (M_PI * 2.0f); flAngle += flPoint)
	{
		Vector3 vStart(r * cosf(flAngle) + Pos.x, r * sinf(flAngle) + Pos.z, Pos.y);
		Vector3 vEnd(r * cosf(flAngle + flPoint) + Pos.x, r * sinf(flAngle + flPoint) + Pos.z, Pos.y);

		float z_temp = vStart.z;
		vStart.z = vStart.y;
		vStart.y = z_temp;

		z_temp = vEnd.z;
		vEnd.z = vEnd.y;
		vEnd.y = z_temp;

		ImVec2 vStartScreen, vEndScreen;
		vStartScreen = Direct3D9.WorldToScreen(vStart);
		if (vStartScreen.x == 0 && vStartScreen.y == 0) break;
		vEndScreen = Direct3D9.WorldToScreen(vEnd);
		if (vEndScreen.x == 0 && vEndScreen.y == 0) break;

		if (!((vStartScreen.x <= SCREENWIDTH * 1.2) && (vStartScreen.x >= SCREENWIDTH / 2 * (-1)) && (vStartScreen.y <= SCREENHEIGHT * 1.5) && (vStartScreen.y >= SCREENHEIGHT / 2 * (-1)))
			&& !((vEndScreen.x <= SCREENWIDTH * 1.2) && (vEndScreen.x >= SCREENWIDTH / 2 * (-1)) && (vEndScreen.y <= SCREENHEIGHT * 1.5) && (vEndScreen.y >= SCREENHEIGHT / 2 * (-1))))
			return;

		if (first)
		{
			First = vStartScreen;
			first = false;
		}

		Line(vStartScreen.x, vStartScreen.y, vEndScreen.x, vEndScreen.y, color, thickness);
		Last = vEndScreen;
	}
	Line(First.x, First.y, Last.x, Last.y, color, thickness);
}

void Draw::CircleRange(const Vector3& worldPos, int numPoints, float radius, RGBA color, float thickness, bool filled) const
{
	if (numPoints >= 200)
		return;
	static ImVec2 points[200];

	float step = 6.2831f / numPoints;
	float theta = 0.f;
	for (int i = 0; i < numPoints; i++, theta += step)
	{
		Vector3 worldSpace = { worldPos.x + radius * cos(theta), worldPos.y, worldPos.z - radius * sin(theta) };
		ImVec2 screenSpace = Direct3D9.WorldToScreen(worldSpace);

		points[i].x = screenSpace.x;
		points[i].y = screenSpace.y;
	}

	if (filled)
		overlay->AddConvexPolyFilled(points, numPoints, ImColor(color.R, color.G, color.B, color.A));
	else
		overlay->AddPolyline(points, numPoints, ImColor(color.R, color.G, color.B, color.A), true, thickness);
}

void Draw::Polygon(const Geometry::Polygon poly, RGBA color)
{
	static ImVec2 points[200];
	int i = 0;
	for (const auto& point : poly.Points)
	{
		ImVec2 pos = Direct3D9.WorldToScreen(point);
		points[i].x = pos.x;
		points[i].y = pos.y;
		//Circle(pos, 10, RGBA(255, 255, 255));
		i++;
	}

	overlay->AddPolyline(points, i, ImColor(color.R, color.G, color.B, color.A), true, 1.f);
}

bool Draw::IsOnScreen(const ImVec2& point, float offsetX, float offsetY) const
{
	return point.x > -offsetX && point.x < (SCREENWIDTH + offsetX) && point.y > -offsetY && point.y < (SCREENHEIGHT + offsetY);
}

bool Draw::IsOnScreen(const Vector3& point, float offsetX, float offsetY) const
{
	return IsOnScreen(Direct3D9.WorldToScreen(point), offsetX, offsetY);
}

Vector3 FixPos(Vector3 pos)
{
	return Vector3(pos.x, pos.y, Local.GetPosition().z);
}

void Draw::Arrow(const Vector3& startPos, const Vector3& endPos, RGBA color)
{
	//todo
	/*Vector3 p1 = endPos - ((startPos - endPos).Normalized() * 30).Perpendicular() + (startPos - endPos).Normalized() * 30;
	Vector3 p2 = endPos - ((startPos - endPos).Normalized() * 30).Perpendicular2() + (startPos - endPos).Normalized() * 30;
	ImVec2 Rp1 = Direct3D9.WorldToScreen(FixPos(p1));
	ImVec2 Rp2 = Direct3D9.WorldToScreen(FixPos(p2));
	ImVec2 RsP = Direct3D9.WorldToScreen(FixPos(startPos));
	ImVec2 ReP = Direct3D9.WorldToScreen(FixPos(endPos));
	Line(startPos.x, startPos.y, endPos.x, endPos.y, color);
	Line(p1.x, p1.y, endPos.x, endPos.y, color);
	Line(p2.x, p2.y, endPos.x, endPos.y, color);*/
}

//bool Draw::InitTextures()
//{
//	ID3D11ShaderResourceView* my_texture = NULL;
//	HRESULT res;
//
//	//todo cleanup this
//	res = D3DXCreateTextureFromFileInMemory(g_pd3dDevice, &g_barrier, sizeof(g_barrier), &my_texture);
//	if (res != S_OK)
//		return false;
//	if (my_texture == NULL)
//		return false;
//	textureBarrier = my_texture;
//
//	res = D3DXCreateTextureFromFileInMemory(g_pd3dDevice, &g_clarity, sizeof(g_clarity), &my_texture);
//	if (res != S_OK)
//		return false;
//	if (my_texture == NULL)
//		return false;
//	textureClarity = my_texture;
//
//	res = D3DXCreateTextureFromFileInMemory(g_pd3dDevice, &g_cleanse, sizeof(g_cleanse), &my_texture);
//	if (res != S_OK)
//		return false;
//	if (my_texture == NULL)
//		return false;
//	textureCleanse = my_texture;
//
//	res = D3DXCreateTextureFromFileInMemory(g_pd3dDevice, &g_exhaust, sizeof(g_exhaust), &my_texture);
//	if (res != S_OK)
//		return false;
//	if (my_texture == NULL)
//		return false;
//	textureExhaust = my_texture;
//
//	res = D3DXCreateTextureFromFileInMemory(g_pd3dDevice, &g_flash, sizeof(g_flash), &my_texture);
//	if (res != S_OK)
//		return false;
//	if (my_texture == NULL)
//		return false;
//	textureFlash = my_texture;
//
//	res = D3DXCreateTextureFromFileInMemory(g_pd3dDevice, &g_ghost, sizeof(g_ghost), &my_texture);
//	if (res != S_OK)
//		return false;
//	if (my_texture == NULL)
//		return false;
//	textureGhost = my_texture;
//
//	res = D3DXCreateTextureFromFileInMemory(g_pd3dDevice, &g_heal, sizeof(g_heal), &my_texture);
//	if (res != S_OK)
//		return false;
//	if (my_texture == NULL)
//		return false;
//	textureHeal = my_texture;
//
//	res = D3DXCreateTextureFromFileInMemory(g_pd3dDevice, &g_ignite, sizeof(g_ignite), &my_texture);
//	if (res != S_OK)
//		return false;
//	if (my_texture == NULL)
//		return false;
//	textureIgnite = my_texture;
//
//	res = D3DXCreateTextureFromFileInMemory(g_pd3dDevice, &g_smite, sizeof(g_smite), &my_texture);
//	if (res != S_OK)
//		return false;
//	if (my_texture == NULL)
//		return false;
//	textureSmite = my_texture;
//
//	res = D3DXCreateTextureFromFileInMemory(g_pd3dDevice, &g_teleport, sizeof(g_teleport), &my_texture);
//	if (res != S_OK)
//		return false;
//	if (my_texture == NULL)
//		return false;
//	textureTeleport = my_texture;
//
//	res = D3DXCreateTextureFromFileInMemory(g_pd3dDevice, &g_kekw, sizeof(g_kekw), &my_texture);
//	if (res != S_OK)
//		return false;
//	if (my_texture == NULL)
//		return false;
//	textureKEKW = my_texture;
//
//	res = D3DXCreateTextureFromFileInMemory(g_pd3dDevice, &g_sks, sizeof(g_sks), &my_texture);
//	if (res != S_OK)
//		return false;
//	if (my_texture == NULL)
//		return false;
//	textureSks = my_texture;
//
//	return true;
//}
//
//void Draw::ImageFromMemory(LPDIRECT3DTEXTURE9 texturename, int x, int y, std::string text, int index, int in_width, int in_height, bool inWindow)
//{
//	if (!texturename)
//		return;
//
//	// todo it flickers when text is changing, probably smth to do with wnd name
//	//also drawing text over image
//	D3DSURFACE_DESC my_texture_desc;
//	/*HRESULT res = texturename->GetLevelDesc(0, &my_texture_desc);
//	IM_ASSERT(res == 0);*/
//
//	if (!inWindow)
//	{
//		bool Open = true;
//		std::string wndName = "##texture" + text + std::to_string(index);
//		const auto flags = ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs;
//
//		ImGui::Begin(wndName.c_str(), &Open, flags);
//		ImGui::SetWindowSize(ImVec2(in_width, in_height), ImGuiCond_Always);
//		ImGui::SetWindowPos(ImVec2(x, y), ImGuiCond_Always);
//	}
//
//	ImGui::Image(texturename, ImVec2((float)in_width, (float)in_height));
//
//	if (!text.empty())
//	{
//		String(text, x + in_width + 5, y, lefted, RGBA(255, 255, 255), Direct3D9.fontTahoma);
//		//ImGui::Text(text.c_str());
//	/*	ImFont* newFont = ImGui::GetFont();
//		newFont->Scale = 2;
//		ImGui::PushFont(newFont);
//		ImGui::GetForegroundDrawList()->AddText(ImVec2(x + in_width / 5, y + in_height / 10), IM_COL32(1, 0, 0, 255), text.c_str());
//		newFont->Scale = 1;
//		ImGui::PopFont();*/
//	}
//
//	if (!inWindow)
//	{
//		ImGui::End();
//	}
//}
//
//void Draw::Sprite(ID3D11ShaderResourceView* tex, float x, float y, float resolution, float scale, float rotation)
//{
//	if (!tex)
//		return;
//
//	// Texture being used is 64x64:
//	D3DXVECTOR2 spriteCentre = D3DXVECTOR2(resolution / 2, resolution / 2);
//
//	// Screen position of the sprite
//	D3DXVECTOR2 trans = D3DXVECTOR2(x - spriteCentre.x, y - spriteCentre.y);
//
//	// Build our matrix to rotate, scale and position our sprite
//	D3DXMATRIX mat;
//
//	D3DXVECTOR2 scaling(scale, scale);
//
//	// out, scaling centre, scaling rotation, scaling, rotation centre, rotation, translation
//	D3DXMatrixTransformation2D(&mat, NULL, 0.0, &scaling, &spriteCentre, rotation, &trans);
//
//	if (!g_pSprite)
//	{
//		D3DXCreateSprite(g_pd3dDevice, &g_pSprite);
//	}
//
//	if (g_pSprite)
//	{
//		g_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
//	}
//	g_pSprite->SetTransform(&mat);
//
//	g_pSprite->Draw(tex, NULL, NULL, NULL, 0xFFFFFFFF);
//	if (g_pSprite)
//	{
//		g_pSprite->End();
//	}
//}
//
////not needed since now every texture is loaded from memory
///*
//LPDIRECT3DTEXTURE9 Draw::LoadTextureFromFile(const char* filename, LPDIRECT3DTEXTURE9* out_texture, int* out_width, int* out_height, LPDIRECT3DDEVICE9 xD)
//{
//	// Load texture from disk
//	LPDIRECT3DTEXTURE9 pTexture;
//	HRESULT hr = D3DXCreateTextureFromFileA(xD, filename, &pTexture);
//	if (hr != S_OK)
//		return (LPDIRECT3DTEXTURE9)false;
//
//	// Retrieve description of the texture surface so we can access its size
//	D3DSURFACE_DESC my_image_desc;
//	pTexture->GetLevelDesc(0, &my_image_desc);
//	*out_texture = pTexture;
//	*out_width = (int)my_image_desc.Width;
//	*out_height = (int)my_image_desc.Height;
//	return pTexture;
//}
//
//void Draw::Image(std::string _filename, int x, int y, std::string text, int index, int in_width, int in_height, bool inWindow)
//{
//	_filename = ".\\images\\" + _filename;
//	bool has_saved_img_Ptr = false;
//	image img_texture;
//	for (image a : image_list) {
//		if ((a.filename.compare(_filename) == 0) && (a.height == in_height) && (a.width == in_width)) {
//			has_saved_img_Ptr = true;
//			img_texture = a;
//			break;
//		}
//	}
//
//	if (!has_saved_img_Ptr) {
//		img_texture.filename = _filename;
//		img_texture.height = in_height;
//		img_texture.width = in_width;
//		img_texture.loc = ImVec2(img_texture.loc.x, img_texture.loc.y);
//		if (!(LoadTextureFromFile(_filename.c_str(), &img_texture.texture, &in_width, &in_height, g_pd3dDevice)))
//		{
//			ImGui::Text("%s not found", _filename.c_str());
//			return;
//		}
//
//		image_list.push_back(img_texture);
//	}
//	if (!inWindow)
//	{
//		bool Open = true;
//		std::string wndName = "##texture" + img_texture.filename + std::to_string(index);
//		const auto flags = ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs;
//
//		ImGui::Begin(wndName.c_str(), &Open, flags);
//		ImGui::SetWindowSize(ImVec2(in_width, in_height), ImGuiCond_Always);
//		ImGui::SetWindowPos(ImVec2(x, y), ImGuiCond_Always);
//	}
//	//ImGui::Text("wndName = %s", wndName.c_str());
//	//ImGui::Text("height x width = %d x %d", in_height, in_width);
//	//ImGui::Text("image_list size = %d", image_list.size());
//	//ImGui::Text("pointer = %p", img_texture.texture);
//	//ImGui::Text("file name = %s", img_texture.filename);
//
//	ImGui::Image((void*)img_texture.texture, ImVec2(in_width, in_height));
//	if (!text.empty())
//	{
//		//DrawString("testtesttesttesttest", x, y, RGBA(255, 255, 255), fontTahoma);
//		//ImGui::Text(text.c_str());]
//		ImFont* newFont = ImGui::GetFont();
//		newFont->Scale = 2;
//		ImGui::PushFont(newFont);
//		ImGui::GetForegroundDrawList()->AddText(ImVec2(x + in_width / 5, y + in_height / 10), IM_COL32(1,0,0,255), text.c_str());
//		newFont->Scale = 1;
//		ImGui::PopFont();
//	}
//
//	if (!inWindow)
//	{
//		ImGui::End();
//	}
//}
//*/
//
//void Draw::Line(int x, int y, int x2, int y2, RGBA rgb, float thickness)
//{
//	g_Line->SetWidth(thickness);
//	D3DXVECTOR2 points[2];
//	points[0].x = x;
//	points[1].x = x2;
//	points[0].y = y;
//	points[1].y = y2;
//	g_Line->Draw(points, 2, D3DCOLOR_ARGB(rgb.A, rgb.R, rgb.G, rgb.B));
//}
//
//void Draw::Line(ImVec2 pos1, ImVec2 pos2, RGBA rgb, float thickness)
//{
//	Line(pos1.x, pos1.y, pos2.x, pos2.y, rgb, thickness);
//}
//
//void Draw::String(std::string text, int x, int y, int orientation, RGBA color, ID3DXFont* font, bool bordered, RGBA bcolor)
//{
//	RECT rect;
//
//	switch (orientation)
//	{
//	case lefted:
//		if (bordered)
//		{
//			SetRect(&rect, x - 1, y, x - 1, y);
//			font->DrawTextA(NULL, text.c_str(), -1, &rect, DT_LEFT | DT_NOCLIP, D3DCOLOR_ARGB(bcolor.A, bcolor.R, bcolor.G, bcolor.B));
//			SetRect(&rect, x + 1, y, x + 1, y);
//			font->DrawTextA(NULL, text.c_str(), -1, &rect, DT_LEFT | DT_NOCLIP, D3DCOLOR_ARGB(bcolor.A, bcolor.R, bcolor.G, bcolor.B));
//			SetRect(&rect, x, y - 1, x, y - 1);
//			font->DrawTextA(NULL, text.c_str(), -1, &rect, DT_LEFT | DT_NOCLIP, D3DCOLOR_ARGB(bcolor.A, bcolor.R, bcolor.G, bcolor.B));
//			SetRect(&rect, x, y + 1, x, y + 1);
//			font->DrawTextA(NULL, text.c_str(), -1, &rect, DT_LEFT | DT_NOCLIP, D3DCOLOR_ARGB(bcolor.A, bcolor.R, bcolor.G, bcolor.B));
//		}
//		SetRect(&rect, x, y, x, y);
//		font->DrawTextA(NULL, text.c_str(), -1, &rect, DT_LEFT | DT_NOCLIP, D3DCOLOR_ARGB(color.A, color.R, color.G, color.B));
//		break;
//	case centered:
//		if (bordered)
//		{
//			SetRect(&rect, x - 1, y, x - 1, y);
//			font->DrawTextA(NULL, text.c_str(), -1, &rect, DT_CENTER | DT_NOCLIP, D3DCOLOR_ARGB(bcolor.A, bcolor.R, bcolor.G, bcolor.B));
//			SetRect(&rect, x + 1, y, x + 1, y);
//			font->DrawTextA(NULL, text.c_str(), -1, &rect, DT_CENTER | DT_NOCLIP, D3DCOLOR_ARGB(bcolor.A, bcolor.R, bcolor.G, bcolor.B));
//			SetRect(&rect, x, y - 1, x, y - 1);
//			font->DrawTextA(NULL, text.c_str(), -1, &rect, DT_CENTER | DT_NOCLIP, D3DCOLOR_ARGB(bcolor.A, bcolor.R, bcolor.G, bcolor.B));
//			SetRect(&rect, x, y + 1, x, y + 1);
//			font->DrawTextA(NULL, text.c_str(), -1, &rect, DT_CENTER | DT_NOCLIP, D3DCOLOR_ARGB(bcolor.A, bcolor.R, bcolor.G, bcolor.B));
//		}
//		SetRect(&rect, x, y, x, y);
//		font->DrawTextA(NULL, text.c_str(), -1, &rect, DT_CENTER | DT_NOCLIP, D3DCOLOR_ARGB(color.A, color.R, color.G, color.B));
//		break;
//	case righted:
//		if (bordered)
//		{
//			SetRect(&rect, x - 1, y, x - 1, y);
//			font->DrawTextA(NULL, text.c_str(), -1, &rect, DT_RIGHT | DT_NOCLIP, D3DCOLOR_ARGB(bcolor.A, bcolor.R, bcolor.G, bcolor.B));
//			SetRect(&rect, x + 1, y, x + 1, y);
//			font->DrawTextA(NULL, text.c_str(), -1, &rect, DT_RIGHT | DT_NOCLIP, D3DCOLOR_ARGB(bcolor.A, bcolor.R, bcolor.G, bcolor.B));
//			SetRect(&rect, x, y - 1, x, y - 1);
//			font->DrawTextA(NULL, text.c_str(), -1, &rect, DT_RIGHT | DT_NOCLIP, D3DCOLOR_ARGB(bcolor.A, bcolor.R, bcolor.G, bcolor.B));
//			SetRect(&rect, x, y + 1, x, y + 1);
//			font->DrawTextA(NULL, text.c_str(), -1, &rect, DT_RIGHT | DT_NOCLIP, D3DCOLOR_ARGB(bcolor.A, bcolor.R, bcolor.G, bcolor.B));
//		}
//		SetRect(&rect, x, y, x, y);
//		font->DrawTextA(NULL, text.c_str(), -1, &rect, DT_RIGHT | DT_NOCLIP, D3DCOLOR_ARGB(color.A, color.R, color.G, color.B));
//		break;
//	}
//}
//
//void Draw::BoxFilled(int x, int y, int w, int h, RGBA rgb)
//{
//	D3DRECT rect = { x, y, x + w, y + h };
//	g_pd3dDevice->Clear(1, &rect, D3DCLEAR_TARGET, D3DCOLOR_ARGB(rgb.A, rgb.R, rgb.G, rgb.B), 0, 0);
//}
//
//void Draw::BoxBorder(int x, int y, int w, int h, RGBA color, int thickness)
//{
//	this->BoxFilled(x, y, w, thickness, color);
//	this->BoxFilled(x, y, thickness, h, color);
//	this->BoxFilled(x + w, y, thickness, h, color);
//	this->BoxFilled(x, y + h, w + thickness, thickness, color);
//}
//
//void Draw::BoxOutlined(int x, int y, int w, int h, RGBA color, float thickness, RGBA bcolor)
//{
//	BoxFilled(x, y, w, h, color);
//	BoxBorder(x - thickness, y - thickness, w + thickness, h + thickness, bcolor, thickness);
//}
//
//void Draw::StringBoxed(std::string text, int x, int y, int orientation, RGBA color, ID3DXFont* font, RGBA bcolor, RGBA background)
//{
//	RECT rect = { x, y, x, y };
//
//	switch (orientation)
//	{
//	case lefted:
//		font->DrawTextA(NULL, text.c_str(), -1, &rect, DT_CALCRECT | DT_LEFT, D3DCOLOR_ARGB(0, 0, 0, 0));
//
//		BoxOutlined(x - 5, rect.top - 5, rect.right - x + 10, rect.bottom - rect.top + 10, background, 1, bcolor);
//		SetRect(&rect, x, y, x, y);
//
//		String(text, x, y, orientation, color, font, true);
//		break;
//	case centered:
//		font->DrawTextA(NULL, text.c_str(), -1, &rect, DT_CALCRECT | DT_CENTER, D3DCOLOR_ARGB(0, 0, 0, 0));
//
//		BoxOutlined(rect.left - 5, rect.top - 5, rect.right - rect.left + 10, rect.bottom - rect.top + 10, background, 1, bcolor);
//		SetRect(&rect, x, y, x, y);
//
//		String(text, x, y, orientation, color, font, true);
//		break;
//	case righted:
//		font->DrawTextA(NULL, text.c_str(), -1, &rect, DT_CALCRECT | DT_RIGHT, D3DCOLOR_ARGB(0, 0, 0, 0));
//
//		BoxOutlined(rect.left - 5, rect.top - 5, rect.right - rect.left + 10, rect.bottom - rect.top + 10, background, 1, bcolor);
//		SetRect(&rect, x, y, x, y);
//
//		String(text, x, y, orientation, color, font, true);
//		break;
//	}
//}
//
//void Draw::Circle(int x, int y, float r, RGBA rgb)
//{
//	VERTEX_2D_DIF verts[CIRCLE_RESOLUTION + 1];
//
//	for (int i = 0; i < CIRCLE_RESOLUTION + 1; i++)
//	{
//		verts[i].x = x + r * cos(M_PI * (i / (CIRCLE_RESOLUTION / 2.0f)));
//		verts[i].y = y + r * sin(M_PI * (i / (CIRCLE_RESOLUTION / 2.0f)));
//		verts[i].z = 0;
//		verts[i].rhw = 1;
//		verts[i].color = D3DCOLOR_ARGB(rgb.A, rgb.R, rgb.G, rgb.B);
//	}
//
//	g_pd3dDevice->SetFVF(VERTEX_2D_DIF::FVF);
//	g_pd3dDevice->DrawPrimitiveUP(D3DPT_LINESTRIP, CIRCLE_RESOLUTION, &verts, sizeof(VERTEX_2D_DIF));
//}
//
//void Draw::CircleFilled(int x, int y, float r, RGBA rgb)
//{
//	VERTEX_2D_DIF verts[CIRCLE_RESOLUTION + 1];
//
//	for (int i = 0; i < CIRCLE_RESOLUTION + 1; i++)
//	{
//		verts[i].x = x + r * cos(M_PI * (i / (CIRCLE_RESOLUTION / 2.0f)));
//		verts[i].y = y + r * sin(M_PI * (i / (CIRCLE_RESOLUTION / 2.0f)));
//		verts[i].z = 0;
//		verts[i].rhw = 1;
//		verts[i].color = D3DCOLOR_ARGB(rgb.A, rgb.R, rgb.G, rgb.B);
//	}
//
//	g_pd3dDevice->SetFVF(VERTEX_2D_DIF::FVF);
//	g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, CIRCLE_RESOLUTION - 1, &verts, sizeof(VERTEX_2D_DIF));
//}
//
//void Draw::CircleFilled(ImVec2 coord, float r, RGBA rgb)
//{
//	CircleFilled(coord.x, coord.y, r, rgb);
//}
//
//void Draw::CircleRange(Vector3 Pos, float points, float r, RGBA color, float thickness)
//{
//	float flPoint = M_PI * 2.0f / points;
//
//	//r += 80;
//	bool first = true;
//	ImVec2 First, Last;
//	for (float flAngle = flPoint; flAngle < (M_PI * 2.0f); flAngle += flPoint)
//	{
//		Vector3 vStart(r * cosf(flAngle) + Pos.x, r * sinf(flAngle) + Pos.z, Pos.y);
//		Vector3 vEnd(r * cosf(flAngle + flPoint) + Pos.x, r * sinf(flAngle + flPoint) + Pos.z, Pos.y);
//
//		float z_temp = vStart.z;
//		vStart.z = vStart.y;
//		vStart.y = z_temp;
//
//		z_temp = vEnd.z;
//		vEnd.z = vEnd.y;
//		vEnd.y = z_temp;
//
//		ImVec2 vStartScreen, vEndScreen;
//
//		vStartScreen = Direct3D9.WorldToScreen(vStart);
//		if (vStartScreen.x == 0 && vStartScreen.y == 0) break;
//		if (!((vStartScreen.x <= SCREENWIDTH * 1.2) && (vStartScreen.x >= SCREENWIDTH / 2 * (-1)) && (vStartScreen.y <= SCREENHEIGHT * 1.5) && (vStartScreen.y >= SCREENHEIGHT / 2 * (-1))))
//			return;
//
//		vEndScreen = Direct3D9.WorldToScreen(vEnd);
//		if (vEndScreen.x == 0 && vEndScreen.y == 0) break;
//
//		if (first)
//		{
//			First = vStartScreen;
//			first = false;
//		}
//
//		Line(vStartScreen.x, vStartScreen.y, vEndScreen.x, vEndScreen.y, color, thickness);
//		Last = vEndScreen;
//	}
//
//	Line(First.x, First.y, Last.x, Last.y, color, thickness);
//}
//
//void Draw::Bar(int x, int y, float value)
//{
//	int g = value * 2.55;
//	int r = 255 - g;
//	int l = value / 3;
//	FillRGB(x - (l / 2) - 1, y - 1, l + 2 + 13, 5, RGBA(1, 0, 0));
//	FillRGB(x - (l / 2), y, l + 13, 3, RGBA(r, g, 0));
//}
//
//void Draw::FillRGB(float x, float y, float w, float h, RGBA color)
//{
//	D3DXVECTOR2 vLine[2];
//
//	vLine[0].x = x + w / 2;
//	vLine[0].y = y;
//	vLine[1].x = x + w / 2;
//	vLine[1].y = y + h;
//
//	Line(vLine[0].x, vLine[0].y, vLine[1].x, vLine[1].y, color, w);
//}
//
//void Draw::GradientBox(int x, int y, int w, int h, RGBA rgb, RGBA rgb2, bool vertical)
//{
//	VERTEX_2D_DIF pVertex[4] = { { x, y, 0.0f, 1.0f, D3DCOLOR_ARGB(rgb.A, rgb.R, rgb.G, rgb.B) },
//		{ x + w, y, 0.0f, 1.0f, vertical ? D3DCOLOR_ARGB(rgb.A, rgb.R, rgb.G, rgb.B) : D3DCOLOR_ARGB(rgb2.A, rgb2.R, rgb2.G, rgb2.B) },
//		{ x, y + h, 0.0f, 1.0f, vertical ? D3DCOLOR_ARGB(rgb2.A, rgb2.R, rgb2.G, rgb2.B) : D3DCOLOR_ARGB(rgb.A, rgb.R, rgb.G, rgb.B) },
//		{ x + w, y + h, 0.0f, 1.0f, D3DCOLOR_ARGB(rgb2.A, rgb2.R, rgb2.G, rgb2.B) } };
//	g_pd3dDevice->SetFVF(VERTEX_2D_DIF::FVF);
//	g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, &pVertex, sizeof(VERTEX_2D_DIF));
//}
//
//void Draw::GradientBoxOutlined(int x, int y, int w, int h, RGBA rgb, RGBA rgb2, bool vertical, float thickness, RGBA bcolor)
//{
//	GradientBox(x, y, w, h, rgb, rgb2, vertical);
//	BoxBorder(x, y, w, h, bcolor, thickness);
//}

Draw* draw = new Draw();