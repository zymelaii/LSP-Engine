#include "mycircle.h"

MyCircle::MyCircle(const lspe::vec2f &c, float r)
	: lspe::shape::Circle<float>(c, r)
{

}

MyCircle& MyCircle::move(const lspe::vec2f &delta)
{
	center += delta;
	return *this;
}

lspe::vec2f MyCircle::getCenter() const
{
	return this->center;
}

float MyCircle::getRadius() const
{
	return this->radius;
}

const char* MyCircle::id() const
{
	return "MyCircle";
}