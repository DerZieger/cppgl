#include <math/quaternion.h>

cppgl::DualQuaternion cppgl::DQB(
        const std::vector<float> &weights,
        const std::vector<cppgl::DualQuaternion> &quats) {
    size_t n = weights.size();
    cppgl::DualQuaternion blended = cppgl::DualQuaternion::Zero();
    for (size_t i = 0; i < n; i++)
        blended += weights[i] * quats[i];

    blended.normalize();
    return blended;
}

CPPGL_HD float dN(const cppgl::DualQuaternion q,
                  const float *weights, float real_norm,
                  const unsigned int component_index,
                  const Eigen::Matrix<float, 8, 6> lie_jacobi,
                  unsigned int deriv_axis) {
    return (real_norm != 0) * (1.f / (2.f * fmax(real_norm, 1e-6f))) *
           (2.f * q[0] * weights[component_index] * lie_jacobi(0, deriv_axis) +
            2.f * q[1] * weights[component_index] * lie_jacobi(1, deriv_axis) +
            2.f * q[2] * weights[component_index] * lie_jacobi(2, deriv_axis) +
            2.f * q[3] * weights[component_index] * lie_jacobi(3, deriv_axis));
}

CPPGL_HD float dDot(const cppgl::DualQuaternion q,
                    const float *weights,
                    const unsigned int component_index,
                    const Eigen::Matrix<float, 8, 6> lie_jacobi,
                    unsigned int deriv_axis) {
    return (weights[component_index] * lie_jacobi(0, deriv_axis) * q[4] +
            q[0] * weights[component_index] * lie_jacobi(4, deriv_axis)) +
           (weights[component_index] * lie_jacobi(1, deriv_axis) * q[5] +
            q[1] * weights[component_index] * lie_jacobi(5, deriv_axis)) +
           (weights[component_index] * lie_jacobi(2, deriv_axis) * q[6] +
            q[2] * weights[component_index] * lie_jacobi(6, deriv_axis)) +
           (weights[component_index] * lie_jacobi(3, deriv_axis) * q[7] +
            q[3] * weights[component_index] * lie_jacobi(7, deriv_axis));
}

CPPGL_HOST Eigen::Matrix<float, 8, 6> cppgl::DQBJacobian(
        const std::vector<float> &weights,
        const std::vector<cppgl::DualQuaternion> &quats,
        const unsigned int component_index,
        const Eigen::Matrix<float, 8, 6> &lie_jacobi) {
    Eigen::Matrix<float, 8, 6> J = Eigen::Matrix<float, 8, 6>::Zero(8, 6);

    cppgl::DualQuaternion blended = cppgl::DualQuaternion::Zero();
    for (size_t i = 0; i < weights.size(); i++)
        blended += weights[i] * quats[i];

    float N, tmp;
    blended.norm(N, tmp);

    float dotprod_unnorm = blended[0] * blended[4] + blended[1] * blended[5] +
                           blended[2] * blended[6] + blended[3] * blended[7];
    float dotprod = (N != 0) * dotprod_unnorm / fmax(pow(N, 2), 1e-6f);

    for (unsigned int deriv_axis = 0; deriv_axis < 3; deriv_axis++) {
        float dn =
                dN(blended, weights.data(), N, component_index, lie_jacobi, deriv_axis);
        float ddot =
                dDot(blended, weights.data(), component_index, lie_jacobi, deriv_axis);
        for (unsigned int axis = 0; axis < 4; axis++) {
            J(axis, deriv_axis) =
                    (N != 0) *
                    (N * weights[component_index] * lie_jacobi(axis, deriv_axis) -
                     blended[axis] * dn) /
                    fmax(pow(N, 2), 1e-5f);
        }
        for (unsigned int axis = 4; axis < 8; axis++) {
            J(axis, deriv_axis) =
                    (N != 0) *
                    ((N * weights[component_index] * lie_jacobi(axis, deriv_axis) -
                      blended[axis] * dn) /
                     fmax(pow(N, 2), 1e-6f) -
                     ((N * weights[component_index] * lie_jacobi(axis - 4, deriv_axis) -
                       blended[axis - 4] * dn) /
                      fmax(pow(N, 2), 1e-6f)) *
                     dotprod -
                     (blended[axis - 4] / N) *
                     (N * N * ddot - dotprod_unnorm * 2.f * N * dn) /
                     fmax(pow(N, 4), 1e-6f));
        }
    }

    for (unsigned int deriv_axis = 3; deriv_axis < 6; deriv_axis++) {
        float ddot =
                dDot(blended, weights.data(), component_index, lie_jacobi, deriv_axis);
        for (unsigned int axis = 4; axis < 8; axis++) {
            J(axis, deriv_axis) =
                    (N != 0) *
                    ((N * weights[component_index] * lie_jacobi(axis, deriv_axis)) /
                     fmax(pow(N, 2), 1e-6f) -
                     (blended[axis - 4] / fmax(N, 1e-6f)) * (N * N * ddot) /
                     fmax(pow(N, 4), 1e-6f));
        }
    }
    return J;
}

