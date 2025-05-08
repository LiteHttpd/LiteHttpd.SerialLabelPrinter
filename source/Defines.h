#pragma once

#define ERROR_UNAVAILABLE_PATH 404
#define ERROR_UNAVAILABLE_METHOD 405
#define ERROR_QUERY_ERROR 400
#define ERROR_DATA_ERROR 415
#define ERROR_LIST_ERROR 1

#define MIME_JSON "application/json"
#define MIME_HTML "text/html;charset=UTF-8"
#define MIME_TEXT "text/plain;charset=UTF-8"

#define HEADER_MIME_TYPE "Content-Type"

#define LOG_WELCOME "Send welcome."
#define LOG_METHOD_ERROR "Send method error."
#define LOG_TEAPOT "Send teapot."
#define LOG_PATH_ERROR "Send path error."
#define LOG_DEVICE_LIST "Send device list: "
#define LOG_LIST_ERROR "Send list error."
#define LOG_QUERY_ERROR "Send query error: "
#define LOG_DEVICE_ERROR "Send device error: "
#define LOG_DEVICE_NAME "Send device name: "
#define LOG_DATA_ERROR "Send data error."
#define LOG_SEND_DATA "Send data for bytes: "

#define MESSAGE_METHOD_ERROR "Unavailable request method!"
#define MESSAGE_PATH_ERROR "Unavailable request path!"
#define MESSAGE_LIST_ERROR "Can't get device list!"
#define MESSAGE_PARAM_MISSED "Missed query param: "
#define MESSAGE_PORT_UNAVAILABLE "Unavailable port: "
#define MESSAGE_BAUDRATE_UNAVAILABLE "Unavailable baudrate: "
#define MESSAGE_BYTESIZE_UNAVAILABLE "Unavailable bytesize: "
#define MESSAGE_PARITY_UNAVAILABLE "Unavailable parity: "
#define MESSAGE_STOPBITS_UNAVAILABLE "Unavailable stopbits: "
#define MESSAGE_DATA_ERROR "Can't decode request data as base64!"
