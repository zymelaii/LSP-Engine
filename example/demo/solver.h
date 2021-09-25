#pragma once

#include <vector>

#include <lspe/lspe.h>

using namespace lspe;

//! an alternative of lspe::ContactInfo
struct DemoContactEdge;
struct DemoContact;

struct DemoContactEdge
{
	RigidBody *other;
	DemoContact * contact;
};

struct DemoContact
{
	DemoContactEdge node[2];

	int   indices[2];  //! for accessment to abtree

	vec2  normal;      //! get from Arbiter
	float penetration; //! get from Arbiter

	float friction;    //! coefficient of friction
	float restitution; //! coefficient of restitution
};

class Solver
{
public:
	Solver() = delete;
	Solver(float _step = 0.1f);
	~Solver();

	void newCircleBody(const vec2 &center, float r);
	[[deprecated]] void newPolygen(); //! randomly generated
	void newEllipseBody(const vec2 &center, float rx, float ry);
	void newTriangleBody(const vec2 &a, const vec2 &b, const vec2 &c);
	void newRectangleBody(bbox2 rect);

	void  preSolve(float dt);
	void   inSolve(float dt);
	void postSolve(float dt);

protected:

private:
	BroadPhase bp;

	Arbiter    arbiter;
	Collider   collider;

	std::vector<RigidBody*>  bodys;
	std::vector<DemoContact> contacts;

	float step;

};
