#include "Draw.h"




LPDIRECT3DTEXTURE9 Draw::LoadTextureFromFile(const char* filename, LPDIRECT3DTEXTURE9* out_texture, int* out_width, int* out_height, LPDIRECT3DDEVICE9 xD)
{
	// Load texture from disk
	LPDIRECT3DTEXTURE9 pTexture;
	HRESULT hr = D3DXCreateTextureFromFileA(xD, filename, &pTexture);
	if (hr != S_OK)
		return (LPDIRECT3DTEXTURE9)false;

	// Retrieve description of the texture surface so we can access its size
	D3DSURFACE_DESC my_image_desc;
	pTexture->GetLevelDesc(0, &my_image_desc);
	*out_texture = pTexture;
	*out_width = (int)my_image_desc.Width;
	*out_height = (int)my_image_desc.Height;
	return pTexture;
}



void Draw::DrawImage(std::string _filename, int x, int y, std::string text, int index, int in_width, int in_height, bool inWindow)
{

	bool has_saved_img_Ptr = false;
	image img_texture;
	for (image a : image_list) {
		if ((a.filename.compare(_filename) == 0) && (a.height == in_height) && (a.width == in_width)) {
			has_saved_img_Ptr = true;
			img_texture = a;
			break;
		}
	}

	if (!has_saved_img_Ptr) {
		img_texture.filename = _filename;
		img_texture.height = in_height;
		img_texture.width = in_width;
		img_texture.loc = ImVec2(img_texture.loc.x, img_texture.loc.y);
		LoadTextureFromFile(_filename.c_str(), &img_texture.texture, &in_width, &in_height, g_pd3dDevice);

		image_list.push_back(img_texture);

	}
	if (!inWindow)
	{
		bool Open = true;
		std::string wndName = "##texture" + img_texture.filename + std::to_string(index);
		const auto flags = ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs;

		ImGui::Begin(wndName.c_str(), &Open, flags);
		ImGui::SetWindowSize(ImVec2(in_width, in_height), ImGuiCond_Always);
		ImGui::SetWindowPos(ImVec2(x, y), ImGuiCond_Always);
	}
	//ImGui::Text("wndName = %s", wndName.c_str());
	//ImGui::Text("height x width = %d x %d", in_height, in_width);
	//ImGui::Text("image_list size = %d", image_list.size());
	//ImGui::Text("pointer = %p", img_texture.texture);
	//ImGui::Text("file name = %s", img_texture.filename);


	//todo draw over image

	ImGui::Image((void*)img_texture.texture, ImVec2(in_width, in_height));
	if (!text.empty())
	{
		//DrawString("testtesttesttesttest", x, y, RGBA(255, 255, 255), fontTahoma);
		ImGui::Text(text.c_str());
	}

	if (!inWindow)
	{
		ImGui::End();
	}
}


void Draw::String(std::string text, int x, int y, RGBA rgb, ID3DXFont* font)
{
	RECT FontPos{ x, y, x + 500, y + 100 };
	x -= 16 / 2; // center text
	font->DrawTextA(0, text.c_str(), text.size(), &FontPos, DT_NOCLIP, D3DCOLOR_ARGB(rgb.A, rgb.R, rgb.G, rgb.B));
}

void Draw::StringOutlined(std::string text, int x, int y, RGBA rgb, ID3DXFont* pFont)
{

	this->String(text, x - 1, y, RGBA(1, 0, 0), pFont);
	this->String(text, x, y - 1, RGBA(1, 0, 0), pFont);
	this->String(text, x + 1, y, RGBA(1, 0, 0), pFont);
	this->String(text, x, y + 1, RGBA(1, 0, 0), pFont);
	this->String(text, x, y, rgb, pFont);

}

void Draw::Rect(int x, int y, int l, int h, RGBA rgb)
{
	D3DRECT rect = { x, y, x + l, y + h };
	g_pd3dDevice->Clear(1, &rect, D3DCLEAR_TARGET, D3DCOLOR_ARGB(rgb.A, rgb.R, rgb.G, rgb.B), 0, 0);

}

void Draw::Line(int x, int y, int x2, int y2, RGBA rgb, float thickness)
{
	g_Line->SetWidth(thickness);
	D3DXVECTOR2 points[2];
	points[0].x = x;
	points[1].x = x2;
	points[0].y = y;
	points[1].y = y2;
	g_Line->Draw(points, 2, D3DCOLOR_ARGB(rgb.A, rgb.R, rgb.G, rgb.B));
}

