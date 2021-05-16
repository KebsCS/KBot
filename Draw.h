#pragma once

#ifndef _DRAW_H_
#define _DRAW_H_

#include "DirectX.h"
#include "Bytes.h"
#include "Definitions.h"
#include "Geometry.h"

//
//class image {
//
//public:
//	std::string filename;
//	LPDIRECT3DTEXTURE9 texture;
//	int width = 64;
//	int height = 64;
//	ImVec2 loc;
//};
//
//static std::list<image> image_list;
//

static ImFont* fontArial = nullptr;
static ImFont* fontTahoma = nullptr;
static ImFont* fontTahomaSmall = nullptr;

class Draw
{
private:

public:
	ImDrawList* overlay;

	Draw()
	{
	}

	void Line(const ImVec2& start, const ImVec2& end, const RGBA& color, float thickness = 1.f);
	void Line(const int& x, const int& y, const int& x2, const int& y2, const RGBA& color, float thickness = 1.f);

	void CircleRange2(const Vector3& Pos, const float& points, float r, const RGBA& color, float thickness = 1.f);

	void CircleRange(const Vector3& worldPos, int numPoints, float radius, RGBA color, float thickness = 1.f, bool filled = 0) const;

	void String(const std::string text, const ImVec2& pos, const RGBA& color = RGBA(255, 255, 255), ImFont* font = fontTahoma);
	void String(const std::string text, const int& x, const int& y, const RGBA& color = RGBA(255, 255, 255), ImFont* font = fontTahoma);

	/*void String(std::string text, int x, int y, int orientation, RGBA color, int font, bool bordered = true, RGBA bcolor = RGBA(1, 0, 0))
	{
		String(x, y, text, color);
	}*/

	void Circle(const ImVec2& pos, float radius, const RGBA& color)
	{
		overlay->AddCircle(pos, radius, ImColor(color.R, color.G, color.B, color.A), 64, 1.f);
	}

	void Circle(const int& x, const int& y, float radius, const RGBA& color)
	{
		Circle(ImVec2(x, y), radius, color);
	}

	void Rect(const Vector4& box, const RGBA& color, float rounding = 0, float thickness = 1.0)
	{
		overlay->AddRect(ImVec2(box.x, box.y), ImVec2(box.z, box.w), ImColor(color.R, color.G, color.B, color.A), rounding, 15, thickness);
	}

	bool IsOnScreen(const ImVec2& point, float offsetX = 0.f, float offsetY = 0.f) const;

	bool IsOnScreen(const Vector3& point, float offsetX = 0.f, float offsetY = 0.f) const;

	void Polygon(const Geometry::Polygon poly, RGBA color);

	void Arrow(const Vector3& startPos, const Vector3& endPos, RGBA color);

	/*
	void DrawLineInternal(int x0, int y0, int x1, int y1, int r, int g, int b, int a);*/

	////todo use image class
	//ID3D11ShaderResourceView* textureBarrier = NULL;
	//ID3D11ShaderResourceView* textureClarity = NULL;
	//ID3D11ShaderResourceView* textureCleanse = NULL;
	//ID3D11ShaderResourceView* textureExhaust = NULL;
	//ID3D11ShaderResourceView* textureFlash = NULL;
	//ID3D11ShaderResourceView* textureGhost = NULL;
	//ID3D11ShaderResourceView* textureHeal = NULL;
	//ID3D11ShaderResourceView* textureIgnite = NULL;
	//ID3D11ShaderResourceView* textureSmite = NULL;
	//ID3D11ShaderResourceView* textureTeleport = NULL;

	//ID3D11ShaderResourceView* textureKEKW = NULL;
	//ID3D11ShaderResourceView* textureSks = NULL;

	////initializes textures from bytes in memory
	//bool InitTextures();

	////not needed since now every texture is loaded from memory
	////void Image(std::string _filename, int x, int y, std::string text = "", int index = 0, int in_width = 64, int in_height = 64, bool inWindow = false);

	////Filename, posx, posy, Text below image,
	////unique index to render few same images, width, height,
	////inWindow if true renders in imgui window, if false renders standalone
	//void ImageFromMemory(ID3D11ShaderResourceView* texturename, int x, int y, std::string text, int index, int in_width, int in_height, bool inWindow = false);

	//void Sprite(ID3D11ShaderResourceView* tex, float x, float y, float resolution = 2, float scale = 1, float rotation = 0);

	////draws line
	//void Line(int x, int y, int x2, int y2, RGBA rgb, float thickness = 1.f);
	//void Line(ImVec2 pos1, ImVec2 pos2, RGBA rgb, float thickness = 1.f);

	////draws text
	////void String(std::string text, int x, int y, int orientation, RGBA color, ID3DXFont* font, bool bordered = true, RGBA bcolor = RGBA(1, 0, 0));

	////draws filled box
	//void BoxFilled(int x, int y, int w, int h, RGBA rgb);

	////draws only box border
	//void BoxBorder(int x, int y, int w, int h, RGBA color, int thickness = 1);

	////draws box with a outline
	//void BoxOutlined(int x, int y, int w, int h, RGBA color, float thickness = 1, RGBA bcolor = RGBA(1, 0, 0));

	////draws string in a box
	////void StringBoxed(std::string text, int x, int y, int orientation, RGBA color, ID3DXFont* font, RGBA bcolor = RGBA(1, 0, 0), RGBA background = RGBA(0, 0, 0, 0));

	////draws only circle outline
	//void Circle(int x, int y, float r, RGBA rgb);

	////draws filled circle
	//void CircleFilled(int x, int y, float r, RGBA rgb);
	//void CircleFilled(ImVec2 coord, float r, RGBA rgb);

	////draws game's circle range
	//void CircleRange(Vector3 Pos, float points, float r, RGBA color, float thickness = 1.f);

	//void Bar(int x, int y, float value);

	//void FillRGB(float x, float y, float w, float h, RGBA color);

	//void GradientBox(int x, int y, int w, int h, RGBA rgb, RGBA rgb2, bool vertical);

	//void GradientBoxOutlined(int x, int y, int w, int h, RGBA rgb, RGBA rgb2, bool vertical, float thickness = 1, RGBA bcolor = RGBA(1, 0, 0));
};
extern Draw* draw;
#endif // !_DRAW_H_
