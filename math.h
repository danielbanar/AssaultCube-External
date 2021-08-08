#pragma once
#include "structs.h"
#define M_PI 3.14159265358979323846
inline float RAD2DEG(float rad)
{
	return rad * 57.295780181884765625f;
}
inline float DEG2RAD(float deg)
{
	return deg * 0.01745329238474369049072265625f;
}
inline float FASTSQRT(float x)
{
	unsigned int i = *(unsigned int*)&x;

	i += 127 << 23;
	// approximation of square root
	i >>= 1;
	return *(float*)&i;
}
Vec3 Forward(const Angle& angle)
{
	return { cosf(DEG2RAD(angle.pitch))*sinf(DEG2RAD(angle.yaw)), cosf(DEG2RAD(angle.pitch))*-cosf(DEG2RAD(angle.yaw)), sinf(DEG2RAD(angle.pitch)) };
}
Vec3 Right(const Angle& angle)
{
	return { cosf(DEG2RAD(angle.yaw)),sinf(DEG2RAD(angle.yaw)),0.f };
}
Vec3 Up()
{
	return { 0, 0, 1 };
}
Vec3 Up(const Angle& angle)
{
	return { sinf(DEG2RAD(angle.pitch)) * -sinf(DEG2RAD(angle.yaw)),sinf(DEG2RAD(angle.pitch)) * cosf(DEG2RAD(angle.yaw)), cosf(DEG2RAD(angle.pitch)) };
}
Angle CalcAngle(const Vec3& src, const Vec3& dst)
{
	Vec3 delta{ (dst.x - src.x), (dst.y - src.y), (src.z - dst.z) };
	float hyp = sqrtf(delta.x*delta.x + delta.y*delta.y);
	Angle angles{ RAD2DEG(-atan2f(delta.x , delta.y)),
				  RAD2DEG(atanf(delta.z / hyp)), 0.0f };
	if (angles.yaw < 0.f)
	{
		angles.yaw += 360.0f;
	}
	if (angles.yaw > 360.f)
	{
		angles.yaw -= 360.0f;
	}
	return angles;
}
float Repeat(float t, float length)
{
	return t - floor(t / length) * length;
}
float DeltaAngle(float current, float target)
{
	float num = Repeat(target - current, 360.f);
	if (num > 180.f)
	{
		num -= 360.f;
	}
	return num;
}
float AngleDistance(const Angle& u, const Angle& v)
{
	Angle delta = { DeltaAngle(u.yaw,v.yaw),u.pitch - v.pitch,0.f };
	return FASTSQRT(delta.yaw*delta.yaw + delta.pitch*delta.pitch);
}
Coord WorldToScreen(Vec3 pos, const ::Matrix& mat, int windowWidth, int windowHeight) {

	Coord screen;
	screen.Invalidate();
	Vec4 clipCoords;
	clipCoords.x = pos.x*mat.m[0] + pos.y*mat.m[4] + pos.z*mat.m[8] + mat.m[12];
	clipCoords.y = pos.x*mat.m[1] + pos.y*mat.m[5] + pos.z*mat.m[9] + mat.m[13];
	clipCoords.z = pos.x*mat.m[2] + pos.y*mat.m[6] + pos.z*mat.m[10] + mat.m[14];
	clipCoords.w = pos.x*mat.m[3] + pos.y*mat.m[7] + pos.z*mat.m[11] + mat.m[15];

	if (clipCoords.w < 0.1f)
		return screen;

	Vec3 NDC;
	NDC.x = clipCoords.x / clipCoords.w;
	NDC.y = clipCoords.y / clipCoords.w;
	NDC.z = clipCoords.z / clipCoords.w;

	screen.x = int((windowWidth * 0.5f * NDC.x) + (NDC.x + windowWidth * 0.5f));
	screen.y = int(-(windowHeight * 0.5f * NDC.y) + (NDC.y + windowHeight * 0.5f));

	return screen;
}

float Distance(const Vec3& v1, const Vec3& v2)
{
	return FASTSQRT(v1.x*v1.x + v1.y*v1.y + v1.z*v1.z);
}