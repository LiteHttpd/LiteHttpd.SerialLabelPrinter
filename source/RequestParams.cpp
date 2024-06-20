#include <RequestParams.h>

int RequestParams::getResponseCode() const {
	return this->responseCode;
}

void RequestParams::reply(
	int code, const std::vector<char>& data) const {
	this->responseCode = code;
	this->replyFunc(this->content, code, data);
}

void RequestParams::replyStart(int code, const std::vector<char>& data) const {
	this->responseCode = code;
	this->replyStartFunc(this->content, code);
}

void RequestParams::replyData(const std::vector<char>& data) const {
	this->replyDataFunc(this->content, data);
}

void RequestParams::replyEnd() const {
	this->replyEndFunc(this->content);
}

void RequestParams::addHeader(
	const std::string& key, const std::string& value) const {
	this->addHeaderFunc(this->content, key, value);
}

void RequestParams::log(LogLevel level, const std::string& data) const {
	this->logFunc(level, data);
}

const RequestParams::FPMResult RequestParams::callFPM(
	const std::string& addr, uint16_t port,
	const std::vector<char>& data, const ParamList& params) const {
	return this->fpmFunc(addr, port, data, params);
}
