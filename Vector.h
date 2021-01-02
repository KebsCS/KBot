#pragma once


#ifndef _VECTOR_H
#define _VECTOR_H


#include <cmath>


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
		else return false;
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





#endif // !_VECTOR_H
