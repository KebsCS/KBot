#pragma once

#ifndef _DEFINITIONS_H_
#define _DEFINITIONS_H_

#pragma warning(disable:4996)

#include <cmath>


// default size
#define SCREENWIDTH ::GetSystemMetrics(SM_CXSCREEN)
#define SCREENHEIGHT ::GetSystemMetrics(SM_CYSCREEN)


#define M_PI 3.14159265358979323846	// pi
#define M_PI_F ((float)(M_PI))	// Shouldn't collide with anything.

#define RandomInt(min, max) (rand() % (max - min + 1) + min) // inclusive random int (e.g 0 to 100)

#define INRANGE(x,a,b)    (x >= a && x <= b) //if a<=x<=b

#define SENDINPUTX(a) (a * 65536 / (SCREENWIDTH)+1) //used to calculate coordinates using SendInput
#define SENDINPUTY(a) (a * 65536 / (SCREENHEIGHT)+1) //used to calculate coordinates using SendInput

#ifndef RAD2DEG
#define RAD2DEG( x  )  ( (float)(x) * (float)(180.f / M_PI_F) ) // Radians to degrees
#endif

#ifndef DEG2RAD
#define DEG2RAD( x  )  ( (float)(x) * (float)(M_PI_F / 180.f) ) // degrees to radians
#endif



#define HUMANIZER

#ifdef HUMANIZER

#define VERYFAST_REACTION_TIME RandomInt(1,10)
#define FAST_REACTION_TIME RandomInt(9,11)
#define MEDIUM_REACTION_TIME RandomInt(90,110)
#define SLOW_REACTION_TIME RandomInt(900,1100)

#else
#define VERYFAST_REACTION_TIME 5
#define FAST_REACTION_TIME 10
#define MEDIUM_REACTION_TIME 100
#define SLOW_REACTION_TIME 1000

#endif

enum text_alignment { lefted, centered, righted };

enum Arrow_Keys : int
{
	LEFT = 1,
	RIGHT = 2,
	UP = 3,
	DOWN = 4


};

enum SpellSlotID
{
	Q = 0,
	W = 1,
	E = 2,
	R = 3,
	Summoner1 = 4,
	Summoner2 = 5,
	Item1 = 6,
	Item2 = 7,
	Item3 = 8,
	Item4 = 9,
	Item5 = 10,
	Item6 = 11,
	Trinket = 12,
	Recall = 13
};

struct fPoint
{
	float x;
	float y;
};

struct RGBA
{
	int R, G, B, A;
	RGBA()
	{
		R = G = B = A = 0;
	}
	RGBA(int r, int g, int b, int a=255)
	{
		R = r;
		G = g;
		B = b;
		A = a;
	}
	RGBA(unsigned int hex) // HEX to RGB
	{
		R = ((hex >> 24) & 0xFF);
		G = ((hex >> 16) & 0xFF);
		B = ((hex >> 8) & 0xFF);
		A = ((hex) & 0xFF);
	}
};

struct Area
{
	int x1;
	int x2;
	int y1;
	int y2;
	Area()
	{
		x1 = x2 = y1 = y2 = 0;
	}
	//left, top, right, bottom
	Area(int _x1, int _y1, int _x2, int _y2)
	{
		x1 = _x1;
		x2 = _x2;
		y1 = _y1;
		y2 = _y2;
	}

};

struct Pixel
{
	unsigned int _color;   //assign colors in HEX (RR GG BB AA)  -  RGBA format (typically alpha is 00)
	int _x;
	int _y;

	Pixel()
	{
		_x = _y = _color = 0;
	}
	Pixel(unsigned int color, int x, int y)
	{
		_x = x;
		_y = y;
		_color = color;
	}
	void Set(unsigned int color, int x, int y)
	{
		_x = x;
		_y = y;
		_color = color;
	}
};


struct Vector3
{
	float X, Y, Z;

	inline Vector3() 
	{
		X = Y = Z = 0;
	}
	inline Vector3(const float x, const float y, const float z)
	{
		X = x; Y = y; Z = z;
	}
	inline void Zero()
	{
		X = Y = Z = 0;
	}

	inline bool operator == (const Vector3& A) const
	{
		if (X == A.X && Y == A.Y && Z == A.Z)
			return true;
	}

	inline Vector3 operator + (const Vector3& A) const
	{
		return Vector3(X + A.X, Y + A.Y, Z + A.Z);
	}

	inline Vector3 operator + (const float A) const
	{
		return Vector3(X + A, Y + A, Z + A);
	}

	inline Vector3 operator * (const float A) const
	{
		return Vector3(A * X, A * Y, A * Z);
	}

	inline Vector3 operator * (const Vector3& A) const
	{
		return Vector3(A.X * X, A.Y * Y, A.Z * Z);
	}

	inline Vector3 operator - (const float A) const
	{
		return Vector3(A * X, A * Y, A * Z);
	}

	inline Vector3 operator - (const Vector3& A) const
	{
		return Vector3(A.X - X, A.Y - Y, A.Z - Z);
	}

	inline Vector3 operator / (const float A) const
	{
		return Vector3(A / X, A / Y, A / Z);
	}

	inline Vector3 operator / (const Vector3& A) const
	{
		return Vector3(A.X / X, A.Y / Y, A.Z / Z);
	}

	float dot(const Vector3& vec) const
	{
		return X * vec.X + Y * vec.Y + Z * vec.Z;
	}

	inline float lengthSquared()
	{
		return X * X + Y * Y + Z * Z;
	}

	inline float length()
	{
		return (float)sqrt(lengthSquared());
	}

	inline Vector3 perpendicularTo()
	{
		return Vector3(Z, Y, -X);
	}

	inline Vector3 Normalize()
	{
		float length = this->length();
		if (length != 0)
		{
			float inv = 1.0f / length;
			X *= inv;
			Y *= inv;
			Z *= inv;
		}
		return Vector3(X, Y, Z);
	}

	inline float DistTo(const Vector3& A) {
		float out = sqrtf(powf(X - A.X, 2) + powf(Y - A.Y, 2) + powf(Z - A.Z, 2));
		return out < 0 ? out * -1 : out;
	}
};

struct Vector4
{
	float X, Y, Z, W;

	inline Vector4()
	{
		X = Y = Z = W = 0;
	}
	inline Vector4(const float x, const float y, const float z, const float w)
	{
		X = x; Y = y; Z = z; W = w;
	}
};




#endif // !_DEFINITIONS_H_
