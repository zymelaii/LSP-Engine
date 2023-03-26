#pragma once

/********************************
 *  @author: ZYmelaii
 *
 *  @bbox2: 2-dimensional bounding box of built-in type float
 *
 *  @brief:
 *
 *  @NOTES: bbox2::lower indicates left-bottom
 *          bbox2::upper indicates right-up
 *******************************/

#include "../lspe/base/base.h"
#include "../lspe/base/vec.h"

namespace lspe {

struct bbox2;

struct bbox2 {
    vec2 lower;
    vec2 upper;
};

vec2  centerOf(const bbox2 &a);
float perimeterOf(const bbox2 &a);
float areaOf(const bbox2 &a);

bbox2 unionOf(const bbox2 &a, const bbox2 &b);
bbox2 intersectionOf(const bbox2 &a, const bbox2 &b);

bool overlap(const bbox2 &a, const bbox2 &b);

bool contain(const bbox2 &a, const bbox2 &b);
bool contain(const bbox2 &a, const vec2 &b);

}; // namespace lspe
