#ifndef LSPE_LINE_H
#define LSPE_LINE_H

#ifndef LSPE_SHAPE_H
#include "../shape/shape.hpp"
#endif

#include "../base/math.hpp"

namespace lspe
{

namespace shape
{

enum { LineSegment = 0, LineRay = 1, LineStraight = 2 };

template <typename T>
class Line
{
public:
	Line(T x1, T y1, T x2, T y2);
	Line(const vec2<T> &a, const vec2<T> &b);

	bool parallel(const Line<T> &rhs) const;
	bool intersect(const Line<T> &rhs) const;
	vec2<T> intersection(const Line<T> &rhs) const;

protected:
	vec2<T> start, end;
	int type;

private:

};

template <typename T> class LSegment;
template <typename T> class LRay;
template <typename T> class LStraight;

template <typename T>
class LSegment : public Line<T>
{
public:
	LSegment(T x1, T y1, T x2, T y2);
	LSegment(const vec2<T> &a, const vec2<T> &b);

};

template <typename T>
class LRay : public Line<T>
{
public:
	LRay(T x1, T y1, T x2, T y2);
	LRay(const vec2<T> &a, const vec2<T> &b);

};

template <typename T>
class LStraight : public Line<T>
{
public:
	LStraight(T x1, T y1, T x2, T y2);
	LStraight(const vec2<T> &a, const vec2<T> &b);

};

typedef LSegment<int> LSegment2i;
typedef LRay<int> LRay2i;
typedef LStraight<int> LStraight2i;

typedef LSegment<float> LSegment2f;
typedef LRay<float> LRay2f;
typedef LStraight<float> LStraight2f;

typedef LSegment<double> LSegment2lf;
typedef LRay<double> LRay2lf;
typedef LStraight<double> LStraight2lf;

};

};

#include "../shape/details.hpp"

#ifdef LSPE_IMPL

namespace lspe::shape
{

template <typename T>
Line<T>::Line(T x1, T y1, T x2, T y2)
	: start(x1, y1), end(x2, y2)
{
	LSPE_ASSERT(start != end);
}

template <typename T>
Line<T>::Line(const vec2<T> &a, const vec2<T> &b)
	: start(a), end(b)
{
	LSPE_ASSERT(start != end);
}

template <typename T>
bool Line<T>::parallel(const Line<T> &rhs) const
{
	return details::parallel(start, end, rhs.start, rhs.end);
}

template <typename T>
bool Line<T>::intersect(const Line<T> &rhs) const
{
	return details::intersect(start, end, type,
		rhs.start, rhs.end, rhs.type);
}

template <typename T>
vec2<T> Line<T>::intersection(const Line<T> &rhs) const
{
	return (vec2<T>)details::intersection(start, end, rhs.start, rhs.end);
}

template <typename T>
LSegment<T>::LSegment(T x1, T y1, T x2, T y2)
	: Line<T>(x1, y1, x2, y2)
{
	this->type = LineSegment;
}

template <typename T>
LSegment<T>::LSegment(const vec2<T> &a, const vec2<T> &b)
	: Line<T>(a, b)
{
	this->type = LineSegment;
}

template <typename T>
LRay<T>::LRay(T x1, T y1, T x2, T y2)
	: Line<T>(x1, y1, x2, y2)
{
	this->type = LineRay;
}

template <typename T>
LRay<T>::LRay(const vec2<T> &a, const vec2<T> &b)
	: Line<T>(a, b)
{
	this->type = LineRay;
}

template <typename T>
LStraight<T>::LStraight(T x1, T y1, T x2, T y2)
	: Line<T>(x1, y1, x2, y2)
{
	this->type = LineStraight;
}

template <typename T>
LStraight<T>::LStraight(const vec2<T> &a, const vec2<T> &b)
	: Line<T>(a, b)
{
	this->type = LineStraight;
}

};

#endif

#endif /* LSPE_POLYGEN_H */