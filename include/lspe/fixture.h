#pragma once

namespace lspe
{

//! customizable bit-masks of body
struct bitmask
{
	int32_t category;    //! 类别掩码
	int32_t contact;     //! 接触组掩码
	int32_t collision;   //! 碰撞组掩码
};               //! 实体掩码

class Fixture
{
public:
	Fixture();
	~Fixture();

protected:

private:

};

}
