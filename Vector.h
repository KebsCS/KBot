#pragma once

#ifndef _Vector3_H
#define _Vector3_H

#include <cmath>
#include <limits>

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#define M_PI_F (float)M_PI
#endif

struct ProjectionInfo;
struct IntersectionResult;

struct Vector4
{
	float     x, y, z, w;
	Vector4() { x = y = z = w = 0.0f; }
	Vector4(float _x, float _y, float _z, float _w) { x = _x; y = _y; z = _z; w = _w; }
};

class Vector3
{
public:
	float x, z, y;

	Vector3();
	Vector3(float x, float y, float z = 0);
	Vector3(float xyz);

	~Vector3();

	bool IsValid() const;

	bool operator==(Vector3 const& other) const;
	bool operator!=(Vector3 const& other) const;
	bool IsZero(float tolerance = 0.01f) const;

	Vector3& operator*=(const Vector3& v);
	Vector3& operator*=(float s);

	Vector3& operator/=(const Vector3& v);
	Vector3& operator/=(float s);

	Vector3& operator+=(const Vector3& v);
	Vector3& operator+=(float fl);

	Vector3& operator-=(const Vector3& v);
	Vector3& operator-=(float fl);

	Vector3 operator-(const Vector3& v) const;
	Vector3 operator-(float mod) const;
	Vector3 operator+(const Vector3& v) const;
	Vector3 operator+(float mod) const;

	Vector3 operator/(const Vector3& v) const;
	Vector3 operator/(float mod) const;
	Vector3 operator*(const Vector3& v) const;
	Vector3 operator*(float mod) const;

	Vector3& operator=(const Vector3& v);

	Vector3& SwitchYZ();
	Vector3& Negate();

	float Length() const;
	float LengthSquared() const;

	Vector3 Normalized() const;
	float NormalizeInPlace() const;

	float Distance(Vector3 const& to) const;
	float Distance(Vector3 const& segment_start, Vector3 const& segment_end, bool only_if_on_segment = false, bool squared = false) const;
	float DistanceSquared(Vector3 const& to) const;

	float DotProduct(Vector3 const& other) const;
	float CrossProduct(Vector3 const& other) const;
	float Polar() const;
	float AngleBetween(Vector3 const& other) const;

	bool Close(float a, float b, float eps) const;

	Vector3 Rotated(float angle) const;
	Vector3 Perpendicular() const;
	Vector3 Perpendicular2() const;
	Vector3 Extend(Vector3 const& to, float distance) const;

	Vector3 Append(Vector3 pos1, Vector3 pos2, float dist) const;

	ProjectionInfo ProjectOn(Vector3 const& segment_start, Vector3 const& segment_end) const;
	IntersectionResult Intersection(Vector3 const& line_segment_end, Vector3 const& line_segment2_start, Vector3 const& line_segment2_end) const;

	Vector3 Scale(float s)
	{
		return Vector3(x * s, y * s, z * s);
	}

	Vector3 Rotate(Vector3 startPos, float theta)
	{
		float dx = this->x - startPos.x;
		float dz = this->z - startPos.z;

		float px = dx * cos(theta) - dz * sin(theta);
		float pz = dx * sin(theta) + dz * cos(theta);
		return { px + startPos.x, this->y, pz + startPos.z };
	}
};

struct ProjectionInfo
{
	bool IsOnSegment;
	Vector3 LinePoint;
	Vector3 SegmentPoint;

	ProjectionInfo(bool is_on_segment, Vector3 const& segment_point, Vector3 const& line_point);
};

struct IntersectionResult
{
	bool Intersects;
	Vector3 Point;

	IntersectionResult(bool intersects = false, Vector3 const& point = Vector3());
};

#endif // !_Vector3_H
