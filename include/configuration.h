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

#include "typedef.h"


namespace DexRobot
{

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