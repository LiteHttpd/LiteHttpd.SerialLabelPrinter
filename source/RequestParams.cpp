#include <RequestParams.h>

int RequestParams::getResponseCode() const {
	return this->responseCode;
}

void RequestParams::reply(
	int code, const std::vector<char>& data) const {
	this->responseCode = code;
	this->replyFunc(this->content, code, data);
}

void RequestParams::addHeader(
	const std::string& key, const std::string& value) const {
	this->addHeaderFunc(this->content, key, value);
}
