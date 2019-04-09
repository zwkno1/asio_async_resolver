
#include <boost/asio.hpp>
#include <memory>

namespace cares
{
namespace detail
{

namespace asio = boost::asio;

using error_code = boost::system::error_code;
using error_category = boost::system::error_category;

using std::shared_ptr;
using std::enable_shared_from_this;
using std::make_shared;

}
}
