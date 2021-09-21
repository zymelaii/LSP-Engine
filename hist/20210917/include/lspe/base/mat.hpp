#ifndef LSPE_MAT_H
#define LSPE_MAT_H

#include "../utils/utils.h"
#include "../base/vec.hpp"

namespace lspe
{

template <typename T> struct mat3x3
{
	vec3<T> v[3];

	mat3x3(const vec3<T> &v0, const vec3<T> &v1, const vec3<T> &v2);
	mat3x3(const mat3x3<T> &rhs);
	mat3x3(const T rhs[9]);

	vec3<T>& operator [](int x);
	T at(int x, int y) const;
	bool operator ==(const mat3x3<T> &rhs) const;
	mat3x3<T>& transpose();
	mat3x3<T>& mul(const mat3x3<T> &rhs);
	static mat3x3<T> multiply(const mat3x3<T> &lhs, const mat3x3<T> &rhs);
};

typedef mat3x3<int> mat3x3i;
typedef mat3x3<long> mat3x3l;
typedef mat3x3<long long> mat3x3ll;
typedef mat3x3<float> mat3x3f;
typedef mat3x3<double> mat3x3lf;

};

#ifdef LSPE_IMPE

namespace lspe
{

template <typename T>
mat3x3<T>::mat3x3(
	const vec3<T> &v0,
	const vec3<T> &v1,
	const vec3<T> &v2
) : v({v0, v1, v2})
{
	
}

template <typename T>
mat3x3<T>::mat3x3(const mat3x3<T> &rhs)
{
	v[0] = rhs.v[0];
	v[1] = rhs.v[1];
	v[2] = rhs.v[2];
}

template <typename T>
mat3x3<T>::mat3x3(const T rhs[9])
{
	v[0].x = rhs[0];
	v[0].y = rhs[1];
	v[0].z = rhs[2];
	v[1].x = rhs[3];
	v[1].y = rhs[4];
	v[1].z = rhs[5];
	v[2].x = rhs[6];
	v[2].y = rhs[7];
	v[2].z = rhs[8];
}

template <typename T>
vec3<T>& mat3x3<T>::operator [](int x)
{
	LSPE_ASSERT(x >= 0 && x < 3);
	return v[x];
}

template <typename T>
T mat3x3<T>::at(int x, int y) const
{
	LSPE_ASSERT(x >= 0 && x < 3);
	LSPE_ASSERT(y >= 0 && y < 3);
	return v[x].data[y];
}

template <typename T>
bool mat3x3<T>::operator ==(const mat3x3<T> &rhs) const
{
	return at(0, 0) == rhs.at(0, 0)
		&& at(0, 1) == rhs.at(0, 1)
		&& at(0, 2) == rhs.at(0, 2)
		&& at(1, 0) == rhs.at(1, 0)
		&& at(1, 1) == rhs.at(1, 1)
		&& at(1, 2) == rhs.at(1, 2)
		&& at(2, 0) == rhs.at(2, 0)
		&& at(2, 1) == rhs.at(2, 1)
		&& at(2, 2) == rhs.at(2, 2);
}

template <typename T>
mat3x3<T>& mat3x3<T>::transpose()
{
	auto tmp = *this;
	v[0] = vec3<T>(tmp[0][0], tmp[1][0], tmp[2][0]);
	v[1] = vec3<T>(tmp[0][1], tmp[1][1], tmp[2][1]);
	v[2] = vec3<T>(tmp[0][2], tmp[1][2], tmp[2][2]);
	return *this;
}

template <typename T>
mat3x3<T>& mat3x3<T>::mul(const mat3x3<T> &rhs)
{
	auto lhs = *this, tmp = rhs;
	tmp.transpose();
	v[0][0] = lhs[0] * tmp[0];
	v[0][1] = lhs[0] * tmp[1];
	v[0][2] = lhs[0] * tmp[2];
	v[1][0] = lhs[1] * tmp[0];
	v[1][1] = lhs[1] * tmp[1];
	v[1][2] = lhs[1] * tmp[2];
	v[2][0] = lhs[2] * tmp[0];
	v[2][1] = lhs[2] * tmp[1];
	v[2][2] = lhs[2] * tmp[2];
	return *this;
}

template <typename T>
mat3x3<T> mat3x3<T>::multiply(const mat3x3<T> &lhs, const mat3x3<T> &rhs)
{
	mat3x3<T> tmp = lhs;
	tmp.transpose();
	return tmp.mul(rhs);
}

};

#endif

#endif /* LSPE_MAT_H */