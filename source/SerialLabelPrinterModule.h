#pragma once

#include <LiteHttpdDev.h>
#include "Device.h"

class SerialLabelPrinterModule final : public ModuleBase {
public:
	void processRequest(const RequestParams& rp) override;

private:
	using DeviceList = Device::ItemList;

	static const std::vector<char> createErrorMessage(
		int code, const std::string& mes);
	static const std::vector<char> createDeviceListMessage(
		const DeviceList& list);
	static const std::vector<char> createWelcomePage();
	static const std::vector<char> createTeapotPage();

	static const std::vector<char> toMemBlock(const std::string& str);
	static bool checkDevicePort(const std::string& port);
};
