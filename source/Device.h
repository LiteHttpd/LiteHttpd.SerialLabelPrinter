#pragma once

#include <tuple>
#include <string>
#include <list>

class Device final {
public:
	Device() = delete;

public:
	using Item = std::tuple<std::string, std::string>;
	using ItemList = std::list<Item>;

	static const std::tuple<bool, ItemList> getDeviceList();

	enum class DeviceResult {
		OK = 0, NotFound, NotAvailable, BadPort, UnknownError,
		MaxNum
	};
	static const std::string getResultDescription(DeviceResult result);

	static DeviceResult getDeviceStatus(const std::string& port);
	static const std::string getDeviceName(const std::string& port);

private:
	static const std::string getCOMPort(const std::string& friendlyName);
	static const std::string getCOMFullName(const std::string& port);
};
