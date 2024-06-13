#ifndef CPPGL_COMMON_H
#define CPPGL_COMMON_H

#ifdef WITH_CUDA
#define CPPGL_HOST __host__
#define CPPGL_DEVICE __device__
#define CPPGL_HD __host__ __device__
#else
#define CPPGL_HOST
#define CPPGL_DEVICE
#define CPPGL_HD
#endif

#endif