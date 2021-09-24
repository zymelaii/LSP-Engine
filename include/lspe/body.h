#pragma once

#include "../lspe/base/base.h"
#include "../lspe/base/vec.h"
#include "../lspe/base/mat.h"
#include "../lspe/shape.h"

namespace lspe
{

struct BodyConfig
{

	//! geometrical shape of body

	Shape shape; //! shape of body

	//! attributes of movements

	vec2  linearVelocity;  //! linear velocity of the centroid of this body
	float angularVelocity; //! angular velocity this body

	float linearDamping;   //! used to reduce the linear velocity
	float angularDamping;  //! used to reduce the angular velocity

	float gravityScale;    //! scale the gravity applied to this body

	//! attributes of world

	struct
	{
		vec2 location;
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

class body
{

private: //! customizable bit-masks of body
	struct
	{
		int32_t category;    //! 类别掩码
		int32_t contact;     //! 接触组掩码
		int32_t collision;   //! 碰撞组掩码
	} bitmask;               //! 实体掩码

private: //! basic body attributes
	float mass;             //! 质量
	float inv_mass;         //! to accelerate computation

	float inertia;          //! 转动惯量
	float inv_inertia;      //! to accelerate computation

	vec2 centroid;          //! 质心
	vec2 position;          //! 坐标

	float rotation;         //! 旋转弧度
	mat2x2 mat_rotation;    //! 旋转矩阵

	vec2 velocity;          //! 速度
	float angular_velocity; //! 角速度

	vec2 force;             //! 受力
	float torque;           //! 力矩

	vec2 frictions;         //! 摩擦系数
	                        //! { static friction, dynamic friction }
	float bounce;           //! 反弹系数

};

};
