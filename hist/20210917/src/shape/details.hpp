#ifndef LSPE_SHAPE_H
#error /shape/details.hpp required /shape/shape.hpp to be included firstly.
#endif

#ifndef LSPE_SHAPE_DETAILS
#define LSPE_SHAPE_DETAILS

#include <float.h>

#include "../base/math.hpp"
#include "../utils/testunit.h"

namespace lspe
{

namespace shape
{

namespace details
{

//! ::START:: line intersection
template <typename T>
bool parallel(const vec2<T> &x1, const vec2<T> &y1,
	const vec2<T> &x2, const vec2<T> &y2);

template <typename T>
void linep2general(const vec2<T> &x1, const vec2<T> &y1,
	const vec2<T> &x2, const vec2<T> &y2, vec3lf *k1, vec3lf *k2);

template <typename T>
bool intersect(const vec2<T> &x1, const vec2<T> &y1, int type1,
	const vec2<T> &x2, const vec2<T> &y2, int type2);

template <typename T>
vec2lf intersection(const vec2<T> &x1, const vec2<T> &y1,
	const vec2<T> &x2, const vec2<T> &y2);

//! ::START:: GJK algorithm
template <typename T>
bool tri_contain_vec2(const array<vec2<T>> &tri, const vec2<T> &p);

template <typename T>
vec2<T> GJK_getFirstDirection(
	const array<vec2<T>> &poly1, const array<vec2<T>> &poly2);

template <typename T>
int GJK_indexOfFarthestPoint(
	const array<vec2<T>> &poly, const vec2<T> &direction);

template <typename T>
vec2<T> GJK_support(const array<vec2<T>> &poly1,
	const array<vec2<T>> &poly2, const vec2<T> &direction);

template <typename T>
bool GJK_containOrigin(vec2<T> &direction, array<vec2<T>> &simplex, int &n);

template <typename T>
bool GJK(const array<vec2<T>> &poly1, const array<vec2<T>> &poly2);

};

};

};

#ifdef LSPE_IMPL

