#include <Utils.h>

namespace utils {
	int getDevKitMajorVersion() {
		return PROJECT_VERSION_MAJOR;
	}

	int getDevKitMinorVersion() {
		return PROJECT_VERSION_MINOR;
	}

	int getDevKitPatchVersion() {
		return PROJECT_VERSION_PATCH;
	}

	const std::string getDevKitVersionStr() {
		return std::to_string(getDevKitMajorVersion()) + "."
			+ std::to_string(getDevKitMinorVersion()) + "."
			+ std::to_string(getDevKitPatchVersion());
	}
}
