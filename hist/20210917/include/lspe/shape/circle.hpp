#ifndef LSPE_CIRCLE_H
#define LSPE_CIRCLE_H

#ifndef LSPE_SHAPE_H
#include "../shape/shape.hpp"
#endif

namespace lspe
{

namespace shape
{

template <typename T>
class Circle : public Shape
{
public:
	Circle() = delete;

	Circle(T x, T y, T r);
	Circle(const vec2<T> &x, T r);
	Circle(const Circle<T> &rhs);

	virtual bool overlap(const Shape &rhs) const;
	virtual bool contain(const Shape &rhs) const;

	virtual bool overlap(const Circle<T> &rhs) const;
	virtual bool overlap(const Polygen<T> &rhs) const;

	virtual bool contain(const vec2<T> &rhs) const;
	virtual bool contain(const Circle<T> &rhs) const;
	virtual bool contain(const Polygen<T> &rhs) const;

	operator BBox2<T>();

	virtual const char* id() const;

protected:
	vec2<T> center;
	T radius;

private:

};

};

};

#ifdef LSPE_IMPL

namespace lspe::shape
{

template <typename T>
Circle<T>::Circle(T x, T y, T r)
	: center(x, y), radius(r)
{

}

template <typename T>
Circle<T>::Circle(const vec2<T> &x, T r)
	: center(x), radius(r)
{

}

template <typename T>
Circle<T>::Circle(const Circle<T> &rhs)
	: center(rhs.center), radius(rhs.radius)
{

}

template <typename T>
bool Circle<T>::overlap(const Shape &rhs) const
{
	return false;
}

template <typename T>
bool Circle<T>::contain(const Shape &rhs) const
{
	return false;
}

template <typename T>
bool Circle<T>::overlap(const Circle<T> &rhs) const
{
	float dist = (center - rhs.center).norm();
	float limit = (float)(radius + rhs.radius);
	return less(dist, limit);
}

template <typename T>
bool Circle<T>::overlap(const Polygen<T> &rhs) const
{
	return false;
}

template <typename T>
bool Circle<T>::contain(const vec2<T> &rhs) const
{
	float dist = (center - rhs).norm();
	return less(dist, (float)radius);
}

template <typename T>
bool Circle<T>::contain(const Circle<T> &rhs) const
{
	float dist = (center - rhs.center).norm();
	float limit = (float)(radius - rhs.radius);
	return less(dist, limit);
}

template <typename T>
bool Circle<T>::contain(const Polygen<T> &rhs) const
{
	return false;
}

template <typename T>
Circle<T>::operator BBox2<T>()
{
	return BBox2<T>(center - radius, center + radius);
}

template <typename T>
const char* Circle<T>::id() const
{
	return "lspe::shape::Circle";
}

};

#endif

#endif /* LSPE_CIRCLE_H */