#include "solver.h"

using namespace lspe::shape;
using namespace lspe::collision;

void freeShape(Shape shape)
{
	switch (shape.type)
	{
		case ShapeType::eLine:    delete (Line*)(shape.data);    break;
		case ShapeType::eCircle:  delete (Circle*)(shape.data);  break;
		case ShapeType::ePolygen: delete (Polygen*)(shape.data); break;
		case ShapeType::eEllipse: delete (Ellipse*)(shape.data); break;
		case ShapeType::eBezier2: delete (Bezier2*)(shape.data); break;
		case ShapeType::eBezier3: delete (Bezier3*)(shape.data); break;
	};
}

Solver::Solver(float _ratio, float _step)
	: arbiter(nullptr),
	bodys(16), contacts(16),
	ratio(_ratio), step(_step)
{
	bodys.clear();
	contacts.clear();
}

Solver::~Solver()
{
	for (auto body : bodys)
	{
		freeShape(body->getShape());
		delete body;
	}
}

int Solver::newCircleBody(const vec2 &center, float r)
{
	Shape shape;

	float invRatio = 1.0 / ratio;

	auto e = new Circle;
	e->center = center;
	e->r = r;

	shape.data = e;
	shape.type = ShapeType::eCircle;

	auto body = new RigidBody;
	body->setShape(shape);
	body->setBodyType(BodyType::eDynamic);

	float area    = areaOf(*e) * invRatio * invRatio;
	float mass    = area * invRatio;
	float inertia = inertiaOf(*e, mass) * invRatio * invRatio;

	body->setMass(mass);
	body->setInertia(inertia);

	bodys.push_back(body);

	auto box = bboxOf(*e);

	int id = bp.addObject(box, body);
	body->getProperty().reserved = id;
	return id;
}

int Solver::newPolygenBody(std::vector<vec2> &vertices)
{
	return 0;
}

int Solver::newEllipseBody(const vec2 &center, float rx, float ry)
{
	Shape shape;

	float invRatio = 1.0 / ratio;

	auto e = new Ellipse;
	e->center = center;
	e->rx = rx;
	e->ry = ry;
	e->rotation = 0.0f;

	vec2 centroid = centroidOf(*e);
	e->center = centroid;

	shape.data = e;
	shape.type = ShapeType::eEllipse;

	auto body = new RigidBody;
	body->setShape(shape);
	body->setBodyType(BodyType::eDynamic);

	float area    = areaOf(*e) * invRatio * invRatio;
	float mass    = area * invRatio;
	float inertia = inertiaOf(*e, mass) * invRatio * invRatio;

	body->setMass(mass);
	body->setInertia(inertia);

	bodys.push_back(body);

	auto box = bboxOf(*e);

	int id = bp.addObject(box, body);
	body->getProperty().reserved = id;
	return id;
}

int Solver::newTriangleBody(const vec2 &a, const vec2 &b, const vec2 &c)
{
	Shape shape;

	float invRatio = 1.0 / ratio;

	auto e = new Polygen;
	e->vertices.resize(3);
	e->vertices[0] = a;
	e->vertices[1] = b;
	e->vertices[2] = c;

	vec2 centroid = centroidOf(*e);
	e->center = centroid;

	shape.data = e;
	shape.type = ShapeType::ePolygen;

	auto body = new RigidBody;
	body->setShape(shape);
	body->setBodyType(BodyType::eDynamic);

	float area    = areaOf(*e) * invRatio * invRatio;
	float mass    = area * invRatio;
	float inertia = inertiaOf(*e, mass) * invRatio * invRatio;

	body->setMass(mass);
	body->setInertia(inertia);

	bodys.push_back(body);

	auto box = bboxOf(*e);

	int id = bp.addObject(box, body);
	body->getProperty().reserved = id;
	return id;
}

