#include "Vector.h"

Vector3::Vector3()
{
	this->x = 0; this->y = 0; this->z = 0;
}

Vector3::Vector3(float x, float y, float z)
{
	this->x = x; this->y = y; this->z = z;
}

Vector3::Vector3(float xyz)
{
	this->x = this->y = this->z = xyz;
}

Vector3::~Vector3() = default;

bool Vector3::IsValid() const
{
	return this->x != 0/* && this->y != 0*/ && this->z != 0;
}

bool Vector3::IsZero(float tolerance) const
{
	return this->x > -tolerance && this->x < tolerance&&
		this->z > -tolerance && this->z < tolerance;
}

bool Vector3::operator==(Vector3 const& other) const
{
	return other.x == this->x && other.y == this->y && other.z == this->z;
}

bool Vector3::operator!=(Vector3 const& other) const
{
	return other.x == this->x && other.y == this->y && other.z == this->z;
}

Vector3& Vector3::operator*=(const Vector3 & v)
{
	this->x *= v.x;
	this->y *= v.y;
	this->z *= v.z;
	return *this;
}

Vector3& Vector3::operator*=(float s)
{
	this->x *= s;
	this->y *= s;
	this->z *= s;
	return *this;
}

Vector3& Vector3::operator/=(const Vector3 & v)
{
	this->x /= v.x;
	this->y /= v.y;
	this->z /= v.z;
	return *this;
}

Vector3& Vector3::operator/=(float s)
{
	this->x /= s;
	this->y /= s;
	this->z /= z;
	return *this;
}

Vector3& Vector3::operator+=(const Vector3 & v)
{
	this->x += v.x;
	this->y += v.y;
	this->z += v.z;
	return *this;
}

Vector3& Vector3::operator+=(float fl)
{
	this->x += fl;
	this->y += fl;
	this->z += fl;
	return *this;
}

Vector3& Vector3::operator-=(const Vector3 & v)
{
	this->x -= v.x;
	this->y -= v.y;
	this->z -= v.z;
	return *this;
}

Vector3& Vector3::operator-=(float fl)
{
	this->x -= fl;
	this->y -= fl;
	this->z -= fl;
	return *this;
}

Vector3 Vector3::operator-(const Vector3 & v) const
{
	Vector3 result(this->x - v.x, this->y - v.y, this->z - v.z);
	return result;
}

Vector3 Vector3::operator-(float mod) const
{
	Vector3 result(this->x - mod, this->y - mod, this->z - mod);
	return result;
}

Vector3 Vector3::operator+(const Vector3 & v) const
{
	Vector3 result(this->x + v.x, this->y + v.y, this->z + v.z);
	return result;
}

Vector3 Vector3::operator+(float mod) const
{
	Vector3 result(this->x + mod, this->y + mod, this->z + mod);
	return result;
}

Vector3 Vector3::operator/(const Vector3 & v) const
{
	Vector3 result(this->x / v.x, this->y / v.y, this->z / v.z);
	return result;
}

Vector3 Vector3::operator/(float mod) const
{
	Vector3 result(this->x / mod, this->y / mod, this->z / mod);
	return result;
}

Vector3 Vector3::operator*(const Vector3 & v) const
{
	Vector3 result(this->x * v.x, this->y * v.y, this->z * v.z);
	return result;
}

Vector3 Vector3::operator*(float mod) const
{
	Vector3 result(this->x * mod, this->y * mod, this->z * mod);
	return result;
}

Vector3& Vector3::operator=(const Vector3 & v) = default;

Vector3& Vector3::SwitchYZ()
{
	const auto temp = this->y;

	this->y = this->z;
	this->z = temp;
	return *this;
}

Vector3& Vector3::Negate()
{
	this->x = -this->x;
	this->y = -this->y;
	this->z = -this->z;

	return *this;
}

float Vector3::Length() const
{
	return sqrtf(this->x * this->x + this->y * this->y + this->z * this->z);
}

float Vector3::LengthSquared() const
{
	return this->x * this->x + this->y * this->y + this->z * this->z;
}

Vector3 Vector3::Normalized() const
{
	auto const length = this->Length();
	if (length != 0)
	{
		auto const inv = 1.0f / length;
		return { this->x * inv, this->y * inv, this->z * inv };
	}

	return *this;
}
float Vector3::NormalizeInPlace() const
{
	auto v = *this;
	auto const l = this->Length();

	if (l != 0.0f)
	{
		v /= l;
	}
	else
	{
		v.x = v.z = 0.0f; v.y = 1.0f;
	}
	return l;
}

float Vector3::Distance(Vector3 const& to) const
{
	Vector3 delta;

	delta.x = x - to.x;
	delta.y = y - to.y;
	delta.z = z - to.z;

	return delta.Length();
}

float Vector3::DistanceSquared(Vector3 const& to) const
{
	Vector3 delta;

	delta.x = x - to.x;
	delta.y = y - to.y;
	delta.y = z - to.z;

	return delta.LengthSquared();
}

