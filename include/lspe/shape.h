#pragma once

#include <vector>

#include "../lspe/base/base.h"
#include "../lspe/base/vec.h"
#include "../lspe/base/mat.h"
#include "../lspe/bbox.h"

namespace lspe
{

typedef void* Shape;

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

shape::Line    rotationOf(float rotation, const shape::Line    &x);
shape::Circle  rotationOf(float rotation, const shape::Circle  &x);
shape::Polygen rotationOf(float rotation, const shape::Polygen &x);
shape::Ellipse rotationOf(float rotation, const shape::Ellipse &x);
shape::Bezier2 rotationOf(float rotation, const shape::Bezier2 &x);
shape::Bezier3 rotationOf(float rotation, const shape::Bezier3 &x);

shape::Line    rotationOf(const mat2x2 &mat_rotation, const shape::Line    &x);
shape::Circle  rotationOf(const mat2x2 &mat_rotation, const shape::Circle  &x);
shape::Polygen rotationOf(const mat2x2 &mat_rotation, const shape::Polygen &x);
shape::Ellipse rotationOf(const mat2x2 &mat_rotation, const shape::Ellipse &x);
shape::Bezier2 rotationOf(const mat2x2 &mat_rotation, const shape::Bezier2 &x);
shape::Bezier3 rotationOf(const mat2x2 &mat_rotation, const shape::Bezier3 &x);

bool contain(const shape::Circle  &a, const vec2 &b);
bool contain(const shape::Polygen &a, const vec2 &b);
bool contain(const shape::Ellipse &a, const vec2 &b);

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
