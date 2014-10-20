#include <thevoid/server.hpp>
#include <thevoid/stream.hpp>

using namespace izenecloud;

class http_server : public thevoid::server<http_server>
{
public:
	virtual bool initialize(const rapidjson::Value &config) {
		(void) config;

		on<on_ping>(
			options::exact_match("/ping"),
			options::methods("GET")
		);
		on<on_timeout>(
			options::exact_match("/timeout"),
			options::methods("GET")
		);
		on<on_get>(
			options::exact_match("/get"),
			options::methods("GET")
		);
		on<on_echo>(
			options::exact_match("/echo"),
			options::methods("GET")
		);
		on<on_ping>(
			options::exact_match("/header-check"),
			options::methods("GET"),
			options::header("X-CHECK", "SecretKey")
		);
	
		return true;
	}

	struct on_ping : public thevoid::simple_request_stream<http_server> {
		virtual void on_request(const thevoid::http_request &req, const boost::asio::const_buffer &buffer) {
			(void) buffer;
			(void) req;

			this->send_reply(thevoid::http_response::ok);
		}
	};

	struct on_timeout : public thevoid::simple_request_stream<http_server> {
		virtual void on_request(const thevoid::http_request &req, const boost::asio::const_buffer &buffer) {
			(void) buffer;
			(void) req;
			if (auto timeout = req.url().query().item_value("timeout")) {
				BH_LOG(logger(), SWARM_LOG_INFO, "timeout: %s", timeout->c_str());
				usleep(atoi(timeout->c_str()) * 1000);
			}

			this->send_reply(thevoid::http_response::ok);
		}
	};

	struct on_get : public thevoid::simple_request_stream<http_server> {
		virtual void on_request(const thevoid::http_request &req, const boost::asio::const_buffer &buffer) {
			(void) buffer;

			std::string data;
			if (auto datap = req.url().query().item_value("data"))
				data = *datap;

			int timeout_ms = 10 + (rand() % 10);
			usleep(timeout_ms * 1000);

			thevoid::http_response reply;
			reply.set_code(thevoid::http_response::ok);
			reply.headers().set_content_length(data.size());

			this->send_reply(std::move(reply), std::move(data));
		}
	};

	struct on_echo : public thevoid::simple_request_stream<http_server> {
		virtual void on_request(const thevoid::http_request &req, const boost::asio::const_buffer &buffer) {
			auto data = boost::asio::buffer_cast<const char*>(buffer);
			auto size = boost::asio::buffer_size(buffer);

			thevoid::http_response reply;
			reply.set_code(thevoid::http_response::ok);
			reply.set_headers(req.headers());
			reply.headers().set_content_length(size);

			this->send_reply(std::move(reply), std::string(data, size));
		}
	};
};

int main(int argc, char **argv)
{
	return izenecloud::thevoid::run_server<http_server>(argc, argv);
}
