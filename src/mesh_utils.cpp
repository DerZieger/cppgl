#include <mesh_utils.h>

void cppgl::laplacian_smoothing(cppgl::GeometryBase &geo,
                                uint32_t iter) {
    for (unsigned int k = 0; k < iter; k++) {
        std::map<uint32_t, Laplacian> laplacian_container;

        for (unsigned int i = 0; i < geo->positions_size(); i++) {
            laplacian_container[i] = Laplacian();
        }
        for (unsigned int i = 0; i < geo->indices_size(); i = i + 3) {
            for (unsigned int j = 0; j < 3; j++) {
                int index_cur = geo->get_index(i + (j % 3));
                int index_next = geo->get_index(i + ((j + 1) % 3));

                laplacian_container[index_cur].sum += geo->get_position(index_next);
                laplacian_container[index_next].sum += geo->get_position(index_cur);
                laplacian_container[index_cur].count++;
                laplacian_container[index_next].count++;
            }
        }
        for (unsigned int i = 0; i < geo->positions_size(); i++) {
            if (laplacian_container[i].count > 0) {
                vec3 laplacian_center =
                        laplacian_container[i].sum / laplacian_container[i].count;
                geo->set_position(i, laplacian_center);
            }
        }
    }
}

void cppgl::laplacian_depth_smoothing(
        cppgl::GeometryBase &geo, uint32_t iter, bool smooth_normals,
        vec3 view_pos, float constribution) {
    for (unsigned int k = 0; k < iter; k++) {
        std::map<uint32_t, Laplacian> laplacian_container;
        std::map<uint32_t, Laplacian> laplacian_normal_container;

        for (unsigned int i = 0; i < geo->positions_size(); i++) {
            laplacian_container[i] = Laplacian();
            if (smooth_normals)
                laplacian_normal_container[i] = Laplacian();
        }
        for (unsigned int i = 0; i < geo->indices_size(); i = i + 3) {
            for (unsigned int j = 0; j < 3; j++) {
                int index_cur = geo->get_index(i + (j % 3));
                int index_next = geo->get_index(i + ((j + 1) % 3));

                laplacian_container[index_cur].sum += geo->get_position(index_next);
                laplacian_container[index_next].sum += geo->get_position(index_cur);
                laplacian_container[index_cur].count++;
                laplacian_container[index_next].count++;

                if (smooth_normals) {
                    laplacian_normal_container[index_cur].sum +=
                            geo->get_normal(index_next);
                    laplacian_normal_container[index_next].sum +=
                            geo->get_normal(index_cur);
                    laplacian_normal_container[index_cur].count++;
                    laplacian_normal_container[index_next].count++;
                }
            }
        }
        for (unsigned int i = 0; i < geo->positions_size(); i++) {
            if (laplacian_container[i].count > 0) {
                vec3 laplacian_center =
                        laplacian_container[i].sum / laplacian_container[i].count;
                vec3 view_ray = (geo->get_position(i) - view_pos).normalized();
                float offset = view_ray.dot(laplacian_center - geo->get_position(i));
                geo->set_position(
                        i, geo->get_position(i) + view_ray * offset * constribution);

                if (smooth_normals) {
                    vec3 laplacian_center_normal = laplacian_normal_container[i].sum /
                                                   laplacian_normal_container[i].count;
                    geo->set_normal(i, laplacian_center_normal);
                }
            }
        }
    }
}