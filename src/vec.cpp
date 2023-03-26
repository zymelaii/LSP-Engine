#include <lspe/base/vec.h>

namespace lspe {

vec2::vec2()
    : x(0)
    , y(0) {}

vec2::vec2(float x, float y)
    : x(x)
    , y(y) {}

vec2::vec2(float arr[2])
    : x(arr[0])
    , y(arr[1]) {}

float& vec2::operator[](int x) {
    LSPE_ASSERT(x >= 0 && x < 2);
    return data[x];
}

const float& vec2::operator[](int x) const {
    LSPE_ASSERT(x >= 0 && x < 2);
    return const_cast<vec2*>(this)->data[x];
}

vec2 vec2::operator-() {
    return {-x, -y};
}

float vec2::norm() const {
    return sqrt(x * x + y * y);
}

vec2 vec2::normalized() const {
    return *this / norm();
}

vec2 vec2::normal() const {
    return vec2(y, -x).normalized();
}

bool vec2::isnull() const {
    return x == Inf && y == Inf;
}

vec3::vec3()
    : x(0)
    , y(0)
    , z(0) {}

vec3::vec3(float x, float y, float z)
    : x(x)
    , y(y)
    , z(z) {}

vec3::vec3(float arr[3])
    : x(arr[0])
    , y(arr[1])
    , z(arr[2]) {}

float& vec3::operator[](int x) {
    LSPE_ASSERT(x >= 0 && x < 3);
    return data[x];
}

const float& vec3::operator[](int x) const {
    LSPE_ASSERT(x >= 0 && x < 3);
    return const_cast<vec3*>(this)->data[x];
}

vec3 vec3::operator-() {
    return {-x, -y, -z};
}

float vec3::norm() const {
    return sqrt(x * x + y * y + z * z);
}

vec3 vec3::normalized() const {
    return *this / norm();
}

bool vec3::isnull() const {
    return x == Inf && y == Inf && z == Inf;
}

}; // namespace lspe
