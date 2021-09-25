#pragma once

#include <vector>

#include "../lspe/base/base.h"
#include "../lspe/base/vec.h"
#include "../lspe/base/mat.h"
#include "../lspe/shape.h"

namespace lspe
{

enum class BodyType
{
	eNull = -1,

	//! static body doesn't move and doesn't perform collision test
	//! with other static or kinematic body
	//! however, dynamic body can still react to the physical quantities
	//! computed from the interaction between the static body and itself
	//! the body's mass is setted to Infinite and its inv_mass to zero
	eStatic,

	//! kinematic body moves under simulation according to its velocity
	//! it doesn't respond to forces and behaves as if it has infinite mass
	//! so the body's mass is setted to Infinite and its inv_mass to zero
	eKinematic,

	//! a dynamic body reacts to any physical quantities
	//! it also perform collision test with all types of body
	eDynamic
};

class Fixture;
class ContactInfo;

struct RigidBodyProperty
{

	//! default property of body

	RigidBodyProperty();

	//! basic body properties

	BodyType type;  //! type of body
	Shape    shape; //! shape of body

	//! attributes of movements

	vec2  linearVelocity;  //! linear velocity of the centroid of this body
	float angularVelocity; //! angular velocity this body

	float linearDamping;   //! used to reduce the linear velocity
	float angularDamping;  //! used to reduce the angular velocity

	float gravityScale;    //! scale the gravity applied to this body

	//! attributes of world

	struct
	{
		vec2  location;
		float angle;
	} world;

	//! interaction configs

	bool enabled;       //!   is this body enabled ?
	bool awake;         //!   is this body weak or sleeping ?
	bool allowSleep;    //! does this body allow sleeping ?
	bool fixedRotation; //! does this body allow extra rotation ?
	bool enableCCD;     //! does this body require continuous collision detection ?

	//! user data

	void *userdata; //! userdata pointer

};

class RigidBody
{

public:
	RigidBody();
	~RigidBody();

	void preUpdate(float dt);
	void postUpdate(float dt);

	BodyType getBodyType() const;
	Shape    getShape() const;

	void setBodyType(BodyType type);
	bool    setShape(Shape shape);

	RigidBodyProperty& getProperty();

	void applyForce2Center(vec2 force, bool wake);
	void applyForce(vec2 force, vec2 point, bool wake);
	void applyTorque(float torque, bool wake);
	void applyLinearImpulse2Center(vec2 linearImpulse, bool wake);
	void applyLinearImpulse(vec2 linearImpulse, vec2 point, bool wake);
	void applyAngularImpulse(float angularImpulse, bool wake);

	vec2 getCentroid() const;

	float    getMass() const;
	float getInertia() const;

	void setMass(float mass);
	[[deprecated]] void setInertia(float inertia);
/**
 *  DEPRECATED: void setInertia(float inertia);
 *  NOTE: there isn't a simple universal algorithm to compute
 *  the inertia as it depends on the concrete shape.
 *  thus this quantity will be computed by the Shape itself
 *  via function call @inertiaOf(Shape, float mass)
 **/

	vec2   getLinearVelocity() const;
	float getAngularVelocity() const;
	float   getLinearDamping() const;
	float  getAngularDamping() const;
	float    getGravityScale() const;

	void  setLinearVelocity(const vec2 &linearVelocity);
	void setAngularVelocity(float angularVelocity);
	void   setLinearDamping(float linearDamping);
	void  setAngularDamping(float angularDamping);
	void    setGravityScale(float gravityScale);

	bool       isEnabled() const;
	bool         isAwake() const;
	bool    isAllowSleep() const;
	bool     isEnableCCD() const;
	bool isFixedRotation() const;

	void       setEnabled(bool flag);
	void         setAwake(bool flag);
	void    setAllowSleep(bool flag);
	void     setEnableCCD(bool flag);
	void setFixedRotation(bool flag);

protected:

private:
	RigidBodyProperty property;

	vec2 centroid;

	float mass;
	float inv_mass;    //! defaultly when inv_mass equals 0
	                   //! type of the body will be changed into eStatic

	float inertia;     //! rotational inertia
	float inv_inertia;

	vec2  force;
	float torque;

	enum RigidBodyFlag
	{
		eEnabled       = 0x0001,
		eAwake         = 0x0002,
		eAutoSleep     = 0x0004,
		eFixedRotation = 0x0008,
		eEnableCCD     = 0x0010
	};

	float sleepTime; //! sleep time is accumulated inside
	                 //! engine when perform updation
	                 //! but not a real time timestamp

	int32_t flags;

};

};
