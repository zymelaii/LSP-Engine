#include <lspe/bbox.h>

namespace lspe {

vec2 centerOf(const bbox2 &a) {
    return (a.lower + a.upper) * 0.5f;
}

float perimeterOf(const bbox2 &a) {
    auto wh = a.upper - a.lower;
    return (wh.x + wh.y) * 2.0f;
}

float areaOf(const bbox2 &a) {
    auto wh = a.upper - a.lower;
    return wh.x * wh.y;
}

bbox2 unionOf(const bbox2 &a, const bbox2 &b) {
    auto lower = vec2(min(a.lower.x, b.lower.x), min(a.lower.y, b.lower.y));
    auto upper = vec2(max(a.upper.x, b.upper.x), max(a.upper.y, b.upper.y));
    return {lower, upper};
}

bbox2 intersectionOf(const bbox2 &a, const bbox2 &b) {
    auto lower = vec2(max(a.lower.x, b.lower.x), max(a.lower.y, b.lower.y));
    auto upper = vec2(min(a.upper.x, b.upper.x), min(a.upper.y, b.upper.y));
    return {lower, upper};
}

bool overlap(const bbox2 &a, const bbox2 &b) {
    auto d1 = b.lower - a.upper;
    if (d1.x > 0 || d1.y > 0) return false;

    auto d2 = a.lower - b.upper;
    if (d2.x > 0 || d2.y > 0) return false;

    return true;
}

bool contain(const bbox2 &a, const bbox2 &b) {
    return a.lower.x <= b.lower.x && a.lower.y <= b.lower.y
        && a.upper.x >= b.upper.x && a.upper.y >= b.upper.y;
}

bool contain(const bbox2 &a, const vec2 &b) {
    return b.x >= a.lower.x && b.x <= a.upper.x && b.y >= a.lower.y
        && b.y <= a.upper.y;
}

}; // namespace lspe
