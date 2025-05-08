#include "Device.h"

#include <windows.h>
#include <setupapi.h>
#include <devguid.h>
#include <regex>
#include <array>

const std::tuple<bool, Device::ItemList> Device::getDeviceList() {
	ItemList result;

	/** Get Device Info List */
	HDEVINFO hDevInfo = SetupDiGetClassDevs(
		&GUID_DEVCLASS_PORTS,
		nullptr,
		nullptr,
		DIGCF_PRESENT);
	if (hDevInfo == INVALID_HANDLE_VALUE) {
		return { false, {} };
	}

	/** Device Info Temp */
	SP_DEVINFO_DATA devInfoData = {};
	devInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

	/** For Each Device */
	for (DWORD i = 0; SetupDiEnumDeviceInfo(hDevInfo, i, &devInfoData); ++i) {
		/** Result */
		std::string port, name;

		/** Get Device Friendly Name */
		{
			TCHAR buffer[256] = { 0 };
			DWORD buffersize = sizeof(buffer);

			/** Get Device Prop */
			if (SetupDiGetDeviceRegistryProperty(
				hDevInfo, &devInfoData, SPDRP_FRIENDLYNAME,
				nullptr, (PBYTE)buffer, buffersize, nullptr)) {

				/** Get Friendly Name */
				name = std::string{ buffer };

				/** Get Port */
				port = Device::getCOMPort(name);
			}
		}
		
		/** Insert Result Into List */
		if (!port.empty()) {
			result.push_back({ port, name });
		}
	}

	/** Destory Device Info List */
	SetupDiDestroyDeviceInfoList(hDevInfo);

	/** Return Result */
	return { true, result };
}

const std::string Device::getResultDescription(DeviceResult result) {
	constexpr std::array<const char*, (int)DeviceResult::MaxNum> list{
		"Succeed!",
		"Serial port device not found!",
		"Device is using!",
		"Serial port format error!",
		"Unknown error!"
	};
	return list[(int)result];
}

Device::DeviceResult Device::getDeviceStatus(const std::string& port) {
	/** Get Full Name */
	auto fullPortName = Device::getCOMFullName(port);

	/** Open Serial */
	HANDLE hSerial = CreateFileA(
		fullPortName.c_str(),
		GENERIC_READ | GENERIC_WRITE,
		0,				/** Don't share */
		nullptr,
		OPEN_EXISTING,
		0,
		nullptr
	);

	/** Check Error */
	if (hSerial == INVALID_HANDLE_VALUE) {
		DWORD err = GetLastError();
		switch (err)
		{
		case ERROR_ACCESS_DENIED:
			/** Not Available */
			return DeviceResult::NotAvailable;
		case ERROR_FILE_NOT_FOUND:
			/** Not Found */
			return DeviceResult::NotFound;
		case ERROR_INVALID_HANDLE:
			/** Bad */
			return DeviceResult::BadPort;
		default:
			/** Unknown Error */
			return DeviceResult::UnknownError;
		}
	}

	/** Close Serial */
	CloseHandle(hSerial);

	/** OK */
	return DeviceResult::OK;
}

const std::string Device::getDeviceName(const std::string& port) {
	/** Get Device List */
	auto [result, list] = Device::getDeviceList();
	if (!result) { return {}; }

	/** Find Port */
	for (auto& [p, n] : list) {
		if (p == port) {
			return n;
		}
	}

	/** Not Found */
	return {};
}

const std::string Device::getCOMPort(const std::string& friendlyName) {
	std::regex comRegex("\\((COM\\d+)\\)");
	std::smatch match;
	if (std::regex_search(friendlyName, match, comRegex)) {
		return match[1];
	}
	return {};
}

const std::string Device::getCOMFullName(const std::string& port) {
	return "\\\\.\\" + port;
}
