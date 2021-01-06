#pragma once

#ifndef _DRAW_H_
#define _DRAW_H_

#include "DirectX.h"
#include "Bytes.h"

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

class Draw
{
private:
	//LPDIRECT3DTEXTURE9 LoadTextureFromFile(const char* filename, LPDIRECT3DTEXTURE9* out_texture, int* out_width, int* out_height, LPDIRECT3DDEVICE9 xD);
public:

	//todo use image class
	LPDIRECT3DTEXTURE9 textureBarrier = NULL;
	LPDIRECT3DTEXTURE9 textureClarity = NULL;
	LPDIRECT3DTEXTURE9 textureCleanse = NULL;
	LPDIRECT3DTEXTURE9 textureExhaust = NULL;
	LPDIRECT3DTEXTURE9 textureFlash = NULL;
	LPDIRECT3DTEXTURE9 textureGhost = NULL;
	LPDIRECT3DTEXTURE9 textureHeal = NULL;
	LPDIRECT3DTEXTURE9 textureIgnite = NULL;
	LPDIRECT3DTEXTURE9 textureSmite = NULL;
	LPDIRECT3DTEXTURE9 textureTeleport = NULL;

	LPDIRECT3DTEXTURE9 textureKEKW = NULL;
	LPDIRECT3DTEXTURE9 textureSks = NULL;

	//initializes textures from bytes in memory
	bool InitTextures();

	//not needed since now every texture is loaded from memory
	//void Image(std::string _filename, int x, int y, std::string text = "", int index = 0, int in_width = 64, int in_height = 64, bool inWindow = false);

	//Filename, posx, posy, Text below image,
	//unique index to render few same images, width, height,
	//inWindow if true renders in imgui window, if false renders standalone
	void ImageFromMemory(LPDIRECT3DTEXTURE9 texturename, int x, int y, std::string text, int index, int in_width, int in_height, bool inWindow = false);

	//draws line
	void Line(int x, int y, int x2, int y2, RGBA rgb, float thickness = 1.f);
	void Line(ImVec2 pos1, ImVec2 pos2, RGBA rgb, float thickness = 1.f);

	//draws text
	void String(std::string text, int x, int y, int orientation, RGBA color, ID3DXFont* font, bool bordered = true, RGBA bcolor = RGBA(1, 0, 0));

	//draws filled box
	void BoxFilled(int x, int y, int w, int h, RGBA rgb);

	//draws box with a border
	void BoxBordered(int x, int y, int w, int h, RGBA color, int thickness = 1);

	//draws only box outline
	void BoxOutlined(int x, int y, int w, int h, RGBA color, float thickness = 1, RGBA bcolor = RGBA(1, 0, 0));

	//draws string in a box
	void StringBoxed(std::string text, int x, int y, int orientation, RGBA color, ID3DXFont* font, RGBA bcolor = RGBA(1, 0, 0), RGBA background = RGBA(0, 0, 0, 0));

	//draws only circle outline
	void Circle(int x, int y, float r, RGBA rgb);

	//draws filled circle
	void CircleFilled(int x, int y, float r, RGBA rgb);
	void CircleFilled(ImVec2 coord, float r, RGBA rgb);

	//draws game's circle range
	void CircleRange(Vector3 Pos, float points, float r, RGBA color, float thickness = 1.f);
};
extern Draw* draw;

#endif // !_DRAW_H_
