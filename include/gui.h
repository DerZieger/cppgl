#pragma once

#include "context.h"
#include "imgui/imgui.h"
#include "anim.h"
#include "mesh.h"
#include "camera.h"
#include "shader.h"
#include "texture.h"
#include "material.h"
#include "geometry.h"
#include "drawelement.h"
#include "framebuffer.h"
#include "data_types.h"

CPPGL_NAMESPACE_BEGIN

// -------------------------------------------
// helpers

#define IM_VEC2_CLASS_EXTRA                                 \
        constexpr ImVec2(const vec2 &f) : x(f.x), y(f.y) {} \
        operator vec2() const { return vec2(x, y); }

#define IM_VEC3_CLASS_EXTRA                                         \
        constexpr ImVec3(const vec3 &f) : x(f.x), y(f.y), z(f.z) {} \
        operator vec3() const { return vec3(x, y, z); }

#define IM_VEC4_CLASS_EXTRA                                                 \
        constexpr ImVec4(const vec4 &f) : x(f.x), y(f.y), z(f.z), w(f.w) {} \
        operator vec4() const { return vec4(x, y, z, w); }

// -------------------------------------------
// callbacks

    void gui_add_callback(const std::string &name, void (*fn)(void));

    void gui_add_callback(const std::string &name, void (*fn)(void *),
                          void *arg);//Can be used to add member function as callback with static wrapper function
    void gui_remove_callback(const std::string &name);

// -------------------------------------------
// main draw call

    void gui_draw();

    void gui_draw_callbacks();

// -------------------------------------------
// helper functions to display properties

    void gui_display_mat4(mat4 &mat);

    void gui_display_camera(Camera &cam);

    void gui_display_texture(const Texture2D &tex, const ivec2 &size = ivec2(300, 300));

    void gui_display_shader(Shader &shader);

    void gui_display_framebuffer(const Framebuffer &fbo);

    void gui_display_material(const Material &mat);

    void gui_display_geometry(const GeometryBase &geom);

    void gui_display_mesh(const Mesh &mesh);

    void gui_display_drawelement(Drawelement &elem);

    void gui_display_animation(const Animation &anim);

    void gui_display_query_timer(const Query &query, const char *label = "");

    void gui_display_query_counter(const Query &query, const char *label = "");

    void gui_display_timers();
// TODO add more

CPPGL_NAMESPACE_END
