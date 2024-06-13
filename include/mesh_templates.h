#ifndef CPPGL_OPENGL_MESH_TEMPLATES_H
#define CPPGL_OPENGL_MESH_TEMPLATES_H

#pragma once

// clang-format off
#include <GL/glew.h>
#include <GL/gl.h>
// clang-format on

#include <vector>

#include "data_types.h"
#include "geometry.h"
#include "mesh.h"

// ----------------------------
// Definition
CPPGL_NAMESPACE_BEGIN

// ------------------------------------------
// LineImpl
    class LineImpl : public MeshImpl {
    public:
        vec3 from;
        vec3 to;

        static Geometry generate_geometry(const std::string &name, const vec3 &from,
                                          const vec3 &to);

        LineImpl(const std::string &name, const vec3 &from, const vec3 &to,
                 const Material &material = Material(), GLenum hint = GL_STATIC_DRAW);

        virtual ~LineImpl();

        static inline std::string type_to_str() { return "LineImpl"; }
    };

    using Line = NamedHandle<LineImpl>;

// ------------------------------------------
// PointCloudImpl
    class PointCloudImpl : public MeshImpl {
    public:
        static Geometry generate_geometry(const std::string &name, const std::vector<vec3> &points, const int size);

        PointCloudImpl(const std::string &name, const std::vector<vec3> &points,
                       const int size,
                       const Material &material = Material(), GLenum hint = GL_STATIC_DRAW);

        virtual ~PointCloudImpl();

        static inline std::string type_to_str() { return "PointCloudImpl"; }
    };

    using PointCloud = NamedHandle<PointCloudImpl>;

// ------------------------------------------
// CuboidImpl
    class CuboidImpl : public MeshImpl {
    public:
        float width;
        float height;
        float depth;
        vec3 center;

        // width = x-axis length
        // height = y-axis length
        // depth = z-axis length
        static Geometry generate_geometry(const std::string &name, float width,
                                          float height, float depth,
                                          const vec3 &center = vec3(0, 0, 0));

        CuboidImpl(const std::string &name, float width, float height, float depth,
                   const vec3 &center = vec3(0, 0, 0),
                   const Material &material = Material());

        virtual ~CuboidImpl();

        static inline std::string type_to_str() { return "CuboidImpl"; }
    };

    using Cuboid = NamedHandle<CuboidImpl>;

// ------------------------------------------
// VoxelGridImpl
    class VoxelGridImpl : public MeshImpl {
    public:
        unsigned int size;
        float scale;
        vec3 center;

        // width = x-axis length
        // height = y-axis length
        // depth = z-axis length
        static Geometry generate_geometry(const std::string &name, unsigned int size,
                                          float scale);

        VoxelGridImpl(const std::string &name, unsigned int size, float scale,
                      const Material &material = Material());

        virtual ~VoxelGridImpl();

        static inline std::string type_to_str() { return "VoxelGridImpl"; }
    };

    using VoxelGrid = NamedHandle<VoxelGridImpl>;

// ------------------------------------------
// VoxelGridXYZImpl
    class VoxelGridXYZImpl : public MeshImpl {
    public:
        vec3 size;
        float scale;
        vec3 center;

        // width = x-axis length
        // height = y-axis length
        // depth = z-axis length
        static Geometry generate_geometry(const std::string &name, const vec3 &size,
                                          float scale);

        VoxelGridXYZImpl(const std::string &name, const vec3 &size, float scale,
                         const Material &material = Material());

        virtual ~VoxelGridXYZImpl();

        static inline std::string type_to_str() { return "VoxelGridXYZImpl"; }
    };

    using VoxelGridXYZ = NamedHandle<VoxelGridXYZImpl>;

// ------------------------------------------
// SphereImpl

    class SphereImpl : public MeshImpl {
    public:
        float radius;
        vec3 center;
        float stack_count;
        float sector_count;

        static Geometry generate_geometry(const std::string &name, float radius,
                                          const vec3 &center, float sector_count = 20,
                                          float stack_count = 20);

        SphereImpl(const std::string &name, float radius, const vec3 &center,
                   const Material &material = Material(), float sector_count = 20,
                   float stack_count = 20);

        virtual ~SphereImpl();

        static inline std::string type_to_str() { return "SphereImpl"; }
    };

    using Sphere = NamedHandle<SphereImpl>;

