#pragma once

/********************************
 *  @author: ZYmelaii
 *
 *  @collision: intro for collision
 *
 *  @brief: use GJK + EPA / MPR algorithm
 *
 *  @NOTES: 
 *******************************/

#include "base/base.h"
#include "base/vec.h"
#include "base/mat.h"
#include "shape.h"

namespace lspe
{

class Arbiter;
class Collider;

namespace collision
{

//! customizable support function for GJK/EPA/MPR algorithm
//! it should return the maximum point of the projection
//! of the shape in the given direction
//! you should assume that the shape is of expected type
//! there will be no redundant checks inside the algorithm
//! tip: Shape is a pointer pointed to real shape object
typedef vec2 (*fnsupport)(Shape x, const vec2 &direction);

//! this is an extended version of fnsupport
//! it allows more given params to spawn a vec2
typedef vec2 (*fnsupport2)(Shape x, Shape y, const vec2 &v, void *extra);

vec2 supportLine    (Shape x, const vec2 &direction); //$ DONE
vec2 supportCircle  (Shape x, const vec2 &direction); //$ DONE
vec2 supportPolygen (Shape x, const vec2 &direction); //$ DONE
vec2 supportEllipse (Shape x, const vec2 &direction); //$ DONE
vec2 supportBezier2 (Shape x, const vec2 &direction);
vec2 supportBezier3 (Shape x, const vec2 &direction);

};

class Arbiter
{
public:

protected:

private:

};

class Collider
{
public:
	Collider();

	//! test collision
	bool collided();

	//! collect more details about the collision
	//! call this only when collided() == true
	Arbiter getArbiter() const;

public:

	//! note: functions below must be called one by one
	//! before you start the collision test

	//! decide the two shapes to test collision
	void setTestPair(Shape a, Shape b);

	//! bind support function for (a, b)
	void bindSupports(
		collision::fnsupport support1,
		collision::fnsupport support2);

	//! bind generator to decide the first direction
	//! it is wanted by GJK/EPA algorithm
	void bindInitialGenerator(
		collision::fnsupport2 generator);

	//! bind an extra pointer to Collider
	//! it can be used within any callback function
	//! whose type is fnsupport2 or Collider::fndebug
	void bindExtraData(void *extra);

	//! clear status collision test
	//! call it before you perform a second test
	void reset();

private:
	//! determine whether the generated simplex contains the origin
	//! wanted by collided()
	bool containOrigin(vec2 &direction, vec2 *simplex, int &n);

private:
	Shape shapes[2];
	collision::fnsupport2 getfirstdirection;
	collision::fnsupport support[2];
	void *extra;

	//! this is setted for collided()
	vec2 simplex[3];

	//! mark whether Collider has performed the collision test
	//! only when tested, getArbiter() is allowed
	bool tested;
	bool iscollided;

	//! it will decide whether a collision test is allowed
	//! note: available only when DEBUG was defined
	int flag;

#ifdef DEBUG

#endif

};

};