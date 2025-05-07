#pragma once

#include <LiteHttpdDev.h>

class SerialLabelPrinterModule final : public ModuleBase {
public:
	void processRequest(const RequestParams& rp) override;
};
