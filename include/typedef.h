#pragma once

#include <stdint.h>

#ifdef __GNUC__
#define PACK( __Declaration__ ) __Declaration__// __attribute__((__packed__))
#endif

#ifdef _MSC_VER
#include <cstddef>
#define PACK( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop) )
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef BOOL
#undef BOOL
#endif
#define BOOL unsigned char

#ifndef TRUE
#define TRUE 1
#else
#undef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#else
#undef FALSE
#define FALSE 0
#endif

#ifdef CHAR
#undef CHAR
#endif

#ifdef UCHAR
#undef UCHAR
#endif

#ifdef __GNUC__
typedef char CHAR;
typedef unsigned char UCHAR;
#elif defined(_MSC_VER)
#define CHAR char
#define UCHAR unsigned char
#endif

#define DEXCAP_SUIT_HANDLE void *

typedef enum
{
    DEX_ERROR = -1,
    DEX_INVALID_DEVICE    = -2,
    DEX_INVALID_INSTANCE  = -3,
    DEX_INVALID_DATA_FMT  = -4,
    DEX_DEV_TYPE_MISMATCH = -5,

    ///
    DEX_SUCCESS            = 0,
    DEX_SUCCESS_WITH_INFO  = 1,
    DEX_REQUEST_TIMEOUT    = 2,
    DEX_SEC_ON_WITHOUT_KEY = 3,
    DEX_BLE_CONN_UNSECURED = 4,
    DEX_STRING_TRUNCATED   = 6,
    DEX_NO_DATA            = 100,
} DEX_RETURN;

typedef enum
{
    // negative for SDK sys errors
    ERROR_BLE_UNSECURE   = -0x09,
    ERROR_BLE_UNPAIRED   = -0x08,
    ERROR_INVALID_IO     = -0x07,
    ERROR_UNSUPPORTED    = -0x06,
    ERROR_CONN_FAILURE   = -0x05,
    ERROR_BLE_UNRESOLVED = -0x04,
    ERROR_AUTH_FAILURE   = -0x03,
    ERROR_INVALID_DEV    = -0x02,
    ERROR_COMMON         = -0x01,

    ERROR_NONE           = 0x00,

    // 02 - FF, firmware errors most
    ERROR_INVALID_MODE = 0x02,
    ERROR_INVALID_LENG = 0x03,
    ERROR_DATA_INVALID = 0x04,
    ERROR_UN_PERMITTED = 0x05,
    ERROR_OUT_OF_RANGE = 0x06,
    ERROR_NVS_W_FAILED = 0x07,
    ERROR_DEVICE_BUSY  = 0x08,
    ERROR_ISNOT_READY  = 0x09,
    ERROR_REQ_TIMEOUT  = 0x0A,
    ERROR_OTHER_UNDEF  = 0x0F,
    ERROR_SENSR_FAULT  = 0x10,
    ERROR_MOTOR_FAULT  = 0x11,
    ERROR_IICSG_FAULT  = 0x12,
    ERROR_POWER_FAULT  = 0x13,
    ERROR_LOW_BATTERY  = 0x14,
    ERROR_UNCALIBRATED = 0x20,
    ERROR_CRC_FAILURE  = 0x21,
    ERROR_NVS_R_FAILED = 0x22,
    ERROR_BTSAVE_FAILED = 0xFB,
    PARAM_SAVE_FAILURE = 0xFE,

    ///
    ERROR_IO_FAILURE = 0x0100,
} ErrorCode;

typedef enum
{
    INVALID   = 0,
    WIREDUSB  = 0x01,
    WLAN80211 = 0x02,
    COMMONUSB = 0x03,     // Reserved for internal use, useless for common user
    BLUETOOTH = 0x04,
    WLAN_TCP  = 0x08,
} ADAPTER_TYPE;

typedef enum
{
    UnDefn = 0x00,
    LGlove = 0x01,
    RGlove = 0x02,
    UpBody = 0x04,
    JSBody = 0x0A,
    WRecvr = 0x20,
} DEXCAP_DEVICE_TYPE;

typedef enum
{
    WAIST,
    L_ARM,
    R_ARM,
    L_THUMB,
    L_INDEX,
    L_MIDDLE,
    L_RING,
    L_LITTLE,
    R_THUMB,
    R_INDEX,
    R_MIDDLE,
    R_RING,
    R_LITTLE,
} BodyPart;

