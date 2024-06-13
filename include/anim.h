#pragma once

#include <any>
#include <map>
#include <string>
#include <vector>
#include "named_handle.h"
#include "data_types.h"
#include "math/eigen_glm_interface.h"

CPPGL_NAMESPACE_BEGIN

// ------------------------------------------
// Animation

    class AnimationImpl {
    public:
        AnimationImpl(const std::string &name);

        virtual ~AnimationImpl();

        // step animation and apply to CameraPtr::current() if running
        void update(float dt_ms);

        void clear();

        size_t length() const;

        void play();

        void pause();

        void stop();

        void reset();

        // add/modify camera path nodes
        size_t push_node(const vec3 &camera_pos, const vec3 &lookat_pos);

        void put_node(size_t i, const vec3 &camera_pos, const vec3 &lookat_pos);

        // add/modify data nodes along camera path
        size_t push_data(const std::string &name, const std::any &data);

        void put_data(size_t i, const std::string &name, const std::any &data);

        // evaluate
        vec3 eval_pos() const;

        vec3 eval_lookat() const;

        template<typename T>
        T eval_data(const std::string &name) const; // with interpolation (requires * operator with float)
        template<typename T>
        T lookup_data(const std::string &name) const; // without interpolation

        // TODO serialization

        // data
        const std::string name;
        float time;
        float ms_between_nodes;
        bool running;
        std::vector<std::pair<vec3, vec3>> camera_path;
        std::map<std::string, std::vector<std::any>> data_path;
    };

    template<typename T>
    T AnimationImpl::lookup_data(const std::string &name) const {
        const size_t i = std::min(size_t(floor(time)), data_path.at(name).size() - 1);
        return std::any_cast<T>(data_path.at(name)[i]);
    }

    template<typename T>
    T AnimationImpl::eval_data(const std::string &name) const {
        const float f = fract(time);
        const auto &data = data_path.at(name);
        const size_t i = size_t(floor(time));
        const auto &lower = data[std::min(i, data.size()) % data.size()];
        const auto &upper = data[std::min(i + 1, data.size()) % data.size()];
        return (1 - f) * std::any_cast<T>(lower) + f * std::any_cast<T>(upper);
    }

    using Animation = NamedHandle<AnimationImpl>;

    template
    class _API NamedHandle<AnimationImpl>; // needed for Windows DLL export

// TODO move this to own module
    Animation current_animation();

    void make_animation_current(const Animation &anim);

CPPGL_NAMESPACE_END
