//
// Created by ryzuo on 25-9-3.
//
#pragma once

#include <memory>

#ifndef WIN32
#include <libserial/SerialPort.h>
#else
#include<Windows.h>
#endif

#include <yaml-cpp/yaml.h>
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

class DexCapConfig final
{
public:
    using PTR = std::shared_ptr<DexCapConfig>;

public:
    DexCapConfig() = delete;
    ~DexCapConfig() = default;

    static DexCapConfig::PTR Load(const std::string & yamlFile);

    std::string GetServerAddress() const;
    uint16_t    GetServerPortNum() const;
    NetProtocolType GetServerProtoType() const;

    std::string GetLeftHandSPortName() const;
    std::string GetRightHandSPortName() const;
    std::string GetBodySPortName() const;

    AdapterType GetLeftHandAdapterType() const;
    AdapterType GetRightHandAdapterType() const;
    AdapterType GetBodyAdapterType() const;

    uint8_t GetLeftHandDeviceId() const;
    uint8_t GetRightHandDeviceId() const;
    uint8_t GetBodyDeviceId() const;

    bool IsDebugOnLeftHand() const;
    bool IsDebugOnRightHand() const;
    bool IsDebugOnBody() const;

    bool IsNetworkEnabled() const;
    bool IsBodyEnabled() const;
    bool IsLeftHandEnabled() const;
    bool IsRightHandEnabled() const;

    DEXCAP_DEVICE_TYPE GetDexCapDeviceType(const std::string & adapterName) const;
    uint8_t GetDexCapDeviceID(const std::string & adapterName) const;
    AdapterType GetAdapterType(const std::string & adapterName) const;
    uint8_t GetDexHandType(const std::string & adapterName) const;

#ifndef WIN32
    LibSerial::BaudRate GetBodyBaudRate() const;
    LibSerial::BaudRate GetLeftHandBaudRate() const;
    LibSerial::BaudRate GetRightHandBaudRate() const;
#else
    DWORD GetBodyBaudRate() const;
    DWORD GetLeftHandBaudRate() const;
    DWORD GetRightHandBaudRate() const;
#endif

    uint16_t GetBodySampleRate() const;
    uint16_t GetLeftHandSampleRate() const;
    uint16_t GetRightHandSampleRate() const;

private:
    explicit DexCapConfig(const std::string & yamlFile);

private:

    YAML::Node configuration;
  
};

}