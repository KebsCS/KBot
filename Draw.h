#pragma once

#ifndef _DRAW_H_
#define _DRAW_H_

#include "DirectX.h"


class Draw 
{
private:
	LPDIRECT3DTEXTURE9 LoadTextureFromFile(const char* filename, LPDIRECT3DTEXTURE9* out_texture, int* out_width, int* out_height, LPDIRECT3DDEVICE9 xD);
public:


	//Filename, posx, posy, Text below image, 
	//unique index to render few same images, width, height, 
	//inWindow if true renders in imgui window, if false renders standalone 
	void Image(std::string _filename, int x, int y, std::string text = "", int index = 0, int in_width = 64, int in_height = 64, bool inWindow = false);

	//draws line
	void Line(int x, int y, int x2, int y2, RGBA rgb, float thickness = 1.f);

	//draws text
	void String(std::string text, int x, int y, int orientation, RGBA color, ID3DXFont* font, bool bordered = true, RGBA bcolor = RGBA(1, 0, 0));

	//draws filled box
	void BoxFilled(int x, int y, int w, int h, RGBA rgb);

	//draws box with a border
	void BoxBordered(int x, int y, int w, int h, RGBA color, int thickness = 1);

	//draws only box outline
	void BoxOutlined(int x, int y, int w, int h, RGBA color, float thickness = 1, RGBA bcolor = RGBA(1, 0, 0));

	//draws string in a box
	void StringBoxed(std::string text, int x, int y, int orientation, RGBA color, ID3DXFont* font, RGBA bcolor = RGBA(1, 0, 0), RGBA background = RGBA(0,0,0,0));

	//draws only circle outline
	void Circle(int x, int y, float r, RGBA rgb);

	//draws filled circle
	void CircleFilled(int x, int y, float r, RGBA rgb);

	//draws game's cricle range
	void CircleRange(Vector3 Pos, float points, float r, RGBA color, float thickness = 1.f);

};
extern Draw* draw;



#endif // !_DRAW_H_
