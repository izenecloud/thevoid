#ifndef IZENECLOUD_THEVOID_HTTP_RESPONSE_H
#define IZENECLOUD_THEVOID_HTTP_RESPONSE_H

#include <swarm/http_response.hpp>

namespace izenecloud {
namespace thevoid {

class http_response : public swarm::http_response
{
public:
	http_response();
	http_response(const boost::none_t &);
	http_response(http_response &&other);
	http_response(const http_response &other);
	~http_response();

	http_response &operator =(http_response &&other);
	http_response &operator =(const http_response &other);
};

} // namespace thevoid
} // namespace izenecloud

#endif // IZENECLOUD_THEVOID_HTTP_RESPONSE_H
