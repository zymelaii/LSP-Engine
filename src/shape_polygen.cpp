#include <lspe/shape.h>

namespace lspe
{

using shape::Polygen;

float areaOf(const Polygen &x)
{
	LSPE_ASSERT(x.vertices.size() >= 3);
	float area = 0.0f;
	for (int i = 2; i < x.vertices.size(); ++i)
	{
		vec2 a = x.vertices[i] - x.vertices[i - 1];
		vec2 b = x.vertices[i] - x.vertices[i - 2];
		area += fabs(cross(a, b));
	}
	return area * 0.5f;
}

vec2 centroidOf(const Polygen &x)
{
	LSPE_ASSERT(x.vertices.size() >= 3);
	vec2 c = x.vertices[0];
	for (int i = 1; i < x.vertices.size(); ++i)
	{
		c += x.vertices[i];
	}
	return (c /= x.vertices.size());
}

bbox2 bboxOf(const Polygen &x)
{
	LSPE_ASSERT(x.vertices.size() >= 3);
	vec2 lower = x.vertices[0], upper = x.vertices[0];
	for (int i = 1; i < x.vertices.size(); ++i)
	{
		const vec2 &p = x.vertices[i];
		if (p.x < lower.x)
		{
			lower.x = p.x;
		} else if (p.x > upper.x)
		{
			upper.x = p.x;
		}
		if (p.y < lower.y)
		{
			lower.y = p.y;
		} else if (p.y > upper.y)
		{
			upper.y = p.y;
		}
	}
	return { lower, upper };
}

Polygen rotationOf(float rotation, const Polygen &x)
{
	LSPE_ASSERT(x.vertices.size() >= 3);
	mat2x2 mat_rotation = getRotateMatrix(rotation);
	return rotationOf(mat_rotation, x);
}

Polygen rotationOf(const mat2x2 &mat_rotation, const Polygen &x)
{
	LSPE_ASSERT(x.vertices.size() >= 3);
	std::vector<vec2> new_vertices(x.vertices.size());
	for (int i = 0; i < x.vertices.size(); ++i)
	{
		new_vertices[i] = mat_rotation * (x.vertices[i] - x.center) + x.center;
	}
	return { x.center, new_vertices };
}

bool contain(const Polygen &a, const vec2 &b)
{
	LSPE_ASSERT(a.vertices.size() >= 3);

	vec2 p = a.vertices[0];

	vec2 ao = -p;
	vec2 v = b + ao;

	for (int i = 2; i < a.vertices.size(); ++i)
	{
		vec2 v1 = ao + a.vertices[i - 1];
		vec2 v2 = ao + a.vertices[i];

		vec2 perp1 = triproduct(v2, v1, v1);
		vec2 perp2 = triproduct(v1, v2, v2);

		if (dot(v, perp1) <= FLT_EPSILON
			&& dot(v, perp2) <= FLT_EPSILON)
		{
			return true;
		}
	}

	return false;
}

};
