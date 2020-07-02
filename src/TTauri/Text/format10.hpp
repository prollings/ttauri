// Copyright 2020 Pokitec
// All rights reserved.

#pragma once

#include <fmt/format.h>
#include <utility>
#include <tuple>
#include <memory>
#include <string>
#include <locale>
#include "translation.hpp"
#include "TTauri/Foundation/type_traits.hpp"

namespace tt {

class format10_base
{
protected:
    std::string fmt;

public:
    format10_base(std::string_view fmt) noexcept :
        fmt(std::string{fmt}) {}

    virtual ~format10_base() = default;

    virtual operator std::string () const noexcept = 0;

    virtual std::unique_ptr<format10_base> make_unique_copy() const noexcept = 0;

    [[nodiscard]] virtual bool equal_to(format10_base &other) const noexcept = 0;
};

/** A c++20 standard conforming std::format() for proper function overload resolution.
 */
template<typename... Args>
std::string cpp20_format(std::locale const &locale, std::string_view fmt, Args const &... args)
{
    return fmt::format(fmt, args...);
}


template<typename... Args>
class format10_impl : public format10_base
{
    std::tuple<remove_cvref_t<Args>...> args;

    using format_func_type = std::string(*)(std::locale const &, std::string_view, remove_cvref_t<Args> const &...);
    using make_unique_type = std::unique_ptr<format10_impl>(*)(std::string_view const &, remove_cvref_t<Args> const &...);

public:
    format10_impl(std::string_view fmt, Args const &... args) noexcept :
        format10_base(fmt),
        args(args...) {}

    operator std::string () const noexcept override {
        auto locale = std::locale{};
        auto translated_fmt = get_translation(fmt);

        format_func_type format_func = cpp20_format;
        return std::apply(format_func, std::tuple_cat(std::tuple(locale, translated_fmt), args));
    }

    std::unique_ptr<format10_base> make_unique_copy() const noexcept override {
        make_unique_type unique_constructor = std::make_unique<format10_impl>;
        return std::apply(unique_constructor, std::tuple_cat(std::tuple(fmt), args));
    }

    [[nodiscard]] bool equal_to(format10_base &other) const noexcept override {
        auto *other_ = dynamic_cast<format10_impl *>(&other);
        return other_ && this->fmt == other_->fmt && this->args == other_->args;
    }
};

template<>
class format10_impl<> : public format10_base
{
public:

    format10_impl(std::string_view fmt) noexcept :
        format10_base(fmt) {}

    operator std::string () const noexcept override {
        return std::string{get_translation(fmt)};
    }

    std::unique_ptr<format10_base> make_unique_copy() const noexcept override {
        return std::make_unique<format10_impl>(fmt);
    }

    [[nodiscard]] bool equal_to(format10_base &other) const noexcept override {
        auto *other_ = dynamic_cast<format10_impl *>(&other);
        return other_ && this->fmt == other_->fmt;
    }
};

class format10 {
    std::unique_ptr<format10_base> impl;

public:
    format10() noexcept :
        impl(std::make_unique<format10_impl<>>(""s)) {}

    template<typename... Args>
    format10(std::string_view fmt, Args const &... args) noexcept :
        impl(std::make_unique<format10_impl<Args...>>(fmt, args...)) {}

    format10(format10 &&other) noexcept = default;
    format10 &operator=(format10 &&other) noexcept = default;

    format10(format10 const &other) noexcept :
        impl(other.impl->make_unique_copy()) {
        tt_assume(other.impl);
    }

    format10 &operator=(format10 const &other) noexcept {
        tt_assume(other.impl);
        impl = other.impl->make_unique_copy();
        return *this;
    }

    operator std::string () const noexcept {
        tt_assume(impl);
        return static_cast<std::string>(*impl);
    }

    [[nodiscard]] friend bool operator==(format10 const &lhs, format10 const &rhs) noexcept {
        tt_assume(lhs.impl);
        tt_assume(rhs.impl);
        return lhs.impl->equal_to(*rhs.impl);
    }

    [[nodiscard]] friend bool operator!=(format10 const &lhs, format10 const &rhs) noexcept {
        return !(lhs == rhs);
    }

};

using format10p = format10;

}

// Add specialization for observable_cast
#include "detail/observable_cast_format10.hpp"
