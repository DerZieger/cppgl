#include "camera.h"
#include "context.h"
#include "imgui/imgui.h"
#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include "math/eigen_glm_interface.h"

CPPGL_NAMESPACE_BEGIN

    static Camera current_cam;

    Camera current_camera() {
        static Camera default_cam("default");
        return current_cam ? current_cam : default_cam;
    }

    void make_camera_current(const Camera &cam) {
        current_cam = cam;
    }

    static mat4 get_projection_matrix(float left, float right, float top, float bottom, float n, float f) {
        mat4 proj = mat4::Zero();
        proj(0, 0) = (2.f * n) / (right - left);
        proj(1, 1) = (2.f * n) / (top - bottom);
        proj(0, 2) = (right + left) / (right - (left));
        proj(1, 2) = (bottom + top) / (top - bottom);
        proj(2, 2) = -(f + n) / (f - n);
        proj(3, 2) = -1.f;
        proj(2, 3) = (-2 * f * n) / (f - n);
        return proj;
    }

// ----------------------------------------------------
// CameraImpl

    float CameraImpl::default_camera_movement_speed = 0.005f;

    CameraImpl::CameraImpl(const std::string &name) : name(name), pos(0, 0, 0), dir(1, 0, 0), up(0, 1, 0),
                                                      fov_degree(70),
                                                      near(0.01f), far(1000), left(-100), right(100), bottom(-100),
                                                      top(100),
                                                      perspective(true), skewed(false), fix_up_vector(true),
                                                      can_move(true) {
        update();
    }

    CameraImpl::~CameraImpl() {}

    void CameraImpl::update() {
        dir = normalize(dir);
        up = normalize(up);
        view = lookAt(pos, pos + dir, up);
        view_normal = transpose(inverse(view));
        proj = perspective ? (skewed ? get_projection_matrix(left, right, top, bottom, near, far)
                                     : cppgl::perspective(fov_degree * float(M_PI / 180), aspect_ratio(), near, far))
                           : ortho(left, right, bottom, top, near, far);
    }

    void CameraImpl::forward(float by) { pos += by * dir; }

    void CameraImpl::backward(float by) { pos -= by * dir; }

    void CameraImpl::leftward(float by) { pos -= by * cross(dir, up); }

    void CameraImpl::rightward(float by) { pos += by * cross(dir, up); }

    void CameraImpl::upward(float by) { pos += by * normalize(cross(cross(dir, up), dir)); }

    void CameraImpl::downward(float by) { pos -= by * normalize(cross(cross(dir, up), dir)); }

    void CameraImpl::yaw(float angle) { dir = normalize(rotate(dir, angle * float(M_PI) / 180.f, up)); }

    void CameraImpl::pitch(float angle) {
        dir = normalize(rotate(dir, angle * float(M_PI) / 180.f, normalize(cross(dir, up))));
        if (!fix_up_vector)
            up = normalize(cross(cross(dir, up), dir));
    }

    void CameraImpl::roll(float angle) { up = normalize(rotate(up, angle * float(M_PI) / 180.f, dir)); }

    void CameraImpl::from_lookat(const vec3 &pos, const vec3 &lookat, const vec3 &up) {
        this->pos = pos;
        this->dir = normalize(lookat - pos);
        this->up = up;
        update();
    }

    float CameraImpl::aspect_ratio() {
        const ivec2 res = Context::resolution();
        return float(res.x()) / float(res.y());
    }

    bool CameraImpl::default_input_handler(double dt_ms) {
        // mouse
        static float rot_speed = 0.05f;
        static vec2 last_pos(-1, -1);
        const vec2 curr_pos = Context::mouse_pos();
        const vec2 diff = last_pos - curr_pos;
        if (last_pos == vec2(-1, -1))
            last_pos = curr_pos;
        if (!current_camera()->can_move) {
            last_pos = curr_pos;
            return false;
        }
        bool moved = false;
        if (!ImGui::GetIO().WantCaptureKeyboard) {
            // keyboard
            if (Context::key_pressed(GLFW_KEY_W)) {
                current_camera()->forward(float(dt_ms * default_camera_movement_speed));
                moved = true;
            }
            if (Context::key_pressed(GLFW_KEY_S)) {
                current_camera()->backward(float(dt_ms * default_camera_movement_speed));
                moved = true;
            }
            if (Context::key_pressed(GLFW_KEY_A)) {
                current_camera()->leftward(float(dt_ms * default_camera_movement_speed));
                moved = true;
            }
            if (Context::key_pressed(GLFW_KEY_D)) {
                current_camera()->rightward(float(dt_ms * default_camera_movement_speed));
                moved = true;
            }
            if (Context::key_pressed(GLFW_KEY_R)) {
                current_camera()->upward(float(dt_ms * default_camera_movement_speed));
                moved = true;
            }
            if (Context::key_pressed(GLFW_KEY_F)) {
                current_camera()->downward(float(dt_ms * default_camera_movement_speed));
                moved = true;
            }
            if (Context::key_pressed(GLFW_KEY_Q)) {
                current_camera()->roll(float(dt_ms * -0.1));
                moved = true;
            }
            if (Context::key_pressed(GLFW_KEY_E)) {
                current_camera()->roll(float(dt_ms * 0.1));
                moved = true;
            }
        }
        if (!ImGui::GetIO().WantCaptureMouse && Context::mouse_button_pressed(GLFW_MOUSE_BUTTON_LEFT)) {
            if (length(diff) > 0.01) {
                current_camera()->pitch(diff.y() * rot_speed);
                current_camera()->yaw(diff.x() * rot_speed);
                moved = true;
            }
        }
        last_pos = curr_pos;
        return moved;
    }

CPPGL_NAMESPACE_END