CPPGL_HD Eigen::Matrix<float, 8, 6> cppgl::DQBJacobian(
        const float *weights, const cppgl::DualQuaternion *quats,
        const unsigned int size, const unsigned int component_index,
        const Eigen::Matrix<float, 8, 6> &lie_jacobi) {
    Eigen::Matrix<float, 8, 6> J = Eigen::Matrix<float, 8, 6>::Zero(8, 6);

    cppgl::DualQuaternion blended = cppgl::DualQuaternion::Zero();
    for (size_t i = 0; i < size; i++)
        blended += weights[i] * quats[i];

    float N, tmp;
    blended.norm(N, tmp);

    float dotprod_unnorm = blended[0] * blended[4] + blended[1] * blended[5] +
                           blended[2] * blended[6] + blended[3] * blended[7];
    float dotprod = (N != 0) * dotprod_unnorm / fmax(pow(N, 2), 1e-6f);

    for (unsigned int deriv_axis = 0; deriv_axis < 3; deriv_axis++) {
        float dn = dN(blended, weights, N, component_index, lie_jacobi, deriv_axis);
        float ddot =
                dDot(blended, weights, component_index, lie_jacobi, deriv_axis);
        for (unsigned int axis = 0; axis < 4; axis++) {
            J(axis, deriv_axis) =
                    (N != 0) *
                    (N * weights[component_index] * lie_jacobi(axis, deriv_axis) -
                     blended[axis] * dn) /
                    fmax(pow(N, 2), 1e-5f);
        }
        for (unsigned int axis = 4; axis < 8; axis++) {
            J(axis, deriv_axis) =
                    (N != 0) *
                    ((N * weights[component_index] * lie_jacobi(axis, deriv_axis) -
                      blended[axis] * dn) /
                     fmax(pow(N, 2), 1e-6f) -
                     ((N * weights[component_index] * lie_jacobi(axis - 4, deriv_axis) -
                       blended[axis - 4] * dn) /
                      fmax(pow(N, 2), 1e-6f)) *
                     dotprod -
                     (blended[axis - 4] / N) *
                     (N * N * ddot - dotprod_unnorm * 2.f * N * dn) /
                     fmax(pow(N, 4), 1e-6f));
        }
    }

    for (unsigned int deriv_axis = 3; deriv_axis < 6; deriv_axis++) {
        float ddot =
                dDot(blended, weights, component_index, lie_jacobi, deriv_axis);
        for (unsigned int axis = 4; axis < 8; axis++) {
            J(axis, deriv_axis) =
                    (N != 0) *
                    ((N * weights[component_index] * lie_jacobi(axis, deriv_axis)) /
                     fmax(pow(N, 2), 1e-6f) -
                     (blended[axis - 4] / N) * (N * N * ddot) / fmax(pow(N, 4), 1e-6f));
        }
    }
    return J;
}

cppgl::mat4 cppgl::DQB(const std::vector<float> &weights,
                       const std::vector<mat4> &transforms) {
    size_t n = transforms.size();
    std::vector<DualQuaternion> quats(n);

    std::transform(transforms.begin(), transforms.end(), quats.begin(),
                   [](const mat4 &rt) { return DualQuaternion(rt); });

    DualQuaternion blended = DQB(weights, quats);
    return blended.matrix();
}
