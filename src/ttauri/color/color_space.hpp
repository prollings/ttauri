// Copyright Take Vos 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "../numeric_array.hpp"
#include "../geometry/matrix.hpp"
#include "../geometry/scale.hpp"

namespace tt {

[[nodiscard]] constexpr matrix3
color_primaries_to_RGBtoXYZ(float wx, float wy, float rx, float ry, float gx, float gy, float bx, float by) noexcept
{
    ttlet w = f32x4{wx, wy, 1.0f - wx - wy};
    ttlet r = f32x4{rx, ry, 1.0f - rx - ry};
    ttlet g = f32x4{gx, gy, 1.0f - gx - gy};
    ttlet b = f32x4{bx, by, 1.0f - bx - by};

    // Calculate whitepoint's tristimulus values from coordinates
    ttlet W = f32x4{1.0f * (w.x() / w.y()), 1.0f, 1.0f * (w.z() / w.y())};

    // C is the chromaticity matrix.
    ttlet C = matrix3{r, g, b};

    // solve tristimulus sums.
    ttlet S = scale3{(~C * W).xyz1()};

    return C * S;
}

} // namespace tt
