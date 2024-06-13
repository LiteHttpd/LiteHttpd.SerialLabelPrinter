#include <RequestParams.h>

void RequestParams::reply(
	int code, const std::vector<char>& data) const {
	this->replyFunc(this->content, code, data);
}

void RequestParams::addHeader(
	const std::string& key, const std::string& value) const {
	this->addHeaderFunc(this->content, key, value);
}
