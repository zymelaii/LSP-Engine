#ifndef LSPE_SHAPE_H
#define LSPE_SHAPE_H

#include "../common.h"

namespace lspe
{

namespace shape
{

class Shape
{
public:
	virtual bool overlap(const Shape &rhs) const = 0;
	virtual bool contain(const Shape &rhs) const = 0;
	virtual const char* id() const;
	// template <typename T> virtual BBox2<T>() = 0;

private:

};

template <typename T> class Circle;
template <typename T> class Polygen;

};

};

#include "../shape/line.hpp"
#include "../shape/circle.hpp"
#include "../shape/polygen.hpp"

#ifdef LSPE_IMPL

namespace lspe::shape
{

const char* Shape::id() const
{
	return "lspe::shape::Shape";
}

};

#endif

#endif /* LSPE_SHAPE_H */