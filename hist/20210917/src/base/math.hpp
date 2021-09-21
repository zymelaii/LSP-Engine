#ifndef LSPE_MATH_H
#define LSPE_MATH_H

#include <math.h>

#include "../base/compare.hpp"

namespace lspe
{

template <typename T> T max(T x, T y);
template <typename T> T min(T x, T y);

template <typename T, typename... G> T max(T first, G... args);
template <typename T, typename... G> T min(T first, G... args);

float sqrt(float x);
double sqrt(double x);
template <typename IntType> double sqrt(IntType x);

float invsqrt(float number);
double invsqrt(double number);
template <typename IntType> double invsqrt(IntType x);

};

#ifdef LSPE_IMPL

namespace lspe
{

template <typename T>
T max(T x, T y)
{
	return greater(x, y) ? x : y;
}

template <typename T>
T min(T x, T y)
{
	return less(x, y) ? x : y;
}

template <typename T, typename... G>
T max(T first, G... args)
{
	static_assert(sizeof...(args) >= 1, "lspe::max expects at least 2 params.");
	return max(first, max(args...));
}

template <typename T, typename... G>
T min(T first, G... args)
{
	static_assert(sizeof...(args) >= 1, "lspe::min expects at least 2 params.");
	return min(first, min(args...));
}

float sqrt(float x)
{
	return ::sqrt(x);
}

double sqrt(double x)
{
	return ::sqrt(x);
}

template <typename IntType>
double sqrt(IntType x)
{
	return sqrt((double)x);
}

float invsqrt(float number)
{	//! quick reciprocal square root algorithm from QUAKE3
	long i;
	float x2, y;
	const float threehalfs = 1.5f;

	x2 = number * 0.5f;
	y = number;
	i = *(long*)&y;
	i = 0x5f375a86 - (i >> 1);
	y = *(float*)&i;
	y = y * (threehalfs - (x2 * y * y)); //! iterate this step if needed
	y = y * (threehalfs - (x2 * y * y));
	y = y * (threehalfs - (x2 * y * y));

	return y;
}

double invsqrt(double number)
{	//! quick reciprocal square root algorithm from QUAKE3 (double edition)
	long long i;
	double x2, y;
	const double threehalfs = 1.5;

	x2 = number * 0.5;
	y = number;
	i = *(long long*)&y;
	i = 0x5fe6ec85e7de30da - (i >> 1);
	y = *(double*)&i;
	y = y * (threehalfs - (x2 * y * y)); // iterate this step if needed
	y = y * (threehalfs - (x2 * y * y));
	y = y * (threehalfs - (x2 * y * y));

	return y;
}

template <typename IntType>
double invsqrt(IntType x)
{
	return invsqrt((double)x);
}

};

#endif

#endif /* LSPE_MATH_H */