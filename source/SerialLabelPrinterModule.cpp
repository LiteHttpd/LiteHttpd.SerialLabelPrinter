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
		if (rp.method == RequestParams::MethodType::POST) {
			/** Get Device Port */
			auto portIt = rp.params.find("port");
			if (portIt == rp.params.end()) {
				/** Send Query Error */
				rp.addHeader(HEADER_MIME_TYPE, MIME_JSON);
				rp.log(RequestParams::LogLevel::WARNING, LOG_QUERY_ERROR "Missed param: port");
				rp.reply(400,
					SerialLabelPrinterModule::createErrorMessage(
						ERROR_QUERY_ERROR, MESSAGE_PARAM_MISSED "port"));
				return;
			}

			/** Get Baudrate */
			auto baudIt = rp.params.find("baudrate");
			if (baudIt == rp.params.end()) {
				/** Send Query Error */
				rp.addHeader(HEADER_MIME_TYPE, MIME_JSON);
				rp.log(RequestParams::LogLevel::WARNING, LOG_QUERY_ERROR "Missed param: baudrate");
				rp.reply(400,
					SerialLabelPrinterModule::createErrorMessage(
						ERROR_QUERY_ERROR, MESSAGE_PARAM_MISSED "baudrate"));
				return;
			}

			/** Get Byte Size */
			auto byteIt = rp.params.find("bytesize");
			if (byteIt == rp.params.end()) {
				/** Send Query Error */
				rp.addHeader(HEADER_MIME_TYPE, MIME_JSON);
				rp.log(RequestParams::LogLevel::WARNING, LOG_QUERY_ERROR "Missed param: bytesize");
				rp.reply(400,
					SerialLabelPrinterModule::createErrorMessage(
						ERROR_QUERY_ERROR, MESSAGE_PARAM_MISSED "bytesize"));
				return;
			}

			/** Get Parity */
			auto parityIt = rp.params.find("parity");
			if (parityIt == rp.params.end()) {
				/** Send Query Error */
				rp.addHeader(HEADER_MIME_TYPE, MIME_JSON);
				rp.log(RequestParams::LogLevel::WARNING, LOG_QUERY_ERROR "Missed param: parity");
				rp.reply(400,
					SerialLabelPrinterModule::createErrorMessage(
						ERROR_QUERY_ERROR, MESSAGE_PARAM_MISSED "parity"));
				return;
			}

			/** Get Stop Bits */
			auto stopIt = rp.params.find("stopbits");
			if (stopIt == rp.params.end()) {
				/** Send Query Error */
				rp.addHeader(HEADER_MIME_TYPE, MIME_JSON);
				rp.log(RequestParams::LogLevel::WARNING, LOG_QUERY_ERROR "Missed param: stopbits");
				rp.reply(400,
					SerialLabelPrinterModule::createErrorMessage(
						ERROR_QUERY_ERROR, MESSAGE_PARAM_MISSED "stopbits"));
				return;
			}

			/** Check Device Port */
			if (!SerialLabelPrinterModule::checkDevicePort(portIt->second)) {
				/** Send Query Error */
				rp.addHeader(HEADER_MIME_TYPE, MIME_JSON);
				rp.log(RequestParams::LogLevel::WARNING, LOG_QUERY_ERROR "Unavailable port: " + portIt->second);
				rp.reply(400,
					SerialLabelPrinterModule::createErrorMessage(
						ERROR_QUERY_ERROR, MESSAGE_PORT_UNAVAILABLE + portIt->second));
				return;
			}

			/** Check Baudrate */
			auto [baudValid, baudrate] = SerialLabelPrinterModule::sToU32(baudIt->second);
			if (!baudValid) {
				/** Send Query Error */
				rp.addHeader(HEADER_MIME_TYPE, MIME_JSON);
				rp.log(RequestParams::LogLevel::WARNING, LOG_QUERY_ERROR "Unavailable baudrate: " + baudIt->second);
				rp.reply(400,
					SerialLabelPrinterModule::createErrorMessage(
						ERROR_QUERY_ERROR, MESSAGE_BAUDRATE_UNAVAILABLE + baudIt->second));
				return;
			}

			/** Check Byte Size */
			auto [bytesizeValid, bytesize] = SerialLabelPrinterModule::sToU8(byteIt->second);
			if (!bytesizeValid) {
				/** Send Query Error */
				rp.addHeader(HEADER_MIME_TYPE, MIME_JSON);
				rp.log(RequestParams::LogLevel::WARNING, LOG_QUERY_ERROR "Unavailable bytesize: " + byteIt->second);
				rp.reply(400,
					SerialLabelPrinterModule::createErrorMessage(
						ERROR_QUERY_ERROR, MESSAGE_BYTESIZE_UNAVAILABLE + byteIt->second));
				return;
			}

			/** Check Parity */
			auto [parityValid, parity] = SerialLabelPrinterModule::sToU8(parityIt->second);
			if (!parityValid) {
				/** Send Query Error */
				rp.addHeader(HEADER_MIME_TYPE, MIME_JSON);
				rp.log(RequestParams::LogLevel::WARNING, LOG_QUERY_ERROR "Unavailable parity: " + parityIt->second);
				rp.reply(400,
					SerialLabelPrinterModule::createErrorMessage(
						ERROR_QUERY_ERROR, MESSAGE_PARITY_UNAVAILABLE + parityIt->second));
				return;
			}

			/** Check Stop Bits */
			auto [stopbitsValid, stopbits] = SerialLabelPrinterModule::sToU8(stopIt->second);
			if (!stopbitsValid) {
				/** Send Query Error */
				rp.addHeader(HEADER_MIME_TYPE, MIME_JSON);
				rp.log(RequestParams::LogLevel::WARNING, LOG_QUERY_ERROR "Unavailable stopbits: " + stopIt->second);
				rp.reply(400,
					SerialLabelPrinterModule::createErrorMessage(
						ERROR_QUERY_ERROR, MESSAGE_STOPBITS_UNAVAILABLE + stopIt->second));
				return;
			}

			/** Decode Base64 Data */
			auto [dataValid, data] = SerialLabelPrinterModule::base64Decode(rp.data);
			if (!dataValid) {
				/** Send Data Error */
				rp.addHeader(HEADER_MIME_TYPE, MIME_JSON);
				rp.log(RequestParams::LogLevel::WARNING, LOG_DATA_ERROR);
				rp.reply(415,
					SerialLabelPrinterModule::createErrorMessage(
						ERROR_DATA_ERROR, MESSAGE_DATA_ERROR));
				return;
			}

			/** Create Config */
			Device::Config config = {
				.baudRate = baudrate,
				.byteSize = bytesize,
				.parity = parity,
				.stopBits = stopbits
			};

			/** Send Data to Device */
			auto status = Device::sendData(portIt->second, config, data);
			rp.log(RequestParams::LogLevel::INFO, LOG_SEND_DATA + std::to_string(data.size()));
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
			auto name = Device::getDeviceName(portIt->second);

			/** Send Device Name */
			rp.addHeader(HEADER_MIME_TYPE, MIME_TEXT);
			rp.log(RequestParams::LogLevel::INFO, LOG_DEVICE_NAME + name);
			rp.reply(200, SerialLabelPrinterModule::toMemBlock(name));
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

std::tuple<bool, uint32_t> SerialLabelPrinterModule::sToU32(const std::string& str) {
	try {
		return { true, std::stoul(str) };
	}
	catch (...) {
		return { false, 0 };
	}
}

std::tuple<bool, uint8_t> SerialLabelPrinterModule::sToU8(const std::string& str) {
	try {
		uint32_t temp = std::stoul(str);
		if (temp > (uint32_t)UINT8_MAX) {
			return { false, 0 };
		}

		return { true, (uint8_t)temp };
	}
	catch (...) {
		return { false, 0 };
	}
}

bool SerialLabelPrinterModule::checkDevicePort(const std::string& port) {
	std::regex comRegex("^COM([1-9][0-9]*)$");
	return std::regex_match(port, comRegex);
}

const std::tuple<bool, std::vector<char>>
SerialLabelPrinterModule::base64Decode(const std::vector<char>& encoded) {
	/** Utils */
	constexpr auto base64Chars = std::string_view{
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz"
		"0123456789+/"
	};

	constexpr auto isBase64Char = [](char c) {
		return std::isalnum(static_cast<unsigned char>(c)) || c == '+' || c == '/';
		};

	constexpr auto b64CharToValue = [](char c)-> uint8_t {
		auto pos = base64Chars.find(c);
		if (pos == std::string_view::npos) {
			return 0;
		}
		return static_cast<uint8_t>(pos);
		};

	/** Result */
	std::vector<char> decoded;
	decoded.reserve(encoded.size() * 3 / 4);

	/** Buffer */
	std::vector<uint8_t> buffer;
	buffer.reserve(4);

	/** Foreach Char */
	for (char c : encoded) {
		/** Stop */
		if (c == '=') break;

		/** Invalid Char */
		if (!isBase64Char(c)) {
			return { false, {} };
		}

		/** Convert Char */
		buffer.push_back(b64CharToValue(c));
		if (buffer.size() == 4) {
			decoded.push_back(static_cast<char>((buffer[0] << 2) | (buffer[1] >> 4)));
			decoded.push_back(static_cast<char>(((buffer[1] & 0x0F) << 4) | (buffer[2] >> 2)));
			decoded.push_back(static_cast<char>(((buffer[2] & 0x03) << 6) | buffer[3]));
			buffer.clear();
		}
	}

	/** Convert Last Char */
	if (!buffer.empty()) {
		while (buffer.size() < 4) { buffer.push_back(0); }

		decoded.push_back(static_cast<char>((buffer[0] << 2) | (buffer[1] >> 4)));
		if (buffer.size() > 2 && buffer[2] != 0) {
			decoded.push_back(static_cast<char>(((buffer[1] & 0x0F) << 4) | (buffer[2] >> 2)));
		}
		if (buffer.size() > 3 && buffer[3] != 0) {
			decoded.push_back(static_cast<char>(((buffer[2] & 0x03) << 6) | buffer[3]));
		}
	}

	/** Return */
	return { true, decoded };
}

LITEHTTPD_MODULE(SerialLabelPrinterModule)
