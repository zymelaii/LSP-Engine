#include "solver.h"

using namespace lspe::shape;
using namespace lspe::collision;

Solver::Solver(float _step)
	: arbiter(nullptr),
	bodys(16), contacts(16), step(_step)
{

}

Solver::~Solver()
{
	for (auto body in bodys)
	{
		delete body->getShape().data;
		delete body;
	}
}

void Solver::newCircleBody(const vec2 &center, float r)
{
	Shape shape;

	auto e = new Circle;
	e->center = center;
	e->r = r;

	shape.data = e;
	shape.type = ShapeType::eCircle;

	auto body = new RigidBody;
	body->setShape(shape);
	body->setBodyType(BodyType::eDynamic);

	float area    = areaOf(*e);
	float mass    = area * 1.0f;
	float inertia = inertiaOf(*e, mass);

	body->setMass(mass);
	body->setInertia(inertia);
}

void Solver::newPolygenBody()
{

}

void Solver::newEllipseBody(const vec2 &center, float rx, float ry)
{
	auto e = new Ellipse;

	e->center = center;
	e->rx = rx;
	e->ry = ry;
	e->rotation = 0.0f;

	vec2 centroid = centroidOf(*e);
	e->center = centroid;

	shape.data = e;
	shape.type = ShapeType::Ellipse;

	auto body = new RigidBody;
	body->setShape(shape);
	body->setBodyType(BodyType::eDynamic);

	float area    = areaOf(*e);
	float mass    = area * 1.0f;
	float inertia = inertiaOf(*e, mass);

	body->setMass(mass);
	body->setInertia(inertia);

	auto box = bboxOf(*e);

	int id = bp.addObject(box, body);
	body->getPenetration().userdata = static_cast<void*>(id);
}

void Solver::newTriangleBody(const vec2 &a, const vec2 &b, const vec2 &c)
{
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

	float area    = areaOf(*e);
	float mass    = area * 1.0f;
	float inertia = inertiaOf(*e, mass);

	body->setMass(mass);
	body->setInertia(inertia);

	auto box = bboxOf(*e);

	int id = bp.addObject(box, body);
	body->getPenetration().userdata = static_cast<void*>(id);
}

void Solver::newRectangleBody(bbox2 rect)
{
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

	float area    = areaOf(*e);
	float mass    = area * 1.0f;
	float inertia = inertiaOf(*e, mass);

	body->setMass(mass);
	body->setInertia(inertia);

	auto box = bboxOf(*e);

	int id = bp.addObject(box, body);
	body->getPenetration().userdata = static_cast<void*>(id);
}

void Solver::preSolve(float dt)
{
	for (auto body : bodys)
	{
		body->preUpdate();
	}
}

void Solver::inSolve(float dt)
{
	bp.updatePairs();

	int pairsCount;
	auto pairs = bp.getPairs(&pairsCount);

	//! no collsion detection should perform
	if (pairsCount == 0) return;

	for (int i = 0; i < pairsCount; ++i)
	{
		RigidBody *bodys[2];
		bodys[0] = (RigidBody*)bp.getUserData(pairs[i].first);
		bodys[1] = (RigidBody*)bp.getUserData(pairs[i].second);

		if (!(bodys[0]->getBodyType() == BodyType::eDynamic
			&& bodys[1]->getBodyType() == BodyType::eDynamic))
		{
			continue;
		}

		uint64_t hash = (pairs[i].first & 0xffff) << 16
			| (pairs[i].second & 0xffff);

		auto it = std::find_if(
			contacts.begin(), contacts.end(), []
			(const DemoContact &a, const DemoContact &b){
				return a.hash == b.hash;
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
			LSPE_DEBUG(
				"Collision Test: "
				"end collision (%d, %d)",
				(*it).indices[0],
				(*it).indices[1]);

			if (it != contacts.end())
			{
				contacts.erase(it);
			}
	
			continue;
		}

		arbiter.setCollider(&collider);
		arbiter.perform();

		if (!arbiter.isCollided())
		{
			LSPE_DEBUG(
				"Collision Test: "
				"end collision (%d, %d)",
				(*it).indices[0],
				(*it).indices[1]);

			if (it != contacts.end())
			{
				contacts.erase(it);
			}

			continue;
		}

		if (it != contacts.end())
		{
			LSPE_DEBUG(
				"Collision Test: "
				"stay collision (%d, %d)",
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

		arbiter.getPenetration(&contact.normal, &arbiter.penetration);

		contact.friction    = 0.0f; //! defaultly smooth surface
		contact.restitution = 1.0f; //! defaultly perfectly elastic collision

		contact.hash = hash;

		contacts.push_back(contact);

		LSPE_DEBUG(
			"Collision Test: "
			"begin collision (%d, %d)",
			contact.indices[0],
			contact.indices[1]);
	}

	for (auto &e : contacts)
	{	//! apply collision response
		
	}
}

void Solver::postSolve(float dt)
{
	for (auto body : bodys)
	{
		body->postUpdate();

		int id = static_cast<int>(body->getProperty().userdata);
		bp.moveObject(id, bboxOf(body->getShape()), vec2(0, 0));
	}
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
		}
	}
}
