#include <float.h>
#include <algorithm>
#include <lspe/collision.h>

namespace lspe
{

static inline vec2 perpendicularFromOrigin(vec2 a, vec2 b);
bool processSimplex2(vec2 &direction, vec2 *simplex);
bool processSimplex3(vec2 &direction, vec2 *simplex);

namespace collision
{

using namespace lspe::shape;

vec2 supportLine(Shape x, const vec2 &direction)
{
	LSPE_ASSERT(x.type == eLine);
	auto p = (Line*)(x.data);
	LSPE_ASSERT(p->type >= 0 && p->type <= 2);
	LSPE_ASSERT(!(p->pa == p->pb));

	float t1 = dot(p->pa, direction);
	float t2 = dot(p->pb, direction);

	return t1 > t2 ? p->pa : p->pb;
}

vec2 supportCircle(Shape x, const vec2 &direction)
{
	LSPE_ASSERT(x.type == eCircle);
	auto p = (Circle*)(x.data);
	LSPE_ASSERT(p->r > 0);

	return direction.normalized() * p->r + p->center;
}

vec2 supportPolygen(Shape x, const vec2 &direction)
{
	LSPE_ASSERT(x.type == ePolygen);
	auto p = (Polygen*)(x.data);
	LSPE_ASSERT(p->vertices.size() >= 3);

	auto &v = p->vertices;
	int index = 0, i;

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
	LSPE_ASSERT(x.type == eEllipse);
	auto p = (Ellipse*)(x.data);
	LSPE_ASSERT(p->rx > 0 && p->ry > 0);

	mat2x2 mat_rotation = getRotateMatrix(p->rotation);
	mat2x2 mat_invrotation = getRotateMatrix(-p->rotation);

	vec2 rd = (mat_invrotation * direction).normalized();
	vec2 ans = { rd.x * p->rx, rd.y * p->ry };

	return mat_rotation * ans + p->center;
}

vec2 supportBezier2(Shape x, const vec2 &direction)
{
	LSPE_ASSERT(x.type == eBezier2);
	auto p = (Bezier2*)(x.data);

	return p->P[0];
}

vec2 supportBezier3(Shape x, const vec2 &direction)
{
	LSPE_ASSERT(x.type == eBezier3);
	auto p = (Bezier3*)(x.data);

	return p->P[0];
}

};

using namespace collision;

Arbiter::Arbiter(Collider *collider, size_t maxIter)
	: active(false), collided(false), epsilon(0.01f)
{

#ifdef DEBUG
	maxIteration = maxIter;
#else
	maxIteration = maxIter > 4 ? maxIter : 4;
#endif

	if (collider != nullptr)
	{
		if (collider->tested && collider->iscollided)
		{
			active = true;
		}
	}

	M.reserve(maxIteration);
	E.reserve(maxIteration);
	I.reserve(maxIteration);

	if (active)
	{
		setCollider(collider);
	}
}

Arbiter::~Arbiter()
{

}

void Arbiter::setEps(float eps)
{
	LSPE_ASSERT(eps > FLT_EPSILON);
	epsilon = eps;
}

bool Arbiter::isActive() const
{
	return active;
}

bool Arbiter::isCollided() const
{
	return collided;
}

void Arbiter::getPenetration(
	vec2 *penetrationVector) const
{
	if (active && collided)
	{
		if (penetrationVector != nullptr)
		{
			*penetrationVector =
				penetration.normal * penetration.distance;
		}
	}
}

void Arbiter::getPenetration(
	vec2 *normal, float *distance) const
{
	if (active && collided)
	{
		if (normal != nullptr)
		{
			*normal = penetration.normal;
		}

		if (distance != nullptr)
		{
			*distance = penetration.distance;
		}
	}
}

void Arbiter::getClosetPoint(vec2 *a, vec2 *b) const
{
	if (active && collided)
	{
		if (a != nullptr)
		{
			*a = closetPoint[0];
		}
		
		if (b != nullptr)
		{
			*b = closetPoint[1];
		}
	}
}

bool Arbiter::perform()
{
	if (!active)
	{	//! Arbiter hasn't binded a Collider yet
		return false;
	}

	if (collided)
	{	//! already performed
		return true;
	}

	vec2 a, b;
	vec2 v, v0;

	a = M[E[I[0]].aId].point;
	b = M[E[I[0]].bId].point;

	v = perpendicularFromOrigin(a, b);

	bool done = false;
	size_t iteration = -1;
	while (++iteration < maxIteration)
	{
		v0 = v;

		vec2 direction = v.normalized();
		vec2 A = support[0](shapes[0],  direction);
		vec2 B = support[1](shapes[1], -direction);
		vec2 P = A - B;

		if (dot(direction, P) < 0)
		{
			LSPE_DEBUG(
				"Arbiter Perform: "
				"bad new Minkowski point "
				"(P isn't on the expected direction)");
			break;
		}

		if ((P - a).norm() + (b - P).norm() < epsilon)
		{
			LSPE_DEBUG(
				"Arbiter Perform: "
				"wead new Minkowski point "
				"(points difference is within epsilon)");
			done = true;
			break;
		}

		//! as EPA finally generates a convex hull
		//! P is supposed to break the edge constructed by a and b
		//! new edges respectively constructed by a and P, P and b
		//! will be orderly inserted into E

		int n = M.size();
		M.resize(n + 1);
		E.resize(n + 1);
		I.resize(n + 1);

		M[n].point = P;
		M[n].fromA = A;
		M[n].fromB = B;

		I[n] = n;

		MetaEdge aP, Pb;

		aP.aId = E[I[0]].aId;
		aP.bId = n;
		aP.perpDistance = perpendicularFromOrigin(
			M[aP.aId].point, M[aP.bId].point).norm();

		Pb.aId = n;
		Pb.bId = E[I[0]].bId;
		Pb.perpDistance = perpendicularFromOrigin(
			M[Pb.aId].point, M[Pb.bId].point).norm();

		E[I[0]] = aP;
		E[I[n]] = Pb;

		std::sort(I.begin(), I.end(), [this](int a, int b) {
			return E[a].perpDistance < E[b].perpDistance;
		});

		a = M[E[I[0]].aId].point;
		b = M[E[I[0]].bId].point;

		if (dot(b - a, b - a) < FLT_EPSILON)
		{
			LSPE_DEBUG(
				"Arbiter Perform: "
				"next direction vector is approaching zero");
			done = true;
			break;
		}

		v = perpendicularFromOrigin(a, b);
		if ((v - v0).norm() < epsilon)
		{
			LSPE_DEBUG(
				"Arbiter Perform: "
				"difference of penetration vector is within epsilon");
			done = true;
			break;
		}
	}

	if (iteration >= maxIteration)
	{
		LSPE_DEBUG("Arbiter Perform: reach max interation");
	}

	if (done)
	{
		collided = true;
		penetration.distance = v.norm();
		penetration.normal = v.normalized();
		getClosetPoint();

		LSPE_DEBUG(
			"Arbiter Perform: "
			"penetration vector=(%f, %f) (iteration=%d)",
			v.x, v.y, iteration);
	} else
	{
		LSPE_DEBUG(
			"Arbiter Perform FAILED! "
			"(iterations >= %d)",
			iteration);
	}

	return true;
}

void Arbiter::setCollider(Collider *collider)
{
	LSPE_ASSERT(collider != nullptr);
	LSPE_ASSERT(collider->tested && collider->iscollided);

	shapes[0]  = collider->shapes[0];
	shapes[1]  = collider->shapes[1];
	support[0] = collider->support[0];
	support[1] = collider->support[1];

	M.resize(3);
	E.resize(3);
	I.resize(3);

	for (int i = 0; i < 3; ++i)
	{
		M[i].point = collider->simplex[i];
		M[i].fromA = collider->fromA[i];
		M[i].fromB = collider->fromB[i];

		E[i].aId = i;
		E[i].bId = (i + 1) % 3;

		vec2 a = M[E[i].aId].point;
		vec2 b = M[E[i].bId].point;

		E[i].perpDistance = perpendicularFromOrigin(a, b).norm();

		I[i] = i;
	}

	std::sort(I.begin(), I.end(), [this](int a, int b) {
		return E[a].perpDistance < E[b].perpDistance;
	});
}

void Arbiter::getClosetPoint()
{
	MetaPoint A = M[E[I[0]].aId];
	MetaPoint B = M[E[I[0]].bId];

	vec2 AB = B.point - A.point;
	float sqab = dot(AB, AB);

	if (sqab < FLT_EPSILON)
	{
		closetPoint[0] = A.point;
		closetPoint[1] = A.point;
	} else
	{
		float r = std::clamp(-dot(AB, A.point) / sqab, 0.0f, 1.0f);
		closetPoint[0] = A.fromA * (1 - r) + B.fromA * r;
		closetPoint[1] = A.fromB * (1 - r) + B.fromB * r;
	}
}

void Arbiter::getContacts()
{

}

Collider::Collider()
	: tested(false), iscollided(false),
	simplexIndex(-1), flag(0)
{
	shapes[0] = { nullptr, eNull };
	shapes[1] = { nullptr, eNull };

	getfirstdirection = nullptr;

	support[0] = nullptr;
	support[1] = nullptr;

	extra = nullptr;
}

vec2 perpendicularFromOrigin(vec2 a, vec2 b)
{
	vec2 ab = b - a;
	float sqab = dot(ab, ab);

	if (sqab < FLT_EPSILON)
	{
		LSPE_DEBUG(
			"perpendicularFromOrigin: "
			"bad input line { (%f,%f), (%f,%f) }",
			a.x, a.y, b.x, b.y);
		return a;
	} else
	{
		return a + ab * (dot(-a, ab) / sqab);
	}
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

	vec2 d = getfirstdirection(shapes[0], shapes[1], d, extra);
	if (dot(d, d) < FLT_EPSILON)
	{
		d = { 1.0f, 0.0f };
		LSPE_DEBUG("Collision Test: take (1, 0) as the first direction");
	};

	addSimplexPoint(d);

	d = -d;

	int iteration = -1;
	while (++iteration < 16)
	{
		if (dot(d, d) < FLT_EPSILON) //! <=> d.norm() == 0
		{
			LSPE_DEBUG(
				"Collision Test Result: "
				"ORIGIN IS ON SIMPLEX EDGES (iteration=%d)",
				iteration + 1);
			tested = true;
			iscollided = true;
			LSPE_ASSERT(simplexIndex == 2);
			return iscollided;
		}

		addSimplexPoint(d);

		if (dot(simplex[simplexIndex], d) < 0)
		{
#if 0
			LSPE_DEBUG(
				"Collision Test Result: "
				"UNEXPECTED SIMPLEX POINT (iteration=%d)",
				iteration + 1);
#endif
			tested = true;
			iscollided = false;
			return iscollided;
		}

		if (simplexContainOrigin(d))
		{
			LSPE_DEBUG(
				"Collision Test Result: "
				"PASS (iteration=%d)",
				iteration + 1);
			tested = true;
			iscollided = true;
			LSPE_ASSERT(simplexIndex == 2);
			return iscollided;
		}

		if (simplexIndex == 2)
		{
			--simplexIndex;
		}
	}

	LSPE_DEBUG("Collision Test FAILED! (iterations >= %d)", iteration);
	LSPE_DEBUG(
		"LAST ITERATION: direction=(%f,%f); "
		"Simplex={(%f,%f),(%f,%f),(%f,%f)};",
		d.x, d.y,
		simplex[0].x, simplex[0].y,
		simplex[1].x, simplex[1].y,
		simplex[2].x, simplex[2].y);

	return false;
}

void Collider::setTestPair(Shape a, Shape b)
{
	if (a.type != eNull)
	{
		shapes[0] = a;
		flag |= 0x01;
	}

	if (b.type != eNull)
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
	simplexIndex = -1;
}

bool processSimplex2(vec2 &direction, vec2 *simplex)
{
	LSPE_ASSERT(simplex != nullptr);

	vec2 a = simplex[0];
	vec2 b = simplex[1];

	vec2 ao =   - a;
	vec2 ab = b - a;

	vec2 ab_perp = triproduct(ab, ao, ab);
	direction = ab_perp.normalized();

	return false;
}

bool processSimplex3(vec2 &direction, vec2 *simplex)
{
	LSPE_ASSERT(simplex != nullptr);

	vec2 &a = simplex[0];
	vec2 &b = simplex[1];
	vec2 &c = simplex[2];

	vec2 ao =   - a;
	vec2 ab = b - a;
	vec2 ac = c - a;

	vec2 ab_perp = triproduct(ac, ab, ab);
	vec2 ac_perp = triproduct(ab, ac, ac);

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

	return false;
}

void Collider::addSimplexPoint(vec2 direction)
{
	++simplexIndex;
	fromA[simplexIndex]   = support[0](shapes[0],  direction);
	fromB[simplexIndex]   = support[1](shapes[1], -direction);
	simplex[simplexIndex] = fromA[simplexIndex] - fromB[simplexIndex];
}

bool Collider::simplexContainOrigin(vec2 &direction)
{
	switch (simplexIndex + 1)
	{
		case 2: return processSimplex2(direction, simplex);
		case 3: return processSimplex3(direction, simplex);
		default: LSPE_ASSERT(false); //! illegal entry
	}

	LSPE_ASSERT(false);
	return false;
}

};
