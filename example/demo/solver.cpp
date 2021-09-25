#include "solver.h"

using namespace lspe::shape;

Solver::Solver(float _step)
	: bodys(16), contacts(16), step(_step)
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
	bp.addObject(box, body);
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
	bp.addObject(box, body);
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
	bp.addObject(box, body);
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
	
}

void Solver::postSolve(float dt)
{
	for (auto body : bodys)
	{
		body->postUpdate();
	}
}
