#pragma once
#ifndef CPPGL_OPENGL_MESH_UTILS_H
#define CPPGL_OPENGL_MESH_UTILS_H

#include "mesh.h"

CPPGL_NAMESPACE_BEGIN

    struct Laplacian {
        vec3 sum;
        float count;

        Laplacian() : sum(0, 0, 0), count(0) {};
    };

    void laplacian_smoothing(GeometryBase &geo, uint32_t iter = 1);

    void laplacian_depth_smoothing(GeometryBase &geo,
                                   uint32_t iter = 1, bool smooth_normals = true,
                                   vec3 view_pos = vec3(0, 0, 0),
                                   float contribution = 1.f);

#if WITH_CUDA
    namespace cuda
    {
      void laplacian_depth_smoothing(reclib::opengl::GeometryBase &geo,
                                     uint32_t iter = 1, bool smooth_normals = true,
                                     vec3 view_pos = vec3(0, 0, 0),
                                     float contribution = 1.f);

    }
#endif // WITH_CUDA

CPPGL_NAMESPACE_END

#endif