namespace lspe::shape::details
{

template <typename T>
bool parallel(
	const vec2<T> &x1, const vec2<T> &y1,
	const vec2<T> &x2, const vec2<T> &y2
)
{
	T t1 = (y1.y - x1.y) * (y2.x - x2.x);
	T t2 = (y2.y - x2.y) * (y1.x - x1.x);
	return equal(t1, t2);
}

template <typename T>
void linep2general(
	const vec2<T> &x1, const vec2<T> &y1,
	const vec2<T> &x2, const vec2<T> &y2,
	vec3lf *k1, vec3lf *k2
)
{
	double A1 = y1.y - x1.y,
		B1 = x1.x - y1.x,
		C1 = y1.x * x1.y - x1.x * y1.y,
		A2 = y2.y - x2.y,
		B2 = x2.x - y2.x,
		C2 = y2.x * x2.y - x2.x * y2.y;
	if (k1 != nullptr)
	{
		*k1 = vec3lf(A1, B1, C1);
	}
	if (k2 != nullptr)
	{
		*k2 = vec3lf(A2, B2, C2);
	}
}

template <typename T>
bool intersect(
	const vec2<T> &x1, const vec2<T> &y1, int type1,
	const vec2<T> &x2, const vec2<T> &y2, int type2
)
{
	if (parallel(x1, y1, x2, y2)) return false;

	if (type1 == 2 && type2 == 2) return true; //! Straight x Straight

	vec3lf k1, k2;
	linep2general(x1, y1, x2, y2, &k1, &k2);

	// double invdivisor = 1.0 / (k2.x * k1.y - k1.x * k2.y);
	// double x0 = (k1.y * k2.z - k2.y * k1.z) * invdivisor;
	// double y0 = (k2.x * k1.z - k1.x * k2.z) * invdivisor;

	double x0 = (k1.y * k2.z - k2.y * k1.z)
		/ (k2.x * k1.y - k1.x * k2.y);

	bool on[2];

	if (type1 == 0)
	{
		on[0] = between(x0, x1.x, y1.x);
	} else if (type1 == 1)
	{
		double k = less(x1.x, y1.x) * 2 - 1;
		on[0] = gte(x0 * k, x1.x * k);
	} else
	{
		on[0] = true;
	}

	if (type2 == 0)
	{
		on[1] = between(x0, x2.x, y2.x);
	} else if (type2 == 1)
	{
		double k = less(x2.x, y2.x) * 2 - 1;
		on[1] = gte(x0 * k, x2.x * k);
	} else
	{
		on[1] = true;
	}

	return on[0] && on[1];
}

template <typename T>
vec2lf intersection(
	const vec2<T> &x1, const vec2<T> &y1,
	const vec2<T> &x2, const vec2<T> &y2
)	//! assuming both lines are straight
{	//! assuming that the two lines intersect
	vec3lf k1, k2;
	linep2general(x1, y1, x2, y2, &k1, &k2);
	double invdivisor = 1.0 / (k2.x * k1.y - k1.x * k2.y);
	double x0 = (k1.y * k2.z - k2.y * k1.z) * invdivisor;
	double y0 = (k2.x * k1.z - k1.x * k2.z) * invdivisor;
	return vec2lf(x0, y0);
}

//! ::START:: GJK algorithm

template <typename T>
bool tri_contain_vec2(
	const array<vec2<T>> &tri,
	const vec2<T> &p
)
{
	LSPE_ASSERT(tri.size() == 3);

	vec2<T> a(tri.at(0) - p), b(tri.at(1) - p), c(tri.at(2) - p);
	T t1 = a.cross(b).z;
	T t2 = b.cross(c).z;
	T t3 = c.cross(a).z;
	return gte(t1 * t2, 0) && gte(t1 * t3, 0) && gte(t2 * t3, 0);
}

template <typename T>
vec2<T> GJK_getFirstDirection(
	const array<vec2<T>> &poly1,
	const array<vec2<T>> &poly2
)
{
	for (int i = 0; i < poly2.size(); ++i)
	{
		vec2<T> direction = poly1.at(0) - poly2.at(i);
		if (!equal(direction, vec2<T>::origin()))
		{
			return direction;
		}
	}

	LSPE_ASSERT(false); //! program should never reach here
	return vec2<T>::origin();
}

template <typename T>
int GJK_indexOfFarthestPoint(
	const array<vec2<T>> &poly,
	const vec2<T> &direction
)
{
	int index = 0;
	T maxval = direction.dot(poly.at(index));

	for (int i = 1; i < poly.size(); ++i)
	{
		T dist = direction.dot(poly.at(i));
		if (greater(dist, maxval))
		{
			maxval = dist;
			index = i;
		}
	}

	return index;
}

template <typename T>
vec2<T> GJK_support(
	const array<vec2<T>> &poly1,
	const array<vec2<T>> &poly2,
	const vec2<T> &direction
)
{
	int i = GJK_indexOfFarthestPoint(poly1, direction);
	int j = GJK_indexOfFarthestPoint(poly2, -direction);
	return poly1.at(i) - poly2.at(j);
}

template <typename T>
bool GJK_containOrigin(vec2<T> &direction, array<vec2<T>> &simplex, int &n)
{
	vec2<T> a = simplex.at(n - 1), ao = -a;
	if (n == 3)
	{
		vec2<T> ab = simplex.at(1) - a;
		vec2<T> ac = simplex.at(2) - a;
		vec2<T> ab_perp = ac.triproduct(ab, ab);
		vec2<T> ac_perp = ab.triproduct(ac, ac);
		if (greater(ab_perp.dot(ao), 0))
		{
			--n;
			direction = ab_perp;
		} else if (greater(ac_perp.dot(ao), 0))
		{
			simplex[1] = simplex[2];
			--n;
			direction = ac_perp;
		} else
		{
			return true;
		}
	} else if (n == 2)
	{
		vec2<T> ab = simplex.at(1) - a;
		vec2<T> ab_perp = ab.triproduct(ao, ab);
		direction = ab_perp;
	} else
	{	//! program should never reach here
		LSPE_ASSERT(false);
	}

	return false;
}

template <typename T>
bool GJK(const array<vec2<T>> &poly1, const array<vec2<T>> &poly2)
{
	vec2<T> direction = GJK_getFirstDirection(poly1, poly2);

	int n = 0;
	array<vec2<T>> simplex(3);
	simplex[n++] = GJK_support(poly1, poly2, direction);

	direction.negate();

	while (true)
	{
		simplex[n++] = GJK_support(poly1, poly2, direction);

		if (equal(direction, vec2<T>::origin()))
		{
			return true;
		}

		if (lte(simplex.at(n - 1).dot(direction), 0))
		{
			return false;
		} else
		{
			if (GJK_containOrigin(direction, simplex, n))
			{
				return true;
			}
		}
	}

	LSPE_ASSERT(false); //! program should never reach here
	return false;
}

};

#endif

#endif /* LSPE_SHAPE_DETAILS */