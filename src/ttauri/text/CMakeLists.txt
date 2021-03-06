# Copyright Take Vos 2020-2021.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)


target_sources(ttauri PRIVATE
    attributed_glyph.cpp
    attributed_glyph.hpp
    attributed_glyph_line.hpp
    attributed_grapheme.hpp
    code_point_iterator.hpp
    editable_text.hpp
    elusive_icon.hpp
    font.cpp
    font.hpp
    font_book.cpp
    font_book.hpp
    font_description.hpp
    font_family_id.hpp
    font_glyph_ids.cpp
    font_glyph_ids.hpp
    font_grapheme_id.hpp
    font_id.hpp
    font_variant.hpp
    font_weight.hpp
    glyph_id.hpp
    glyph_metrics.hpp
    grapheme.cpp
    grapheme.hpp
    grapheme_iterator.hpp
    gstring.cpp
    gstring.hpp
    language.cpp
    language.hpp
    $<${TT_MACOS}:${CMAKE_CURRENT_SOURCE_DIR}/language_macos.mm>
    $<${TT_WIN32}:${CMAKE_CURRENT_SOURCE_DIR}/language_win32.cpp>
    po_parser.cpp
    po_parser.hpp
    shaped_text.cpp
    shaped_text.hpp
    text_decoration.hpp
    text_style.cpp
    text_style.hpp
    translation.cpp
    translation.hpp
    true_type_font.cpp
    true_type_font.hpp
    ttauri_icon.hpp
    unicode_bidi.cpp
    unicode_bidi.hpp
    unicode_bidi_bracket_type.hpp
    unicode_bidi_class.hpp
    unicode_composition.cpp
    unicode_composition.hpp
    unicode_description.cpp
    unicode_description.hpp
    unicode_general_category.hpp
    unicode_grapheme_cluster_break.hpp
    unicode_normalization.cpp
    unicode_normalization.hpp
    unicode_text_segmentation.cpp
    unicode_text_segmentation.hpp
    unicode_ranges.cpp
    unicode_ranges.hpp
)


target_sources(ttauri_tests PRIVATE
    unicode_bidi_tests.cpp
    unicode_text_segmentation_tests.cpp
    unicode_normalization_tests.cpp
)
