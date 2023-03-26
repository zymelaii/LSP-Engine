#include <lspe/shape.h>

namespace lspe {

using shape::Circle;

float areaOf(const Circle &x) {
    LSPE_ASSERT(x.r > 0);
    return Pi * x.r * x.r;
}

vec2 centroidOf(const Circle &x) {
    LSPE_ASSERT(x.r > 0);
    return x.center;
}

bbox2 bboxOf(const Circle &x) {
    LSPE_ASSERT(x.r > 0);
    return {x.center - x.r, x.center + x.r};
}

void translate(Circle &x, const vec2 &displacement) {
    x.center += displacement;
}

void doRotation(Circle &x, float rotation) {}

void doRotation(Circle &x, const mat2x2 &mat_rotation) {}

Circle rotationOf(const Circle &x, float rotation) {
    LSPE_ASSERT(x.r > 0);

    return x;
}

Circle rotationOf(const Circle &x, const mat2x2 &mat_rotation) {
    LSPE_ASSERT(x.r > 0);

    return x;
}

bool contain(const Circle &a, const vec2 &b) {
    vec2 c = b - a.center;
    return dot(c, c) < a.r * a.r;
}

float inertiaOf(const Circle &a, float mass) {
    LSPE_ASSERT(mass > 0);
    return mass * (a.r * a.r * 0.5);
}

}; // namespace lspe
