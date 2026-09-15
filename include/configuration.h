#pragma once

#include <memory>

#ifdef WIN32
#include<Windows.h>
#endif

#include "cpp/TypeDef.hpp"

#ifdef __cplusplus
extern "C"
{
#endif

#define MAX_UDEV_PORT_NAME_LENG 16

typedef struct dexcap_device_serial_port_info
{
    uint8_t device_type;
    char serial_port_name[MAX_UDEV_PORT_NAME_LENG];
} serial_port_device_info;

struct dexcap_device_serial_port_info * alloc_serial_port_device_list();
void free_serial_port_device_list(dexcap_device_serial_port_info * device_list);
void enumerate_serial_port_devices(ProductVersion prod_ver, dexcap_device_serial_port_info * device_list, size_t * count);

#ifdef __cplusplus
}
#endif

namespace DexRobot
{

struct DEV_SERIAL_PORT_INFO_V3
{
    static const char * GLOVE_UDEV_VID;
    static const char * GLOVE_UDEV_PID;
    static const char * UBODY_UDEV_VID;
    static const char * UBODY_UDEV_PID;
};

struct DEV_SERIAL_PORT_INFO_V4
{
    static const char * GLOVE_UDEV_VID;
    static const char * GLOVE_UDEV_PID;
    static const char * UBODY_UDEV_VID;
    static const char * UBODY_UDEV_PID;
};

}