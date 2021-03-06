// Copyright Take Vos 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#pragma once


#include "required.hpp"
#include <emmintrin.h>
#include <wmmintrin.h>
#include <utility>
#include <array>
#include <type_traits>

namespace tt {

[[nodiscard]] inline size_t hash_mix_two(size_t hash1, size_t hash2) noexcept
{
    ttlet round = _mm_set_epi64x(0x123456789abcdef0ULL, 0x0fedcba987654321ULL);

    auto hash = _mm_set_epi64x(hash1, hash2);
    hash = _mm_aesenc_si128(hash, round);
    hash = _mm_aesenc_si128(hash, round);

    std::array<uint64_t,2> buffer;
    _mm_storeu_si64(buffer.data(), hash);
    return buffer[0];
}

template<typename First, typename Second, typename... Args>
[[nodiscard]] size_t hash_mix(First &&first, Second &&second, Args &&... args) noexcept {
    if constexpr (sizeof...(args) == 0) {
        return hash_mix_two(
            std::hash<std::remove_cvref_t<First>>{}(std::forward<First>(first)),
            std::hash<std::remove_cvref_t<Second>>{}(std::forward<Second>(second))
        );
    } else {
        return hash_mix_two(
            std::hash<std::remove_cvref_t<First>>{}(std::forward<First>(first)),
            hash_mix(std::forward<Second>(second), std::forward<Args>(args)...)
        );
    }
}


}