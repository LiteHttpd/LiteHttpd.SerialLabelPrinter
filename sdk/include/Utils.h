#pragma once

#include <string>
#include <tuple>

namespace utils {
	int getDevKitMajorVersion();
	int getDevKitMinorVersion();
	int getDevKitPatchVersion();

	std::tuple<int, int, int> getDevKitVersion();

	const std::string getDevKitVersionStr();
}
