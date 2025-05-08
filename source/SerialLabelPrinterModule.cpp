#include "SerialLabelPrinterModule.h"

#include <CJsonObject.hpp>
#include <regex>

#include "Defines.h"
#include "Pages.h"

void SerialLabelPrinterModule::processRequest(const RequestParams& rp) {
	/** Root */
	if (rp.path == "/") {
		if (rp.method == RequestParams::MethodType::GET) {
			/** Send Welcome Page */
			rp.addHeader(HEADER_MIME_TYPE, MIME_HTML);
			rp.log(RequestParams::LogLevel::INFO, LOG_WELCOME);
			rp.reply(200,
				SerialLabelPrinterModule::createWelcomePage());
			return;
		}
		
		/** Send Method Error */
		rp.addHeader(HEADER_MIME_TYPE, MIME_JSON);
		rp.log(RequestParams::LogLevel::WARNING, LOG_WELCOME);
		rp.reply(405,
			SerialLabelPrinterModule::createErrorMessage(
				ERROR_UNAVAILABLE_METHOD, MESSAGE_METHOD_ERROR));
		return;
	}

	/** Coffee */
	if (rp.path == "/coffee") {
		/** Send Teapot Page */
		rp.addHeader(HEADER_MIME_TYPE, MIME_HTML);
		rp.log(RequestParams::LogLevel::WARNING, LOG_TEAPOT);
		rp.reply(418,
			SerialLabelPrinterModule::createTeapotPage());
		return;
	}

	/** List */
	if (rp.path == "/list") {
		if (rp.method == RequestParams::MethodType::GET) {
			/** Get Device List */
			auto [result, list] = Device::getDeviceList();

			/** Send Device List */
			if (result) {
				rp.addHeader(HEADER_MIME_TYPE, MIME_JSON);
				rp.log(RequestParams::LogLevel::INFO,
					LOG_DEVICE_LIST + std::to_string(list.size()));
				rp.reply(200,
					SerialLabelPrinterModule::createDeviceListMessage(list));
				return;
			}
			
			/** Send Device List Error */
			rp.addHeader(HEADER_MIME_TYPE, MIME_JSON);
			rp.log(RequestParams::LogLevel::WARNING, LOG_LIST_ERROR);
			rp.reply(500,
				SerialLabelPrinterModule::createErrorMessage(
					ERROR_LIST_ERROR, MESSAGE_LIST_ERROR));
			return;
		}

		/** Send Method Error */
		rp.addHeader(HEADER_MIME_TYPE, MIME_JSON);
		rp.log(RequestParams::LogLevel::WARNING, LOG_WELCOME);
		rp.reply(405,
			SerialLabelPrinterModule::createErrorMessage(
				ERROR_UNAVAILABLE_METHOD, MESSAGE_METHOD_ERROR));
		return;
	}

	/** Device */
	if (rp.path == "/device") {
		if (rp.method == RequestParams::MethodType::GET) {
			/** Get Device Port */
			auto pit = rp.params.find("port");
			if (pit == rp.params.end()) {
				/** Send Query Error */
				rp.addHeader(HEADER_MIME_TYPE, MIME_JSON);
				rp.log(RequestParams::LogLevel::WARNING, LOG_QUERY_ERROR "Missed param: port");
				rp.reply(400,
					SerialLabelPrinterModule::createErrorMessage(
						ERROR_QUERY_ERROR, MESSAGE_PARAM_MISSED "port"));
				return;
			}

			/** Check Device Port */
			if (!SerialLabelPrinterModule::checkDevicePort(pit->second)) {
				/** Send Query Error */
				rp.addHeader(HEADER_MIME_TYPE, MIME_JSON);
				rp.log(RequestParams::LogLevel::WARNING, LOG_QUERY_ERROR "Unavailable port: " + pit->second);
				rp.reply(400,
					SerialLabelPrinterModule::createErrorMessage(
						ERROR_QUERY_ERROR, MESSAGE_PORT_UNAVAILABLE + pit->second));
				return;
			}

			/** Get Device Status */
			auto status = Device::getDeviceStatus(pit->second);
			if (status != Device::DeviceResult::OK) {
				/** Get Error Messsage */
				auto error = Device::getResultDescription(status);

				/** Send Device Error */
				rp.addHeader(HEADER_MIME_TYPE, MIME_JSON);
				rp.log(RequestParams::LogLevel::WARNING, LOG_DEVICE_ERROR + error);
				rp.reply(503,
					SerialLabelPrinterModule::createErrorMessage(-(int)status, error));
				return;
			}

			/** Get Device Name */
			auto name = Device::getDeviceName(pit->second);

			/** Send Device Name */
			rp.addHeader(HEADER_MIME_TYPE, MIME_TEXT);
			rp.log(RequestParams::LogLevel::INFO, LOG_DEVICE_NAME + name);
			rp.reply(200, SerialLabelPrinterModule::toMemBlock(name));
			return;
		}
	}

	/** Send 404 */
	rp.addHeader(HEADER_MIME_TYPE, MIME_JSON);
	rp.log(RequestParams::LogLevel::WARNING, LOG_PATH_ERROR);
	rp.reply(404,
		SerialLabelPrinterModule::createErrorMessage(
			ERROR_UNAVAILABLE_PATH, MESSAGE_PATH_ERROR));
}

const std::vector<char> SerialLabelPrinterModule::createErrorMessage(
	int code, const std::string& mes) {
	neb::CJsonObject obj;
	obj.Add("code", code);
	obj.Add("message", mes);

	auto str = obj.ToString();
	return SerialLabelPrinterModule::toMemBlock(str);
}

const std::vector<char> SerialLabelPrinterModule::createDeviceListMessage(
	const DeviceList& list) {
	neb::CJsonObject array;

	for (auto& [port, name] : list) {
		neb::CJsonObject obj;
		obj.Add("id", port);
		obj.Add("name", name);

		array.Add(obj);
	}

	auto str = array.ToString();
	return SerialLabelPrinterModule::toMemBlock(str);
}

const std::vector<char> SerialLabelPrinterModule::createWelcomePage() {
	return SerialLabelPrinterModule::toMemBlock(htmlPage);
}

const std::vector<char> SerialLabelPrinterModule::createTeapotPage() {
	return SerialLabelPrinterModule::toMemBlock(html418);
}

const std::vector<char> SerialLabelPrinterModule::toMemBlock(const std::string& str) {
	std::vector<char> mem;
	mem.resize(str.size());
	std::memcpy(mem.data(), str.c_str(), str.size());

	return mem;
}

bool SerialLabelPrinterModule::checkDevicePort(const std::string& port) {
	std::regex comRegex("^COM([1-9][0-9]*)$");
	return std::regex_match(port, comRegex);
}

LITEHTTPD_MODULE(SerialLabelPrinterModule)
