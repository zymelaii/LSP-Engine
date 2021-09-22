#include <float.h>
#include <lspe/collision.h>

namespace lspe
{

namespace collision
{

using namespace lspe::shape;

vec2 supportLine(Shape x, const vec2 &direction)
{
	auto p = (Line*)x;
	LSPE_ASSERT(p->type >= 0 && p->type <= 2);
	LSPE_ASSERT(!(p->pa == p->pb));

#if 0
	vec2 v = p->pa - p->pb;
	float t1 = dot(v, direction);
	float t2 = dot(-v, direction);
#endif

	float t1 = dot(p->pa, direction);
	float t2 = dot(p->pb, direction);

	return t1 > t2 ? p->pa : p->pb;
}

vec2 supportCircle(Shape x, const vec2 &direction)
{
	auto p = (Circle*)x;
	LSPE_ASSERT(p->r > 0);

	return direction.normalized() * p->r + p->center;
}

vec2 supportPolygen(Shape x, const vec2 &direction)
{
	auto p = (Polygen*)x;
	LSPE_ASSERT(p->vertices.size() >= 3);

	auto &v = p->vertices;
	int index = 0, i;

	// float maxval = dot(direction, v[1] - v[0]);

	// for (i = 1; i < v.size() - 1; ++i)
	// {
	// 	float val = dot(direction, v[i + 1] - v[i]);
	// 	if (val > maxval)
	// 	{
	// 		maxval = val;
	// 		index = i;
	// 	}
	// }

	// if (dot(direction, v[0] - v[i]) > maxval)
	// {
	// 	index = i;
	// }

	// return v[(index + 1) % v.size()] - v[index];

	float maxval = dot(direction, v[0]);
	for (int i = 1; i < v.size(); ++i)
	{
		float val = dot(direction, v[i]);
		if (val > maxval)
		{
			maxval = val;
			index = i;
		}
	}

	return v[index];
}

vec2 supportEllipse(Shape x, const vec2 &direction)
{
	auto p = (Ellipse*)x;
	LSPE_ASSERT(p->rx > 0 && p->ry > 0);

	mat2x2 mat_rotation = getRotateMatrix(p->rotation);
	// mat2x2 mat_invrotation = getRotateMatrix(-p->rotation);

	vec2 ans;

	// vec2 d2 = mat_invrotation * direction;
	// if (d2.x >= -FLT_EPSILON && d2.x <= FLT_EPSILON)
	// {
	// 	ans.x = 0;
	// 	ans.y = d2.y > 0 ? p->ry : -p->ry;
	// } else
	// {
	// 	float k = d2.y / d2.x;
	// 	float bbkk = p->ry * p->ry * k * k;
	// 	float sqabk = sqrt(p->rx * p->rx + bbkk);

	// 	ans.x = sqabk - bbkk / sqabk;
	// 	ans.y = bbkk / k / sqabk;
	// }

	vec2 nd = direction.normalized();
	ans = { nd.x * p->rx, nd.y * p->ry };

	return mat_rotation * ans + p->center;
}

vec2 supportBezier2(Shape x, const vec2 &direction)
{
	auto p = (Bezier2*)x;

	return p->P[0];
}

vec2 supportBezier3(Shape x, const vec2 &direction)
{
	auto p = (Bezier3*)x;

	return p->P[0];
}

};

using namespace collision;

Collider::Collider()
	: tested(false), iscollided(false),
	flag(0)
{
	shapes[0] = nullptr;
	shapes[1] = nullptr;

	getfirstdirection = nullptr;

	support[0] = nullptr;
	support[1] = nullptr;

	extra = nullptr;

	expandedSimplex.clear();
	expandedSimplex.reserve(3);
}

/********************************
 *  @author: ZYmelaii
 *
 *  @Collider: Collider::collided()
 *
 *  @brief: detect whether two objects collide
 *
 *  @NOTES: apply GJK algorithm
 *******************************/
bool Collider::collided()
{
	LSPE_ASSERT(flag == 0x1f);

	vec2 d, pd;
	d = getfirstdirection(shapes[0], shapes[1], d, extra);
	if (d.norm() < FLT_EPSILON)
	{
		d = { 1.0f, 0.0f };
		LSPE_DEBUG("Collision Test: take (1, 0) as the first direction");
	};

	int n = 0;
	simplex[n++] = support[0](shapes[0], d) - support[1](shapes[1], -d);

	d = -d;

	int iteration = 0;
	while (iteration++ < 32)
	{

#ifdef DEBUG
		if (d.norm() < FLT_EPSILON)
#else
		if (dot(d, d) < FLT_EPSILON) //! accelerate computation
#endif
		{
			LSPE_DEBUG(
				"Collision Test Result: "
				"ORIGIN IS ON SIMPLEX EDGES (iteration=%d)",
				iteration + 1);
			tested = true;
			iscollided = true;
			LSPE_ASSERT(n == 3);
			return iscollided;
		}

		simplex[n++] = support[0](shapes[0], d) - support[1](shapes[1], -d);

		if (dot(simplex[n - 1], d) < 0)
		{
			LSPE_DEBUG(
				"Collision Test Result: "
				"UNEXPECTED SIMPLEX POINT (iteration=%d)",
				iteration + 1);
			tested = true;
			iscollided = false;
			return iscollided;
		}

		if (containOrigin(d, simplex, n))
		{
			LSPE_DEBUG(
				"Collision Test Result: "
				"GOOD (iteration=%d)",
				iteration + 1);
			++n;
			tested = true;
			iscollided = true;
			LSPE_ASSERT(n == 3);
			return iscollided;
		}
	}

	LSPE_DEBUG("Collision Test FAILED! (iterations >= %d)", iteration - 1);
	LSPE_DEBUG(
		"LAST ITERATION: direction=(%f,%f); "
		"Simplex={(%f,%f),(%f,%f),(%f,%f)};",
		d.x, d.y,
		simplex[0].x, simplex[0].y,
		simplex[1].x, simplex[1].y,
		simplex[2].x, simplex[2].y);

	return false;
}

vec2 Collider::computePenetration()
{
	LSPE_ASSERT(flag == 0x1f);
	LSPE_ASSERT(tested);

	LSPE_ASSERT(expandedSimplex.size() == 0);

	expandedSimplex.push_back(simplex[0]);
	expandedSimplex.push_back(simplex[1]);
	expandedSimplex.push_back(simplex[2]);

	vec2 direction, M0(0, 0);

	while (true)
	{
		vec2 a = expandedSimplex.back();
		vec2 b = expandedSimplex.front();
		vec2 l = b - a;

		float tmp = cross(a, b);
		float minval = tmp * tmp / dot(l, l);

		int index = expandedSimplex.size() - 1;
		for (int i = 0; i < expandedSimplex.size() - 1; ++i)
		{
			a = expandedSimplex[i];
			b = expandedSimplex[i + 1];
			l = b - a;

			tmp = cross(a, b);
			float val = tmp * tmp / dot(l, l);

			if (val < minval)
			{
				minval = val;
				index = i;
			}
		}

		a = expandedSimplex[index];
		b = expandedSimplex[(index + 1) % expandedSimplex.size()];
		l = b - a;

		direction = { l.y, -l.x };
		if (dot(a, direction) < dot(a, -direction))
		{
			direction = -direction;
		}

		vec2 M = support[0](shapes[0], direction)
			- support[1](shapes[1], -direction);

		if (dot(M, direction) < 0)
		{
			LSPE_DEBUG("Compute Penetration Vector: "
				"new Minkowski Point wasn't in the expected direction; "
				"direction=(%f,%f); "
				"Minkowski Point=(%f,%f)",
				direction.x, direction.y, M.x, M.y);
			break;
		}

#if 0
		if (M == a || M == b)
		{
			LSPE_DEBUG("Compute Penetration Vector: "
				"M3 == M1 or M3 == M2");
			break;
		}
#endif

		bool shouldQuit = false;
		for (int i = 0; i < expandedSimplex.size(); ++i)
		{
			if (M == expandedSimplex[i])
			{
				shouldQuit = true;
				break;
			}
		}
		if (shouldQuit) break;

		if ((M - a).norm() + (M - b).norm() > FLT_EPSILON)
		{
			expandedSimplex.push_back(M);
			LSPE_DEBUG("Compute Penetration Vector: "
				"added Minkowski Point (%f,%f); "
				"S(ExpandedSimplex)=%d;",
				M.x, M.y, expandedSimplex.size());
		} else
		{
			LSPE_DEBUG("Compute Penetration Vector: "
				"|M1 - M3| + |M2 - M3| < e");
			break;
		}
	}

	return direction;
}

void Collider::setTestPair(Shape a, Shape b)
{
	if (a != nullptr)
	{
		shapes[0] = a;
		flag |= 0x01;
	}

	if (b != nullptr)
	{
		shapes[1] = b;
		flag |= 0x02;
	}
}

void Collider::bindSupports(fnsupport support1, fnsupport support2)
{
	if (support1 != nullptr)
	{
		support[0] = support1;
		flag |= 0x04;
	}

	if (support2 != nullptr)
	{
		support[1] = support2;
		flag |= 0x08;
	}
}

void Collider::bindInitialGenerator(fnsupport2 generator)
{
	if (generator != nullptr)
	{
		getfirstdirection = generator;
		flag |= 0x10;
	}
}

void Collider::bindExtraData(void *extra)
{
	this->extra = extra;
}

void* Collider::getExtraData()
{
	return extra;
}

void Collider::reset()
{
	tested = false;
	iscollided = false;

	expandedSimplex.clear();
}

bool processSimplex2(vec2 &direction, vec2 *simplex)
{
	LSPE_ASSERT(simplex != nullptr);

	vec2 a = simplex[0];
	vec2 b = simplex[1];

	vec2 ao = -a;
	vec2 ab = b - a;

#if 0
	if (dot(ao, ab) > 0)
	{
		vec2 ab_perp = triproduct(ab, ao, ab);
		direction = ab_perp.normalized();
	} else
	{
		direction = ao;
	}
#endif

#if 1
	vec2 ab_perp = triproduct(ab, ao, ab);
	direction = ab_perp.normalized();
#endif

	return false;
}

bool processSimplex3(vec2 &direction, vec2 *simplex)
{
	LSPE_ASSERT(simplex != nullptr);

	vec2 &a = simplex[0];
	vec2 &b = simplex[1];
	vec2 &c = simplex[2];

	vec2 ao = -a;
	vec2 ab = b - a;
	vec2 ac = c - a;

	vec2 ab_perp = triproduct(ac, ab, ab);
	vec2 ac_perp = triproduct(ab, ac, ac);

#if 0
	if (dot(ao, ac_perp) > 0)
	{
		if (dot(ao, ac) > 0)
		{
			b = c;
			direction = ac_perp.normalized();
		} else
		{
			return processSimplex2(direction, simplex);
		}
	} else if (dot(ab, ab_perp) > 0)
	{
		direction = ab_perp.normalized();
		// return processSimplex2(direction, simplex);
	} else
	{
		return true;
	}
#endif

#if 1
	if (dot(ao, ab_perp) > 0)
	{
		direction = ab_perp.normalized();
	} else if (dot(ao, ac_perp) > 0)
	{
		b = c;
		direction = ac_perp.normalized();
	} else
	{
		return true;
	}
#endif

	return false;
}

bool Collider::containOrigin(vec2 &direction, vec2 *simplex, int &n)
{
	switch (n)
	{
		case 2: return processSimplex2(direction, simplex);
		case 3: --n;
				return processSimplex3(direction, simplex);
		default: LSPE_ASSERT(false); //! illegal entry
	}

	LSPE_ASSERT(false);
	return false;
}

};
