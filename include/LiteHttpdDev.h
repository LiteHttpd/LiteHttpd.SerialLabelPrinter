#pragma once

#include "ModuleBase.h"
#include "Utils.h"
#include "Macros.h"

#define LITEHTTPD_MODULE_CREATE_FUNCTION createModule
#define LITEHTTPD_MODULE_DESTORY_FUNCTION destoryModule

#define LITEHTTPD_MODULE_CREATE_FUNCTION_NAME "createModule"
#define LITEHTTPD_MODULE_DESTORY_FUNCTION_NAME "destoryModule"

#define LITEHTTPD_MODULE_CREATE(className) \
extern "C" HTTPDDEV_EXPORT ModuleBase* LITEHTTPD_MODULE_CREATE_FUNCTION() { \
	return new className; \
}

#define LITEHTTPD_MODULE_DESTORY(className) \
extern "C" HTTPDDEV_EXPORT void LITEHTTPD_MODULE_DESTORY_FUNCTION(ModuleBase* ptr) { \
	delete ptr; \
}

#define LITEHTTPD_MODULE(className) \
LITEHTTPD_MODULE_CREATE(className) \
LITEHTTPD_MODULE_DESTORY(className)
