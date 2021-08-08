#pragma once
#include <limits>
#include <iostream>

struct Coord
{
	int x = 0, y = 0;
	Coord() {};
	Coord(int X, int Y) : x(X), y(Y) {};
	bool operator==(const Coord& other)
	{
		return x == other.x && y == other.y;
	}
	bool operator!=(const Coord& other)
	{
		return x != other.x || y != other.y;
	}
	Coord operator+(const Coord& other)
	{
		return { x + other.x, y + other.y };
	}
	Coord operator-(const Coord& other)
	{
		return { x - other.x, y - other.y };
	}
	Coord operator*(const Coord& other)
	{
		return { x * other.x, y * other.y };
	}
	Coord operator*(int fl)//maybe float
	{
		return { x * fl, y * fl };
	}
	Coord operator/(const Coord& other)
	{
		return { x / other.x, y / other.y };
	}
	Coord operator/(int fl)//same thing here
	{
		return { x / fl, y / fl };
	}
	bool IsValid() const
	{
		return x != INT_MAX && y != INT_MAX;
	}
	void Invalidate()
	{
		x = y = INT_MAX;
	}
};

struct Angle
{
	float yaw = 0.f, pitch = 0.f, roll = 0.f;
	Angle operator+(const Angle& v) const
	{
		return { yaw + v.yaw, pitch + v.pitch,roll + v.roll };
	}
	Angle operator-(const Angle& v) const
	{
		return { yaw - v.yaw,pitch - v.pitch, roll - v.roll };
	}
};

struct Vec2
{
	float x = 0.f, y = 0.f;
	Vec2() {};
	Vec2(float X, float Y) : x(X), y(Y) {};
	bool operator==(const Vec2& other)
	{
		return x == other.x && y == other.y;
	}
	bool operator!=(const Vec2& other)
	{
		return x != other.x || y != other.y;
	}
	Vec2 operator+(const Vec2& other)
	{
		return { x + other.x, y + other.y };
	}
	Vec2 operator-(const Vec2& other)
	{
		return { x - other.x, y - other.y };
	}
	Vec2 operator*(const Vec2& other)
	{
		return { x * other.x, y * other.y };
	}
	Vec2 operator*(float fl)
	{
		return { x * fl, y * fl };
	}
	Vec2 operator/(const Vec2& other)
	{
		return { x / other.x, y / other.y };
	}
	Vec2 operator/(float fl)
	{
		return { x / fl, y / fl };
	}

	Vec2& operator+=(const Vec2& v)
	{
		x += v.x; y += v.y;
		return *this;
	}
	Vec2& operator-=(const Vec2& v)
	{
		x -= v.x; y -= v.y;
		return *this;
	}
	Vec2& operator*=(float fl)
	{
		x *= fl;
		y *= fl;
		return *this;
	}
	Vec2& operator*=(const Vec2& v)
	{
		x *= v.x;
		y *= v.y;
		return *this;
	}
	Vec2& operator/=(const Vec2& v)
	{
		x /= v.x;
		y /= v.y;
		return *this;
	}
	Vec2& operator+=(float fl)
	{
		x += fl;
		y += fl;
		return *this;
	}
	Vec2& operator/=(float fl)
	{
		x /= fl;
		y /= fl;
		return *this;
	}
	Vec2& operator-=(float fl)
	{
		x -= fl;
		y -= fl;
		return *this;
	}
	bool IsValid() const
	{
		return std::isfinite(x) && std::isfinite(y);
	}
	void Invalidate()
	{
		x = y = std::numeric_limits<float>::infinity();
	}
	void Zero()
	{
		x = y = 0.0f;
	}
};
//Vector 3
struct Vec3
{
	float x = 0.f, y = 0.f, z = 0.f;
	Vec3() {};
	Vec3(float X, float Y, float Z) : x(X), y(Y), z(Z) {};
	bool operator==(const Vec3& other)
	{
		return x == other.x && y == other.y && z == other.z;
	}
	bool operator!=(const Vec3& other)
	{
		return x != other.x || y != other.y || z != other.z;
	}
	Vec3 operator+(const Vec3& other)
	{
		return { x + other.x, y + other.y,z + other.z };
	}
	Vec3 operator-(const Vec3& other)
	{
		return { x - other.x, y - other.y,z - other.z };
	}
	Vec3 operator*(const Vec3& other)
	{
		return { x * other.x, y * other.y,z * other.z };
	}
	Vec3 operator*(float fl)
	{
		return { x * fl, y * fl, z * fl };
	}
	Vec3 operator/(const Vec3& other)
	{
		return { x / other.x, y / other.y,z / other.z };
	}
	Vec3 operator/(float fl)
	{
		return { x / fl, y / fl, z / fl };
	}
	Vec3& operator+=(const Vec3& v)
	{
		x += v.x; y += v.y; z += v.z;
		return *this;
	}
	Vec3& operator-=(const Vec3& v)
	{
		x -= v.x; y -= v.y; z -= v.z;
		return *this;
	}
	Vec3& operator*=(float fl)
	{
		x *= fl;
		y *= fl;
		z *= fl;
		return *this;
	}
	Vec3& operator*=(const Vec3& v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		return *this;
	}
	Vec3& operator/=(const Vec3& v)
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;
		return *this;
	}
	Vec3& operator+=(float fl)
	{
		x += fl;
		y += fl;
		z += fl;
		return *this;
	}
	Vec3& operator/=(float fl)
	{
		x /= fl;
		y /= fl;
		z /= fl;
		return *this;
	}
	Vec3& operator-=(float fl)
	{
		x -= fl;
		y -= fl;
		z -= fl;
		return *this;
	}
	float Length() const
	{
		return sqrtf(x * x + y * y + z * z);
	}
	float Length2D() const
	{
		return sqrtf(x * x + y * y);
	}
	bool IsValid() const
	{
		return std::isfinite(x) && std::isfinite(y) && std::isfinite(z);
	}
	void Invalidate()
	{
		x = y = z = std::numeric_limits<float>::infinity();
	}
	void Zero()
	{
		x = y = z = 0.0f;
	}
};
struct Vec4
{
	float x = 0.f, y = 0.f, z = 0.f, w = 0.f;
};
struct Matrix
{
	float m[16];
};

struct PlayerEntity
{
	PlayerEntity() {};
	char pad0[4];//0x0
	Vec3 eyePos;//0x4
	Vec3 acceleration;//0x10
	char pad1[24];//0x30
	Vec3 pos;//0x34
	union
	{
		Angle viewAngles;//0x40
		Vec3 vecViewAngles;//0x40
	};
	char pad2[8];//0x4C
	int airtime;//0x54
	char pad3[160];//0xF4
	int health;//0xF8
	char pad4[40];//0xFC
	int sniperammo;//0x124
	int mp5ammo;//0x128
	char pad5[32];//0x12C
	int snipermag;//0x14C
	int mp5mag;//0x150
	char pad6[4];//0x154
	int grenades;//0x158
	char pad7[201];//0x162
	char name[16];//0x225
	bool Alive()
	{
		return health > 0;
	}
};
