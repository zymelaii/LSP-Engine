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

	vec2 crossPointFromCentroid[2];
};

struct DemoContact
{
	DemoContactEdge node[2];

	int indices[2];    //! for accessment to abtree

	//! we assume that cost time of collision response
	//! is extremly short
	//! so no modification would be applied to
	//! penetration vector

	vec2  normal;      //! get from Arbiter
	float penetration; //! get from Arbiter

	const float maxPenetration = 0.02f;

	float friction;    //! coefficient of friction
	float restitution; //! coefficient of restitution

	struct
	{
		float normal;
		float tangent;
	} force;           //! seperated by normal

	uint32_t hash;     //! to check if two contacts is the same
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

	void  preSolve(float dt); //! preprocess physics behavior
	void   inSolve(float dt); //! solve complex physics phenomenon
	                          //! exactly collision response in current version
	void postSolve(float dt); //! apply all the results

	static vec2 centerOf(Shape shape);

protected:
	static collision::fnsupport getDefaultSupport(ShapeType type);

private:
	BroadPhase bp;

	Arbiter  arbiter;
	Collider collider;

	//! RigidBody pointer may be used in other field
	//! indices of RigidBody are expected to be of increasing order
	std::vector<RigidBody*>  bodys;

	//! contacts stored all the onCollsion bodys
	//! in current version, we use default contact attributes
	//! (except for penetration vector)
	std::vector<DemoContact> contacts;

	float step;

};
