#pragma once

#include <string>
#include <map>
#include <functional>
#include <vector>

class RequestParams final {
public:
	RequestParams() = default;

public:
	enum class ProtocolType {
		HTTP, HTTPS
	};
	ProtocolType protocol = ProtocolType::HTTP;

	std::string addr;
	uint16_t port = 0;

	std::string path;
	std::map<std::string, std::string> params;

	enum class MethodType {
		GET, HEAD, POST, PUT, DELETE_, CONNECT, OPTIONS, TRACE, PATCH,
		PROPFIND, PROPPATCH, MKCOL, LOCK, UNLOCK, COPY, MOVE
	};
	MethodType method = MethodType::GET;

	std::map<std::string, std::string> headers;
	std::vector<char> data;

	std::string peerAddr;
	uint16_t peerPort = 0;

public:
	int getResponseCode() const;

public:
	void reply(int code, const std::vector<char>& data) const;
	void addHeader(const std::string& key, const std::string& value) const;

private:
	friend class RequestParamsBuilder;

	void* content = nullptr;
	using ReplyFunc = std::function<void(void*, int, const std::vector<char>&)>;
	using AddHeaderFunc = std::function<void(void*, const std::string&, const std::string&)>;

	ReplyFunc replyFunc;
	AddHeaderFunc addHeaderFunc;

	mutable int responseCode = 0;
};