float Vector3::DotProduct(Vector3 const& other) const
{
	return this->x * other.x + this->y * other.y + this->z * other.z;
}

float Vector3::CrossProduct(Vector3 const& other) const
{
	return other.y * this->x - other.x * this->y;
}

float Vector3::Polar() const
{
	if (this->Close(x, 0.f, 0.f))
	{
		if (y > 0.f)
		{
			return 90.f;
		}
		return y < 0.f ? 270.f : 0.f;
	}

	auto theta = atan(y / x) * 180.f / M_PI;
	if (x < 0.f)
	{
		theta = theta + 180.f;
	}
	if (theta < 0.f)
	{
		theta = theta + 360.f;
	}
	return theta;
}

float Vector3::AngleBetween(Vector3 const& other) const
{
	auto theta = Polar() - other.Polar();
	if (theta < 0.f)
	{
		theta = theta + 360.f;
	}
	if (theta > 180.f)
	{
		theta = 360.f - theta;
	}
	return theta;
}

bool Vector3::Close(float a, float b, float eps) const
{
	if (abs(eps) < FLT_EPSILON)
	{
		eps = static_cast<float>(1e-9);
	}
	return abs(a - b) <= eps;
}

// Expects angle in radians!
Vector3 Vector3::Rotated(float angle) const
{
	auto const c = cos(angle);
	auto const s = sin(angle);

	return { static_cast<float>(x * c - z * s), y, static_cast<float>(z * c + x * s) };
}

Vector3 Vector3::Perpendicular() const
{
	return { -z,y,x };
}

Vector3 Vector3::Perpendicular2() const
{
	return { z,y,-x };
}

Vector3 Vector3::Extend(Vector3 const& to, float distance) const
{
	const auto from = *this;
	const auto result = from + (to - from).Normalized() * distance;
	return result;
}

Vector3 Vector3::Append(Vector3 pos1, Vector3 pos2, float dist) const
{
	return pos2 + (pos2 - pos1).Normalized() * dist;
}

ProjectionInfo::ProjectionInfo(const bool is_on_segment, Vector3 const& segment_point, Vector3 const& line_point) :
	IsOnSegment(is_on_segment), LinePoint(line_point), SegmentPoint(segment_point)
{
}

IntersectionResult::IntersectionResult(const bool intersects, Vector3 const& point) :
	Intersects(intersects), Point(point)
{
}

ProjectionInfo Vector3::ProjectOn(Vector3 const& segment_start, Vector3 const& segment_end) const
{
	float rs;
	auto const cx = x;
	auto const cz = z;
	auto const ax = segment_start.x;
	auto const az = segment_start.z;
	auto const bx = segment_end.x;
	auto const bz = segment_end.z;

	auto const cy = y;
	auto const ay = segment_start.y;
	auto const by = segment_end.y;

	const auto rl = ((cx - ax) * (bx - ax) + (cz - az) * (bz - az) + (cy - ay) * (by - ay)) / (pow(bx - ax, 2) + pow(bz - az, 2) + pow(by - ay, 2));
	const auto point_line = Vector3(ax + rl * (bx - ax), ay + rl * (by - ay), az + rl * (bz - az));

	//	const auto rl = ((cx - ax) * (bx - ax) + (cz - az) * (bz - az)) / (pow(bx - ax, 2) + pow(bz - az, 2));
	//	const auto point_line = Vector3(ax + rl * (bx - ax), 0, az + rl * (bz - az));

	if (rl < 0)
	{
		rs = 0;
	}
	else if (rl > 1)
	{
		rs = 1;
	}
	else
	{
		rs = rl;
	}

	auto const is_on_segment = rs == rl;
	auto const point_segment = is_on_segment ? point_line : Vector3(ax + rs * (bx - ax), ay + rs * (by - ay), az + rs * (bz - az));
	//auto const point_segment = is_on_segment ? point_line : Vector3(ax + rs * (bx - ax), 0, az + rs * (bz - az));

	return ProjectionInfo(is_on_segment, point_segment, point_line);
}

IntersectionResult Vector3::Intersection(Vector3 const& line_segment_end, Vector3 const& line_segment2_start,
	Vector3 const& line_segment2_end) const
{
	const Vector3 side1 = { line_segment_end.x - this->x, line_segment_end.y - this->y,line_segment_end.z - this->z };
	const Vector3 side2 = { line_segment2_end.x - line_segment2_start.x, line_segment2_end.y - line_segment2_start.y,line_segment2_end.z - line_segment2_start.z };

	const auto s = (-side1.z * (this->x - line_segment2_start.x) + side1.x * (this->z - line_segment2_start.z)) / (-side2.x * side1.z + side1.x * side2.z);
	const auto t = (side2.x * (this->z - line_segment2_start.z) - side2.z * (this->x - line_segment2_start.x)) / (-side2.x * side1.z + side1.x * side2.z);

	if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
		return { true,{ this->x + t * side1.x,  this->y + t * side1.y, this->z + t * side1.z } };

	return { false,{} };
}