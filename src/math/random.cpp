/**
 * Copyright (c) 2021 Darius Rückert
 * Licensed under the MIT License.
 * See LICENSE file for more information.
 *    MIT License
 *
 *    Copyright (c) 2021 Darius Rückert <darius.rueckert@fau.de>
 *
 *    Permission is hereby granted, free of charge, to any person obtaining a copy
 *            of this software and associated documentation files (the "Software"), to deal
 *    in the Software without restriction, including without limitation the rights
 *            to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *            copies of the Software, and to permit persons to whom the Software is
 *    furnished to do so, subject to the following conditions:
 *
 *    The above copyright notice and this permission notice shall be included in all
 *            copies or substantial portions of the Software.
 *
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *            AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *    SOFTWARE.
 */

#include <data_types.h>
#include <math/eigen_glm_interface.h>
#include <math/random.h>

#include <chrono>
#include <random>

#include "cassert.h"

CPPGL_NAMESPACE_BEGIN

    namespace random {
        inline std::mt19937 &generator() {
            static thread_local std::mt19937 gen(
                    std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
            return gen;
        }

        void setSeed(uint64_t seed) { generator().seed(seed); }

        bool sampleBool(double s) {
            // we need this because the line below is 'inclusive'
            if (s == 1)
                return true;
            return sampleDouble(0, 1) < s;
        }

        double sampleDouble(double min, double max) {
            std::uniform_real_distribution<double> dis(min, max);
            return dis(generator());
        }

        float sampleFloat(float min, float max) {
            std::uniform_real_distribution<float> dis(min, max);
            return dis(generator());
        }

        int rand() {
            std::uniform_int_distribution<int> dis(0, std::numeric_limits<int>::max());
            return dis(generator());
        }

        uint64_t urand64() {
            std::uniform_int_distribution<uint64_t> dis(
                    0, std::numeric_limits<uint64_t>::max());
            return dis(generator());
        }

        int uniformInt(int low, int high) {
            std::uniform_int_distribution<int> dis(low, high);
            return dis(generator());
        }

        double gaussRand(double mean, double stddev) {
            std::normal_distribution<double> dis(mean, stddev);
            return dis(generator());
        }

        std::vector<int> uniqueIndices(int sampleCount, int indexSize) {
            _CPPGL_ASSERT_LE(sampleCount, indexSize);

            std::vector<bool> used(indexSize, false);
            std::vector<int> data(sampleCount);

            for (int j = 0; j < sampleCount;) {
                int s = uniformInt(0, indexSize - 1);
                if (!used[s]) {
                    data[j] = s;
                    used[s] = true;
                    j++;
                }
            }
            return data;
        }

        uint64_t generateTimeBasedSeed() {
            uint64_t time =
                    std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

            std::mt19937 gen(time);
            std::uniform_int_distribution<uint64_t> dis(
                    0, std::numeric_limits<uint64_t>::max());
            for (int i = 0; i < 100; ++i) {
                time = dis(gen);
            }
            return time;
        }

        Vec3 ballRand(double radius) {
            _CPPGL_ASSERT_GE(radius, 0);

            // Credits to random.inl from the glm library
            auto r2 = radius * radius;
            Vec3 low(-radius, -radius, -radius);
            Vec3 high(radius, radius, radius);
            double lenRes;
            Vec3 result;
            do {
                result = linearRand(low, high);
                lenRes = result.squaredNorm();
            } while (lenRes > r2);
            return result;
        }

        Vec3 sphericalRand(double radius) {
            float z = random::sampleDouble(-1.0, 1.0);
            float a = random::sampleDouble(0.0, M_PI * 2.0);

            float r = sqrt(1.0 - z * z);

            float x = r * cos(a);
            float y = r * sin(a);

            return Vec3(x, y, z) * radius;
        }

    } // namespace random

    vec2 diskRand(float Radius) {
        vec2 Result(0, 0);
        float LenRadius = 0;

        do {
            Result = linearRand(make_vec2(-Radius), make_vec2(Radius));
            LenRadius = length(Result);
        } while (LenRadius > Radius);

        return Result;
    }

CPPGL_NAMESPACE_END