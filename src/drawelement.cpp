#include "drawelement.h"
#include "camera.h"
#include <iostream>
#include "math/eigen_glm_interface.h"
#include "mesh_templates.h"

CPPGL_NAMESPACE_BEGIN

// -------------------------------------------------------
// DrawelementImpl
// -------------------------------------------------------

    DrawelementImpl::DrawelementImpl(const std::string &name,
                                     const Shader &shader,
                                     const Mesh &mesh,
                                     const mat4 &model)
            : name(name),
              model(model),
              shader(shader),
              mesh(mesh),
              wireframe_mode(false),
              disable_render(false),
              is_grouped(false) {}

    DrawelementImpl::~DrawelementImpl() {}

    void DrawelementImpl::destroy_handles() {
        if (this->mesh.initialized()) {
            this->mesh->destroy_handles();
            if (Mesh::valid(mesh->name)) {
                auto elem = Mesh::find(mesh->name);
                this->mesh = Mesh();
                elem.free();
            }
        }
    }

    void DrawelementImpl::set_model_transform(const mat4 &trans) {
        model = trans;
    }

    void DrawelementImpl::set_model_translation(const vec3 &trans) {
        model.block<3, 1>(0, 3) = trans;
    }

    void DrawelementImpl::set_model_rotation(const mat3 &rot) {
        model.block<3, 3>(0, 0) = rot;
    }

    void DrawelementImpl::bind() const {
        if (disable_render)
            return;

        if (shader) {
            shader->bind();
            if (mesh)
                mesh->bind(shader);
            shader->uniform("model", model);
            shader->uniform("model_normal", (mat4) transpose(inverse(model)));
            shader->uniform("view", current_camera()->view);
            shader->uniform("view_normal", current_camera()->view_normal);
            shader->uniform("proj", current_camera()->proj);
        }
    }

    void DrawelementImpl::unbind() const {
        if (disable_render)
            return;

        if (mesh)
            mesh->unbind();
        if (shader)
            shader->unbind();
    }

    void DrawelementImpl::set_wireframe_mode(bool mode) {
        wireframe_mode = mode;
    }

    void DrawelementImpl::set_disable_render(bool mode) {
        disable_render = mode;
    }

    void DrawelementImpl::draw() const {
        if (disable_render)
            return;

        for (auto iter = pre_draw_funcs.begin(); iter != pre_draw_funcs.end();
             iter++) {
            iter->second();
        }

        if (wireframe_mode) {
            draw_wireframe();
        } else {
            if (mesh)
                mesh->draw();
        }

        for (auto iter = post_draw_funcs.begin(); iter != post_draw_funcs.end();
             iter++) {
            iter->second();
        }
    }

    void DrawelementImpl::draw_bb() const {
        if (disable_render)
            return;

        if (Drawelement::valid(name + "_bb")) {
            Drawelement d =
                    Drawelement::find(name + "_bb");
            d->bind();
            d->draw_wireframe();
            d->unbind();
        } else {
            vec3 bb_max = mesh->geometry->bb_max;
            vec3 bb_min = mesh->geometry->bb_min;

            if (!Material::valid("default_bb_material")) {
                Material m("default_bb_material");
                m->vec4_map["color"] = vec4(1, 1, 0, 1);
            }
            if (!Shader::valid("uniform_color")) {
                Shader("uniform_color", "MVP.vs", "color4Uniform.fs");
            }

            BoundingBox bb(
                    name + "_bb", bb_min, bb_max,
                    Material::find("default_bb_material"));

            Drawelement d(
                    name + "_bb", Shader::find("uniform_color"), bb);
            d->model = model;

            d->bind();
            d->draw_wireframe();
            d->unbind();
        }
    }

    void DrawelementImpl::draw_wireframe() const {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        if (mesh)
            mesh->draw();
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    void DrawelementImpl::draw_index(uint32_t index) const {
        if (mesh)
            mesh->draw_index(index);
    }

// -------------------------------------------------------
// GroupedDrawelementsImpl
// -------------------------------------------------------

    GroupedDrawelementsImpl::GroupedDrawelementsImpl(
            const std::string &name,
            const std::vector<Drawelement> &elems)
            : name(name), elems(elems), wireframe_mode(false), disable_render(false) {
        for (unsigned int i = 0; i < elems.size(); i++) {
            this->elems[i]->is_grouped = true;
        }
    }

    GroupedDrawelementsImpl::~GroupedDrawelementsImpl() {}

    void GroupedDrawelementsImpl::add_drawelement(Drawelement &d) {
        elems.push_back(d);
        d->is_grouped = true;
    }

    void GroupedDrawelementsImpl::destroy_handles() {
        for (unsigned int i = 0; i < elems.size(); i++) {
            elems[i]->is_grouped = false;
            elems[i]->destroy_handles();
        }
    }

    void GroupedDrawelementsImpl::set_model_transform(const mat4 &trans) {
        for (unsigned int i = 0; i < elems.size(); i++) {
            elems[i]->set_model_transform(trans);
        }
    }

    void GroupedDrawelementsImpl::set_model_translation(
            const vec3 &trans) {
        for (unsigned int i = 0; i < elems.size(); i++) {
            elems[i]->set_model_translation(trans);
        }
    }

    void GroupedDrawelementsImpl::set_model_rotation(
            const mat3 &rot) {
        for (unsigned int i = 0; i < elems.size(); i++) {
            elems[i]->set_model_rotation(rot);
        }
    }

    void GroupedDrawelementsImpl::set_wireframe_mode(bool mode) {
        wireframe_mode = mode;
        for (unsigned int i = 0; i < elems.size(); i++) {
            elems[i]->set_wireframe_mode(mode);
        }
    }

    void GroupedDrawelementsImpl::set_disable_render(bool mode) {
        disable_render = mode;
        for (unsigned int i = 0; i < elems.size(); i++) {
            elems[i]->set_disable_render(mode);
        }
    }

    void GroupedDrawelementsImpl::bind_draw_unbind() const {
        if (disable_render)
            return;
        for (unsigned int i = 0; i < elems.size(); i++) {
            elems[i]->bind();
            elems[i]->draw();
            elems[i]->unbind();
        }
    }

    void GroupedDrawelementsImpl::bind_draw_bb_unbind() const {
        if (disable_render)
            return;
        for (unsigned int i = 0; i < elems.size(); i++) {
            elems[i]->bind();
            elems[i]->draw_bb();
            elems[i]->unbind();
        }
    }

    void GroupedDrawelementsImpl::bind_draw_wireframe_unbind()
    const {
        for (unsigned int i = 0; i < elems.size(); i++) {
            elems[i]->bind();
            elems[i]->draw_wireframe();
            elems[i]->unbind();
        }
    }

CPPGL_NAMESPACE_END
