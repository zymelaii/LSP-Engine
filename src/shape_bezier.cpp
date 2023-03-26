#include <lspe/shape.h>

namespace lspe {

using shape::Bezier2;
using shape::Bezier3;

vec2 centroidOf(const Bezier2 &x) {
    float k[3];
    k[0] = (1 - x.anchor) * (1 - x.anchor);
    k[1] = 2 * x.anchor * (1 - x.anchor);
    k[2] = x.anchor * x.anchor;
    return x.P[0] * k[0] + x.P[1] * k[1] + x.P[2] * k[2];
}

bbox2 bboxOf(const Bezier2 &x) {
    vec2 lower, upper;
    lower.x = min(x.P[0].x, x.P[1].x, x.P[2].x);
    lower.y = min(x.P[0].y, x.P[1].y, x.P[2].y);
    upper.x = max(x.P[0].x, x.P[1].x, x.P[2].x);
    upper.y = max(x.P[0].y, x.P[1].y, x.P[2].y);
    return {lower, upper};
}

void translate(Bezier2 &x, const vec2 &displacement) {
    x.P[0] += displacement;
    x.P[1] += displacement;
    x.P[2] += displacement;
}

void doRotation(Bezier2 &x, float rotation) {
    mat2x2 mat_rotation = getRotateMatrix(rotation);
    doRotation(x, mat_rotation);
}

void doRotation(Bezier2 &x, const mat2x2 &mat_rotation) {
    vec2 center = centroidOf(x);

    x.P[0] = mat_rotation * (x.P[0] - center) + center;
    x.P[1] = mat_rotation * (x.P[1] - center) + center;
    x.P[2] = mat_rotation * (x.P[2] - center) + center;
}

Bezier2 rotationOf(const Bezier2 &x, float rotation) {
    mat2x2 mat_rotation = getRotateMatrix(rotation);

    Bezier2 newBezier2(x);

    doRotation(newBezier2, mat_rotation);

    return newBezier2;
}

Bezier2 rotationOf(const Bezier2 &x, const mat2x2 &mat_rotation) {
    Bezier2 newBezier2(x);

    doRotation(newBezier2, mat_rotation);

    return newBezier2;
}

vec2 centroidOf(const Bezier3 &x) {
    float k[4];
    k[0] = (1 - x.anchor) * (1 - x.anchor) * (1 - x.anchor);
    k[1] = 3 * x.anchor * (1 - x.anchor) * (1 - x.anchor);
    k[2] = 3 * x.anchor * x.anchor * (1 - x.anchor);
    k[3] = x.anchor * x.anchor * x.anchor;
    return x.P[0] * k[0] + x.P[1] * k[1] + x.P[2] * k[2] + x.P[3] * k[3];
}

bbox2 bboxOf(const Bezier3 &x) {
    vec2 lower, upper;
    lower.x = min(x.P[0].x, x.P[1].x, x.P[2].x, x.P[3].x);
    lower.y = min(x.P[0].y, x.P[1].y, x.P[2].y, x.P[3].y);
    upper.x = max(x.P[0].x, x.P[1].x, x.P[2].x, x.P[3].x);
    upper.y = max(x.P[0].y, x.P[1].y, x.P[2].y, x.P[3].y);
    return {lower, upper};
}

void translate(Bezier3 &x, const vec2 &displacement) {
    x.P[0] += displacement;
    x.P[1] += displacement;
    x.P[2] += displacement;
    x.P[3] += displacement;
}

void doRotation(Bezier3 &x, float rotation) {
    mat2x2 mat_rotation = getRotateMatrix(rotation);
    doRotation(x, mat_rotation);
}

void doRotation(Bezier3 &x, const mat2x2 &mat_rotation) {
    vec2 center = centroidOf(x);

    x.P[0] = mat_rotation * (x.P[0] - center) + center;
    x.P[1] = mat_rotation * (x.P[1] - center) + center;
    x.P[2] = mat_rotation * (x.P[2] - center) + center;
    x.P[3] = mat_rotation * (x.P[3] - center) + center;
}

Bezier3 rotationOf(const Bezier3 &x, float rotation) {
    mat2x2 mat_rotation = getRotateMatrix(rotation);

    Bezier3 newBezier3(x);

    doRotation(newBezier3, mat_rotation);

    return newBezier3;
}

Bezier3 rotationOf(const Bezier3 &x, const mat2x2 &mat_rotation) {
    Bezier3 newBezier3(x);

    doRotation(newBezier3, mat_rotation);

    return newBezier3;
}

}; // namespace lspe
