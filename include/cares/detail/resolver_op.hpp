#pragma once

#include <array>

#include <ares.h>

#include <cares/detail/config.hpp>
#include <cares/detail/error.hpp>
#include <cares/detail/iterator.hpp>

namespace cares
{
namespace detail
{

class resolver_op : public enable_shared_from_this<resolver_op>
{
public:
    resolver_op(asio::io_context & io_context)
        : io_context_(io_context)
	{
        ec_.assign(::ares_init(&channel_), get_error_category());
    }

	~resolver_op()
	{
		::ares_destroy(channel_);
    }

    template <typename T>
    void async_resolve_v4(const char * host, T && callback)
    {
        if(ec_)
        {
            io_context_.post(std::bind(std::forward<T>(callback), ec_, iterator{}));
        }
        callback_ = std::forward<T>(callback);
        ::ares_gethostbyname(channel_, host, AF_INET, &on_resolve, this);
        start();
    }

    template <typename T>
    void async_resolve_v6(const char * host, T && callback)
    {
        if(ec_)
        {
            io_context_.post(std::bind(std::forward<T>(callback), ec_, iterator{}));
        }
        callback_ = std::forward<T>(callback);
        ::ares_gethostbyname(channel_, host, AF_INET6, &on_resolve, this);
        start();
    }

    static void on_resolve(void *arg, int status, int timeouts, struct hostent * h)
	{
        auto c = reinterpret_cast<resolver_op *>(arg);
        c->callback_(error_code{status, get_error_category()}, h ? iterator{h->h_addr_list, h->h_addrtype} : iterator{});
    }

    void start()
	{
        auto self = shared_from_this();

        std::array<ares_socket_t, ARES_GETSOCK_MAXNUM> sockets = {{0}};
        int bitmask = ::ares_getsock(channel_, sockets.data(), sockets.size());
		for (int i = 0; i < ARES_GETSOCK_MAXNUM; ++i)
		{
            if (ARES_GETSOCK_READABLE(bitmask, i) || ARES_GETSOCK_WRITABLE(bitmask, i))
            {
                auto sock = make_shared<asio::posix::stream_descriptor>(io_context_, sockets[i]);

				if (ARES_GETSOCK_READABLE(bitmask, i))
				{
                    sock->async_wait(asio::posix::stream_descriptor::wait_read, [self, sock](const error_code & ec)
                    {
                        self->handle_read(ec, sock);
                    });
                }

				if (ARES_GETSOCK_WRITABLE(bitmask, i))
                {
                    sock->async_wait(asio::posix::stream_descriptor::wait_write, [self, sock](const error_code & ec)
                    {
                        self->handle_write(ec, sock);
                    });
				}		
			}
		}
    }

    void handle_read(const error_code & ec, shared_ptr<asio::posix::stream_descriptor> sock)
	{
        ::ares_process_fd(channel_, sock->native_handle(), ARES_SOCKET_BAD);
        start();
    }

    void handle_write(const error_code & ec, shared_ptr<asio::posix::stream_descriptor> sock)
    {
        ::ares_process_fd(channel_, ARES_SOCKET_BAD, sock->native_handle());
        start();
    }

private:
    asio::io_context & io_context_;
    ares_channel channel_;

    std::function<void(const error_code &, iterator)> callback_;
    error_code ec_;
};
	
}
}
