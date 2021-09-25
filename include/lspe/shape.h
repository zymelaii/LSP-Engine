#pragma once

#include <vector>

#include "../lspe/base/base.h"
#include "../lspe/base/vec.h"
#include "../lspe/base/mat.h"
#include "../lspe/bbox.h"

namespace lspe
{

enum class ShapeType
{
	eNull = -1,
	eLine,
	eCircle,
	ePolygen,
	eEllipse,
	eBezier2,
	eBezier3,
	eUserType
};

struct Shape
{
	void     *data;
	ShapeType type;
};

namespace shape
{

struct Line;
struct Circle;
struct Polygen; //! convex polygen
struct Ellipse;
struct Bezier2;
struct Bezier3;

};

float areaOf(const shape::Circle  &x);
float areaOf(const shape::Polygen &x);
float areaOf(const shape::Ellipse &x);

vec2 centroidOf(const shape::Line    &x);
vec2 centroidOf(const shape::Circle  &x);
vec2 centroidOf(const shape::Polygen &x);
vec2 centroidOf(const shape::Ellipse &x);
vec2 centroidOf(const shape::Bezier2 &x);
vec2 centroidOf(const shape::Bezier3 &x);

bbox2 bboxOf(const shape::Line    &x);
bbox2 bboxOf(const shape::Circle  &x);
bbox2 bboxOf(const shape::Polygen &x);
bbox2 bboxOf(const shape::Ellipse &x);
bbox2 bboxOf(const shape::Bezier2 &x);
bbox2 bboxOf(const shape::Bezier3 &x);

void translate(shape::Line    &x, const vec2 &displacement);
void translate(shape::Circle  &x, const vec2 &displacement);
void translate(shape::Polygen &x, const vec2 &displacement);
void translate(shape::Ellipse &x, const vec2 &displacement);
void translate(shape::Bezier2 &x, const vec2 &displacement);
void translate(shape::Bezier3 &x, const vec2 &displacement);

void doRotation(shape::Line    &x, float rotation);
void doRotation(shape::Circle  &x, float rotation);
void doRotation(shape::Polygen &x, float rotation);
void doRotation(shape::Ellipse &x, float rotation);
void doRotation(shape::Bezier2 &x, float rotation);
void doRotation(shape::Bezier3 &x, float rotation);

void doRotation(shape::Line    &x, const mat2x2 &mat_rotation);
void doRotation(shape::Circle  &x, const mat2x2 &mat_rotation);
void doRotation(shape::Polygen &x, const mat2x2 &mat_rotation);
void doRotation(shape::Ellipse &x, const mat2x2 &mat_rotation);
void doRotation(shape::Bezier2 &x, const mat2x2 &mat_rotation);
void doRotation(shape::Bezier3 &x, const mat2x2 &mat_rotation);

shape::Line    rotationOf(const shape::Line    &x, float rotation);
shape::Circle  rotationOf(const shape::Circle  &x, float rotation);
shape::Polygen rotationOf(const shape::Polygen &x, float rotation);
shape::Ellipse rotationOf(const shape::Ellipse &x, float rotation);
shape::Bezier2 rotationOf(const shape::Bezier2 &x, float rotation);
shape::Bezier3 rotationOf(const shape::Bezier3 &x, float rotation);

shape::Line    rotationOf(const shape::Line    &x, const mat2x2 &mat_rotation);
shape::Circle  rotationOf(const shape::Circle  &x, const mat2x2 &mat_rotation);
shape::Polygen rotationOf(const shape::Polygen &x, const mat2x2 &mat_rotation);
shape::Ellipse rotationOf(const shape::Ellipse &x, const mat2x2 &mat_rotation);
shape::Bezier2 rotationOf(const shape::Bezier2 &x, const mat2x2 &mat_rotation);
shape::Bezier3 rotationOf(const shape::Bezier3 &x, const mat2x2 &mat_rotation);

bool contain(const shape::Circle  &a, const vec2 &b);
bool contain(const shape::Polygen &a, const vec2 &b);
bool contain(const shape::Ellipse &a, const vec2 &b);

float inertiaOf(const shape::Circle  &a, float mass);
float inertiaOf(const shape::Polygen &a, float mass);
float inertiaOf(const shape::Ellipse &a, float mass);

namespace shape
{

struct Line
{
	vec2 pa;
	vec2 pb;      //! line: pa->pb
	int type;     //! 0 stands for line segment
	              //! 1 stands for ray
	              //! 2 stands for straight line
	float anchor; //! k := anchor -> rotate anchor = pa*k+pb*(1-k)
};

struct Circle
{
	vec2 center;
	float r;
};

struct Polygen
{
	vec2 center;
	std::vector<vec2> vertices;
};

struct Ellipse
{
	vec2 center;
	float rotation;
	float rx;
	float ry;
};

struct Bezier2
{
	vec2 P[3];    //! B(t;2)=(1-t)^2*P0+2t(1-t)*P1+t^2*P2 (0<=t<=1)
	float step;   //! 0 < step <= 1
	float anchor; //! rotate anchor = B(anchor;2)
};

struct Bezier3
{
	vec2 P[4];    //! B(t;3)=(1-t)^3*P0+3t(1-t)^2*P1+3t^2(1-t)*P2+t^3*P3 (0<=t<=1)
	float step;   //! 0 < step <= 1
	float anchor; //! rotate anchor = B(anchor;2)
};

};

};
