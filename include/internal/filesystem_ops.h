#ifndef INTERNAL_UTILS_H
#define INTERNAL_UTILS_H

#include <vector>

#include "filesystem.h"
#include "named_handle.h"

namespace fs = std::filesystem;

#include <vector>

CPPGL_NAMESPACE_BEGIN

    namespace utils {
        void CreateDirectories(const fs::path &path);

        void CreateDirectories(const std::vector<fs::path> &paths);

        fs::path SubtractPaths(const fs::path &upper, const fs::path &lower);
    } // namespace utils

CPPGL_NAMESPACE_END

#endif