#ifndef LSPE_POLYGEN_H
#define LSPE_POLYGEN_H

#include <initializer_list>

#ifndef LSPE_SHAPE_H
#include "../shape/shape.hpp"
#endif

namespace lspe
{

namespace shape
{

template <typename T>
class Polygen : public Shape
{	//! only consider convex polygons
public:
	Polygen() = delete;
	Polygen(const T *arr2d, size_t n);
	Polygen(const array<vec2<T>> &rhs);
	Polygen(std::initializer_list<T> rhs);
	Polygen(const Polygen<T> &rhs);

	virtual bool overlap(const Shape &rhs) const;
	virtual bool contain(const Shape &rhs) const;

	virtual bool overlap(const Circle<T> &rhs) const;
	virtual bool overlap(const Polygen<T> &rhs) const;

	virtual bool contain(const vec2<T> &rhs) const;

	virtual bool contain(const Circle<T> &rhs) const;
	virtual bool contain(const Polygen<T> &rhs) const;

	size_t size() const;

	operator BBox2<T>();

	virtual const char* id() const;

protected:
	array<vec2<T>> data;

private:

};

};

};

#ifdef LSPE_IMPL

namespace lspe::shape
{

template <typename T>
Polygen<T>::Polygen(const T *arr2d, size_t n)
	: data(n + 1)
{
	int i = 0, j = 0;
	while (i < n)
	{
		data[i].x = arr2d[j++];
		data[i].y = arr2d[j++];
		++i;
	}
	data[i] = data[0];
}

template <typename T>
Polygen<T>::Polygen(const array<vec2<T>> &rhs)
	: data(rhs.size() + 1)
{
	int i = 0;
	for (; i < rhs.size(); ++i)
	{
		data[i] = rhs.at(i);
	}
	data[i] = data[0];
}

template <typename T>
Polygen<T>::Polygen(std::initializer_list<T> rhs)
	: data(rhs.size() + 1)
{
	int i = 0;
	for (auto e : rhs)
	{
		data[i++] = e;
	}
	data[i] = data[0];
}

template <typename T>
Polygen<T>::Polygen(const Polygen<T> &rhs)
	: data(rhs.data)
{

}

template <typename T>
bool Polygen<T>::overlap(const Shape &rhs) const
{
	return false;
}

template <typename T>
bool Polygen<T>::contain(const Shape &rhs) const
{
	return false;
}

template <typename T>
bool Polygen<T>::overlap(const Circle<T> &rhs) const
{
	return false;
}

template <typename T>
bool Polygen<T>::overlap(const Polygen<T> &rhs) const
{
	return details::GJK(data, rhs.data);
}

template <typename T>
bool Polygen<T>::contain(const Circle<T> &rhs) const
{
	return false;
}

template <typename T>
bool Polygen<T>::contain(const Polygen<T> &rhs) const
{
	return false;
}

template <typename T>
bool Polygen<T>::contain(const vec2<T> &rhs) const
{
	return false;
}

template <typename T>
size_t Polygen<T>::size() const
{
	return data.size() - 1;
}

template <typename T>
Polygen<T>::operator BBox2<T>()
{
	return BBox2<T>(data);
}

template <typename T>
const char* Polygen<T>::id() const
{
	return "lspe::shape::Polygen";
}

};

#endif

#endif /* LSPE_POLYGEN_H */