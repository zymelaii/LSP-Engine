#ifndef LSPE_VEC_H
#define LSPE_VEC_H

#include "../utils/utils.h"
#include "../base/math.hpp"

namespace lspe
{

template <typename> union vec2;
template <typename> union vec3;
template <typename> union vec4;

typedef vec2<int> vec2i;
typedef vec3<int> vec3i;
typedef vec4<int> vec4i;

typedef vec2<long> vec2l;
typedef vec3<long> vec3l;
typedef vec4<long> vec4l;

typedef vec2<long long> vec2ll;
typedef vec3<long long> vec3ll;
typedef vec4<long long> vec4ll;

typedef vec2<float> vec2f;
typedef vec3<float> vec3f;
typedef vec4<float> vec4f;

typedef vec2<double> vec2lf;
typedef vec3<double> vec3lf;
typedef vec4<double> vec4lf;

template <typename T> union vec2
{
	T data[2];
	struct { T x, y; };

	vec2<T>();
	vec2<T>(T x, T y);
	vec2<T>(const vec2<T> &rhs);
	vec2<T>(vec2<T> &&rhs);

	vec2<T>& negate();
	vec2<T> negative() const;

	T dot(const vec2<T> &rhs) const;
	vec3<T> cross(const vec2<T> &rhs) const;
	vec2<T> triproduct(const vec2<T> &b, const vec2<T> &c) const;

	template <typename G = T> G norm() const;

	vec2<T>& operator +=(T rhs);
	vec2<T>& operator -=(T rhs);
	vec2<T>& operator *=(T rhs);
	vec2<T>& operator /=(T rhs);

	vec2<T>& operator +=(const vec2<T>& rhs);
	vec2<T>& operator -=(const vec2<T>& rhs);

	vec2<T> operator +(T rhs) const;
	vec2<T> operator -(T rhs) const;
	vec2<T> operator *(T rhs) const;
	vec2<T> operator /(T rhs) const;

	vec2<T> operator +(const vec2<T>& rhs) const;
	vec2<T> operator -(const vec2<T>& rhs) const;

	vec2<T> operator +() const;
	vec2<T> operator -() const;

	vec2<T>& operator =(const vec2<T> &rhs);

	T operator [](int x);

	bool operator ==(const vec2<T> &rhs) const;
	bool operator !=(const vec2<T> &rhs) const;
	
	template <typename R> operator vec2<R>();

	static vec2<T> origin();
};

template <typename T> union vec3
{
	T data[3];
	struct { T x, y, z; };

	vec3<T>();
	vec3<T>(T x, T y, T z);
	vec3<T>(const vec3<T> &rhs);
	vec3<T>(vec3<T> &&rhs);

	vec3<T>& negate();
	vec3<T> negative() const;

	T dot(const vec3<T> &rhs) const;
	vec3<T> cross(const vec3<T> &rhs) const;

	template <typename G = T> G norm() const;

	vec3<T>& operator +=(T rhs);
	vec3<T>& operator -=(T rhs);
	vec3<T>& operator *=(T rhs);
	vec3<T>& operator /=(T rhs);

	vec3<T>& operator +=(const vec3<T>& rhs);
	vec3<T>& operator -=(const vec3<T>& rhs);

	vec3<T> operator +(T rhs) const;
	vec3<T> operator -(T rhs) const;
	vec3<T> operator *(T rhs) const;
	vec3<T> operator /(T rhs) const;

	vec3<T> operator +(const vec3<T>& rhs) const;
	vec3<T> operator -(const vec3<T>& rhs) const;

	vec3<T> operator +() const;
	vec3<T> operator -() const;

	vec3<T>& operator =(const vec3<T> &rhs);

	T operator [](int x);

	bool operator ==(const vec3<T> &rhs) const;
	bool operator !=(const vec3<T> &rhs) const;
	
	template <typename R> operator vec3<R>();

	static vec3<T> origin();
};

template <typename T> union vec4
{
	T data[4];
	struct { T x, y, z, w; };

	vec4<T>();
	vec4<T>(T x, T y, T z, T w);
	vec4<T>(const vec4<T> &rhs);
	vec4<T>(vec4<T> &&rhs);

	vec4<T>& negate();
	vec4<T> negative() const;

	T dot(const vec4<T> &rhs) const;
	vec4<T> cross(const vec4<T> &rhs) const;

	template <typename G = T> G norm() const;

	vec4<T>& operator +=(T rhs);
	vec4<T>& operator -=(T rhs);
	vec4<T>& operator *=(T rhs);
	vec4<T>& operator /=(T rhs);

	vec4<T>& operator +=(const vec4<T>& rhs);
	vec4<T>& operator -=(const vec4<T>& rhs);

	vec4<T> operator +(T rhs) const;
	vec4<T> operator -(T rhs) const;
	vec4<T> operator *(T rhs) const;
	vec4<T> operator /(T rhs) const;

	vec4<T> operator +(const vec4<T>& rhs) const;
	vec4<T> operator -(const vec4<T>& rhs) const;

	vec4<T> operator +() const;
	vec4<T> operator -() const;

	vec4<T>& operator =(const vec4<T> &rhs);

	T operator [](int x);

	bool operator ==(const vec4<T> &rhs) const;
	bool operator !=(const vec4<T> &rhs) const;
	
	template <typename R> operator vec4<R>();

	static vec4<T> origin();
};

};

