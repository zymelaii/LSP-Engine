#ifndef LSPE_COMPARE_H
#define LSPE_COMPARE_H

#include <cmath>
#include <numeric>

namespace lspe
{

template <typename T> bool equal(T x, T y);
template <typename T, typename G> bool equal(T x, G y);
template <> bool equal(float x, float y);
template <> bool equal(double x, double y);
template <typename T> bool greater(T x, T y);
template <typename T, typename G> bool greater(T x, G y);
template <> bool greater(float x, float y);
template <> bool greater(double x, double y);
template <typename T> bool less(T x, T y);
template <typename T, typename G> bool less(T x, G y);
template <> bool less(float x, float y);
template <> bool less(double x, double y);
template <typename T> bool gte(T x, T y);
template <typename T, typename G> bool gte(T x, G y);
template <typename T> bool lte(T x, T y);
template <typename T, typename G> bool lte(T x, G y);
template <typename T> bool between(T x, T l, T r);
template <typename T, typename G> bool between(T x, G l, G r);
template <typename T> bool eqrange(T x, T y, T delta);
template <typename T, typename G, typename R> bool eqrange(T x, G y, R delta);

};

#ifdef LSPE_IMPL

namespace lspe
{

template <typename T>
bool equal(T x, T y)
{	//! a wrapped equal function
	return x == y;
}

template <typename T, typename G>
bool equal(T x, G y)
{
	return equal(x, (T)y);
}

template <>
bool equal(float x, float y)
{
	return fabs(x - y) < ::std::numeric_limits<float>::epsilon();
}

template <>
bool equal(double x, double y)
{
	return fabs(x - y) < ::std::numeric_limits<double>::epsilon();
}

template <typename T>
bool greater(T x, T y)
{	//! a wrapped greater function
	return x > y;
}

template <typename T, typename G>
bool greater(T x, G y)
{
	return greater(x, (T)y);
}

template <>
bool greater(float x, float y)
{
	return x > y + ::std::numeric_limits<float>::epsilon();
}

template <>
bool greater(double x, double y)
{
	return x > y + ::std::numeric_limits<double>::epsilon();
}

template <typename T>
bool less(T x, T y)
{	//! a wrapped less function
	return x < y;
}

template <typename T, typename G>
bool less(T x, G y)
{
	return less(x, (T)y);
}

template <>
bool less(float x, float y)
{
	return x < y - ::std::numeric_limits<float>::epsilon();
}

template <>
bool less(double x, double y)
{
	return x < y - ::std::numeric_limits<double>::epsilon();
}

template <typename T>
bool gte(T x, T y)
{	//! a wrapped greater-than-or-equal function
	return greater(x, y) || equal(x, y);
}

template <typename T, typename G>
bool gte(T x, G y)
{
	return greater(x, (T)y) || equal(x, (T)y);
}

template <typename T>
bool lte(T x, T y)
{	//! a wrapped less-than-or-equal function
	return less(x, y) || equal(x, y);
}

template <typename T, typename G>
bool lte(T x, G y)
{
	return less(x, (T)y) || equal(x, (T)y);
}

template <typename T>
bool between(T x, T l, T r)
{
	return gte(x, l) && lte(x, r);
}

template <typename T, typename G>
bool between(T x, G l, G r)
{
	return gte(x, (T)l) && lte(x, (T)r);
}

template <typename T>
bool eqrange(T x, T y, T delta)
{
	return between(x, y - delta, y + delta);
}

template <typename T, typename G, typename R>
bool eqrange(T x, G y, R delta)
{
	return between(x, (T)(y - delta), (T)(y + delta));
}

};

#endif

#endif /* LSPE_COMPARE_H */