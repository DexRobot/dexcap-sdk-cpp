#pragma once

#include <stddef.h>
#include <stdint.h>

#ifdef __GNUC__
#define PACK( __Declaration__ ) __Declaration__// __attribute__((__packed__))
#endif

#ifdef _MSC_VER
#define PACK( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop) )
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef _WIN32
#ifndef BOOL
#define BOOL int
#else
#undef BOOL
#define BOOL int
#endif
#else
typedef int BOOL;
#endif

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
    DEX_REQUEST_TIMEOUT = 2,
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

    ///
    ERROR_IO_FAILURE = 0x0100,
} ErrorCode;

typedef enum
{
    INVALID   = 0,
    WIREDUSB  = 0x01,
    WIRELESS  = 0x02,
    COMMONUSB = 0x03,     // Reserved for internal use, useless for common user
    BLUETOOTH = 0x04,
    MODBUSUSB = 0x06,     //Currently only for imu
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
    W_JOINT_B,    // DOF of IMU, End point of waist
    W_JOINT_1,
    W_JOINT_2,
    W_JOINT_3,
    W_JOINT_4,
    W_JOINT_5,
    W_JOINT_U,    // A dummy DOF of center of shoulder
    L_JOINT_1,
    L_JOINT_2,
    L_JOINT_3,
    L_JOINT_4,
    L_JOINT_5,
    L_JOINT_6,
    L_JOINT_7,
    L_JOINT_8,
    L_JOINT_9,
    R_JOINT_1,
    R_JOINT_2,
    R_JOINT_3,
    R_JOINT_4,
    R_JOINT_5,
    R_JOINT_6,
    R_JOINT_7,
    R_JOINT_8,
    R_JOINT_9,
} ExoSkeletonJointID;

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
    uint16_t Reserved;
    uint64_t timestamp;
} SkeletonJointAngles);

PACK(typedef struct MainBatteryState_t
{
    int16_t  Currency; // Positive for charging, negative for discharging, in mA
    uint16_t Voltage;  // As in mV
    uint16_t RemainPower;  // Percentage of remaining power
    uint16_t Temperature;  // As in centigrade value divided by 10
    uint16_t StatusBitmap;
    uint16_t Reserved;
} MainBatteryState);

PACK(typedef struct BodyBoardState_t
{
    BOOL Enabled;  // Whether sensors are enabled
    BOOL CharingL;
    BOOL CharingR;
    BOOL BLEConnected;
    BOOL NeedCharge;
} BodyBoardState);

PACK(typedef struct InertialUnitData_t
{
    float roll;
    float pitch;
    float yaw;
    float quat[4];     // quaternion
    float accel[3];    // accelerometer (x, y, z)
    float gyscp[3];    // gyroscope (x, y, z)
    float magnt[3];    // magnetometer (x, y, z)
    float air_pressure; // air pressure
    int8_t temp;         // Temperature
    uint32_t system_time;//system time
    uint64_t timestamp;
} InertialUnitData);

PACK(typedef struct InetMUData_t
{
    uint32_t sys_time;//system time of imu,for debug
    float poseData[17];
    int8_t temp;         // Temperature
}InetMUData);

PACK(typedef struct DexCapJointData_t
{
    uint32_t mask;
    uint16_t LGlove[24];
    uint16_t ExBody[24];
    uint16_t RGlove[24];
    InetMUData InetMU;
    uint64_t timestamp;
} DexCapJointData);

PACK(typedef struct DexCapEndPoses_t
{
    double LArm[4][4];
    double RArm[4][4];
    uint64_t timestamp;
} DexCapEndPoses);

typedef void (* DexCapJointDataProc)(const DexCapJointData *);

#ifdef __cplusplus
}
#endif
