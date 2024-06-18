#pragma once

#include <ares.h>

#include <cares/detail/config.hpp>

namespace cares {
namespace detail {

class cares_error_category : public error_category
{
public:
    const char* name() const noexcept { return "c-ares"; }

    std::string message(int rc) const noexcept { return ::ares_strerror(rc); }
};

inline detail::cares_error_category& get_error_category()
{
    static detail::cares_error_category category;
    return category;
}

} // namespace detail
} // namespace cares
