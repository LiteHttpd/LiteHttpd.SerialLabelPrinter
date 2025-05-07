#include <ModuleBase.h>
#include <Utils.h>

std::tuple<int, int, int> ModuleBase::getDevKitVersion() const {
	return utils::getDevKitVersion();
}
