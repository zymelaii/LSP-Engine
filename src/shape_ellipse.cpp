#include <lspe/shape.h>

namespace lspe
{

using shape::Ellipse;

float areaOf(const Ellipse &x)
{
	LSPE_ASSERT(x.rx > 0 && x.ry > 0);
	return Pi * x.rx * x.ry;
}

vec2 centroidOf(const Ellipse &x)
{
	LSPE_ASSERT(x.rx > 0 && x.ry > 0);
	return x.center;
}

bbox2 bboxOf(const Ellipse &x)
{	//@Reference: https://www.iquilezles.org/www/articles/ellipses/ellipses.htm
	LSPE_ASSERT(x.rx > 0 && x.ry > 0);
	mat2x2 mat_rotation = getRotateMatrix(x.rotation);
	vec2 u = mat_rotation * vec2(x.rx, 0);
	vec2 v = mat_rotation * vec2(0, x.ry);
	vec2 e(sqrt(u.x * u.x + v.x * v.x), sqrt(u.y * u.y + v.y * v.y));
	return { x.center - e, x.center + e };
}

void translate(Ellipse &x, const vec2 &displacement)
{
	x.center += displacement;
}

Ellipse rotationOf(float rotation, const Ellipse &x)
{
	LSPE_ASSERT(x.rx > 0 && x.ry > 0);
	return { x.center, rotation, x.rx, x.ry };
}

Ellipse rotationOf(const mat2x2 &mat_rotation, const Ellipse &x)
{
	LSPE_ASSERT(x.rx > 0 && x.ry > 0);
	float rotation = acos(mat_rotation[0][0]);
	return { x.center, rotation, x.rx, x.ry };
}

bool contain(const Ellipse &a, const vec2 &b)
{
	mat2x2 mat_rotation = getRotateMatrix(-a.rotation);
	vec2 c = mat_rotation * (b - a.center);
	return c.x * c.x / (a.rx * a.rx)
		+ c.y * c.y / (a.ry * a.ry) < 1;
}

//! I'm not sure whether the computation is right @ZYMelaii
float inertiaOf(const Ellipse &a, float mass)
{
	LSPE_ASSERT(mass > 0);
	return mass * (a.rx * a.ry * 0.5 + dot(a.center, a.center));
}

};
