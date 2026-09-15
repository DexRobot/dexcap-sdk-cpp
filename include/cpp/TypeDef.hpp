#pragma once

#include <cstdint>
#include <array>
#include <functional>
#include "typedef.h"

#ifdef _WIN32
#undef AdapterType
#undef ERROR_INVALID_DATA
#endif

namespace DexRobot
{

typedef ADAPTER_TYPE AdapterType;
using ConnectionType = AdapterType;

enum class NetProtocolType
{
    NA = 0,
    TCP,
    UDP
};

enum class LOG_LEVEL
{
    DX_CLOSE = 0,
    DX_FATAL,
    DX_ERROR,
    DX_WARN,
    DX_INFO,
    DX_DEBUG,
};

enum class WLPowerLevel
{
    LEVEL0 = 0,
    LEVEL1 = 1,
    LEVEL2 = 2,
    LEVEL3 = 3,
    LEVEL4 = 4,
    LEVELX = 0xFF,
};

struct DexCapErrorInfo
{
    uint8_t deviceId;
    uint8_t errorFunc;
    uint8_t errorCode;
    uint8_t extParams[5];
    char messages[256];
};

}
