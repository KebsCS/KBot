#pragma once

#ifndef _DRAW_H
#define _DRAW_H

#include "DirectX.h"


class Draw 
{
private:
	LPDIRECT3DTEXTURE9 LoadTextureFromFile(const char* filename, LPDIRECT3DTEXTURE9* out_texture, int* out_width, int* out_height, LPDIRECT3DDEVICE9 xD);
	Direct3D9Render Direct3D;
public:


	//Filename, posx, posy, Text below image, 
	//unique index to render few same images, width, height, 
	//inWindow if true renders in imgui window, if false renders standalone 
	void DrawImage(std::string _filename, int x, int y, std::string text = "", int index = 0, int in_width = 64, int in_height = 64, bool inWindow = false);

	void String(std::string text, int x, int y, RGBA rgb, ID3DXFont* font);
	void StringOutlined(std::string text, int x, int y, RGBA rgb, ID3DXFont* pFont);
	void Rect(int x, int y, int l, int h, RGBA rgb);
	void Line(int x, int y, int x2, int y2, RGBA rgb, float thickness = 1.f);
	void DrawCircle(float x, float y, float r, RGBA rgb);
	void DrawCircleFilled(float x, float y, float r, RGBA rgb);
	void BorderBox(int x, int y, int l, int h, int thickness, RGBA color);
	void DrawCircleRange(Vector3 vPos, float flPoints, float flRadius, RGBA color, float flThickness = 1.f);

};




#endif // !_DRAW_H
