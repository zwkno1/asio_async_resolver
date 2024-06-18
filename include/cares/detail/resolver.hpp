#pragma once

#include <cares/detail/config.hpp>
#include <cares/detail/iterator.hpp>
#include <cares/detail/resolver_service.hpp>

namespace cares {
namespace detail {

class resolver
{
public:
    resolver(asio::io_context& io_context)
      : io_context_(io_context)
      , service_(asio::use_service<resolver_service>(io_context))
    {
    }

    inline asio::io_context& get_io_context() { return io_context_; }

    template <typename Callback>
    void async_resolve_v4(const char* host, Callback&& callback)
    {
        auto op = std::make_shared<detail::resolver_op>(io_context_);
        op->async_resolve_v4(host, std::forward<Callback>(callback));
    }

    template <typename Callback>
    void async_resolve_v6(const char* host, Callback&& callback)
    {
        auto op = std::make_shared<detail::resolver_op>(io_context_);
        op->async_resolve_v6(host, std::forward<Callback>(callback));
    }

private:
    asio::io_context& io_context_;

    resolver_service& service_;
};

} // namespace detail
} // namespace cares
