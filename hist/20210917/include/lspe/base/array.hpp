#ifndef LSPE_ARRAY_H
#define LSPE_ARRAY_H

#include <initializer_list>

#include "../utils/assert.h"

namespace lspe
{

template <class T>
class array
{
public:
	array() = delete;
	array(size_t len);
	array(const array<T> &rhs);
	array(std::initializer_list<T> rhs);
	~array();

	T& operator [](int x);
	const T& at(int x) const;
	const size_t size() const;

private:
	size_t len;
	T *data;
};

};

#ifdef LSPE_IMPL

namespace lspe
{

template <class T>
array<T>::array(size_t len) : len(len)
{
	data = new T[len];
}

template <class T>
array<T>::array(const array<T> &rhs) : len(rhs.len)
{
	data = new T[len];
	for (int i = 0; i < len; ++i)
	{
		data[i] = rhs.data[i];
	}
}

template <class T>
array<T>::array(std::initializer_list<T> rhs)
{
	len = rhs.size();
	data = new T[len];
	int i = 0;
	for (auto e : rhs)
	{
		data[i++] = e;
	}
}

template <class T>
array<T>::~array()
{
	LSPE_ASSERT(data != nullptr);
	delete[] data;
	data = nullptr;
}

template <class T>
T& array<T>::operator [](int x)
{
	LSPE_ASSERT(x >= 0 && x < len);
	return data[x];
}

template <class T>
const T& array<T>::at(int x) const
{
	LSPE_ASSERT(x >= 0 && x < len);
	return data[x];
}

template <class T>
const size_t array<T>::size() const
{
	return len;
}

};

#endif

#endif /* LSPE_ARRAY_H */