#define LSPE_IMPL
#define DEBUG

#include <iostream>
#include <iomanip>
#include <string.h>

#include <lspe/common.h>
#include <lspe/shape/shape.hpp>

namespace testunit
{

void mathcompare()
{
	TEST_BEGIN(mathcompare);

	TEST_INFO("TARGET=lspe/base/compare.hpp");

	TEST_EXPR(lspe::equal(0, 0)							);

	TEST_EXPR(lspe::equal(0.0f, 0.0)					);
	TEST_EXPR(lspe::equal(0.0, 0.0)						);

	TEST_EXPR(lspe::equal(5.7913785f, 5.7913785)		);
	TEST_EXPR(lspe::equal(5.7913785, 5.7913785)			);

	TEST_EXPR(lspe::greater(32, 16)						);
	TEST_EXPR(lspe::greater(5.7913785f, 5.7913783)		);
	TEST_EXPR(lspe::greater(5.7913785, 5.7913783)		);

	TEST_EXPR(lspe::less(16, 32)						);
	TEST_EXPR(lspe::less(5.7913783f, 5.7913785)			);
	TEST_EXPR(lspe::less(5.7913783, 5.7913785)			);

	TEST_EXPR(lspe::gte(5.7913785f, 5.7913783)			);
	TEST_EXPR(lspe::gte(5.7913785, 5.7913783)			);

	TEST_EXPR(lspe::lte(5.7913783f, 5.7913785)			);
	TEST_EXPR(lspe::lte(5.7913783, 5.7913785)			);

	TEST_EXPR(lspe::between(2, 1, 3)					);
	TEST_EXPR(lspe::between(2, 1.999f, 2.001f)			);
	TEST_EXPR(lspe::between(2, 1.999, 2.001)			);

	TEST_EXPR(lspe::between(2.375f, 2.37, 2.38)			);
	TEST_EXPR(lspe::between(2.375, 2.37, 2.38)			);
	
	TEST_EXPR(lspe::eqrange(16, 16, 0.5)				);
	TEST_EXPR(lspe::eqrange(5.7913784f, 5.791378, 1e-5)	);
	TEST_EXPR(lspe::eqrange(5.7913784, 5.791378, 1e-5)	);

	TEST_END;
}

void mathfunc()
{
	TEST_BEGIN(mathfunc);

	TEST_INFO("TARGET=lspe/base/math.hpp");

	TEST_EXPR(lspe::equal(lspe::max(2, 1, 3, 6, 3, 313, 3), 313)				);
	TEST_EXPR(lspe::equal(lspe::min(2, 1, 3, 6, 3, 313, 3), 1)					);

	TEST_EXPR(lspe::equal(lspe::sqrt(0.0f), 0)									);
	TEST_EXPR(lspe::equal(lspe::sqrt(0.0), 0)									);

	TEST_EXPR(lspe::equal(lspe::sqrt(1.0f), 1)									);
	TEST_EXPR(lspe::equal(lspe::sqrt(1.0), 1)									);

	TEST_EXPR(lspe::equal(lspe::sqrt(0.0000010351f), 0.0010173986436004f)		);
	TEST_EXPR(lspe::equal(lspe::sqrt(0.0000010351), 0.0010173986436004)			);

	TEST_EXPR(lspe::equal(lspe::sqrt(4), 2)										);
	TEST_EXPR(lspe::equal(lspe::sqrt(4.0f), 2)									);
	TEST_EXPR(lspe::equal(lspe::sqrt(4.0), 2)									);

	TEST_EXPR(lspe::equal(lspe::sqrt(5.0f), 2.23606797749979)					);
	TEST_EXPR(lspe::equal(lspe::sqrt(5.0), 2.23606797749979)					);

	TEST_EXPR(lspe::equal(lspe::sqrt(745318.0f), 863.3180178821707)				);
	TEST_EXPR(lspe::equal(lspe::sqrt(745318.0), 863.3180178821707)				);

	TEST_EXPR(lspe::eqrange(lspe::invsqrt(1.0f), 1.0, 1e-5)						);
	TEST_EXPR(lspe::eqrange(lspe::invsqrt(1.0), 1.0, 1e-5)						);

	TEST_EXPR(lspe::eqrange(lspe::invsqrt(0.0000010351f), 982.89889, 1e-5)		);
	TEST_EXPR(lspe::eqrange(lspe::invsqrt(0.0000010351), 982.89889, 1e-5)		);

	TEST_EXPR(lspe::eqrange(lspe::invsqrt(4), 0.5, 1e-5)						);
	TEST_EXPR(lspe::eqrange(lspe::invsqrt(4.0f), 0.5, 1e-5)						);
	TEST_EXPR(lspe::eqrange(lspe::invsqrt(4.0), 0.5, 1e-5)						);

	TEST_EXPR(lspe::eqrange(lspe::invsqrt(5.0f), 0.44721, 1e-5)					);
	TEST_EXPR(lspe::eqrange(lspe::invsqrt(5.0), 0.44721, 1e-5)					);

	TEST_EXPR(lspe::eqrange(lspe::invsqrt(745318.0f), 0.00116, 1e-5)			);
	TEST_EXPR(lspe::eqrange(lspe::invsqrt(745318.0), 0.00116, 1e-5)				);

	TEST_END;
}

void vecops()
{
	TEST_BEGIN(vecops);

	TEST_INFO("TARGET=lspe/base/vec.hpp");

	TEST_EXPR(lspe::vec2i(3, 6) == lspe::vec2i(3, 6)						);
	TEST_EXPR(lspe::vec2f(3.0f, 6.0f) == lspe::vec2f(3.0f, 6.0f)			);
	TEST_EXPR(lspe::vec2lf(3.0, 6.0) == lspe::vec2lf(3.0, 6.0)				);

	TEST_EXPR((lspe::vec2f)lspe::vec2i(3, 6) == lspe::vec2f(3.0f, 6.0f)		);
	TEST_EXPR((lspe::vec2i)lspe::vec2f(3.0f, 6.0f) == lspe::vec2i(3, 6)		);

	{
		lspe::vec2i v(-1, 1);
		TEST_EXPR(lspe::equal(v.data[0], -1) && lspe::equal(v.data[1], 1)	);
		TEST_EXPR(lspe::equal(v.x, -1) && lspe::equal(v.y, 1)				);
		TEST_EXPR(lspe::equal(v[0], -1) && lspe::equal(v[1], 1)				);
		TEST_EXPR(lspe::equal(v.norm(), lspe::sqrt(2))						);
		
		lspe::vec2i u(3, 7);
		int k = 3;
		TEST_EXPR(lspe::equal(u.dot(v), 4)									);
		TEST_EXPR(lspe::equal(u * k, lspe::vec2i(9, 21))					);
		TEST_EXPR(lspe::equal(u + v, lspe::vec2lf(2, 8))					);
	}

	{
		lspe::vec2f v(-1, 1);
		TEST_EXPR(lspe::equal(v.data[0], -1) && lspe::equal(v.data[1], 1)	);
		TEST_EXPR(lspe::equal(v.x, -1) && lspe::equal(v.y, 1)				);
		TEST_EXPR(lspe::equal(v[0], -1) && lspe::equal(v[1], 1)				);
		TEST_EXPR(lspe::equal(v.norm(), lspe::sqrt(2))						);
		
		lspe::vec2f u(3, 7);
		float k = 3.0f;
		TEST_EXPR(lspe::equal(u.dot(v), 4)									);
		TEST_EXPR(lspe::equal(u * k, lspe::vec2f(9.0f, 21.0f))				);
		TEST_EXPR(lspe::equal(u + v, lspe::vec2lf(2.0f, 8.0f))				);
	}

	{
		lspe::vec2lf v(-1, 1);
		TEST_EXPR(lspe::equal(v.data[0], -1) && lspe::equal(v.data[1], 1)	);
		TEST_EXPR(lspe::equal(v.x, -1) && lspe::equal(v.y, 1)				);
		TEST_EXPR(lspe::equal(v[0], -1) && lspe::equal(v[1], 1)				);
		TEST_EXPR(lspe::equal(v.norm(), lspe::sqrt(2))						);
		
		lspe::vec2lf u(3, 7);
		double k = 3.0;
		TEST_EXPR(lspe::equal(u.dot(v), 4)									);
		TEST_EXPR(lspe::equal(u * k, lspe::vec2lf(9.0, 21.0))				);
		TEST_EXPR(lspe::equal(u + v, lspe::vec2lf(2.0, 8.0))				);
	}

	{
		lspe::vec3i v(-1, 1, 16);
		TEST_EXPR(lspe::equal(v.data[0], -1)
			&& lspe::equal(v.data[1], 1)
			&& lspe::equal(v.data[2], 16)									);
		TEST_EXPR(lspe::equal(v.x, -1)
			&& lspe::equal(v.y, 1)
			&& lspe::equal(v.z, 16)											);
		TEST_EXPR(lspe::equal(v[0], -1)
			&& lspe::equal(v[1], 1)
			&& lspe::equal(v[2], 16)										);
		TEST_EXPR(lspe::equal(v.norm(), lspe::sqrt(258))					);
	}

	{
		lspe::vec3f v(-1, 1, 16);
		TEST_EXPR(lspe::equal(v.data[0], -1)
			&& lspe::equal(v.data[1], 1)
			&& lspe::equal(v.data[2], 16)									);
		TEST_EXPR(lspe::equal(v.x, -1)
			&& lspe::equal(v.y, 1)
			&& lspe::equal(v.z, 16)											);
		TEST_EXPR(lspe::equal(v[0], -1)
			&& lspe::equal(v[1], 1)
			&& lspe::equal(v[2], 16)										);
		TEST_EXPR(lspe::equal(v.norm(), lspe::sqrt(258))					);
	}

	{
		lspe::vec3lf v(-1, 1, 16);
		TEST_EXPR(lspe::equal(v.data[0], -1)
			&& lspe::equal(v.data[1], 1)
			&& lspe::equal(v.data[2], 16)									);
		TEST_EXPR(lspe::equal(v.x, -1)
			&& lspe::equal(v.y, 1)
			&& lspe::equal(v.z, 16)											);
		TEST_EXPR(lspe::equal(v[0], -1)
			&& lspe::equal(v[1], 1)
			&& lspe::equal(v[2], 16)										);
		TEST_EXPR(lspe::equal(v.norm(), lspe::sqrt(258))					);
	}

	{
		lspe::vec4i v(-1, 1, 16, -8);
		TEST_EXPR(lspe::equal(v.data[0], -1)
			&& lspe::equal(v.data[1], 1)
			&& lspe::equal(v.data[2], 16)
			&& lspe::equal(v.data[3], -8)									);
		TEST_EXPR(lspe::equal(v.x, -1)
			&& lspe::equal(v.y, 1)
			&& lspe::equal(v.z, 16)
			&& lspe::equal(v.w, -8)											);
		TEST_EXPR(lspe::equal(v[0], -1)
			&& lspe::equal(v[1], 1)
			&& lspe::equal(v[2], 16)
			&& lspe::equal(v[3], -8)										);
		TEST_EXPR(lspe::equal(v.norm(), lspe::sqrt(322))					);
	}

	{
		lspe::vec4f v(-1, 1, 16, -8);
		TEST_EXPR(lspe::equal(v.data[0], -1)
			&& lspe::equal(v.data[1], 1)
			&& lspe::equal(v.data[2], 16)
			&& lspe::equal(v.data[3], -8)									);
		TEST_EXPR(lspe::equal(v.x, -1)
			&& lspe::equal(v.y, 1)
			&& lspe::equal(v.z, 16)
			&& lspe::equal(v.w, -8)											);
		TEST_EXPR(lspe::equal(v[0], -1)
			&& lspe::equal(v[1], 1)
			&& lspe::equal(v[2], 16)
			&& lspe::equal(v[3], -8)										);
		TEST_EXPR(lspe::equal(v.norm(), lspe::sqrt(322))					);
	}

	{
		lspe::vec4lf v(-1, 1, 16, -8);
		TEST_EXPR(lspe::equal(v.data[0], -1)
			&& lspe::equal(v.data[1], 1)
			&& lspe::equal(v.data[2], 16)
			&& lspe::equal(v.data[3], -8)									);
		TEST_EXPR(lspe::equal(v.x, -1)
			&& lspe::equal(v.y, 1)
			&& lspe::equal(v.z, 16)
			&& lspe::equal(v.w, -8)											);
		TEST_EXPR(lspe::equal(v[0], -1)
			&& lspe::equal(v[1], 1)
			&& lspe::equal(v[2], 16)
			&& lspe::equal(v[3], -8)										);
		TEST_EXPR(lspe::equal(v.norm(), lspe::sqrt(322))					);
	}

	TEST_END;
}

void lineops()
{
	TEST_BEGIN(lineops);

	TEST_INFO("TARGET=lspe/base/line.hpp");

	using namespace lspe::shape;

	lspe::vec2lf O(0.0, 0.0), E(1.0, 0.0), W(-1.0, 0.0),
		N(0.0, 1.0), S(0.0, -1.0);

	LSegment2lf
		o_n_seg(O, N),
		o_e_seg(O, E),
		o_sw_seg(O, S + W),
		sw_o_seg(S + W, O),
		ne_sw_seg(N + E, S + W),
		sw_ne_seg(S + W, N + E);

	LRay2lf
		o_n_ray(O, N),
		o_e_ray(O, E),
		o_sw_ray(O, S + W),
		sw_o_ray(S + W, O),
		ne_sw_ray(N + E, S + W),
		sw_ne_ray(S + W, N + E);

	LStraight2lf
		o_n_str(O, N),
		o_e_str(O, E),
		o_sw_str(O, S + W),
		sw_o_str(S + W, O),
		ne_sw_str(N + E, S + W),
		sw_ne_str(S + W, N + E);

	TEST_EXPR(o_e_seg.parallel(o_e_seg));
	TEST_EXPR(o_e_seg.parallel(o_e_ray));
	TEST_EXPR(o_e_seg.parallel(o_e_str));

	TEST_EXPR(o_e_ray.parallel(o_e_seg));
	TEST_EXPR(o_e_ray.parallel(o_e_ray));
	TEST_EXPR(o_e_ray.parallel(o_e_str));

	TEST_EXPR(o_e_str.parallel(o_e_seg));
	TEST_EXPR(o_e_str.parallel(o_e_ray));
	TEST_EXPR(o_e_str.parallel(o_e_str));

	TEST_EXPR(!o_e_seg.parallel(o_n_seg));
	TEST_EXPR(!o_e_seg.parallel(o_n_ray));
	TEST_EXPR(!o_e_seg.parallel(o_n_str));

	TEST_EXPR(!o_e_ray.parallel(o_n_seg));
	TEST_EXPR(!o_e_ray.parallel(o_n_ray));
	TEST_EXPR(!o_e_ray.parallel(o_n_str));

	TEST_EXPR(!o_e_str.parallel(o_n_seg));
	TEST_EXPR(!o_e_str.parallel(o_n_ray));
	TEST_EXPR(!o_e_str.parallel(o_n_str));

	TEST_EXPR(!o_e_seg.intersect(o_e_seg));
	TEST_EXPR(!o_e_seg.intersect(o_e_ray));
	TEST_EXPR(!o_e_seg.intersect(o_e_str));

	TEST_EXPR(!o_e_ray.intersect(o_e_seg));
	TEST_EXPR(!o_e_ray.intersect(o_e_ray));
	TEST_EXPR(!o_e_ray.intersect(o_e_str));

	TEST_EXPR(o_e_str.intersect(o_n_seg));
	TEST_EXPR(o_e_str.intersect(o_n_ray));
	TEST_EXPR(o_e_str.intersect(o_n_str));

	TEST_EXPR(o_e_seg.intersect(o_n_seg));
	TEST_EXPR(o_e_seg.intersect(o_n_ray));
	TEST_EXPR(o_e_seg.intersect(o_n_str));

	TEST_EXPR(o_e_ray.intersect(o_n_seg));
	TEST_EXPR(o_e_ray.intersect(o_n_ray));
	TEST_EXPR(o_e_ray.intersect(o_n_str));

	TEST_EXPR(o_e_str.intersect(o_n_seg));
	TEST_EXPR(o_e_str.intersect(o_n_ray));
	TEST_EXPR(o_e_str.intersect(o_n_str));

	TEST_END;
}

void polyGJK()
{
	TEST_BEGIN(polyGJK);

	TEST_INFO("TARGET=lspe/base/polygen.hpp");

	using namespace lspe::shape::details;
	using Polygen = lspe::shape::Polygen<float>;
	using lspe::vec2f;
	using v2array = lspe::array<vec2f>;

	v2array tri { {0, 0}, {1, 0}, {0.5, 0.5} };

	TEST_EXPR(tri_contain_vec2(tri, vec2f(0.3f, 0.2f)));
	TEST_EXPR(tri_contain_vec2(tri, vec2f(0.5f, 0.5f)));
	TEST_EXPR(!tri_contain_vec2(tri, vec2f(1.7f, 3.9f)));

	float vertex_1[] { -1, -1, -1, 1, 1, 1, 1, -1 };
	float vertex_2[] { 0, 1, 1, 0, 2, 1, 1, 2 };
	float vertex_3[] { 0, 0.5, 0.5, 0.5, 0, 0 };
	float vertex_4[] { 14, 3, 13, 53, 7, 7 };
	float vertex_5[] { 0.5, 0.5, 2, 0.5, 4, 6, 7, 7, 6, 4, 0.5, 2 };
	float vertex_6[] { 0, 0.6, 0.6, 0.6, 0, 0 };

	Polygen rect_1(vertex_1, 4);
	Polygen rect_2(vertex_2, 4);
	Polygen rect_3(vertex_3, 3);
	Polygen rect_4(vertex_4, 3);
	Polygen rect_5(vertex_5, 6);
	Polygen rect_6(vertex_6, 3);

	TEST_EXPR(rect_1.overlap(rect_2));
	TEST_EXPR(rect_1.overlap(rect_3));
	TEST_EXPR(rect_5.overlap(rect_1));
	TEST_EXPR(rect_5.overlap(rect_2));
	TEST_EXPR(rect_5.overlap(rect_3));
	TEST_EXPR(!rect_5.overlap(rect_4));
	TEST_EXPR(rect_5.overlap(rect_5));
	TEST_EXPR(rect_5.overlap(rect_6));
	TEST_EXPR(!rect_1.overlap(rect_4));
	TEST_EXPR(!rect_2.overlap(rect_4));
	TEST_EXPR(!rect_3.overlap(rect_4));

	TEST_END;
}

void lspeid()
{
	TEST_BEGIN(shapeops);

	TEST_INFO("TARGET=lspe/base/shape.hpp polygen.hpp circle.hpp");

	using lspe::shape::Shape;
	using lspe::shape::Polygen;
	using lspe::shape::Circle;

	float v[] = { 0, 0, 0, 1, 1, 0 };

	Circle<float> circle(lspe::vec2f(0, 0), 3);
	Polygen<float> poly(v, 3);

	TEST_EXPR(strcmp(poly.id(), "lspe::shape::Polygen") == 0				);
	TEST_EXPR(strcmp(circle.id(), "lspe::shape::Circle") == 0				);
	TEST_EXPR(strcmp(((Shape*)&poly)->id(), "lspe::shape::Polygen") == 0	);
	TEST_EXPR(strcmp(((Shape*)&circle)->id(), "lspe::shape::Circle") == 0	);

	TEST_END;
}

void bbox2d()
{
	TEST_BEGIN(bbox2d);

	

	TEST_END;
}

};

int main(int argc, char const *argv[])
{

	testunit::mathcompare();
	testunit::mathfunc();
	testunit::vecops();
	testunit::lineops();
	testunit::polyGJK();
	testunit::lspeid();
	testunit::bbox2d();

	return 0;
}