#ifdef LSPE_IMPL

namespace lspe
{

template <typename T>
vec2<T>::vec2()
{
	memset(data, 0, sizeof(T) * 2);
}

template <typename T>
vec2<T>::vec2(T x, T y)
	: x(x), y(y)
{

}

template <typename T>
vec2<T>::vec2(const vec2<T> &rhs)
	: x(rhs.x), y(rhs.y)
{
		
}

template <typename T>
vec2<T>::vec2(vec2<T> &&rhs)
	: x(rhs.x), y(rhs.y)
{
		
}

template <typename T>
vec2<T>& vec2<T>::negate()
{
	x = -x;
	y = -y;
	return *this;
}

template <typename T>
vec2<T> vec2<T>::negative() const
{
	vec2<T> tmp(*this);
	return tmp.negate();
}

template <typename T>
T vec2<T>::dot(const vec2<T> &rhs) const
{
	return x * rhs.x + y * rhs.y;
}

template <typename T>
vec3<T> vec2<T>::cross(const vec2<T> &rhs) const
{
	return vec3<T>((T)0, (T)0, x * rhs.y - rhs.x * y);
}

template <typename T>
vec2<T> vec2<T>::triproduct(const vec2<T> &b, const vec2<T> &c) const
{
	return b * c.dot(*this) - *this * c.dot(b);
}

template <typename T>
template <typename G>
G vec2<T>::norm() const
{
	return sqrt(x * x + y * y);
}

#define F(op, rhsType, ac1, ac2)		 	\
template <typename T>						\
vec2<T>& vec2<T>::operator op(rhsType rhs)	\
{											\
	x op ac1;								\
	y op ac2;								\
	return *this;							\
}

F(+=, T, rhs, rhs);
F(+=, const vec2<T>&, rhs.x, rhs.y);

F(-=, T, rhs, rhs);
F(-=, const vec2<T>&, rhs.x, rhs.y);

F(*=, T, rhs, rhs);
F(/=, T, rhs, rhs);

#undef F

#define F(op, op2, rhsType) 					\
template <typename T>							\
vec2<T> vec2<T>::operator op(rhsType rhs) const	\
{												\
	vec2<T> v(*this);							\
	v op2 rhs;									\
	return v;									\
}

F(+, +=, T);
F(+, +=, const vec2<T>&);

F(-, -=, T);
F(-, -=, const vec2<T>&);

F(*, *=, T);
F(/, /=, T);

#undef F

template <typename T>
vec2<T> vec2<T>::operator +() const
{
	return *this;
}

template <typename T>
vec2<T> vec2<T>::operator -() const
{
	return negative();
}

template <typename T>
vec2<T>& vec2<T>::operator =(const vec2<T> &rhs)
{
	x = rhs.x;
	y = rhs.y;
	return *this;
}

template <typename T>
T vec2<T>::operator [](int x)
{
	LSPE_ASSERT(x >= 0 && x < 2);
	return data[x];
}

template <typename T>
bool vec2<T>::operator ==(const vec2<T> &rhs) const
{
	return equal(x, rhs.x) && equal(y, rhs.y);
}

template <typename T>
bool vec2<T>::operator !=(const vec2<T> &rhs) const
{
	return !equal(x, rhs.x) || !equal(y, rhs.y);
}

template <typename T>
template <typename R>
vec2<T>::operator vec2<R>()
{
	return vec2<R>((R)x, (R)y);
}

template <typename T>
vec2<T> vec2<T>::origin()
{
	return vec2<T>();
}

template <typename T>
vec3<T>::vec3()
{
	memset(data, 0, sizeof(T) * 3);
}

template <typename T>
vec3<T>::vec3(T x, T y, T z)
	: x(x), y(y), z(z)
{

}

template <typename T>
vec3<T>::vec3(const vec3<T> &rhs)
	: x(rhs.x), y(rhs.y), z(rhs.z)
{
		
}

template <typename T>
vec3<T>::vec3(vec3<T> &&rhs)
	: x(rhs.x), y(rhs.y), z(rhs.z)
{
		
}

template <typename T>
vec3<T>& vec3<T>::negate()
{
	x = -x;
	y = -y;
	z = -z;
	return *this;
}

template <typename T>
vec3<T> vec3<T>::negative() const
{
	vec3<T> tmp(*this);
	return tmp.negate();
}

template <typename T>
T vec3<T>::dot(const vec3<T> &rhs) const
{
	return x * rhs.x + y * rhs.y + z * rhs.z;
}

template <typename T>
vec3<T> vec3<T>::cross(const vec3<T> &rhs) const
{
	T a = y * rhs.z - rhs.y * z;
	T b = rhs.x * z - rhs.z * x;
	T c = x * rhs.y - rhs.x * y;
	return vec3<T>(a, b, c);
}

template <typename T>
template <typename G>
G vec3<T>::norm() const
{
	return sqrt(x * x + y * y + z * z);
}

#define F(op, rhsType, ac1, ac2, ac3)	 	\
template <typename T>						\
vec3<T>& vec3<T>::operator op(rhsType rhs)	\
{											\
	x op ac1;								\
	y op ac2;								\
	z op ac3;								\
	return *this;							\
}

F(+=, T, rhs, rhs, rhs);
F(+=, const vec3<T>&, rhs.x, rhs.y, rhs.z);

F(-=, T, rhs, rhs, rhs);
F(-=, const vec3<T>&, rhs.x, rhs.y, rhs.z);

F(*=, T, rhs, rhs, rhs);
F(/=, T, rhs, rhs, rhs);

#undef F

#define F(op, op2, rhsType) 					\
template <typename T>							\
vec3<T> vec3<T>::operator op(rhsType rhs) const	\
{												\
	vec3<T> v(*this);							\
	v op2 rhs;									\
	return v;									\
}

F(+, +=, T);
F(+, +=, const vec3<T>&);

F(-, -=, T);
F(-, -=, const vec3<T>&);

F(*, *=, T);
F(/, /=, T);

#undef F

template <typename T>
vec3<T> vec3<T>::operator +() const
{
	return *this;
}

template <typename T>
vec3<T> vec3<T>::operator -() const
{
	return negative();
}

template <typename T>
vec3<T>& vec3<T>::operator =(const vec3<T> &rhs)
{
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;
	return *this;
}

template <typename T>
T vec3<T>::operator [](int x)
{
	LSPE_ASSERT(x >= 0 && x < 3);
	return data[x];
}

template <typename T>
bool vec3<T>::operator ==(const vec3<T> &rhs) const
{
	return equal(x, rhs.x) && equal(y, rhs.y)
		&& equal(z, rhs.z);
}

template <typename T>
bool vec3<T>::operator !=(const vec3<T> &rhs) const
{
	return !equal(x, rhs.x) || !equal(y, rhs.y)
		|| !equal(z, rhs.z);
}

template <typename T>
template <typename R>
vec3<T>::operator vec3<R>()
{
	return vec3<R>((R)x, (R)y, (R)z);
}

template <typename T>
vec3<T> vec3<T>::origin()
{
	return vec3<T>();
}

template <typename T>
vec4<T>::vec4()
{
	memset(data, 0, sizeof(T) * 4);
}

template <typename T>
vec4<T>::vec4(T x, T y, T z, T w)
	: x(x), y(y), z(z), w(w)
{

}

template <typename T>
vec4<T>::vec4(const vec4<T> &rhs)
	: x(rhs.x), y(rhs.y), z(rhs.z), w(rhs.w)
{
		
}

template <typename T>
vec4<T>::vec4(vec4<T> &&rhs)
	: x(rhs.x), y(rhs.y), z(rhs.z), w(rhs.w)
{
		
}

template <typename T>
vec4<T>& vec4<T>::negate()
{
	x = -x;
	y = -y;
	z = -z;
	w = -w;
	return *this;
}

template <typename T>
vec4<T> vec4<T>::negative() const
{
	vec4<T> tmp(*this);
	return tmp.negate();
}

template <typename T>
T vec4<T>::dot(const vec4<T> &rhs) const
{
	return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w;
}

template <typename T>
template <typename G>
G vec4<T>::norm() const
{
	return sqrt(x * x + y * y + z * z + w * w);
}

#define F(op, rhsType, ac1, ac2, ac3, ac4) 	\
template <typename T>						\
vec4<T>& vec4<T>::operator op(rhsType rhs)	\
{											\
	x op ac1;								\
	y op ac2;								\
	z op ac3;								\
	w op ac4;								\
	return *this;							\
}

F(+=, T, rhs, rhs, rhs, rhs);
F(+=, const vec4<T>&, rhs.x, rhs.y, rhs.z, rhs.w);

F(-=, T, rhs, rhs, rhs, rhs);
F(-=, const vec4<T>&, rhs.x, rhs.y, rhs.z, rhs.w);

F(*=, T, rhs, rhs, rhs, rhs);
F(/=, T, rhs, rhs, rhs, rhs);

#undef F

#define F(op, op2, rhsType) 					\
template <typename T>							\
vec4<T> vec4<T>::operator op(rhsType rhs) const	\
{												\
	vec4<T> v(*this);							\
	v op2 rhs;									\
	return v;									\
}

F(+, +=, T);
F(+, +=, const vec4<T>&);

F(-, -=, T);
F(-, -=, const vec4<T>&);

F(*, *=, T);
F(/, /=, T);

#undef F

template <typename T>
vec4<T> vec4<T>::operator +() const
{
	return *this;
}

template <typename T>
vec4<T> vec4<T>::operator -() const
{
	return negative();
}

template <typename T>
vec4<T>& vec4<T>::operator =(const vec4<T> &rhs)
{
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;
	w = rhs.w;
	return *this;
}

template <typename T>
T vec4<T>::operator [](int x)
{
	LSPE_ASSERT(x >= 0 && x < 4);
	return data[x];
}

template <typename T>
bool vec4<T>::operator ==(const vec4<T> &rhs) const
{
	return equal(x, rhs.x) && equal(y, rhs.y)
		&& equal(z, rhs.z) && equal(w, rhs.w);
}

template <typename T>
bool vec4<T>::operator !=(const vec4<T> &rhs) const
{
	return !equal(x, rhs.x) || !equal(y, rhs.y)
		|| !equal(z, rhs.z) || !equal(w, rhs.w);
}

template <typename T>
template <typename R>
vec4<T>::operator vec4<R>()
{
	return vec4<R>((R)x, (R)y, (R)z, (R)w);
}

template <typename T>
vec4<T> vec4<T>::origin()
{
	return vec4<T>();
}

template <> //! specialize T -> int
template <> //! specialize G -> double
double vec2i::norm() const
{
	return sqrt(x * x + y * y);
}

template <> //! specialize T -> long
template <> //! specialize G -> double
double vec2l::norm() const
{
	return sqrt(x * x + y * y);
}

template <> //! specialize T -> long long
template <> //! specialize G -> double
double vec2ll::norm() const
{
	return sqrt(x * x + y * y);
}

template <> //! specialize T -> int
template <> //! specialize G -> double
double vec3i::norm() const
{
	return sqrt(x * x + y * y + z * z);
}

template <> //! specialize T -> long
template <> //! specialize G -> double
double vec3l::norm() const
{
	return sqrt(x * x + y * y + z * z);
}

template <> //! specialize T -> long long
template <> //! specialize G -> double
double vec3ll::norm() const
{
	return sqrt(x * x + y * y + z * z);
}

template <> //! specialize T -> int
template <> //! specialize G -> double
double vec4i::norm() const
{
	return sqrt(x * x + y * y + z * z + w * w);
}

template <> //! specialize T -> long
template <> //! specialize G -> double
double vec4l::norm() const
{
	return sqrt(x * x + y * y + z * z + w * w);
}

template <> //! specialize T -> long long
template <> //! specialize G -> double
double vec4ll::norm() const
{
	return sqrt(x * x + y * y + z * z + w * w);
}

};

#endif

#endif