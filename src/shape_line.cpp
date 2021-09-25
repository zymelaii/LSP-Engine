#include <lspe/shape.h>

namespace lspe
{

using shape::Line;

vec2 centroidOf(const Line &x)
{
	LSPE_ASSERT(x.type >= 0 && x.type <= 2);
	LSPE_ASSERT(!(x.pa == x.pb));
	return x.pa * x.anchor + x.pb * (1 - x.anchor);
}

bbox2 bboxOf(const Line &x)
{
	LSPE_ASSERT(x.type >= 0 && x.type <= 2);
	LSPE_ASSERT(!(x.pa == x.pb));
	vec2 lower(min(x.pa.x, x.pb.x), min(x.pa.y, x.pb.y));
	vec2 upper(max(x.pa.x, x.pb.x), max(x.pa.y, x.pb.y));
	return { lower, upper };
}

void translate(Line &x, const vec2 &displacement)
{
	x.pa += displacement;
	x.pb += displacement;
}

void doRotation(Line &x, float rotation)
{
	LSPE_ASSERT(x.type >= 0 && x.type <= 2);
	LSPE_ASSERT(!(x.pa == x.pb));

	mat2x2 mat_rotation = getRotateMatrix(rotation);
	doRotation(x, mat_rotation);
}

void doRotation(Line &x, const mat2x2 &mat_rotation)
{
	vec2 center = centroidOf(x);
	x.pa = mat_rotation * (x.pa - center) + center;
	x.pb = mat_rotation * (x.pb - center) + center;
}

Line rotationOf(const Line &x, float rotation)
{
	LSPE_ASSERT(x.type >= 0 && x.type <= 2);
	LSPE_ASSERT(!(x.pa == x.pb));

	mat2x2 mat_rotation = getRotateMatrix(rotation);

	Line newLine(x);

	doRotation(newLine, mat_rotation);

	return newLine;
}

Line rotationOf(const Line &x, const mat2x2 &mat_rotation)
{
	LSPE_ASSERT(x.type >= 0 && x.type <= 2);
	LSPE_ASSERT(!(x.pa == x.pb));

	Line newLine(x);

	doRotation(newLine, mat_rotation);

	return newLine;
}

};
