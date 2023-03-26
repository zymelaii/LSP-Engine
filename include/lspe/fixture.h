#pragma once

#include "../lspe/base/base.h"

namespace lspe {

//! customizable bit-masks of body
struct bitmask {
    int32_t category;  //! category bit mask
    int32_t contact;   //! contact group bit mask
    int32_t collision; //! collision group bit mask
};                     //! bit masks designed for body

class Fixture {
public:
    Fixture();
    ~Fixture();

protected:

private:
};

} // namespace lspe
