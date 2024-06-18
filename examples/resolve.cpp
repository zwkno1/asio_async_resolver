#include <iostream>

#include <boost/asio.hpp>
#include <cares/resolver.hpp>

int main(int argc, char* argv[])
{
    boost::asio::io_context ioc;
    cares::resolver resolver(ioc);
    for (int i = 1; i < argc; ++i) {
        const char* host = argv[i];
        std::cout << "start resolve: " << host << std::endl;
        auto callback = [host](const boost::system::error_code& ec, cares::iterator iter) {
            if (ec) {
                std::cerr << "resolve " << host << " failure: " << ec.message() << std::endl;
                return;
            }

            for (; iter != cares::iterator{}; ++iter) {
                std::cout << host << ": " << *iter << std::endl;
            }
        };
        resolver.async_resolve_v4(host, callback);
        resolver.async_resolve_v6(host, callback);
    }
    ioc.run();
}