static const int CIRCLE_RESOLUTION = 64;

struct VERTEX_2D_DIF 
{ 
	float x, y, z, rhw;
	D3DCOLOR color;
	static const DWORD FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE;
};


void  Draw::DrawCircle(float x, float y, float r, RGBA rgb)
{
	VERTEX_2D_DIF verts[CIRCLE_RESOLUTION + 1];

	for (int i = 0; i < CIRCLE_RESOLUTION + 1; i++)
	{
		verts[i].x = x + r * cos(M_PI_F * (i / (CIRCLE_RESOLUTION / 2.0f)));
		verts[i].y = y + r * sin(M_PI_F * (i / (CIRCLE_RESOLUTION / 2.0f)));
		verts[i].z = 0;
		verts[i].rhw = 1;
		verts[i].color = D3DCOLOR_ARGB(rgb.A, rgb.R, rgb.G, rgb.B);
	}

	g_pd3dDevice->SetFVF(VERTEX_2D_DIF::FVF);
	g_pd3dDevice->DrawPrimitiveUP(D3DPT_LINESTRIP, CIRCLE_RESOLUTION, &verts, sizeof(VERTEX_2D_DIF));
}



void  Draw::DrawCircleFilled(float x, float y, float r, RGBA rgb)
{
	VERTEX_2D_DIF verts[CIRCLE_RESOLUTION + 1];

	for (int i = 0; i < CIRCLE_RESOLUTION + 1; i++)
	{
		verts[i].x = x + r * cos(M_PI_F * (i / (CIRCLE_RESOLUTION / 2.0f)));
		verts[i].y = y + r * sin(M_PI_F * (i / (CIRCLE_RESOLUTION / 2.0f)));
		verts[i].z = 0;
		verts[i].rhw = 1;
		verts[i].color = D3DCOLOR_ARGB(rgb.A, rgb.R, rgb.G, rgb.B);
	}

	g_pd3dDevice->SetFVF(VERTEX_2D_DIF::FVF);
	g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, CIRCLE_RESOLUTION - 1, &verts, sizeof(VERTEX_2D_DIF));
}


void Draw::BorderBox(int x, int y, int l, int h, int thickness, RGBA color)
{
	this->Rect(x, y, l, thickness, color);
	this->Rect(x, y, thickness, h, color);
	this->Rect(x + l, y, thickness, h, color);
	this->Rect(x, y + h, l + thickness, thickness, color);
}



void Draw::DrawCircleRange(Vector3 vPos, float flPoints, float flRadius, RGBA color, float flThickness)
{

	float flPoint = M_PI_F * 2.0f / flPoints;
	

	flRadius = flRadius + 100;
	bool first = true;
	ImVec2 First, Last;
	for (float flAngle = flPoint; flAngle < (M_PI_F * 2.0f); flAngle += flPoint)
	{
		Vector3 vStart(flRadius * cosf(flAngle) + vPos.X, flRadius * sinf(flAngle) + vPos.Z, vPos.Y);
		Vector3 vEnd(flRadius * cosf(flAngle + flPoint) + vPos.X, flRadius * sinf(flAngle + flPoint) + vPos.Z, vPos.Y);

		float z_temp = vStart.Z;
		vStart.Z = vStart.Y;
		vStart.Y = z_temp;

		z_temp = vEnd.Z;
		vEnd.Z = vEnd.Y;
		vEnd.Y = z_temp;

		ImVec2 vStartScreen, vEndScreen;
		vStartScreen = Direct3D9.WorldToScreen(vStart);
		vEndScreen = Direct3D9.WorldToScreen(vEnd);

		if (vStartScreen.x == 0 && vStartScreen.y == 0) break;
		if (vEndScreen.x == 0 && vEndScreen.y == 0) break;

		if (first)
		{
			First = vStartScreen;
			first = false;
		}

		if (!((vStartScreen.x <= SCREENWIDTH * 1.2) && (vStartScreen.x >= SCREENWIDTH / 2 * (-1)) && (vStartScreen.y <= SCREENHEIGHT * 1.5) && (vStartScreen.y >= SCREENHEIGHT / 2 * (-1))))
			return;

		Line(vStartScreen.x, vStartScreen.y, vEndScreen.x, vEndScreen.y, color, flThickness);
		Last = vEndScreen;
	}

	Line(First.x, First.y, Last.x, Last.y, color, flThickness);
}