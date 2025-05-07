#pragma once

#include "RequestParams.h"

class ModuleBase {
public:
	ModuleBase() = default;
	virtual ~ModuleBase() = default;

	std::tuple<int, int, int> getDevKitVersion() const;

public:
	virtual void processRequest(const RequestParams& rp) = 0;
};
