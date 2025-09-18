//
// Created by ryzuo on 25-8-20.
//
#pragma once

#include <cstdint>
#include <functional>

namespace DexRobot
{

class MsgNode;

enum DEX_RETURN
{
    DEX_ERROR = -1,
    DEX_INVALID_DEVICE = -2,

    DEX_SUCCESS = 0,
    DEX_SUCCESS_WITH_INFO = 1,
    DEX_NO_DATA = 100,
};

enum ErrorCode
{
    COMMON_ERROR   = -0x01,
    INVALID_DEVICE = -0x02,
    NONE_ERROR     = 0x00,
    INVALID_FUNC_CODE = 0x01,
    INVALID_PARAM_LEN = 0x02,
    INVALID_SNSR_DATA = 0x03,
    DEG_SNSOR_ERROR   = 0x04,
    VIB_MOTOR_ERROR   = 0x05,
    OTHER_ERROR       = 0x06,
    NO_VALID_DATA     = 0x0D,
    REQUEST_TIMEOUT   = 0x0F,
    PARM_SAVE_FAILURE = 0x10,
};

enum class AdapterType
{
    INVALID = 0,
    WIREDUSB,
    WIRELESS,
};

enum class NetProtocolType
{
    NA = 0,
    TCP,
    UDP
};

enum class ExoApparatus
{
    UnDefn = 0x00,
    LGlove = 0x01,
    RGlove = 0x02,
    UpBody = 0x03,
    WRecvr = 0x04,
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

typedef std::function<void(const MsgNode*, const MsgNode*, const MsgNode*)> DataCallback;

}
