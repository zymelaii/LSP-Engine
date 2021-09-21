#pragma once

/********************************
 *  @author: ZYmelaii
 *
 *  @object: vec2 vec3
 *
 *  @brief: 2/3-dimensional vector of built-in type float
 *
 *  @NOTES: 
 *******************************/

#include "../base/base.h"

namespace lspe
{

struct vec2;
struct vec3;

static inline float dot   (const vec2 &a, const vec2 &b);
static inline float cross (const vec2 &a, const vec2 &b);

static inline float dot   (const vec3 &a, const vec3 &b);
static inline vec3  cross (const vec3 &a, const vec3 &b);

//! triple product (a × b) × c
static inline vec2 triproduct(const vec2 &a, const vec2 &b, const vec2 &c);

static inline vec2 operator + (const vec2 &a, float b);
static inline vec2 operator + (const vec2 &a, const vec2 &b);
static inline vec2 operator - (const vec2 &a, float b);
static inline vec2 operator - (const vec2 &a, const vec2 &b);
static inline vec2 operator * (const vec2 &a, float b);
static inline vec2 operator * (float a,       const vec2 &b);
static inline vec2 operator / (const vec2 &a, float b);

static inline vec2& operator += (vec2 &a,     float b);
static inline vec2& operator += (vec2 &a,     const vec2 &b);
static inline vec2& operator -= (vec2 &a,     float b);
static inline vec2& operator -= (vec2 &a,     const vec2 &b);
static inline vec2& operator *= (vec2 &a,     float b);
static inline vec2& operator /= (vec2 &a,     float b);

static inline vec3 operator + (const vec3 &a, float b);
static inline vec3 operator + (const vec3 &a, const vec3 &b);
static inline vec3 operator - (const vec3 &a, float b);
static inline vec3 operator - (const vec3 &a, const vec3 &b);
static inline vec3 operator * (const vec3 &a, float b);
static inline vec3 operator * (float a,       const vec3 &b);
static inline vec3 operator / (const vec3 &a, float b);

static inline vec3& operator += (vec3 &a,       float b);
static inline vec3& operator += (vec3 &a,       const vec3 &b);
static inline vec3& operator -= (vec3 &a,       float b);
static inline vec3& operator -= (vec3 &a,       const vec3 &b);
static inline vec3& operator *= (vec3 &a,       float b);
static inline vec3& operator /= (vec3 &a,       float b);

static inline bool operator == (const vec2 &a, const vec2 &b);
static inline bool operator == (const vec3 &a, const vec3 &b);

struct vec2
{
	union
	{
		float data[2];
		struct {
			float x;
			float y;
		};
	};

	vec2();
	vec2(float x, float y);
	vec2(float arr[2]);

	float& operator [](int x);
	const float& operator [](int x) const;

	vec2 operator -(); //! 反向向量

	float norm() const; //! 向量模长
	vec2 normalized() const; //! 获取单位向量
	vec2 normal() const; //! 法线

	bool isnull() const; //! null(Inf, Inf)
};

struct vec3
{
	union
	{
		float data[3];
		struct {
			float x;
			float y;
			float z;
		};
	};

	vec3();
	vec3(float x, float y, float z);
	vec3(float arr[3]);

	float& operator [](int x);
	const float& operator [](int x) const;

	vec3 operator -();

	float norm() const;
	vec3 normalized() const;

	bool isnull() const;
};

};

namespace lspe
{

float dot(const vec2 &a, const vec2 &b)
{
	return a.x * b.x + a.y * b.y;
}

float cross(const vec2 &a, const vec2 &b)
{
	return a.x * b.y - a.y * b.x;
}

float dot(const vec3 &a, const vec3 &b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

vec3 cross(const vec3 &a, const vec3 &b)
{
	return { a.y * b.z - a.z * b.y, 
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x };
}

vec2 triproduct(const vec2 &a, const vec2 &b, const vec2 &c)
{
	return b * dot(c, a) - a * dot(c, b);
}

vec2 operator +(const vec2 &a, float b)
{
	return { a.x + b, a.y + b };
}

vec2 operator +(const vec2 &a, const vec2 &b)
{
	return { a.x + b.x, a.y + b.y };
}

vec2 operator -(const vec2 &a, float b)
{
	return { a.x - b, a.y - b };
}

vec2 operator -(const vec2 &a, const vec2 &b)
{
	return { a.x - b.x, a.y - b.y };
}

vec2 operator *(const vec2 &a, float b)
{
	return { a.x * b, a.y * b };
}

vec2 operator *(float a, const vec2 &b)
{
	return { a * b.x, a * b.y };
}

vec2 operator /(const vec2 &a, float b)
{
	return { a.x / b, a.y / b };
}

vec2& operator +=(vec2 &a, float b)
{
	return (a.x += b, a.y += b, a);
}

vec2& operator +=(vec2 &a, const vec2 &b)
{
	return (a.x += b.x, a.y += b.y, a);
}

vec2& operator -=(vec2 &a, float b)
{
	return (a.x -= b, a.y -= b, a);
}

vec2& operator -=(vec2 &a, const vec2 &b)
{
	return (a.x -= b.x, a.y -= b.y, a);
}

vec2& operator *=(vec2 &a, float b)
{
	return (a.x *= b, a.y *= b, a);
}

vec2& operator /=(vec2 &a, float b)
{
	return (a.x /= b, a.y /= b, a);
}

vec3 operator +(const vec3 &a, float b)
{
	return { a.x + b, a.y + b, a.z + b };
}

vec3 operator +(const vec3 &a, const vec3 &b)
{
	return { a.x + b.x, a.y + b.y, a.z + b.z };
}

vec3 operator -(const vec3 &a, float b)
{
	return { a.x - b, a.y - b, a.z - b };
}

vec3 operator -(const vec3 &a, const vec3 &b)
{
	return { a.x - b.x, a.y - b.y, a.z - b.z };
}

vec3 operator *(const vec3 &a, float b)
{
	return { a.x * b, a.y * b, a.z * b };
}

vec3 operator *(float a, const vec3 &b)
{
	return { a * b.x, a * b.y, a * b.z };
}

vec3 operator /(const vec3 &a, float b)
{
	return { a.x / b, a.y / b, a.z / b };
}

vec3& operator +=(vec3 &a, float b)
{
	return (a.x += b, a.y += b, a.z += b, a);
}

vec3& operator +=(vec3 &a, const vec3 &b)
{
	return (a.x += b.x, a.y += b.y, a.z += b.z, a);
}

vec3& operator -=(vec3 &a, float b)
{
	return (a.x -= b, a.y -= b, a.z -= b, a);
}

vec3& operator -=(vec3 &a, const vec3 &b)
{
	return (a.x -= b.x, a.y -= b.y, a.z -= b.z, a);
}

vec3& operator *=(vec3 &a, float b)
{
	return (a.x *= b, a.y *= b, a.z *= b, a);
}

vec3& operator /=(vec3 &a, float b)
{
	return (a.x /= b, a.y /= b, a.z /= b, a);
}

bool operator ==(const vec2 &a, const vec2 &b)
{
	return a.x == b.x && a.y == b.y;
}

bool operator ==(const vec3 &a, const vec3 &b)
{
	return a.x == b.x && a.y == b.y && a.z == b.z;
}

};
