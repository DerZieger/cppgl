#ifndef CPPGL_ASSERT_H
#define CPPGL_ASSERT_H
#pragma once

#include <assert.h>

#include <string>
#include "data_types.h"

#ifndef CPPGL_DEBUG_MODE
// disables all assertions from <assert.h>
#ifndef NDEBUG
#define NDEBUG
#endif
#endif

#ifndef NDEBUG
#define _CPPGL_ASSERT(x)                                                     \
  do                                                                         \
  {                                                                          \
    if (!(x))                                                                \
    {                                                                        \
      std::cerr << "cppgl assertion FAILED: \"" << #x << "\" (" << (bool)(x) \
                << ")\n  at " << __FILE__ << " line " << __LINE__ << "\n";   \
      std::exit(1);                                                          \
    }                                                                        \
  } while (0)
#define _CPPGL_ASSERT_EQ(x, y)                                                \
  do                                                                          \
  {                                                                           \
    if ((x) != (y))                                                           \
    {                                                                         \
      std::cerr << "cppgl assertion FAILED: " << #x << " == " << #y << " ("   \
                << (x) << " != " << (y) << ")\n  at " << __FILE__ << " line " \
                << __LINE__ << "\n";                                          \
      std::exit(1);                                                           \
    }                                                                         \
  } while (0)
#define _CPPGL_ASSERT_NE(x, y)                                                \
  do                                                                          \
  {                                                                           \
    if ((x) == (y))                                                           \
    {                                                                         \
      std::cerr << "cppgl assertion FAILED: " << #x << " != " << #y << " ("   \
                << (x) << " == " << (y) << ")\n  at " << __FILE__ << " line " \
                << __LINE__ << "\n";                                          \
      std::exit(1);                                                           \
    }                                                                         \
  } while (0)
#define _CPPGL_ASSERT_LE(x, y)                                               \
  do                                                                         \
  {                                                                          \
    if ((x) > (y))                                                           \
    {                                                                        \
      std::cerr << "cppgl assertion FAILED: " << #x << " <= " << #y << " ("  \
                << (x) << " > " << (y) << ")\n  at " << __FILE__ << " line " \
                << __LINE__ << "\n";                                         \
      std::exit(1);                                                          \
    }                                                                        \
  } while (0)
#define _CPPGL_ASSERT_GE(x, y)                                               \
  do                                                                         \
  {                                                                          \
    if ((x) < (y))                                                           \
    {                                                                        \
      std::cerr << "cppgl assertion FAILED: " << #x << " >= " << #y << " ("  \
                << (x) << " < " << (y) << ")\n  at " << __FILE__ << " line " \
                << __LINE__ << "\n";                                         \
      std::exit(1);                                                          \
    }                                                                        \
  } while (0)
#define _CPPGL_ASSERT_LT(x, y)                                                \
  do                                                                          \
  {                                                                           \
    if ((x) >= (y))                                                           \
    {                                                                         \
      std::cerr << "cppgl assertion FAILED: " << #x << " < " << #y << " ("    \
                << (x) << " >= " << (y) << ")\n  at " << __FILE__ << " line " \
                << __LINE__ << "\n";                                          \
      std::exit(1);                                                           \
    }                                                                         \
  } while (0)
#define _CPPGL_ASSERT_GT(x, y)                                                \
  do                                                                          \
  {                                                                           \
    if ((x) <= (y))                                                           \
    {                                                                         \
      std::cerr << "cppgl assertion FAILED: " << #x << " > " << #y << " ("    \
                << (x) << " <= " << (y) << ")\n  at " << __FILE__ << " line " \
                << __LINE__ << "\n";                                          \
      std::exit(1);                                                           \
    }                                                                         \
  } while (0)
#else
#define _CPPGL_ASSERT(x)
#define _CPPGL_ASSERT_EQ(x, y)
#define _CPPGL_ASSERT_GE(x, y)
#define _CPPGL_ASSERT_NE(x, y)
#define _CPPGL_ASSERT_LE(x, y)
#define _CPPGL_ASSERT_LT(x, y)
#endif

#endif