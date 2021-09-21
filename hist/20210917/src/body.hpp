#ifndef LSPE_BODY_H
#define LSPE_BODY_H

#include "common.h"
#include "motion.hpp"

class lspe
{

class Body
{
	// MotionConf mconf;

	vec2f position;
	float rotation;
	vec2f velocity;
	float angular_vclocity;

	float torque;
};

};

#include "rigidbody.hpp"
#include "softbody.hpp"

#endif /* LSPE_BODY_H */