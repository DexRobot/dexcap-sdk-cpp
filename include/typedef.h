#pragma once

#include <stddef.h>
#include <stdint.h>

#ifdef __GNUC__
#define PACK( __Declaration__ ) __Declaration__// __attribute__((__packed__))
#endif

#ifdef __MSC_VER
#define PACK( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop) )
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#define DEXCAP_SUIT_HANDLE void *

typedef enum
{
    DEX_ERROR = -1,
    DEX_INVALID_DEVICE = -2,
    DEX_INVALID_INSTANCE = -3,
    DEX_INVALID_DATA_FMT = -4,
    DEX_DEV_TYPE_MISMATCH = -5,

    ///
    DEX_SUCCESS = 0,
    DEX_SUCCESS_WITH_INFO = 1,
    DEX_NO_DATA = 100,
} DEX_RETURN;

typedef enum
{
    // negative for SDK sys errors
    ERROR_INVALID_IO   = -0x07,
    ERROR_UNSUPPORTED  = -0x06,
    ERROR_CONN_FAILURE = -0x05,
    ERROR_BLE_UNRESOLVED = -0x04,
    ERROR_AUTH_FAILURE = -0x03,
    ERROR_INVALID_DEV  = -0x02,
    ERROR_COMMON       = -0x01,
    ERROR_NONE         = 0x00,

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
} ErrorCode;

typedef enum
{
    INVALID   = 0,
    WIREDUSB  = 0x01,
    WIRELESS  = 0x02,
    COMMONUSB = 0x03,     // Reserved for internal use, useless for common user
    BLUETOOTH = 0x04,
} ADAPTER_TYPE;

typedef enum
{
    UnDefn = 0x00,
    LGlove = 0x01,
    RGlove = 0x02,
    UpBody = 0x04,
    IMUnit = 0x08,
    WRecvr = 0x20,
} DEXCAP_DEVICE_TYPE;

typedef enum ProductVersion_t
{
    NA = 0x00, V3 = 0x03, V4 = 0x04,
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

PACK(typedef struct SkeletonJointAngles_t
{
    uint16_t LArm1;
    uint16_t LArm2;
    uint16_t LArm3;
    uint16_t LArm4;
    uint16_t LArm5;
    uint16_t LArm6;
    uint16_t LArm7;
    uint16_t LArm8;
    uint16_t LArm9;
    uint16_t RArm1;
    uint16_t RArm2;
    uint16_t RArm3;
    uint16_t RArm4;
    uint16_t RArm5;
    uint16_t RArm6;
    uint16_t RArm7;
    uint16_t RArm8;
    uint16_t RArm9;
    uint16_t Back1;
    uint16_t Back2;
    uint16_t Back3;
    uint16_t Back4;
    uint16_t Back5;
    uint16_t BatteryState;
    uint64_t timestamp;
} SkeletonJointAngles);

PACK(typedef struct InertialUnitData_t
{
    double roll;
    double pitch;
    double yaw;
    double accel[3];  // accelerometer (x, y, z)
    double gyscp[3];  // gyroscope (x, y, z)
    double magnt[3];  // magnetometer (x, y, z)
    uint64_t timestamp;
} InertialUnitData);

PACK(typedef struct DexCapJointData_t
{
    uint32_t mask;
    uint16_t LGlove[24];
    uint16_t ExBody[24];
    uint16_t RGlove[24];
    double   InetMU[15];
    uint64_t timestamp;
} DexCapJointData);

typedef void (* DexCapJointDataProc)(const DexCapJointData *);

#ifdef __cplusplus
}
#endif
