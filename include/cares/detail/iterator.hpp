#pragma once

#include <ares.h>
#include <netinet/in.h>

#include <cares/detail/config.hpp>

namespace cares {
namespace detail {

class iterator
{
public:
    using difference_type = std::ptrdiff_t;

    using value_type = asio::ip::address;

    iterator(ares_addrinfo_node* addr = nullptr)
      : addr_(addr)
    {
    }

    iterator& operator++()
    {
        if (addr_) {
            addr_ = addr_->ai_next;
        }
        return *this;
    }

    bool operator==(const iterator& rhs) { return (addr_ == rhs.addr_); }

    bool operator!=(const iterator& rhs) { return !(operator==(rhs)); }

    asio::ip::address operator*() const
    {
        if (addr_->ai_family == AF_INET) {
            asio::ip::address_v4::bytes_type buf;
            auto a = (sockaddr_in*)addr_->ai_addr;
            std::memcpy(buf.data(), &a->sin_addr.s_addr, buf.size());
            return asio::ip::make_address_v4(buf);
        } else if (addr_->ai_family == AF_INET6) {
            asio::ip::address_v6::bytes_type buf;
            auto a = (sockaddr_in6*)addr_->ai_addr;
            std::memcpy(buf.data(), &a->sin6_addr, buf.size());
            return asio::ip::make_address_v6(buf);
        }
        return asio::ip::address{};
    }

private:
    ares_addrinfo_node* addr_;
};

} // namespace detail
} // namespace cares
