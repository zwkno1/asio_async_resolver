#pragma once

#include <memory>

#include <cares/detail/config.hpp>
#include <cares/detail/error.hpp>
#include <cares/detail/resolver_op.hpp>

namespace cares {
namespace detail {

template <typename T>
class resolver_service_base : public asio::io_context::service
{
public:
    static asio::io_context::id id;
    resolver_service_base(asio::io_context& io_context)
      : asio::io_context::service(io_context)
    {
        error_code ec(::ares_library_init(ARES_LIB_INIT_ALL), get_error_category());
        asio::detail::throw_error(ec, "ares_library_init");
    }

    void shutdown_service() { ::ares_library_cleanup(); }
};

template <typename T>
asio::io_context::id resolver_service_base<T>::id;

using resolver_service = resolver_service_base<void>;

} // namespace detail
} // namespace cares
