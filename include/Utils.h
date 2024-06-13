#pragma once

#include <string>

namespace utils {
	int getDevKitMajorVersion();
	int getDevKitMinorVersion();
	int getDevKitPatchVersion();

	std::tuple<int, int, int> getDevKitVersion();

	const std::string getDevKitVersionStr();
}
