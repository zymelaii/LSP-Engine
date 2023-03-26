#include <lspe/base/mat.h>

namespace lspe {

mat2x2 getRotateMatrix(float rotation) {
    vec2 sc(sin(rotation), cos(rotation));
    return {
        {sc.y, -sc.x},
        sc
    };
}

mat3x3 getRotateMatrix(vec3 rotation) {
    double cs[3]{cos(rotation.x), cos(rotation.y), cos(rotation.z)};
    double ss[3]{sin(rotation.x), sin(rotation.y), sin(rotation.z)};

    mat3x3 mat_rx(1, 0, 0, 0, cs[0], -ss[0], 0, ss[0], cs[0]);
    mat3x3 mat_ry(cs[1], 0, ss[1], 0, 1, 0, -ss[1], 0, cs[1]);
    mat3x3 mat_rz(cs[2], -ss[2], 0, ss[2], cs[2], 0, 0, 0, 1);

    return mat_rx * mat_ry * mat_rz;
}

const mat2x2 mat2x2::I(1, 0, 0, 1);

mat2x2::mat2x2() {
    vec2 a1{0, 0};
    vec2 a2{0, 0};
    data = {a1, a2};
}

mat2x2::mat2x2(const std::array<vec2, 2> &mat)
    : data(mat) {}

mat2x2::mat2x2(float a11, float a12, float a21, float a22) {
    vec2 a1{a11, a12};
    vec2 a2{a21, a22};
    data = {a1, a2};
}

mat2x2::mat2x2(float arr[4]) {
    vec2 a1{arr[0], arr[1]};
    vec2 a2{arr[2], arr[3]};
    data = {a1, a2};
}

mat2x2::mat2x2(const std::initializer_list<vec2> &l) {
    LSPE_ASSERT(l.size() == 2);

    int i = 0;
    for (auto &e : l) { data[i++] = e; }
}

vec2 &mat2x2::operator[](int x) {
    LSPE_ASSERT(x >= 0 && x < 2);
    return data[x];
}

const vec2 &mat2x2::operator[](int x) const {
    LSPE_ASSERT(x >= 0 && x < 2);
    return const_cast<mat2x2 *>(this)->data[x];
}

float mat2x2::det() const {
    return data[0][0] * data[1][1] - data[0][1] * data[1][0];
}

mat2x2 mat2x2::inverse() const {
    float  invdet = 1.0 / det();
    mat2x2 ans    = mat2x2(data[1][1], -data[0][1], -data[1][0], data[0][0]);
    return ans * invdet;
}

mat2x2 mat2x2::transpose() const {
    auto ans = data;
    std::swap(ans[0][1], ans[1][0]);
    return ans;
}

const mat3x3 mat3x3::I(1, 0, 0, 0, 1, 0, 0, 0, 1);

mat3x3::mat3x3() {
    vec3 a1{0, 0, 0};
    vec3 a2{0, 0, 0};
    vec3 a3{0, 0, 0};
    data = {a1, a2, a3};
}

mat3x3::mat3x3(const std::array<vec3, 3> &mat)
    : data(mat) {}

mat3x3::mat3x3(
    float a11,
    float a12,
    float a13,
    float a21,
    float a22,
    float a23,
    float a31,
    float a32,
    float a33) {
    vec3 a1{a11, a12, a13};
    vec3 a2{a21, a22, a23};
    vec3 a3{a31, a32, a33};
    data = {a1, a2, a3};
}

mat3x3::mat3x3(float arr[9]) {
    vec3 a1{arr[0], arr[1], arr[2]};
    vec3 a2{arr[3], arr[4], arr[5]};
    vec3 a3{arr[6], arr[7], arr[8]};
    data = {a1, a2, a3};
}

mat3x3::mat3x3(const std::initializer_list<vec3> &l) {
    LSPE_ASSERT(l.size() == 3);

    int i = 0;
    for (auto &e : l) { data[i++] = e; }
}

vec3 &mat3x3::operator[](int x) {
    LSPE_ASSERT(x >= 0 && x < 3);
    return data[x];
}

const vec3 &mat3x3::operator[](int x) const {
    LSPE_ASSERT(x >= 0 && x < 3);
    return const_cast<mat3x3 *>(this)->data[x];
}

float mat3x3::det() const {
    return data[0][0] * data[1][1] * data[2][2]
         + data[0][1] * data[1][2] * data[2][0]
         + data[0][2] * data[1][0] * data[2][1]
         - data[0][2] * data[1][1] * data[2][0]
         - data[0][0] * data[1][2] * data[2][1]
         - data[0][1] * data[1][0] * data[2][2];
}

mat3x3 mat3x3::inverse() const {
    return mat3x3::I; //! this is to be archived.
}

mat3x3 mat3x3::transpose() const {
    auto ans = data;
    std::swap(ans[0][1], ans[1][0]);
    std::swap(ans[0][2], ans[2][0]);
    std::swap(ans[1][2], ans[2][1]);
    return ans;
}

}; // namespace lspe