typedef enum
{
    L_JOINT_1 = 0x01,
    L_JOINT_2 = 0x02,
    L_JOINT_3 = 0x03,
    L_JOINT_4 = 0x04,
    L_JOINT_5 = 0x05,
    L_JOINT_6 = 0x06,
    L_JOINT_7 = 0x07,
    L_JOINT_8 = 0x08,
    L_JOINT_9 = 0x09,
    R_JOINT_1 = 0x11,
    R_JOINT_2 = 0x12,
    R_JOINT_3 = 0x13,
    R_JOINT_4 = 0x14,
    R_JOINT_5 = 0x15,
    R_JOINT_6 = 0x16,
    R_JOINT_7 = 0x17,
    R_JOINT_8 = 0x18,
    R_JOINT_9 = 0x19,
} ExoSkeletonJointID;

typedef enum ProductVersion_t
{
    NA = 0x00, V3 = 0x03, V4 = 0x04,
    V4C1 = 0x4C1, V4C2 = 0x4C2, V4C3 = 0x4C3,
} ProductVersion;

PACK(typedef struct GloveJointAngles_t
{
    uint16_t ThumbDIP;
    uint16_t ThumbPIP;
    uint16_t ThumbMCP;
    uint16_t ThumbSWP;
    uint16_t ThumbROP;
    uint16_t IndexDIP;
    uint16_t IndexPIP;
    uint16_t IndexMCP;
    uint16_t IndexSWP;
    uint16_t MiddleDIP;
    uint16_t MiddlePIP;
    uint16_t MiddleMCP;
    uint16_t MiddleSWP;
    uint16_t RingDIP;
    uint16_t RingPIP;
    uint16_t RingMCP;
    uint16_t RingSWP;
    uint16_t LittleDIP;
    uint16_t LittlePIP;
    uint16_t LittleMCP;
    uint16_t LittleSWP;
    uint16_t BatteryState;
    uint32_t ErrorMask;
    uint64_t timestamp;
} GloveJointAngles);

PACK(typedef struct Joystick_t
{
    int16_t  RockerX  : 16;
    int16_t  RockerY  : 16;
    uint16_t TgrDistA : 16;
    uint16_t TgrDistB : 16;
    uint8_t  ButtonA  : 1;
    uint8_t  ButtonB  : 1;
    uint8_t  RockerZ  : 1;
    uint8_t  TriggerA : 1;
    uint8_t  TriggerB : 1;
    int16_t  Reserved : 11;
} Joystick);

PACK(typedef struct SkeletonArmsData_t
{
    uint16_t LArm1;
    uint16_t LArm2;
    uint16_t LArm3;
    uint16_t LArm4;
    uint16_t LArm5;
    uint16_t LArm6;
    uint16_t LArm7;
    uint16_t RArm1;
    uint16_t RArm2;
    uint16_t RArm3;
    uint16_t RArm4;
    uint16_t RArm5;
    uint16_t RArm6;
    uint16_t RArm7;
    Joystick LJoyS;  // Left Joystick
    Joystick RJoyS;  // Right Joystick
    uint64_t timestamp;
} SkeletonArmsData);

PACK(typedef struct MainBatteryStatus_t
{
    int16_t  Currency; // Positive for charging, negative for discharging, in mA
    uint16_t Voltage;  // As in mV
    uint16_t RemainPower;  // Percentage of remaining power
    uint16_t Temperature;  // As in centigrade value divided by 10
    uint16_t StatusBitmap;
} MainBatteryStatus);

typedef struct SystemStatus_t
{
    BOOL  Enabled    : 1;  // Whether sensors are enabled
    UCHAR Reserved1  : 2;
    BOOL  WifiState  : 1;
    BOOL  BootState  : 1;
    BOOL  NeedCharge : 1;
    BOOL  Reserved2  : 1;
    BOOL  LJoyConn   : 1;
    BOOL  RJoyConn   : 1;
    BOOL  Reserved3  : 7;
} SystemStatus;

typedef struct SuitStatusData_t
{
    SkeletonArmsData jointData;
    MainBatteryStatus mainBatteryState;
    SystemStatus systemStatus;
} SuitStatusData;

PACK(typedef struct DexCapEndPoses_t
{
    double LArm[4][4];
    double RArm[4][4];
    uint64_t timestamp;
} DexCapEndPoses);

typedef void (* DexCapSuitDataProc)(const SuitStatusData *);

#ifdef __cplusplus
}
#endif
