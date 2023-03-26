#include <lspe/shape.h>

namespace lspe {

using shape::Polygen;

float areaOf(const Polygen &x) {
    LSPE_ASSERT(x.vertices.size() >= 3);
    float area = 0.0f;
    for (int i = 2; i < x.vertices.size(); ++i) {
        vec2 a = x.vertices[i] - x.vertices[i - 1];
        vec2 b = x.vertices[i] - x.vertices[i - 2];
        area   += fabs(cross(a, b));
    }
    return area * 0.5f;
}

vec2 centroidOf(const Polygen &x) {
    LSPE_ASSERT(x.vertices.size() >= 3);
    vec2 c = x.vertices[0];
    for (int i = 1; i < x.vertices.size(); ++i) { c += x.vertices[i]; }
    return (c /= x.vertices.size());
}

bbox2 bboxOf(const Polygen &x) {
    LSPE_ASSERT(x.vertices.size() >= 3);
    vec2 lower = x.vertices[0], upper = x.vertices[0];
    for (int i = 1; i < x.vertices.size(); ++i) {
        const vec2 &p = x.vertices[i];
        if (p.x < lower.x) {
            lower.x = p.x;
        } else if (p.x > upper.x) {
            upper.x = p.x;
        }
        if (p.y < lower.y) {
            lower.y = p.y;
        } else if (p.y > upper.y) {
            upper.y = p.y;
        }
    }
    return {lower, upper};
}

void translate(Polygen &x, const vec2 &displacement) {
    x.center += displacement;
    for (auto &e : x.vertices) { e += displacement; }
}

void doRotation(Polygen &x, float rotation) {
    LSPE_ASSERT(x.vertices.size() >= 3);

    mat2x2 mat_rotation = getRotateMatrix(rotation);
    doRotation(x, mat_rotation);
}

void doRotation(Polygen &x, const mat2x2 &mat_rotation) {
    for (auto &e : x.vertices) { e = mat_rotation * (e - x.center) + x.center; }
}

Polygen rotationOf(const Polygen &x, float rotation) {
    LSPE_ASSERT(x.vertices.size() >= 3);

    mat2x2 mat_rotation = getRotateMatrix(rotation);

    Polygen newPolygen(x);

    doRotation(newPolygen, mat_rotation);

    return newPolygen;
}

Polygen rotationOf(const Polygen &x, const mat2x2 &mat_rotation) {
    LSPE_ASSERT(x.vertices.size() >= 3);

    Polygen newPolygen(x);

    doRotation(newPolygen, mat_rotation);

    return newPolygen;
}

bool contain(const Polygen &a, const vec2 &b) {
    LSPE_ASSERT(a.vertices.size() >= 3);

    vec2 p = a.vertices[0];

    vec2 ao = -p;
    vec2 v  = b + ao;

    for (int i = 2; i < a.vertices.size(); ++i) {
        vec2 v1 = ao + a.vertices[i - 1];
        vec2 v2 = ao + a.vertices[i];

        vec2 perp1 = triproduct(v2, v1, v1);
        vec2 perp2 = triproduct(v1, v2, v2);

        if (dot(v, perp1) <= FLT_EPSILON && dot(v, perp2) <= FLT_EPSILON) {
            return true;
        }
    }

    return false;
}

float inertiaOf(const Polygen &a, float mass) {
    LSPE_ASSERT(mass > 0);

    float inertia = 0;
    float area    = 0;

    for (int i = 0; i < a.vertices.size(); ++i) {
        vec2 u = a.vertices[i] - a.vertices[0];
        vec2 v = a.vertices[(i + 1) % a.vertices.size()] - a.vertices[0];

        float xIntegrate = u.x * u.x + v.x * u.x + v.x * v.x;
        float yIntegrate = u.y * u.y + v.y * u.y + v.y * v.y;

        float dArea = fabs(cross(u, v)) * 0.5;

        inertia += (xIntegrate + yIntegrate) * dArea;
        area    += dArea;
    }

    inertia *= mass / area / 6;

    //! defaultly, the center of the polygen is its centroid

    // centroid = centroidOf(a);
    // inertia += mass * dot(centroid, centroid) - dot(a.center, a.center);

    return inertia;
}

}; // namespace lspe
