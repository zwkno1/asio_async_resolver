#pragma once

#include <iterator>
#include <cares/detail/config.hpp>

namespace cares 
{
namespace detail
{

class iterator : public std::iterator<std::forward_iterator_tag, asio::ip::address>
{
    static char * const * null_data()
    {
        static char * const data[1] = { nullptr };
        return data;
    }

public:
    iterator(char * const * data = null_data(), int type = 0)
        : data_(data)
        , type_(type)
    {
    }

    iterator & operator++()
	{
        ++data_;
		return *this;
    }

    bool operator==(const iterator & rhs)
	{
        return (*data_ == *rhs.data_);
    }

    bool operator!=(const iterator & rhs)
	{
		return !(operator==(rhs));
    }

    asio::ip::address operator*()
    {
        if(type_ == AF_INET)
        {
            asio::ip::address_v4::bytes_type buf;
            std::memcpy(buf.data(), *data_, buf.size());
            return asio::ip::make_address_v4(buf);
        }
        else if (type_ == AF_INET6)
        {
            asio::ip::address_v6::bytes_type buf;
            std::memcpy(buf.data(), *data_, buf.size());
            return asio::ip::make_address_v6(buf);
        }
        return asio::ip::address{};
    }

private:
    char * const * data_;

    int type_;
};

}
}

