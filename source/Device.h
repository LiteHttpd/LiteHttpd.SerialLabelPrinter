#pragma once

#include <tuple>
#include <string>
#include <list>
#include <vector>

class Device final {
public:
	Device() = delete;

public:
	using Item = std::tuple<std::string, std::string>;
	using ItemList = std::list<Item>;

	static const std::tuple<bool, ItemList> getDeviceList();

	enum class DeviceResult {
		OK = 0, NotFound, NotAvailable, BadPort, UnknownError,
		GetStateError, InitError,
		MaxNum
	};
	static const std::string getResultDescription(DeviceResult result);

	static DeviceResult getDeviceStatus(const std::string& port);
	static const std::string getDeviceName(const std::string& port);

	struct Config final {
		uint32_t baudRate = 9600;
		uint8_t byteSize = 8;
		uint8_t parity = 0;
		uint8_t stopBits = 0;
	};
	static DeviceResult sendData(
		const std::string& port, const Config& config,
		const std::vector<char>& data);

private:
	static const std::string getCOMPort(const std::string& friendlyName);
	static const std::string getCOMFullName(const std::string& port);
};
