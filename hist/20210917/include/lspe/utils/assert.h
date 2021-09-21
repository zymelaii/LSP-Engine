#ifndef LSPE_ASSERT_H
#define LSPE_ASSERT_H

#include <assert.h>

#ifdef LSPE_DEBUG
#define LSPE_ASSERT assert
#else
#define LSPE_ASSERT(...)
#endif

#endif /* LSPE_ASSERT_H */