// ------------------------------------------
// EllipsoidImpl

    class EllipsoidImpl : public MeshImpl {
    public:
        vec3 pc1;
        vec3 pc2;
        vec3 pc3;
        vec3 center;
        float stack_count;
        float sector_count;

        static Geometry generate_geometry(const std::string &name, const vec3 &pc1,
                                          const vec3 &pc2, const vec3 &pc3,
                                          const vec3 &center, float sector_count = 20,
                                          float stack_count = 20);

        EllipsoidImpl(const std::string &name, const vec3 &pc1, const vec3 &pc2,
                      const vec3 &pc3, const vec3 &center,
                      const Material &material = Material(), float sector_count = 20,
                      float stack_count = 20);

        virtual ~EllipsoidImpl();

        static inline std::string type_to_str() { return "EllipsoidImpl"; }
    };

    using Ellipsoid = NamedHandle<EllipsoidImpl>;

// ------------------------------------------
//  CylinderImpl

    class CylinderImpl : public MeshImpl {
    public:
        float radius;
        float height;
        vec3 axis;
        vec3 center;
        float stack_count;
        float sector_count;

        static Geometry generate_geometry(const std::string &name, const float radius,
                                          const float height, const vec3 &axis,
                                          const vec3 &center, float sector_count = 20,
                                          float stack_count = 20);

        CylinderImpl(const std::string &name, const float radius, const float height,
                     const vec3 &axis, const vec3 &center,
                     const Material &material = Material(), float sector_count = 20,
                     float stack_count = 20);

        virtual ~CylinderImpl();

        static inline std::string type_to_str() { return "CylinderImpl"; }
    };

    using Cylinder = NamedHandle<CylinderImpl>;

// ------------------------------------------
// FrustumImpl

    class FrustumImpl : public MeshImpl {
    public:
        static Geometry generate_geometry(const std::string &name, const vec3 &pos,
                                          const mat4 &view, const mat4 &proj);

        FrustumImpl(const std::string &name, const vec3 &pos, const mat4 &view,
                    const mat4 &proj, const Material &material = Material());

        virtual ~FrustumImpl();

        static inline std::string type_to_str() { return "FrustumImpl"; }
    };

    using Frustum = NamedHandle<FrustumImpl>;

// ------------------------------------------
// BoundingBoxImpl

    class BoundingBoxImpl : public MeshImpl {
    public:
        static Geometry generate_geometry(const std::string &name, const vec3 &bb_min,
                                          const vec3 &bb_max);

        BoundingBoxImpl(const std::string &name, const vec3 &bb_min,
                        const vec3 &bb_max, const Material &material = Material());

        virtual ~BoundingBoxImpl();

        static inline std::string type_to_str() { return "BoundingBoxImpl"; }
    };

    using BoundingBox = NamedHandle<BoundingBoxImpl>;

CPPGL_NAMESPACE_END

template
class _API cppgl::NamedHandle<cppgl::SphereImpl>;      // needed for Windows DLL export
template
class _API cppgl::NamedHandle<cppgl::CuboidImpl>;      // needed for Windows DLL export
template
class _API cppgl::NamedHandle<cppgl::FrustumImpl>;     // needed for Windows DLL export
template
class _API cppgl::NamedHandle<cppgl::LineImpl>;        // needed for Windows DLL export
template
class _API cppgl::NamedHandle<cppgl::VoxelGridImpl>;   // needed for Windows DLL export
template
class _API cppgl::NamedHandle<cppgl::EllipsoidImpl>;   // needed for Windows DLL export
template
class _API cppgl::NamedHandle<cppgl::CylinderImpl>;    // needed for Windows DLL export
template
class _API cppgl::NamedHandle<cppgl::BoundingBoxImpl>; // needed for Windows DLL export
template
class _API cppgl::NamedHandle<cppgl::PointCloudImpl>; // needed for Windows DLL export


#endif