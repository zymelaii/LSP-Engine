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

Line rotationOf(float rotation, const Line &x)
{
	LSPE_ASSERT(x.type >= 0 && x.type <= 2);
	LSPE_ASSERT(!(x.pa == x.pb));
	mat2x2 mat_rotation = getRotateMatrix(rotation);
	return rotationOf(mat_rotation, x);
}

Line rotationOf(const mat2x2 &mat_rotation, const Line &x)
{
	LSPE_ASSERT(x.type >= 0 && x.type <= 2);
	LSPE_ASSERT(!(x.pa == x.pb));
	vec2 center = centroidOf(x);
	vec2 p[2];
	p[0] = mat_rotation * (x.pa - center) + center;
	p[1] = mat_rotation * (x.pb - center) + center;
	return { p[0], p[1], x.type, x.anchor };
}

};
