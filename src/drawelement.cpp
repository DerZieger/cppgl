#include "drawelement.h"
#include "camera.h"
#include <iostream>

DrawelementImpl::DrawelementImpl(const MeshPtr& mesh, const ShaderPtr& shader) : model(glm::mat4(1)), shader(shader), mesh(mesh) {}

DrawelementImpl::~DrawelementImpl() {}

void DrawelementImpl::bind() const {
    if (shader) {
        shader->bind();
        if (mesh) mesh->bind(shader);
        shader->uniform("model", model);
        shader->uniform("view", current_camera()->view);
        shader->uniform("view_normal", current_camera()->view_normal);
        shader->uniform("proj", current_camera()->proj);
    }
}

void DrawelementImpl::unbind() const {
    if (mesh) mesh->unbind();
    if (shader) shader->unbind();
}

void DrawelementImpl::draw() const {
    if (mesh)
        mesh->draw();
}
