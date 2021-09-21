#ifndef LSPE_BBOX_H
#define LSPE_BBOX_H

#include "base/vec.hpp"
#include "base/array.hpp"

namespace lspe
{

template <typename T>
class BBox2
{

public:
	BBox2() = delete;
	BBox2(const vec2<T> &a, const vec2<T> &b);
	BBox2(const array<vec2<T>> &rhs);

	bool overlap(const BBox2<T> &rhs) const;
	bool contain(const vec2<T> &rhs) const;
	bool contain(const BBox2<T> &rhs) const;

	vec2<T> leftbottom() const;
	vec2<T> righttop() const;

private:
	vec2<T> _min; //! (minx, miny)
	vec2<T> _max; //! (maxx, maxy)
};

};

#ifdef LSPE_IMPL

namespace lspe
{

template <typename T>
BBox2<T>::BBox2(const vec2<T> &a, const vec2<T> &b)
	: _min(min(a.x, b.x), min(a.y, b.y)),
	_max(max(a.x, b.x), max(a.y, b.y))
{

}

template <typename T>
BBox2<T>::BBox2(const array<vec2<T>> &rhs)
{
	LSPE_ASSERT(rhs.size() <= 2);
	_min = rhs.at(0);
	_max = rhs.at(0);
	for (int i = 1; i < rhs.size(); ++i)
	{
		auto t = rhs.at(i);

		if (less(t.x, _min.x)) _min.x = t.x;
		if (less(t.y, _min.y)) _min.y = t.y;
		if (greater(t.x, _max.x)) _max.x = t.x;
		if (greater(t.y, _max.y)) _max.y = t.y;
	}
}

template <typename T>
bool BBox2<T>::overlap(const BBox2<T> &rhs) const
{
	return contain(rhs._min) || contain(rhs._max);
}

template <typename T>
bool BBox2<T>::contain(const vec2<T> &rhs) const
{
	return greater(rhs.x, _min.x)
		&& greater(rhs.y, _min.y)
		&& less(rhs.x, _max.x)
		&& less(rhs.y, _max.y);
}

template <typename T>
bool BBox2<T>::contain(const BBox2<T> &rhs) const
{
	return contain(rhs._min) && contain(rhs._max);
}

template <typename T>
vec2<T> BBox2<T>::leftbottom() const
{
	return _min;
}

template <typename T>
vec2<T> BBox2<T>::righttop() const
{
	return _max;
}

};

#endif

#endif /* LSPE_BBOX_H */