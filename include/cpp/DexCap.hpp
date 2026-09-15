#pragma once

#include <map>
#include <future>

#ifdef WIN32
#include <WinSock2.h>
#include <Windows.h>
#else
#include <libserial/SerialPort.h>
using namespace LibSerial;
#endif

#include "TypeDef.hpp"
#include "Utils.hpp"

namespace DexRobot
{

typedef DEXCAP_DEVICE_TYPE ExoApparatus;


class DexCapSuit
{
public:
    DexCapSuit() = delete;
    explicit DexCapSuit(AdapterType connectionType, ProductVersion version=V4C1);
    virtual ~DexCapSuit();

    virtual ExoApparatus ConnectDevice(const std::string & deviceAddress);
    virtual bool DisconnectDevice(const std::string & deviceAddress);
    virtual bool DisconnectDevice(ExoApparatus device);

    [[nodiscard]] std::string GetAdapterName(ExoApparatus device) const;

    ProductVersion & productVersion();
    [[nodiscard]] const ProductVersion & productVersion() const;
    [[nodiscard]] uint8_t GetDeviceID(ExoApparatus device) const;

    std::string getSerialNumber(ExoApparatus device);

    DEX_RETURN Start();
    [[nodiscard]] bool Start(ExoApparatus device) const;

    [[nodiscard]] bool IsConnected(ExoApparatus device);
    [[nodiscard]] bool IsSensorEnabled(ExoApparatus device) const;
    [[nodiscard]] bool IsChargeNeeded(ExoApparatus device) const;

    [[nodiscard]] bool IsRunning() const;
    [[nodiscard]] bool IsRunning(ExoApparatus device) const;

    DEX_RETURN Pause();
    [[nodiscard]] bool Pause(ExoApparatus device) const;

    DEX_RETURN Resume();
    [[nodiscard]] bool Resume(ExoApparatus device) const;

    DEX_RETURN Close();
    DEX_RETURN Close(ExoApparatus device);

    [[nodiscard]] AdapterType GetAdapterType() const;

    [[nodiscard]] ExoApparatus GetDeviceType(const std::string & adapterName) const;

    [[nodiscard]] const SuitStatusData & GetSuitJointState() const;
    [[nodiscard]] const SkeletonArmsData & GetArmsJointsState() const;
    [[nodiscard]] const Joystick & GetLJoyStickState() const;
    [[nodiscard]] const Joystick & GetRJoyStickState() const;

    [[nodiscard]] const DexCapEndPoses & GetEndPose() const;

    [[nodiscard]] uint16_t GetBatteryLevel(ExoApparatus device) const;
    [[nodiscard]] const MainBatteryStatus *GetMainBatteryStatus() const;

    void VibeMotors(ExoApparatus hand, const std::vector<uint8_t> &) const;
    [[nodiscard]] std::string GetFirmwareVersion(ExoApparatus device) const;

    [[nodiscard]] bool anyError(ExoApparatus device) const;
    [[nodiscard]] ErrorCode getErrorCode(ExoApparatus device) const;
    [[nodiscard]] std::string getErrorMessage(ExoApparatus device) const;

    [[nodiscard]] ErrorCode getErrorCode() const;
    const std::string & getErrorMessage();

    void registerStatusDataProc(const DexCapSuitDataProc & callback);

private:
    class DexCapSuitImpl *impl;
};

}
