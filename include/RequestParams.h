#pragma once

#include <string>
#include <map>
#include <functional>
#include <vector>
#include <tuple>
#include <cstdint>

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
	std::string query;
	using ParamList = std::map<std::string, std::string>;
	ParamList params;

	enum class MethodType {
		GET, HEAD, POST, PUT, DELETE_, CONNECT, OPTIONS, TRACE, PATCH,
		PROPFIND, PROPPATCH, MKCOL, LOCK, UNLOCK, COPY, MOVE
	};
	MethodType method = MethodType::GET;

	ParamList headers;
	std::vector<char> data;

	std::string peerAddr;
	uint16_t peerPort = 0;

public:
	int getResponseCode() const;

public:
	enum class LogLevel {
		DEBUG, INFO, WARNING, ERROR_, CRITICAL
	};

	void reply(int code, const std::vector<char>& data) const;
	void replyStart(int code, const std::vector<char>& data) const;
	void replyData(const std::vector<char>& data) const;
	void replyEnd() const;
	void addHeader(const std::string& key, const std::string& value) const;
	void log(LogLevel level, const std::string& data) const;
	using FPMResult = std::tuple<std::vector<char>, bool>;
	const FPMResult callFPM(const std::string& addr, uint16_t port,
		const std::vector<char>& data, const ParamList& params = ParamList{}) const;

private:
	friend class RequestParamsBuilder;

	void* content = nullptr;
	using ReplyFunc = std::function<void(void*, int, const std::vector<char>&)>;
	using ReplyStartFunc = std::function<void(void*, int)>;
	using ReplyDataFunc = std::function<void(void*, const std::vector<char>&)>;
	using ReplyEndFunc = std::function<void(void*)>;
	using AddHeaderFunc = std::function<void(void*, const std::string&, const std::string&)>;
	using LogFunc = std::function<void(LogLevel, const std::string&)>;
	using FPMFunc = std::function<const FPMResult(const std::string&, uint16_t,
		const std::vector<char>&, const ParamList&)>;

	ReplyFunc replyFunc;
	ReplyStartFunc replyStartFunc;
	ReplyDataFunc replyDataFunc;
	ReplyEndFunc replyEndFunc;
	AddHeaderFunc addHeaderFunc;
	LogFunc logFunc;
	FPMFunc fpmFunc;

	mutable int responseCode = 0;
};
