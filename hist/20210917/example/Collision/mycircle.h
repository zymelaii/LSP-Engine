#ifndef MYCIRCLE_H
#define MYCIRCLE_H

#include "stable.h"

class MyCircle : public lspe::shape::Circle<float>
{
public:
	MyCircle(const lspe::vec2f &c, float r);

	MyCircle& move(const lspe::vec2f &delta);

	lspe::vec2f getCenter() const;
	float getRadius() const;

	const char* id() const;
};

#endif // MYCIRCLE_H
