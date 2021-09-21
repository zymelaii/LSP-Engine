#include <lspe/shape.h>

namespace lspe
{

using shape::Bezier2;
using shape::Bezier3;

vec2 centroidOf(const Bezier2 &x)
{
	float k[3];
	k[0] = (1 - x.anchor) * (1 - x.anchor);
	k[1] = 2 * x.anchor * (1 - x.anchor);
	k[2] = x.anchor * x.anchor;
	return x.P[0] * k[0] + x.P[1] * k[1] + x.P[2] * k[2];
}

bbox2 bboxOf(const Bezier2 &x)
{
	vec2 lower, upper;
	lower.x = min(x.P[0].x, x.P[1].x, x.P[2].x);
	lower.y = min(x.P[0].y, x.P[1].y, x.P[2].y);
	upper.x = max(x.P[0].x, x.P[1].x, x.P[2].x);
	upper.y = max(x.P[0].y, x.P[1].y, x.P[2].y);
	return { lower, upper };
}

Bezier2 rotationOf(float rotation, const Bezier2 &x)
{
	mat2x2 mat_rotation = getRotateMatrix(rotation);
	return rotationOf(mat_rotation, x);
}

Bezier2 rotationOf(const mat2x2 &mat_rotation, const Bezier2 &x)
{
	vec2 center = centroidOf(x);
	vec2 P[3];
	P[0] = mat_rotation * (x.P[0] - center) + center;
	P[1] = mat_rotation * (x.P[1] - center) + center;
	P[2] = mat_rotation * (x.P[2] - center) + center;
	return { P[0], P[1], P[2], x.step, x.anchor };
}

vec2 centroidOf(const Bezier3 &x)
{
	float k[4];
	k[0] = (1 - x.anchor) * (1 - x.anchor) * (1 - x.anchor);
	k[1] = 3 * x.anchor * (1 - x.anchor) * (1 - x.anchor);
	k[2] = 3 * x.anchor * x.anchor * (1 - x.anchor);
	k[3] = x.anchor * x.anchor * x.anchor;
	return x.P[0] * k[0] + x.P[1] * k[1]
		+ x.P[2] * k[2] + x.P[3] * k[3];
}

bbox2 bboxOf(const Bezier3 &x)
{
	vec2 lower, upper;
	lower.x = min(x.P[0].x, x.P[1].x, x.P[2].x, x.P[3].x);
	lower.y = min(x.P[0].y, x.P[1].y, x.P[2].y, x.P[3].y);
	upper.x = max(x.P[0].x, x.P[1].x, x.P[2].x, x.P[3].x);
	upper.y = max(x.P[0].y, x.P[1].y, x.P[2].y, x.P[3].y);
	return { lower, upper };
}

Bezier3 rotationOf(float rotation, const Bezier3 &x)
{
	mat2x2 mat_rotation = getRotateMatrix(rotation);
	return rotationOf(mat_rotation, x);
}

Bezier3 rotationOf(const mat2x2 &mat_rotation, const Bezier3 &x)
{
	vec2 center = centroidOf(x);
	vec2 P[4];
	P[0] = mat_rotation * (x.P[0] - center) + center;
	P[1] = mat_rotation * (x.P[1] - center) + center;
	P[2] = mat_rotation * (x.P[2] - center) + center;
	P[3] = mat_rotation * (x.P[3] - center) + center;
	return { P[0], P[1], P[2], P[3], x.step, x.anchor };
}

};
