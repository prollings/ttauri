# Copyright Take Vos 2020.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

target_sources(ttauri PRIVATE
    base_n.hpp
    gzip.cpp
    gzip.hpp
    inflate.cpp
    inflate.hpp
    JSON.cpp
    JSON.hpp
    png.cpp
    png.hpp
    SHA2.hpp
    zlib.cpp
    zlib.hpp
    BON8.hpp
    BON8.cpp
)

target_sources(ttauri_tests PRIVATE
    JSON_tests.cpp
    gzip_tests.cpp
    base_n_tests.cpp
    SHA2_tests.cpp
)
