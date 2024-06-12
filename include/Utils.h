#pragma once

#include <string>

namespace utils {
	int getDevKitMajorVersion();
	int getDevKitMinorVersion();
	int getDevKitPatchVersion();

	const std::string getDevKitVersionStr();
}
