#pragma once

/********************************
 *  @author: ZYmelaii
 *
 *  @object: mat2x2 mat3x3
 *
 *  @brief: 2x2/3x3 matrix of built-in type float
 *
 *  @NOTES: inner is completed with vec2/vec3
 *******************************/

#include <initializer_list>
#include <array>

#include "../base/base.h"
#include "../base/vec.h"

namespace lspe {

class mat2x2;
class mat3x3;

mat2x2 getRotateMatrix(float rotation);
mat3x3 getRotateMatrix(vec3 rotation);

static inline mat2x2 operator+(const mat2x2 &a, const mat2x2 &b);
static inline mat2x2 operator+(const mat2x2 &a, float b);
static inline mat2x2 operator+(float a, const mat2x2 &b);
static inline mat2x2 operator-(const mat2x2 &a, const mat2x2 &b);
static inline mat2x2 operator-(const mat2x2 &a, float b);
static inline mat2x2 operator-(float a, const mat2x2 &b);
static inline mat2x2 operator*(const mat2x2 &a, const mat2x2 &b);
static inline mat2x2 operator*(const mat2x2 &a, float b);
static inline mat2x2 operator*(float a, const mat2x2 &b);

static inline vec2 operator*(const vec2 &a, const mat2x2 &b); //! 行向量 × 矩阵
static inline vec2 operator*(const mat2x2 &a, const vec2 &b); //! 矩阵 × 列向量

static inline mat2x2 &operator+=(mat2x2 &a, const mat2x2 &b);
static inline mat2x2 &operator+=(mat2x2 &a, float b);
static inline mat2x2 &operator-=(mat2x2 &a, const mat2x2 &b);
static inline mat2x2 &operator-=(mat2x2 &a, float b);
static inline mat2x2 &operator*=(mat2x2 &a, const mat2x2 &b);
static inline mat2x2 &operator*=(mat2x2 &a, float b);

static inline vec2 &operator*=(vec2 &a, const mat2x2 &b); //! 行向量 × 矩阵

static inline mat3x3 operator+(const mat3x3 &a, const mat3x3 &b);
static inline mat3x3 operator+(const mat3x3 &a, float b);
static inline mat3x3 operator+(float a, const mat3x3 &b);
static inline mat3x3 operator-(const mat3x3 &a, const mat3x3 &b);
static inline mat3x3 operator-(const mat3x3 &a, float b);
static inline mat3x3 operator-(float a, const mat3x3 &b);
static inline mat3x3 operator*(const mat3x3 &a, const mat3x3 &b);
static inline mat3x3 operator*(const mat3x3 &a, float b);
static inline mat3x3 operator*(float a, const mat3x3 &b);

static inline vec3 operator*(const vec3 &a, const mat3x3 &b); //! 行向量 × 矩阵
static inline vec3 operator*(const mat3x3 &a, const vec3 &b); //! 矩阵 × 列向量

static inline mat3x3 &operator+=(mat3x3 &a, const mat3x3 &b);
static inline mat3x3 &operator+=(mat3x3 &a, float b);
static inline mat3x3 &operator-=(mat3x3 &a, const mat3x3 &b);
static inline mat3x3 &operator-=(mat3x3 &a, float b);
static inline mat3x3 &operator*=(mat3x3 &a, const mat3x3 &b);
static inline mat3x3 &operator*=(mat3x3 &a, float b);

static inline vec3 &operator*=(vec3 &a, const mat3x3 &b); //! 行向量 × 矩阵

class mat2x2 {
public:
    static const mat2x2 I; //! 单位矩阵

public:
    mat2x2();
    mat2x2(const std::array<vec2, 2> &mat);
    mat2x2(float a11, float a12, float a21, float a22);
    mat2x2(float arr[4]);
    mat2x2(const std::initializer_list<vec2> &l);

    vec2       &operator[](int x);
    const vec2 &operator[](int x) const;

    float  det() const;       //! 行列式求值
    mat2x2 inverse() const;   //! 逆矩阵
    mat2x2 transpose() const; //! 转置矩阵

private:
    std::array<vec2, 2> data;
};

class mat3x3 {
public:
    static const mat3x3 I;

public:
    mat3x3();
    mat3x3(const std::array<vec3, 3> &mat);
    mat3x3(
        float a11,
        float a12,
        float a13,
        float a21,
        float a22,
        float a23,
        float a31,
        float a32,
        float a33);
    mat3x3(float arr[9]);
    mat3x3(const std::initializer_list<vec3> &l);

    vec3       &operator[](int x);
    const vec3 &operator[](int x) const;

    float  det() const;
    mat3x3 inverse() const;
    mat3x3 transpose() const;

private:
    std::array<vec3, 3> data;
};

}; // namespace lspe