int Solver::newRectangleBody(bbox2 rect)
{
	Shape shape;

	float invRatio = 1.0 / ratio;

	auto e = new Polygen;
	vec2 pa = rect.lower;
	vec2 pc = rect.upper;
	vec2 pb = { pa.x, pc.y };
	vec2 pd = { pc.x, pa.y };

	e->vertices.resize(4);
	e->vertices[0] = pa;
	e->vertices[1] = pb;
	e->vertices[2] = pc;
	e->vertices[3] = pd;

	vec2 centroid = centroidOf(*e);
	e->center = centroid;

	shape.data = e;
	shape.type = ShapeType::ePolygen;

	auto body = new RigidBody;
	body->setShape(shape);
	body->setBodyType(BodyType::eDynamic);

	float area    = areaOf(*e) * invRatio * invRatio;
	float mass    = area * invRatio;
	float inertia = inertiaOf(*e, mass) * invRatio * invRatio;

	body->setMass(mass);
	body->setInertia(inertia);

	bodys.push_back(body);

	auto box = bboxOf(*e);

	int id = bp.addObject(box, body);
	body->getProperty().reserved = id;
	return id;
}

const std::vector<RigidBody*>& Solver::getObjects() const
{
	return bodys;
}

void* Solver::getUserdata(int id)
{
	return bp.getUserdata(id);
}

void Solver::preSolve()
{
	for (auto body : bodys)
	{
		body->preUpdate(step);
	}
}

void Solver::inSolve()
{
	bp.updatePairs();

	int pairsCount;
	auto pairs = bp.getPairs(&pairsCount);

	//! no collsion detection should perform
	if (pairsCount == 0) return;

	for (int i = 0; i < pairsCount; ++i)
	{
		RigidBody *bodys[2];
		bodys[0] = (RigidBody*)bp.getUserdata(pairs[i].first);
		bodys[1] = (RigidBody*)bp.getUserdata(pairs[i].second);

		if (!(bodys[0]->getBodyType() == BodyType::eDynamic
			&& bodys[1]->getBodyType() == BodyType::eDynamic))
		{
			continue;
		}

		uint64_t hash = (pairs[i].first & 0xffff) << 16
			| (pairs[i].second & 0xffff);

		auto it = std::find_if(
			contacts.begin(), contacts.end(), [hash]
			(const DemoContact &a){
				return a.hash == hash;
			});

		collider.reset();

		collider.setTestPair(
			bodys[0]->getShape(),
			bodys[1]->getShape());

		collider.bindSupports(
			getDefaultSupport(bodys[0]->getShape().type),
			getDefaultSupport(bodys[1]->getShape().type));

		collider.bindInitialGenerator(
		[](Shape x, Shape y, const vec2&, void*) {
			return Solver::centerOf(x) - Solver::centerOf(y);
		});

		bool collided = collider.collided();

		if (!collided)
		{
			if (it != contacts.end())
			{
				LSPE_DEBUG(
					"Collision Test: "
					"(%d, %d) ended by Collider",
					(*it).indices[0],
					(*it).indices[1]);

				contacts.erase(it);
			}
			continue;
		}

		arbiter.resetCollider(&collider);
		arbiter.perform();

		if (!arbiter.isCollided())
		{
			if (it != contacts.end())
			{
				LSPE_DEBUG(
					"Collision Test: "
					"(%d, %d) ended by Collider",
					(*it).indices[0],
					(*it).indices[1]);

				contacts.erase(it);
			}
			continue;
		}

		if (it != contacts.end())
		{
			LSPE_DEBUG(
				"Collision Test: "
				"Stay Collision (%d, %d)",
				(*it).indices[0],
				(*it).indices[1]);

			continue;
		}

		DemoContact contact;

		contact.node[0].other = bodys[1];
		contact.node[0].contact = &contact;
		arbiter.getClosetPoint(
			contact.node[0].crossPointFromCentroid + 1,
			contact.node[0].crossPointFromCentroid);

		contact.node[1].other = bodys[0];
		contact.node[1].contact = &contact;
		arbiter.getClosetPoint(
			contact.node[0].crossPointFromCentroid + 1,
			contact.node[0].crossPointFromCentroid);

		contact.indices[0] = pairs[i].first;
		contact.indices[1] = pairs[i].second;

		arbiter.getPenetration(&contact.normal, &contact.penetration);

		contact.friction    = 0.0f; //! defaultly smooth surface
		contact.restitution = 1.0f; //! defaultly perfectly elastic collision

		contact.hash = hash;

		contacts.push_back(contact);

		LSPE_DEBUG(
			"Collision Test: "
			"Begin Collision (%d, %d)",
			contact.indices[0],
			contact.indices[1]);
	}

	for (auto &e : contacts)
	{	//! apply collision response
		auto  a   = e.node[1].other;
		auto  b   = e.node[0].other;
		vec2  oa  = a->getCentroid();
		vec2  ob  = b->getCentroid();
		vec2  pa  = e.node[0].crossPointFromCentroid[0];
		vec2  pb  = e.node[0].crossPointFromCentroid[1];
		vec2  ra  = pa - oa;
		vec2  rb  = pb - ob;
		float wa  = a->getProperty().angularVelocity;
		float wb  = b->getProperty().angularVelocity;
		vec2  vpa = a->getProperty().linearVelocity + wa * ra;
		vec2  vpb = b->getProperty().linearVelocity + wb * rb;
		float ima = a->getInvMass();
		float imb = b->getInvMass();
		float iia = a->getInvInertia();
		float iib = b->getInvInertia();
		vec2  n   = e.normal;
		float Mn  = 1.0 / (
			  ima + cross(ra, n) * cross(ra, n) * iia
			+ imb + cross(rb, n) * cross(rb, n) * iib);

		float lambda = -dot(n, vpa - vpb) * Mn;

		vec2 impulseForce = lambda * n * step;
		LSPE_DEBUG("Apply Collision Impulse: (%f, %f) N*s", impulseForce.x, impulseForce.y);
		a->applyLinearImpulse( impulseForce, ra, true);
		b->applyLinearImpulse(-impulseForce, rb, true);
	}
}

