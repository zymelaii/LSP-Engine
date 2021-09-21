#include <lspe/shape.h>

namespace lspe
{

using shape::Circle;

float areaOf(const Circle &x)
{
	LSPE_ASSERT(x.r > 0);
	return Pi * x.r * x.r;
}

vec2 centroidOf(const Circle &x)
{
	LSPE_ASSERT(x.r > 0);
	return x.center;
}

bbox2 bboxOf(const Circle &x)
{
	LSPE_ASSERT(x.r > 0);
	return { x.center - x.r, x.center + x.r };
}

Circle rotationOf(float rotation, const Circle &x)
{
	LSPE_ASSERT(x.r > 0);
	return x;
}

Circle rotationOf(const mat2x2 &mat_rotation, const Circle &x)
{
	LSPE_ASSERT(x.r > 0);
	return x;
}

bool contain(const Circle &a, const vec2 &b)
{
	vec2 c = b - a.center;
	return dot(c, c) < a.r * a.r;
}

};
