#pragma once

#include <string>
#include <memory>
#include <glm/glm.hpp>
#include "named_handle.h"

// ----------------------------------------------------
// Camera

class CameraImpl {
public:
    CameraImpl();
    virtual ~CameraImpl();

    void update();

    // move
    void forward(float by);
    void backward(float by);
    void leftward(float by);
    void rightward(float by);
    void upward(float by);
    void downward(float by);

    // rotate
    void yaw(float angle);
    void pitch(float angle);
    void roll(float angle);

    // load/store
    void store(glm::vec3& pos, glm::quat& rot) const;
    void load(const glm::vec3& pos, const glm::quat& rot);

    // compute aspect ratio from current viewport
    static float aspect_ratio();

    // default camera keyboard/mouse handler for basic movement
    static float default_camera_movement_speed;
    static bool default_input_handler(double dt_ms);

    // data
    glm::vec3 pos, dir, up;             // camera coordinate system
    float fov_degree, near, far;        // perspective projection
    float left, right, bottom, top;     // orthographic projection
    bool perspective;                   // switch between perspective and orthographic (default: perspective)
    bool fix_up_vector;                 // keep up vector fixed to avoid camera drift
    glm::mat4 view, view_normal, proj;  // camera matrices (computed via a call update())
};

using CameraPtr = NamedHandle<CameraImpl>;

// TODO move to CameraImpl::current()
CameraPtr current_camera();
void make_camera_current(const CameraPtr& cam);