void Solver::postSolve()
{
	for (auto body : bodys)
	{
		body->postUpdate(step);

		int id = body->getProperty().reserved;
		bp.moveObject(id, bboxOf(body->getShape()), vec2(0, 0));
	}
}

void Solver::traverse(abt::fnvisit visit, void *extra, int method)
{
	bp.traverse(visit, extra, method);
}

vec2 Solver::centerOf(Shape shape)
{
	LSPE_ASSERT(shape.type != ShapeType::eNull);
	LSPE_ASSERT(shape.data != nullptr);

	switch (shape.type)
	{
		case ShapeType::eLine:    return centroidOf(*(Line*)(shape.data));
		case ShapeType::eCircle:  return ((Circle *)(shape.data))->center;
		case ShapeType::ePolygen: return ((Polygen*)(shape.data))->center;
		case ShapeType::eEllipse: return ((Ellipse*)(shape.data))->center;
		default:
		{
			LSPE_DEBUG(
				"Solver::centerOf: "
				"user-defined shape type has no centerOf (etc.) function yet");
			return { 0, 0 };
		}
	}
}

float Solver::getRatio() const
{
	return ratio;
}

fnsupport Solver::getDefaultSupport(ShapeType type)
{
	LSPE_ASSERT(type != ShapeType::eNull);

	switch (type)
	{
		case ShapeType::eLine:    return supportLine;
		case ShapeType::eCircle:  return supportCircle;
		case ShapeType::ePolygen: return supportPolygen;
		case ShapeType::eEllipse: return supportEllipse;
		default:
		{
			LSPE_DEBUG(
				"Solver::getDefaultSupport: "
				"user-defined shape type has no support function yet");
			return 0L;
		}
	}
}