namespace lspe {

mat2x2 operator+(const mat2x2 &a, const mat2x2 &b) {
    return {a[0] + b[0], a[1] + b[1]};
}

mat2x2 operator+(const mat2x2 &a, float b) {
    return a + mat2x2::I * b;
}

mat2x2 operator+(float a, const mat2x2 &b) {
    return mat2x2::I * a + b;
}

mat2x2 operator-(const mat2x2 &a, const mat2x2 &b) {
    return {a[0] - b[0], a[1] - b[1]};
}

mat2x2 operator-(const mat2x2 &a, float b) {
    return a + mat2x2::I * b;
}

mat2x2 operator-(float a, const mat2x2 &b) {
    return mat2x2::I * a + b;
}

mat2x2 operator*(const mat2x2 &a, const mat2x2 &b) {
    auto tb = b.transpose();
    return {
        dot(a[0], tb[0]), dot(a[0], tb[1]), dot(a[1], tb[0]), dot(a[1], tb[1])};
}

mat2x2 operator*(const mat2x2 &a, float b) {
    return {a[0] * b, a[1] * b};
}

mat2x2 operator*(float a, const mat2x2 &b) {
    return {a * b[0], a * b[1]};
}

vec2 operator*(const vec2 &a, const mat2x2 &b) {
    auto tb = b.transpose();
    return {dot(a, tb[0]), dot(a, tb[1])};
}

vec2 operator*(const mat2x2 &a, const vec2 &b) {
    return {dot(a[0], b), dot(a[1], b)};
}

mat2x2 &operator+=(mat2x2 &a, const mat2x2 &b) {
    return (a[0] += b[0], a[1] += b[1], a);
}

mat2x2 &operator+=(mat2x2 &a, float b) {
    return (a[0] += b, a[1] += b, a);
}

mat2x2 &operator-=(mat2x2 &a, const mat2x2 &b) {
    return (a[0] -= b[0], a[1] -= b[1], a);
}

mat2x2 &operator-=(mat2x2 &a, float b) {
    return (a[0] -= b, a[1] -= b, a);
}

mat2x2 &operator*=(mat2x2 &a, const mat2x2 &b) {
    return (a = a * b);
}

mat2x2 &operator*=(mat2x2 &a, float b) {
    return (a[0] *= b, a[1] *= b, a);
}

vec2 &operator*=(vec2 &a, const mat2x2 &b) {
    return (a = a * b);
}

mat3x3 operator+(const mat3x3 &a, const mat3x3 &b) {
    return {a[0] + b[0], a[1] + b[1]};
}

mat3x3 operator+(const mat3x3 &a, float b) {
    return a + mat3x3::I * b;
}

mat3x3 operator+(float a, const mat3x3 &b) {
    return mat3x3::I * a + b;
}

mat3x3 operator-(const mat3x3 &a, const mat3x3 &b) {
    return {a[0] - b[0], a[1] - b[1]};
}

mat3x3 operator-(const mat3x3 &a, float b) {
    return a + mat3x3::I * b;
}

mat3x3 operator-(float a, const mat3x3 &b) {
    return mat3x3::I * a + b;
}

mat3x3 operator*(const mat3x3 &a, const mat3x3 &b) {
    auto tb = b.transpose();
    return {
        dot(a[0], tb[0]),
        dot(a[0], tb[1]),
        dot(a[0], tb[2]),
        dot(a[1], tb[0]),
        dot(a[1], tb[1]),
        dot(a[1], tb[2]),
        dot(a[2], tb[0]),
        dot(a[2], tb[1]),
        dot(a[2], tb[2])};
}

mat3x3 operator*(const mat3x3 &a, float b) {
    return {a[0] * b, a[1] * b, a[2] * b};
}

mat3x3 operator*(float a, const mat3x3 &b) {
    return {a * b[0], a * b[1], a * b[2]};
}

vec3 operator*(const vec3 &a, const mat3x3 &b) {
    auto tb = b.transpose();
    return {dot(a, tb[0]), dot(a, tb[1]), dot(a, tb[2])};
}

vec3 operator*(const mat3x3 &a, const vec3 &b) {
    return {dot(a[0], b), dot(a[1], b), dot(a[2], b)};
}

mat3x3 &operator+=(mat3x3 &a, const mat3x3 &b) {
    return (a[0] += b[0], a[1] += b[1], a[2] += b[2], a);
}

mat3x3 &operator+=(mat3x3 &a, float b) {
    return (a[0] += b, a[1] += b, a[2] += b, a);
}

mat3x3 &operator-=(mat3x3 &a, const mat3x3 &b) {
    return (a[0] -= b[0], a[1] -= b[1], a[2] -= b[2], a);
}

mat3x3 &operator-=(mat3x3 &a, float b) {
    return (a[0] -= b, a[1] -= b, a[2] -= b, a);
}

mat3x3 &operator*=(mat3x3 &a, const mat3x3 &b) {
    return (a = a * b);
}

mat3x3 &operator*=(mat3x3 &a, float b) {
    return (a[0] *= b, a[1] *= b, a[2] *= b, a);
}

vec3 &operator*=(vec3 &a, const mat3x3 &b) {
    return (a = a * b);
}

}; // namespace lspe
