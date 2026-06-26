#pragma once

#include "typedef.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Create an DexCap Suit instance of V4
 */
DEX_RETURN dexcap_create_suit_instance(DEXCAP_SUIT_HANDLE * hSuit);

/**
 * @brief Connect a DexCap device to a suit instance, with given device_path.
 * @param hSuit Input. The handle of DexCap Suit instance. It must be created by
 * a call of dexcap_create_suit_instance() first.
 * @param devicePath Input. Represent the DexCap device you want to connect, normally
 * it's a DexCap Glove, or a DexCap ExoSkeleton arms. Given value of this argument must
 * be enumeration path of serial port of this device, or its bluetooth name. It must be
 * matched with the given value of adapter_type.
 * @param deviceType Output/Input. Once connection of device_path is established, this
 * argument will be evaluated as UpBody/LGLove/RGlove this function, otherwise it will
 * be set as UnDef. However, for IMU, this argument must be explicitly set to "InetMU",
 * otherwise this function will connect it as a common device, and try to retrieve its
 * device type, which will cause a failure.
 * @param adapterType Input. Indicate the adapter type of given device_path. If the
 * device is plugged via a USB-Serial cable, this argument must be WIREDUSB, if you
 * want to connect this device via Bluetooth, then this argument must be BLUETOOTH.
 */
DEX_RETURN dexcap_connect_suit_device(DEXCAP_SUIT_HANDLE hSuit, const char * devicePath,
    DEXCAP_DEVICE_TYPE * deviceType, ADAPTER_TYPE adapterType);

BOOL dexcap_is_device_connected(DEXCAP_SUIT_HANDLE hSuit, ADAPTER_TYPE adapterType);

DEX_RETURN dexcap_disconnect_all_devices(DEXCAP_SUIT_HANDLE hSuit);
DEX_RETURN dexcap_disconnect_suit_device(DEXCAP_SUIT_HANDLE hSuit, DEXCAP_DEVICE_TYPE deviceType);

DEX_RETURN dexcap_start_suit_sampling(DEXCAP_SUIT_HANDLE hSuit);
DEX_RETURN dexcap_start_device_sampling(DEXCAP_SUIT_HANDLE hSuit, DEXCAP_DEVICE_TYPE deviceType);

BOOL dexcap_is_device_sampling(DEXCAP_SUIT_HANDLE hSuit, DEXCAP_DEVICE_TYPE adapterType);

DEX_RETURN dexcap_stop_suit_sampling(DEXCAP_SUIT_HANDLE hSuit);
DEX_RETURN dexcap_stop_device_sampling(DEXCAP_SUIT_HANDLE hSuit, DEXCAP_DEVICE_TYPE deviceType);

DEX_RETURN dexcap_get_l_glove_data(DEXCAP_SUIT_HANDLE hSuit, GloveJointAngles *jointData);
DEX_RETURN dexcap_get_r_glove_data(DEXCAP_SUIT_HANDLE hSuit, GloveJointAngles *jointData);
DEX_RETURN dexcap_get_ex_body_data(DEXCAP_SUIT_HANDLE hSuit, SkeletonJointAngles *jointData);
DEX_RETURN dexcap_get_im_unit_data(DEXCAP_SUIT_HANDLE hSuit, InertialUnitData *imuData);
DEX_RETURN dexcap_get_joint_data(DEXCAP_SUIT_HANDLE hSuit, DexCapJointData *jointData);

DEX_RETURN dexcap_get_l_battery_state(DEXCAP_SUIT_HANDLE hSuit, uint16_t * voltage);
DEX_RETURN dexcap_get_r_battery_state(DEXCAP_SUIT_HANDLE hSuit, uint16_t * voltage);
DEX_RETURN dexcap_get_main_battery_state(DEXCAP_SUIT_HANDLE hSuit, MainBatteryState *batteryState);

DEX_RETURN register_joint_data_callback(DEXCAP_SUIT_HANDLE hSuit, DexCapJointDataProc callback);

#ifdef __cplusplus
}
#endif