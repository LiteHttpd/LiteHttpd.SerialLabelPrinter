#include "SerialLabelPrinterModule.h"

void SerialLabelPrinterModule::processRequest(const RequestParams& rp) {
	rp.reply(200, {});
}

LITEHTTPD_MODULE(SerialLabelPrinterModule)
